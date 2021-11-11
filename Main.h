#include "src/gui.h"
#include "src/openGL.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
	wxGLCanvas* canvas;
	wxGLContext* context;
private:
    void OnExit(wxCommandEvent& event);
	void render(wxPaintEvent& evt);
	
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_PAINT(MyFrame::render)
wxEND_EVENT_TABLE()

