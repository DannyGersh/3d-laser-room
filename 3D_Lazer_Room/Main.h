#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

#include "GL/glew.h"
#include <wx/wx.h>
#include "wx/glcanvas.h"
#include "wx/clrpicker.h"
#include "wx/slider.h"
#include <wx/regex.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OBJfile.h"

#include "rayTracer.h"
#include "GLfunctions.h"

#include "makeMP4video.h"

#define ID_LASER_COLOUR		2000
#define ID_MESH_COLOUR		2001
#define ID_LASER_SLIDER		2002
#define ID_MESH_SLIDER		2003
#define ID_LASER_SPEED		2004
#define ID_REFLECTIONS		2005
#define ID_RECORD_GIFF		2006



class GLcanvas;

float r, g, b;
void rnd_colors() {
	r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void GLerror()
{
	GLenum err = glGetError();

	switch (err)
	{

	case GL_NO_ERROR:
		break;

	case GL_INVALID_ENUM:
		wxLogError("GLenum argument out of range\n");
	case GL_INVALID_VALUE:
		wxLogError("Numeric argument out of range.\n");
	case GL_INVALID_OPERATION:
		wxLogError("Operation illegal in current state.\n");
	case GL_STACK_OVERFLOW:
		wxLogError("Function would cause a stack overflow.\n");
	case GL_STACK_UNDERFLOW:
		wxLogError("Function would cause a stack underflow.\n");
	case GL_OUT_OF_MEMORY:
		wxLogError("Not enough memory left to execute function.\n");
	}

}



class App : public wxApp
{
public:
	virtual bool OnInit();
};

class Frame : public wxFrame
{
public:
	Frame();
	GLcanvas* canvas;
	wxGLContext* context;

	wxTextCtrl* GLversion;
	wxTextCtrl* freeTEXT;
	wxTextCtrl* bigTEXT;

	wxColourPickerCtrl* laserCOLOUR;
	wxSlider* laserSlider;
	wxColourPickerCtrl* meshCOLOUR;
	wxSlider* meshSlider;

	wxTextCtrl* Lrotation[3];
	wxTextCtrl* reflections;
	wxTextCtrl* laserSPEED;

	wxButton* recordGIFF;
	bool ISrecording = false;

private:
	void OnExit(wxCommandEvent& event);
	void ONlaserCOLOUR(wxColourPickerEvent& event);
	void ONmeshCOLOUR(wxColourPickerEvent& event);
	void ONlaserSlider(wxScrollEvent& event);
	void ONmeshSlider(wxScrollEvent& event);
	void ONlaserROTATION(wxCommandEvent& event);
	void ONreflections(wxCommandEvent& event);
	void ONlaserSPEED(wxCommandEvent& event);
	void ONrecordGIFF(wxCommandEvent& event);

};

wxIMPLEMENT_APP(App);



class GLcanvas : public wxGLCanvas
{
public:
	GLcanvas(wxWindow* parent, Frame* frame, int* attribList = NULL);
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnMouseWeel(wxMouseEvent& event);

public:
	Frame* frame;
	wxGLContext* context;

	mat4 trans;
	clock_t begin;

	// GLuint programID;
	GLuint ProgramID;
	GLuint SHAD_vertex;
	GLuint SHAD_uniCOLORfragment;
	GLuint SHAD_multiCOLORfragment;
	mat4 cam_uniform;
	mat4 camera;

	// C = camera, L = lazer
	int reflections = 500;
	float Cspeed = 5.0f;
	float Lspeed = 0.01f;
	vec3 Crotate = { 0.0f, 0.0f, 0.0f };
	vec3 Lrotate = { 0.0f, 0.0f, 0.0f };
	float CscaleX{ .5 }, CscaleY{ .5 }, CscaleZ{ .5 };


public:
	OBJfile file;
	std::vector<Face> faces;
	std::vector<Line> lines;

	vec4 Lcolour{ 0,1,1,1 };
	vec4 Mcolour{ .3,.1,1,.5 };

	wxDECLARE_EVENT_TABLE();
};




wxBEGIN_EVENT_TABLE(GLcanvas, wxGLCanvas)
EVT_PAINT(GLcanvas::OnPaint)
EVT_KEY_DOWN(GLcanvas::OnKeyDown)
EVT_MOUSEWHEEL(GLcanvas::OnMouseWeel)
wxEND_EVENT_TABLE()


