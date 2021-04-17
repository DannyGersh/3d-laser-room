#include "pch.h"
#include "DebugLog.h"
#include "GLfunctions.h"

#define ID_LASER_COLOUR		2000
#define ID_MESH_COLOUR		2001
#define ID_LASER_SLIDER		2002
#define ID_MESH_SLIDER		2003
#define ID_LASER_SPEED		2004
#define ID_MESH_SPEED		2005
#define ID_REFLECTIONS		2006

namespace SH // shaders
{
	GLuint Vertex;
	GLuint Fragment;
	GLuint UniColor;
};
namespace PR // programs
{
	GLuint Simple;
	GLuint UniColorLaser;
	GLuint UniColorMesh;
};
namespace DB // DebugLog extension
{
	TypeCallback wxSimple = [](std::string str, const char* file, int line) { wxLogError(str.c_str()); };
};


namespace VAR // global variables
{
	glm::mat4 cam;
	float Lspeed{5}; // Laser speed
	float Mspeed{5}; // Mash speed
	glm::vec3 Rotation(0.,0.,0.);
	unsigned int Reflections{3};
	
	GLuint MeshVertexBuffer;
	GLuint MeshIndexBuffer;
	GLuint LaserVertexBuffer;
	
	std::vector<glm::vec3> box{
		{-1,1,-1},{1,1,-1},{1,1,1},{-1,1,1},
		{-1,-1,-1},{1,-1,-1},{1,-1,1},{-1,-1,1},
	};
	std::vector<unsigned int> boxIndex{
		0,1,5,6,2,3,7,4,5,1,2,6,7,3,0,4
	};
	
	std::vector<glm::vec3> Laser{{0,0,0},{1,1,0}};
};
namespace CTL // Controls
{
	wxColourPickerCtrl* LaserColour;
	wxColourPickerCtrl* MeshColour;
	wxSlider* LaserSlider;
	wxSlider* MeshSlider;
	wxTextCtrl* LaserRotation[3];
	wxTextCtrl* LaserSPEED;
	wxTextCtrl* MeshSPEED;
	wxTextCtrl* Reflections;
};



class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};


class BasicGLPane : public wxGLCanvas
{
public:
	wxGLContext* context;
	BasicGLPane(wxFrame* parent, int* args): wxGLCanvas(parent, wxID_ANY,  args, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas")) {};
	bool Resized = false;

public:
	void OnRender(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	
	DECLARE_EVENT_TABLE()
};
wxBEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_PAINT(BasicGLPane::OnRender)
EVT_SIZE(BasicGLPane::OnSize)
EVT_KEY_DOWN(BasicGLPane::OnKeyDown)
wxEND_EVENT_TABLE()




class MyFrame : public wxFrame
{
public:
    MyFrame();
	BasicGLPane* canvas;
private:	
	void OnLaserColour(wxColourPickerEvent& evt);
	void OnLaserSlider(wxScrollEvent& evt);
	void OnMeshColour(wxColourPickerEvent& evt);
	void OnMeshSlider(wxScrollEvent& evt);
	void OnLaserRotation(wxCommandEvent& evt);
	void OnLaserSPEED(wxCommandEvent& evt);
	void OnMeshSPEED(wxCommandEvent& evt);
	void OnReflections(wxCommandEvent& evt);
};


