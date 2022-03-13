#include "src/gui.h"
#include "src/engine.h"
#define TIMER_ID 125156
typedef std::wstring WS;
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
	objl::Loader objFile;
	objl::Mesh box;
	glm::vec2 size;
	
	GLuint buffer;
	GLuint index_buffer;
	
	Camera cam;
	object o;
	object d;
	GLuint blockIndex ;
	//object box2;
	glm::vec3 pos = glm::vec3(0,0,0);
	glm::vec3 keyStatePos = glm::vec3(0,0,0);
	bool keyState[2] = { 0, 0 };
	wxTimer timer;
	
private:
	void render(wxPaintEvent& evt);
	void OnResize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
EVT_PAINT(Canvas::render)
EVT_SIZE(Canvas::OnResize)
EVT_TIMER(TIMER_ID, Canvas::OnTimer)
EVT_KEY_UP(Canvas::OnKeyUp)
EVT_KEY_DOWN(Canvas::OnKeyDown)
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
