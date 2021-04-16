#include "pch.h"
#include "DebugLog.h"
#include <wx/clrpicker.h>
#include <wx/event.h>

#define ID_LASER_COLOUR		2000
#define ID_MESH_COLOUR		2001
#define ID_LASER_SLIDER		2002
#define ID_MESH_SLIDER		2003
#define ID_LASER_SPEED		2004
#define ID_MESH_SPEED		2005
#define ID_REFLECTIONS		2006

namespace SH
{
	GLuint Vertex;
	GLuint Fragment;
	GLuint UniColor;
};
namespace PR
{
	GLuint Simple;
	GLuint UniColorLaser;
	GLuint UniColorMesh;
};
namespace DB
{
	TypeCallback wxSimple = [](std::string str, const char* file, int line) { wxLogError(str.c_str()); };
};



class MyFrame : public wxFrame
{
public:
    MyFrame();
	wxGLCanvas* canvas;
	wxGLContext* context;
	wxBoxSizer* topsizer;
	bool Resized;
	
	struct ControlVariables
	{
		glm::mat4 cam;
		float Lspeed{5}; // Laser speed
		float Mspeed{5}; // Mash speed
		int Reflections{3};
	} CTLV;
	
	
	struct Control
	{
		wxColourPickerCtrl* LaserColour;
		wxColourPickerCtrl* MeshColour;
		wxSlider* LaserSlider;
		wxSlider* MeshSlider;
		wxTextCtrl* LaserRotation[3];
		wxTextCtrl* LaserSPEED;
		wxTextCtrl* MeshSPEED;
		wxTextCtrl* Reflections;
	} CTL;
	struct Sizer
	{
		wxBoxSizer* Gui = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* Main = new wxBoxSizer(wxHORIZONTAL);
	} SIZER;
	
	std::vector<glm::vec3> box{
		{-1,1,-1},{1,1,-1},{1,1,1},{-1,1,1},
		{-1,-1,-1},{1,-1,-1},{1,-1,1},{-1,-1,1},
	};
	std::vector<unsigned int> boxIndex{
		0,1,5,6,2,3,7,4,5,1,2,6,7,3,0,4
	};
	
private:
    void OnExit(wxCommandEvent& event);
	void OnRender(wxPaintEvent& evt);
	void OnSize(wxSizeEvent& evt);
	
	void OnLaserColour(wxColourPickerEvent& evt);
	void OnLaserSlider(wxScrollEvent& evt);
	void OnMeshColour(wxColourPickerEvent& evt);
	void OnMeshSlider(wxScrollEvent& evt);
	void OnLaserRotation(wxCommandEvent& evt);
	void OnLaserSPEED(wxCommandEvent& evt);
	void OnMeshSPEED(wxCommandEvent& evt);
	void OnReflections(wxCommandEvent& evt);
	
	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_PAINT(MyFrame::OnRender)
EVT_SIZE(MyFrame::OnSize)
//EVT_COMMAND_ENTER(ID_MESH_SPEED, MyFrame::OnMeshSPEED)
wxEND_EVENT_TABLE()




class MyApp : public wxApp
{
public:
    virtual bool OnInit();
	MyFrame* frame;
	void OnKeyDown(wxKeyEvent& evt);
	
private:
	wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(MyApp, wxApp)
EVT_KEY_DOWN(MyApp::OnKeyDown)
wxEND_EVENT_TABLE()

