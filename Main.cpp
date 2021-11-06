#include "Main.h"


wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "wx_gl")
{
    // init glCanvas, glContext
	{
		int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
		canvas = new wxGLCanvas(this, wxID_ANY,  args, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"));
		context = new wxGLContext(canvas);	
		context->SetCurrent(*canvas);
	}

	topsizer = new wxBoxSizer( wxVERTICAL );
	
	//topsizer->Add(
	//	canvas,
    //    1,            // make vertically stretchable
    //    wxEXPAND |    // make horizontally stretchable
    //    wxALL,        //   and make border all around
    //    10 ); 
	
	topsizer->Add(new wxTextCtrl(this, 4545845, "x", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 1, wxALL, 2);
	
	SetSizerAndFit(topsizer);

	Maximize();
	Show();
}
	
void MyFrame::render(wxPaintEvent& evt)
{
    canvas->SetCurrent(*context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
	
	// randomly change color of backgrownd via glClear
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glClearColor(r, g, b, 1.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	glFlush();
    canvas->SwapBuffers();
}



