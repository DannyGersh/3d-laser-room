#include "pch.h"
#include "DebugLog.h"

#define ID_LASER_COLOUR		2000
#define ID_MESH_COLOUR		2001
#define ID_LASER_SLIDER		2002
#define ID_MESH_SLIDER		2003
#define ID_LASER_SPEED		2004
#define ID_REFLECTIONS		2005

namespace SH
{
	GLuint Vertex;
	GLuint Fragment;
	GLuint UniColor;
};
namespace PR
{
	GLuint Program;
}

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
	void OnKey(wxKeyEvent& evt)
	{
		switch ( evt.GetKeyCode() )
		{
			case WXK_RETURN:
				exit(0);
		}
	}
	
private:
	wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(MyApp, wxApp)
EVT_KEY_UP(MyApp::OnKey)
wxEND_EVENT_TABLE()


class MyFrame : public wxFrame
{
public:
    MyFrame();
	wxGLCanvas* canvas;
	wxGLContext* context;
	wxBoxSizer* topsizer;
	bool Resized;
	glm::mat4 cam;
	
private:
    void OnExit(wxCommandEvent& event);
	void OnRender(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	//void OnKey(wxKeyEvent& evt);
	
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_PAINT(MyFrame::OnRender)
EVT_SIZE(MyFrame::OnSize)
wxEND_EVENT_TABLE()

