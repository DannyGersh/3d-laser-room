#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <wx/wx.h>
#include "GL/glew.h"
#include "wx/glcanvas.h"
#include "wx/clrpicker.h"
#include "wx/slider.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OBJfile.h"
#include "rayTracer.h"
#include "GLfunctions.h"
#include <ctime>


#define ID_LASER_COLOUR		2000
#define ID_MESH_COLOUR		2001
#define ID_LASER_SLIDER		2002
#define ID_MESH_SLIDER		2003

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
	std::vector<wxTextCtrl*> textCTRLS;
	wxTextCtrl* bigTEXT;

	wxColourPickerCtrl* laserCOLOUR;
	wxColourPickerCtrl* meshCOLOUR;
	wxSlider* laserSlider;
	wxSlider* meshSlider;

	wxTextCtrl* Lrotation[3];
	wxTextCtrl* reflections;
	wxTextCtrl* laserSPEED;

	bool finSETUP{ 0 };

private:
	void OnExit(wxCommandEvent& event);
	void ONlaserCOLOUR(wxColourPickerEvent& event);
	void ONmeshCOLOUR(wxColourPickerEvent& event);
	void ONlaserSlider(wxScrollEvent& event);
	void ONmeshSlider(wxScrollEvent& event);

};

wxIMPLEMENT_APP(App);



class GLcanvas : public wxGLCanvas
{
public:
	GLcanvas(wxWindow *parent, Frame* frame, int *attribList = NULL);
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnMouseWeel(wxMouseEvent& event);

private:
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
	float CrotateX{ 0 }, CrotateY{ 0 }, CrotateZ{ 0 };
	float LrotateX{ 0 }, LrotateY{ 0 }, LrotateZ{ 0 };
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


