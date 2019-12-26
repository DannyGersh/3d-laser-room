#include <wx/wxprec.h>
#include "Main.h"




bool App::OnInit()
{
	Frame *frame = new Frame();
	frame->Show(true);
	return true;
}

Frame::Frame()
	: wxFrame(NULL, wxID_ANY, "Hello World")
{
	wxBoxSizer* mainSIZER = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* statusSIZER = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* canvas_textSIZER = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* textSIZER = new wxBoxSizer(wxVERTICAL);

	// gl stuff
	{
		canvas = new GLcanvas(this, this, NULL);
		wxGLContext* context = new wxGLContext(canvas);
		canvas->SetCurrent(*context);
		glewInit();
		GLerror();
	}

	// init ctrls
	{
		bigTEXT = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
		textSIZER->Add(bigTEXT, 1, 1);
		bigTEXT->SetBackgroundColour({ 200,200,200 });

		GLversion = new wxTextCtrl(this, 0, glGetString(GL_VERSION));
		freeTEXT = new wxTextCtrl(this, 0, "POOP");

		GLversion->SetBackgroundColour({ 150,150,150 });
		freeTEXT->SetBackgroundColour({ 150,150,150 });
		GLversion->SetMinSize({ GLversion->GetSize().x + 100, GLversion->GetSize().y });
	}

	// init sizers
	{
		canvas_textSIZER->Add(canvas, 1, wxEXPAND);
		canvas_textSIZER->Add(textSIZER, 0, wxEXPAND);

		statusSIZER->Add(GLversion);
		statusSIZER->Add(freeTEXT, 1, wxEXPAND);

		mainSIZER->Add(canvas_textSIZER, 1, wxEXPAND);
		mainSIZER->Add(statusSIZER, 0, wxEXPAND);

		SetSizerAndFit(mainSIZER);
	}

	SetPosition({ 50, 50 });
	SetClientSize(1000, 800);
	Show();
}

void Frame::OnExit(wxCommandEvent& event)
{
	SetMinSize({ 400, 400 });
	Close(true);
}


GLcanvas::GLcanvas(wxWindow *parent, Frame* _frame, int *attribList)
	: wxGLCanvas(parent, wxID_ANY, attribList, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
	file("C:/danny/blender/testOBJ.obj"),
	//file("C:/danny/blender/untitled.obj"),
	//file("C:/danny/blender/poop.obj"),
	frame(_frame),

	trans(1)
{
	for (int i = 0; i < file.indices.size(); i += 3)
	{
		face face;
		face.a = file.vertices[file.indices[i + 0]];
		face.b = file.vertices[file.indices[i + 1]];
		face.c = file.vertices[file.indices[i + 2]];
		faces.push_back(face);
	}
}

void GLcanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	vec3 pa;
	vec3 pb;
	vec3 oldDirection{ 0,0,0 };

	Ray ray;

	// stuff
	{
		// set context
		{
			begin = clock();

			context = new wxGLContext(this);
			SetCurrent(*context);
		}

		// set GL state
		{
			glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wire frame

			programID = LoadShaders("shaders/vertex.shader", "shaders/fragment.shader");
			glUseProgram(programID);
		}

		// camera
		{
			mat4 camera(1);
			scale(programID, camera, { CscaleX - 0.5,CscaleY - 0.5,CscaleZ - 0.5 });
			rotate(programID, camera, CrotateX + 30, { 0,1,0 });
			rotate(programID, camera, CrotateY + 30, { 1,0,0 });
			trans *= camera;
		}

		// line rotation
		{
			mat4 lazer(1);
			vec4 pa4{ 0,0,0,0 }, pb4{ -5,0,0,0 };
			pb4 = pb4 * glm::rotate(lazer, float(radians(LrotateX)), vec3{ 0,0,1 });
			pb4 = pb4 * glm::rotate(lazer, float(radians(LrotateY)), vec3{ 0,1,0 });
			pa = vtv(pa4); pb = vtv(pb4);

		}

		// draw the scene
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			initOBJfile(file);
			glDrawElements(
				GL_TRIANGLES,
				file.indices.size(),
				GL_UNSIGNED_INT,
				(void*)0
			);
		}
	}

	int poop = 0;
	for (int i = 0; i < 200; i++)
	{
		for (int f = 0; f < faces.size(); f++)
		{
			ray = wanWAYray(faces[f].a, faces[f].b, faces[f].c, pa, pb);

			if (ray.state == Ray::_INtriangle)
			{
				//q("IN");
				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				drawLINE(pa, ray.intersect, { r,g,b }, { g,r,b });
				pa = ray.intersect;
				pb = ray.intersect + ray.direction;
				
				oldDirection = ray.direction;
				poop++;
				break;
			}
			else if (ray.state == Ray::_Onedge || ray.state == Ray::_Onvertex)
			{
				q("EDGE");
				mat4 lazer(1);
				vec4 pb4 = vtv(pb);
				pb4 = pb4 * glm::rotate(lazer, float(radians(0.01f)), vec3{ 1,1,1 });
				pb = vtv(pb4);
				poop++;
				f--;
			}
			else
			{
				//q("NOOO");
			}
		}
	}
	
	q(poop);

	// swap buffor, cleanup
	{
		glFlush();
		SwapBuffers();

		GLerror();
		delete context;
		
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		frame->freeTEXT->SetValue(std::to_string(elapsed_secs));
		frame->bigTEXT->SetValue(str);
		str = "";
	}
}




void GLcanvas::OnKeyDown(wxKeyEvent& event)
{
	float Cspeed = 5;
	float Lspeed = .005;

	switch (event.GetKeyCode())
	{
	case WXK_RETURN:
		frame->Destroy();

	case WXK_LEFT:
		CrotateX += Cspeed;
		break;
	case WXK_RIGHT:
		CrotateX -= Cspeed;
		break;
	case WXK_UP:
		CrotateY += Cspeed;
		break;
	case WXK_DOWN:
		CrotateY -= Cspeed;
		break;

	case 'A':
		LrotateX -= Lspeed;
		break;
	case 'D':
		LrotateX += Lspeed;
		break;
	case 'W':
		LrotateY += Lspeed;
		break;
	case 'S':
		LrotateY -= Lspeed;
		break;

	default:
		event.Skip();
		return;
	}

	Refresh(false);

}

void GLcanvas::OnMouseWeel(wxMouseEvent& event)
{
	//wxMessageBox("POOP");
	if (event.GetWheelRotation() > 0)
	{
		CscaleX += .1f;
		CscaleY += .1f;
		CscaleZ += .1f;
		Refresh();
	}
	else
	{
		CscaleX -= .1f;
		CscaleY -= .1f;
		CscaleZ -= .1f;
		Refresh();
	}
}
