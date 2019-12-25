#pragma once
#include <wx/wx.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "GL/glew.h"
#include "wx/glcanvas.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp> 

#include "OBJfile.h"
#include "rayTracer.h"
#include "GLfunctions.h"
#include <ctime>




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
	wxGLCanvas* canvas;
	wxGLContext* context;
	wxTextCtrl* GLversion;
	wxTextCtrl* freeTEXT;
	std::vector<wxTextCtrl*> textCTRLS;
	wxTextCtrl* bigTEXT;

private:
	void OnExit(wxCommandEvent& event);
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

	GLuint programID;
	mat4 trans;
	clock_t begin;

	// C = camera, L = lazer
	float CrotateX{ 0 }, CrotateY{ 0 }, CrotateZ{ 0 };
	float LrotateX{ 0 }, LrotateY{ 0 }, LrotateZ{ 0 };
	float CscaleX{ 1 }, CscaleY{ 1 }, CscaleZ{ 1 };

private:
	OBJfile file;
	struct face {
		glm::vec3 a, b, c;
	};
	std::vector<face> faces;

	wxDECLARE_EVENT_TABLE();
};




wxBEGIN_EVENT_TABLE(GLcanvas, wxGLCanvas)
EVT_PAINT(GLcanvas::OnPaint)
EVT_KEY_DOWN(GLcanvas::OnKeyDown)
EVT_MOUSEWHEEL(GLcanvas::OnMouseWeel)
wxEND_EVENT_TABLE() 