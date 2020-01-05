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
	wxBoxSizer* canvas_GUIsizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* GUIsizer = new wxBoxSizer(wxVERTICAL);

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
		// lazer colorPick
		{
			GUIsizer->AddSpacer(10);
			laserCOLOUR = new wxColourPickerCtrl(this, ID_LASER_COLOUR);
			laserCOLOUR->SetColour(wxColour(canvas->Lcolour.x * 255, 0, 0));
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "Lazer colour:"));
			GUIsizer->Add(laserCOLOUR);
			GUIsizer->AddSpacer(10);
			
			laserSlider = new wxSlider(this, ID_LASER_SLIDER, 255, 0, 255);
			GUIsizer->Add(laserSlider);
			GUIsizer->AddSpacer(10);

			Bind(wxEVT_COLOURPICKER_CHANGED, &Frame::ONlaserCOLOUR, this, ID_LASER_COLOUR);
			Bind(wxEVT_SCROLL_THUMBTRACK, &Frame::ONlaserSlider, this, ID_LASER_SLIDER);
		}

		// mesh colorPick
		{
			meshCOLOUR = new wxColourPickerCtrl(this, ID_MESH_COLOUR);
			meshCOLOUR->SetColour(wxColour(canvas->Mcolour.x * 255, canvas->Mcolour.y * 255, canvas->Mcolour.z * 255));
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "Mesh colour:"));
			GUIsizer->Add(meshCOLOUR);
			GUIsizer->AddSpacer(10);

			meshSlider = new wxSlider(this, ID_MESH_SLIDER, 255/2, 0, 255);
			GUIsizer->Add(meshSlider);
			GUIsizer->AddSpacer(10);

			Bind(wxEVT_COLOURPICKER_CHANGED, &Frame::ONmeshCOLOUR, this, ID_MESH_COLOUR);
			Bind(wxEVT_SCROLL_THUMBTRACK, &Frame::ONmeshSlider, this, ID_MESH_SLIDER);
		}

		// lazer rotation
		{
			wxBoxSizer* s[3] = {
				new wxBoxSizer(wxHORIZONTAL),
				new wxBoxSizer(wxHORIZONTAL),
				new wxBoxSizer(wxHORIZONTAL)
			};
			Lrotation[0] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			Lrotation[1] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			Lrotation[2] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			s[0]->Add(Lrotation[0]); s[0]->AddSpacer(10); s[0]->Add(new wxStaticText(this, wxID_ANY, "x"));
			s[1]->Add(Lrotation[1]); s[1]->AddSpacer(10); s[1]->Add(new wxStaticText(this, wxID_ANY, "y"));
			s[2]->Add(Lrotation[2]); s[2]->AddSpacer(10); s[2]->Add(new wxStaticText(this, wxID_ANY, "z"));
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "Lazer rotation:"));
			GUIsizer->Add(s[0]);
			GUIsizer->Add(s[1]);
			GUIsizer->Add(s[2]);
			GUIsizer->AddSpacer(10);
		}

		// speed and reflections
		{
			laserSPEED = new wxTextCtrl(this, wxID_ANY);
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "laser speed:"));
			GUIsizer->Add(laserSPEED);
			GUIsizer->AddSpacer(10);

			reflections = new wxTextCtrl(this, wxID_ANY);
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "Reflections:"));
			GUIsizer->Add(reflections);
			GUIsizer->AddSpacer(10);
		}


		bigTEXT = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(150,0), wxTE_MULTILINE);
		GUIsizer->Add(bigTEXT, 1, 1);
		bigTEXT->SetBackgroundColour({ 200,200,200 });

		GLversion = new wxTextCtrl(this, 0, glGetString(GL_VERSION));
		freeTEXT = new wxTextCtrl(this, 0, "POOP");

		GLversion->SetBackgroundColour({ 150,150,150 });
		freeTEXT->SetBackgroundColour({ 150,150,150 });
		GLversion->SetMinSize({ GLversion->GetSize().x + 100, GLversion->GetSize().y });
	}

	// init sizers
	{
		canvas_GUIsizer->Add(canvas, 1, wxEXPAND);
		canvas_GUIsizer->Add(GUIsizer, 0, wxEXPAND);

		statusSIZER->Add(GLversion);
		statusSIZER->Add(freeTEXT, 1, wxEXPAND);

		mainSIZER->Add(canvas_GUIsizer, 1, wxEXPAND);
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
void Frame::ONlaserCOLOUR(wxColourPickerEvent& event)
{
	wxColour c = laserCOLOUR->GetColour();
	float r = (float)c.Red() / 255;
	float g = (float)c.Green() / 255;
	float b = (float)c.Blue() / 255;
	this->canvas->Lcolour = vec4{ r,g,b,canvas->Lcolour.w };
	canvas->Refresh();
}
void Frame::ONmeshCOLOUR(wxColourPickerEvent& event)
{
	wxColour c = meshCOLOUR->GetColour();
	float r = (float)c.Red() / 255;
	float g = (float)c.Green() / 255;
	float b = (float)c.Blue() / 255;
	this->canvas->Mcolour = vec4{ r,g,b,canvas->Mcolour.w };
	canvas->Refresh();
}
void Frame::ONlaserSlider(wxScrollEvent& event)
{
	canvas->Lcolour.w = (float)laserSlider->GetValue() / 255;
	canvas->Refresh();
}
void Frame::ONmeshSlider(wxScrollEvent& event)
{
	canvas->Mcolour.w = (float)meshSlider->GetValue() / 255;
	canvas->Refresh();
}



GLcanvas::GLcanvas(wxWindow *parent, Frame* _frame, int *attribList)
	: wxGLCanvas(parent, wxID_ANY, attribList, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
	file("box.obj"),
	frame(_frame), 
	trans(1)
{
	for (int i = 0; i < file.indices.size(); i += 3)
	{
		Face face;
		face.p[0] = file.vertices[file.indices[i + 0]];
		face.p[1] = file.vertices[file.indices[i + 1]];
		face.p[2] = file.vertices[file.indices[i + 2]];
		face.l[0] = Line{ face.p[0],face.p[1], (face.p[0] - face.p[1]) };
		face.l[1] = Line{ face.p[0],face.p[2], (face.p[0] - face.p[2]) };
		face.l[2] = Line{ face.p[1],face.p[2], (face.p[1] - face.p[2]) };
		face.n = cross(face.p[1] - face.p[0], face.p[2] - face.p[0]);
		// face.n = { -face.n.x,-face.n.y,-face.n.z };
		face.init_Equation();

		faces.push_back(face);
	}

	for (auto& f: faces) {
		for (auto& ff : faces) {
			if (f != ff) 
			{
				for (int l = 0; l < 3; l++) {
					for (int ll = 0; ll < 3; ll++) 
					{
						if (f.l[l] == ff.l[ll]) {
							f.e.push_back({ { &f,&ff }, f.l[l], f.n + ff.n });
						}
					}
				}
			}
		}
	}

}

void GLcanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	vec3 pa;
	vec3 pb;
	vec3 intersect;
	Face face;
	int last = Ray::_INtriangle;
	int count{ 0 };

	//vector<vec3> lines{ pa };

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
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			programID = LoadShaders("shaders/vertex.shader", "shaders/fragment.shader");
			glUseProgram(programID);
		}

		// camera
		{
			mat4 camera(1);
			wxSize screenSIZE = this->GetSize();
			float sx = screenSIZE.x, sy = screenSIZE.y;

			float x = CscaleX + .09, y = CscaleY + .09, z = CscaleZ + .09;
			float r = sx / sy;
			x = x / r;

			scale(programID, camera, { x, y, z });
			rotate(programID, camera, CrotateX + 30, { 0,1,0 });
			rotate(programID, camera, CrotateY + 30, { 1,0,0 });
		}

		// lazer
		{
			mat4 lazer(1);
			vec4 pa4{ 0,0,0,0 }, pb4{ .5,0,0,0 };
			pb4 = pb4 * glm::rotate(lazer, float(radians(LrotateX)), vec3{ 0,0,1 });
			pb4 = pb4 * glm::rotate(lazer, float(radians(LrotateY)), vec3{ 0,1,0 });
			pa = vtv(pa4); pb = vtv(pb4);
		}

		// draw the scene
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			initOBJfile(file, Mcolour);
			glDrawElements(
				GL_TRIANGLES,
				file.indices.size(),
				GL_UNSIGNED_INT,
				(void*)0
			);

			//edge e = edges[7];
			//drawTriangle(e.face[0]->p[0], e.face[0]->p[1], e.face[0]->p[2], { 1,0,0,1 });
			//drawTriangle(e.face[1]->p[0], e.face[1]->p[1], e.face[1]->p[2], { 0,1,0,1 });
			//drawTriangle(e.line.a, e.line.b, { 0,0,0 }, { 0,0,1,1 });
		}
	}

	// ray tracer
	{
		for (int t = 0; t < 200; t++) {
			float l = MAX;
			for (auto f : faces) {

				vec3 _intersect = GETintersection(f.eplane, Eline{ pa, pb });

				if (sameDir(pa - pb, pa - _intersect))
				{
					int r = testRayTriangle(f.p[0], f.p[1], f.p[2], _intersect);
					if (r != Ray::_NOintersection) {
						float ll = length(pa - _intersect);
						if (ll < l) { l = ll; intersect = _intersect; face = f; }
						last = Ray::_INtriangle;
					}
					if (r == Ray::_Onedge) {
						float ll = length(pa - _intersect);
						if (ll < l) { l = ll; intersect = _intersect; }
						last = Ray::_Onedge;
					}
				}
			}

			drawLINE2(pa, intersect, Lcolour);
			//lines.push_back(intersect);

			if (last == Ray::_Onedge) {
				count++;

				vec3 n[2];
				for (int i = 0; i < 3; i++) {
					if (face.e[i].l.isONline(intersect)) {
						n[0] = face.e[i].face[0]->n;
						n[1] = face.e[i].face[1]->n;
					}
				}
				vec3 direction = reflect(n[0] + n[1], intersect - pa);
				pb = intersect + direction;
				pa = intersect;
			}
			if (last == Ray::_INtriangle) {
				count++;
				vec3 direction = reflect(face.n, intersect - pa);
				pb = intersect + direction;
				pa = intersect;
			}
		}
		qq(count);

	}

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
	float Cspeed = 5.0f;
	float Lspeed = .05f;

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

