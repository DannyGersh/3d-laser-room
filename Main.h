#include "src/gui.h"
#include "src/openGL.h"
//#include "depend/debug.h"
#define WS const wchar_t*

namespace fs = std::filesystem;

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


namespace dir
{
	fs::directory_entry exe;
	fs::directory_entry res;
	fs::directory_entry shaders;
}
namespace file
{
	std::wstring box;
	std::wstring vertex;
	std::wstring unicolor_fragment;
}
namespace shad
{
	GLuint vertex;
	GLuint unicolorFrag;
}
namespace prog
{
	GLuint unicolor;
}
