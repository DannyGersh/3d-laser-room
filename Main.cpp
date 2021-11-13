#include "Main.h"

#ifdef NDEBUG 
wxIMPLEMENT_APP(MyApp);
#else
wxIMPLEMENT_APP_CONSOLE(MyApp);
#endif

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "wx_gl")
{
    // initialization - glCanvas, glContext, glew
	{
		int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
		canvas = new wxGLCanvas(this, wxID_ANY,  args, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"));
		context = new wxGLContext(canvas);	
		context->SetCurrent(*canvas);
		
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::string errStr1 = "Could not initialize glew:\n";
			std::string errStr2 = errStr1 + reinterpret_cast<const char*>(glewGetErrorString(err));
			wxMessageBox(errStr2 , "Error", wxICON_ERROR | wxOK);
			exit(-1);
		}
	
	}
	
	compileSHADER("res/shaders/vertex.shader", GL_VERTEX_SHADER);
	//
	Gui* gui = new Gui(this, canvas);
	
	{
		objl::Loader file;
		file.LoadFile("res/box.obj");
	}
	
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


