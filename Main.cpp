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

	wxBoxSizer* sizerMain = new wxBoxSizer( wxHORIZONTAL );
	
	wxScrolledWindow* guiLaserWindow = new wxScrolledWindow(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
	wxBoxSizer* sizerGui = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerGui_lasers = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerGui_main = new wxBoxSizer(wxVERTICAL);
	
	sizerGui->Add(sizerGui_lasers, 1, wxEXPAND | wxALL, 10);
	sizerGui->Add(new wxButton(this, wxID_ANY, "poop"), 1, wxEXPAND | wxALL, 10);
	sizerGui->Add(sizerGui_main, 1, wxEXPAND | wxALL, 10);
	
	guiLaserWindow->SetSizerAndFit(sizerGui_lasers);
	guiLaserWindow->SetScrollRate(5,5);
	//sizerGui_lasers->FitInside(guiLaserWindow);
	
	sizerMain->Add(canvas, 1, wxEXPAND | wxALL, 10 ); 
	sizerMain->Add(guiLaserWindow, 1, wxEXPAND | wxRIGHT | wxUP | wxDOWN | wxBORDER_DOUBLE , 10 );	
	//sizerMain->Add(guiLaserWindow, wxSizerFlags().Expand().Border(wxALL, 10));
	
	LaserUnitGui* l1 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);	
	LaserUnitGui* l2 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);	
	LaserUnitGui* l3 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);	
	LaserUnitGui* l4 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);	
	LaserUnitGui* l5 = new LaserUnitGui(guiLaserWindow, sizerGui_lasers);	
	
	guiLaserWindow->FitInside();
	sizerGui_lasers->FitInside(guiLaserWindow);
	
	SetSizerAndFit(sizerMain);
	SetSize(100,100,500,500);
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



