#include "src/gui.h"
#include "src/openGL.h"
namespace fs = std::filesystem;



class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};



class Canvas : public wxGLCanvas
{
public:
    Canvas(wxFrame* parent);
	wxGLContext* context;
	objl::Loader file;
private:
	void render(wxPaintEvent& evt);
	void OnResize(wxSizeEvent& event);
	
	wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
EVT_PAINT(Canvas::render)
EVT_SIZE(Canvas::OnResize)
wxEND_EVENT_TABLE()



class MyFrame : public wxFrame
{
public:
    MyFrame();
	Canvas* canvas;
private:
    void OnExit(wxCommandEvent& event);
};



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
