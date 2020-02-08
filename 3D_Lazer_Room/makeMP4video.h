/*
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

 /**
  * @file
  * libavformat API example.
  *
  * Output a media file in any supported libavformat format. The default
  * codecs are used.
  * @example muxing.c
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

#pragma warning(disable : 4996)
extern "C"
{
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P

#define SCALE_FLAGS SWS_BICUBIC

typedef struct OutputStream {
    AVStream* st;
    AVCodecContext* enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame* frame;
    AVFrame* tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext* sws_ctx;
    struct SwrContext* swr_ctx;
} OutputStream;

static void log_packet(const AVFormatContext* fmt_ctx, const AVPacket* pkt)
{
    AVRational* time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    //printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
    //    av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
    //    av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
    //    av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
    //    pkt->stream_index);
}

static int write_frame(AVFormatContext* fmt_ctx, const AVRational* time_base, AVStream* st, AVPacket* pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;

    /* Write the compressed frame to the media file. */
    log_packet(fmt_ctx, pkt);
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

static void add_stream(int width, int height, OutputStream* ost, AVFormatContext* oc,
    AVCodec** codec,
    enum AVCodecID codec_id)
{
    AVCodecContext* c;
    int i;

    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
            avcodec_get_name(codec_id));
        exit(1);
    }

    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    ost->st->id = oc->nb_streams - 1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }
    ost->enc = c;

    switch ((*codec)->type) {
    case AVMEDIA_TYPE_AUDIO:
        c->sample_fmt = (*codec)->sample_fmts ?
            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        c->bit_rate = 64000;
        c->sample_rate = 44100;
        if ((*codec)->supported_samplerates) {
            c->sample_rate = (*codec)->supported_samplerates[0];
            for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                if ((*codec)->supported_samplerates[i] == 44100)
                    c->sample_rate = 44100;
            }
        }
        c->channels = av_get_channel_layout_nb_channels(c->channel_layout);
        c->channel_layout = AV_CH_LAYOUT_STEREO;
        if ((*codec)->channel_layouts) {
            c->channel_layout = (*codec)->channel_layouts[0];
            for (i = 0; (*codec)->channel_layouts[i]; i++) {
                if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                    c->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
        c->channels = av_get_channel_layout_nb_channels(c->channel_layout);
        ost->st->time_base = { 1, c->sample_rate };
        break;

    case AVMEDIA_TYPE_VIDEO:
        c->codec_id = codec_id;

        c->bit_rate = 400000;
        /* Resolution must be a multiple of two. */
        c->width = width;
        c->height = height;
        /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
         * identical to 1. */
        ost->st->time_base = { 1, STREAM_FRAME_RATE };
        c->time_base = ost->st->time_base;

        c->gop_size = 12; /* emit one intra frame every twelve frames at most */
        c->pix_fmt = STREAM_PIX_FMT;
        if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B-frames */
            c->max_b_frames = 2;
        }
        if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             * This does not happen with normal video, it just happens here as
             * the motion of the chroma plane does not match the luma plane. */
            c->mb_decision = 2;
        }
        break;

    default:
        break;
    }

    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

static AVFrame* alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame* picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }

    return picture;
}

static void open_video(AVFormatContext* oc, AVCodec* codec, OutputStream* ost)
{
    int ret;
    AVCodecContext* c = ost->enc;

    /* open the codec */
    ret = avcodec_open2(c, codec, 0);
    if (ret < 0) {
        //fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
        printf("Could not open video codec");
        exit(1);
    }

    /* allocate and init a re-usable frame */
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        ost->tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }

    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}



static void fill_yuv_image(AVFrame* pict, int width, int height, BYTE* r, BYTE* g, BYTE* b)
{
    int x, y;
    int rr{ 100 }, gg{ 100 }, bb{ 100 };
     
    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = 
            (0.257 * rr) + (0.504 * gg) + (0.098 * bb) + 16;
    
    ///* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 
                -(0.148 * rr) - (0.291 * gg) + (0.439 * bb) + 128;
            pict->data[2][y * pict->linesize[2] + x] = 
                (0.439 * rr) - (0.368 * gg) - (0.071 * bb) + 128;
        }
    }


    ///* Y */
    //for (y = 0; y < height; y++)
    //    for (x = 0; x < width; x++)
    //        pict->data[0][y * pict->linesize[0] + x] =
    //        (0.257 * r[x + y * width]) + (0.504 * g[x + y * width]) + (0.098 * b[x + y * width]) + 16;
    //
    /////* Cb and Cr */
    //for (y = 0; y < height / 2; y++) {
    //    for (x = 0; x < width / 2; x++) {
    //        pict->data[1][y * pict->linesize[1] + x] =
    //            -(0.148 * r[x + y * width]) - (0.291 * g[x + y * width]) + (0.439 * b[x + y * width]) + 128;
    //        pict->data[2][y * pict->linesize[2] + x] =
    //            (0.439 * r[x + y * width]) - (0.368 * g[x + y * width]) - (0.071 * b[x + y * width]) + 128;
    //    }
    //}
}

static AVFrame* get_video_frame(OutputStream* ost, BYTE* r, BYTE* g, BYTE* b)
{
    AVCodecContext* c = ost->enc;

    /* check if we want to generate more frames */
    if (av_compare_ts(ost->next_pts, c->time_base,
        STREAM_DURATION, { 1, 1 }) >= 0)
        return NULL;

    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally; make sure we do not overwrite it here */
    if (av_frame_make_writable(ost->frame) < 0)
        exit(1);

    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        /* as we only generate a YUV420P picture, we must convert it
         * to the codec pixel format if needed */
        if (!ost->sws_ctx) {
            ost->sws_ctx = sws_getContext(c->width, c->height,
                AV_PIX_FMT_YUV420P,
                c->width, c->height,
                c->pix_fmt,
                SCALE_FLAGS, NULL, NULL, NULL);
            if (!ost->sws_ctx) {
                fprintf(stderr,
                    "Could not initialize the conversion context\n");
                exit(1);
            }
        }
        fill_yuv_image(ost->tmp_frame, c->width, c->height, r, g, b);
        sws_scale(ost->sws_ctx, (const uint8_t* const*)ost->tmp_frame->data,
            ost->tmp_frame->linesize, 0, c->height, ost->frame->data,
            ost->frame->linesize);
    }
    else {
        fill_yuv_image(ost->frame, c->width, c->height, r, g, b);
    }

    ost->frame->pts = ost->next_pts++;

    return ost->frame;
}

static int write_video_frame(AVFormatContext* oc, OutputStream* ost, BYTE* r, BYTE* g, BYTE* b)
{
    int ret;
    AVCodecContext* c;
    AVFrame* frame;
    int got_packet = 0;
    AVPacket pkt = { 0 };

    c = ost->enc;

    frame = get_video_frame(ost, r, g, b);

    av_init_packet(&pkt);

    /* encode the image */
    ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
    if (ret < 0) {
        //fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
        printf("Error encoding video frame");
        exit(1);
    }

    if (got_packet) {
        ret = write_frame(oc, &c->time_base, ost->st, &pkt);
    }
    else {
        ret = 0;
    }

    if (ret < 0) {
        //fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
        printf("Error while writing video frame");
        exit(1);
    }

    return (frame || got_packet) ? 0 : 1;
}

static void close_stream(AVFormatContext* oc, OutputStream* ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
    sws_freeContext(ost->sws_ctx);
    swr_free(&ost->swr_ctx);
}



int makeMP4video(const char* filename, int width, int height, BYTE* r, BYTE* g, BYTE* b)
{
    OutputStream video_st = { 0 };
    AVOutputFormat* fmt;
    AVFormatContext* oc;
    AVCodec* video_codec{ 0 };
    int ret;
    int encode_video = 0;

     avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;

    fmt = oc->oformat;

    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        add_stream(width, height, &video_st, oc, &video_codec, fmt->video_codec);
        encode_video = 1;
    }

    open_video(oc, video_codec, &video_st);
    av_dump_format(oc, 0, filename, 1);

    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            printf("Could not open filename");
            return 1;
        }
    }

    ret = avformat_write_header(oc, 0);
    if (ret < 0) {
        printf("Error occurred when opening output file");
        return 1;
    }

    while (encode_video) {
        if (
            encode_video &&
            (av_compare_ts(video_st.next_pts, video_st.enc->time_base,
                0, AVRational({ 0,0 })) <= 0))
        {
            encode_video = !write_video_frame(oc, &video_st, r, g, b);
        }
    }

    av_write_trailer(oc);
    close_stream(oc, &video_st);
    if (!(fmt->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb);
    avformat_free_context(oc);

    return 0;
}
