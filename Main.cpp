#include <wx/wxprec.h>
#include "Main.h"


bool App::OnInit()
{
	Frame* frame = new Frame();
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
			laserCOLOUR->SetColour(wxColour(canvas->Lcolour.x * 255, canvas->Lcolour.y * 255, canvas->Lcolour.z * 255));
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

			meshSlider = new wxSlider(this, ID_MESH_SLIDER, 255 / 2, 0, 255);
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
			wxTextValidator* laserSPEED_validator = new wxTextValidator(wxFILTER_NUMERIC);
			laserSPEED = new wxTextCtrl(this, ID_LASER_SPEED, to_string(canvas->Lspeed), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *laserSPEED_validator);
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "laser speed:"));
			GUIsizer->Add(laserSPEED);
			GUIsizer->AddSpacer(10);

			wxTextValidator* reflection_validator = new wxTextValidator(wxFILTER_DIGITS);

			reflections = new wxTextCtrl(this, ID_REFLECTIONS, to_string(canvas->reflections), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *reflection_validator);
			GUIsizer->Add(new wxStaticText(this, wxID_ANY, "Reflections:"));
			GUIsizer->Add(reflections);
			GUIsizer->AddSpacer(10);

			Bind(wxEVT_TEXT_ENTER, &Frame::ONlaserSPEED, this, ID_LASER_SPEED);
			Bind(wxEVT_TEXT_ENTER, &Frame::ONreflections, this, ID_REFLECTIONS);
		}

		GLversion = new wxTextCtrl(this, 0, glGetString(GL_VERSION));
		freeTEXT = new wxTextCtrl(this, 0, "POOP");

		GUIsizer->Add(new wxStaticText(this, wxID_ANY, "Use arrow keys and w,a,s,d keys"));

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
	Maximize();
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
void Frame::ONreflections(wxCommandEvent& event)
{
	int _reflections = stoi(string(reflections->GetValue()));
	canvas->reflections = _reflections;
	canvas->Refresh();
}
void Frame::ONlaserSPEED(wxCommandEvent& event)
{
	canvas->Lspeed = atof(laserSPEED->GetValue().c_str());
	canvas->Refresh();
}


GLcanvas::GLcanvas(wxWindow* parent, Frame* _frame, int* attribList)
	: wxGLCanvas(parent, wxID_ANY, attribList, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
	file("box.obj"),
	frame(_frame),
	trans(1)
{
	for (unsigned i = 0; i < file.indices.size(); i += 3)
	{
		Face face;
		face.p[0] = file.vertices[file.indices[i + 0]];
		face.p[1] = file.vertices[file.indices[i + 1]];
		face.p[2] = file.vertices[file.indices[i + 2]];
		face.l[0] = Line{ face.p[0],face.p[1] };
		face.l[1] = Line{ face.p[0],face.p[2] };
		face.l[2] = Line{ face.p[1],face.p[2] };
		face.n = cross(face.p[1] - face.p[0], face.p[2] - face.p[0]);
		// face.n = { -face.n.x,-face.n.y,-face.n.z };
		face.init_Equation();

		faces.push_back(face);
	}

	for (auto& f : faces) {
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
	Line line;
	vec3 intersect;
	vec3 normal[2];
	int last = Ray::_INtriangle;
	int count{ 0 };

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

			SHAD_vertex = compileSHADER("shaders/vertex.shader", GL_VERTEX_SHADER);
			SHAD_uniCOLORfragment = compileSHADER("shaders/unicolor_fragment.shader", GL_FRAGMENT_SHADER);
			SHAD_multiCOLORfragment = compileSHADER("shaders/fragment.shader", GL_FRAGMENT_SHADER);
			ProgramID = glCreateProgram();
			linkPROGRAM(ProgramID, SHAD_vertex, SHAD_multiCOLORfragment);
		}

		// camera
		{
			camera = mat4(1);
			wxSize screenSIZE = this->GetSize();
			float sx = screenSIZE.x, sy = screenSIZE.y;

			float x = CscaleX + .09, y = CscaleY + .09, z = CscaleZ + .09;
			float r = sx / sy;
			x = x / r;

			scale(ProgramID, camera, { x, y, z });
			rotate(ProgramID, camera, Crotate.x, { 0,1,0 });
			rotate(ProgramID, camera, Crotate.y, { 1,0,0 });
		}

		// laser
		{
			mat4 lazer(1);
			vec4 pa4{ 0,0,0,0 }, pb4{ .5,0,0,0 };
			pb4 = pb4 * glm::rotate(lazer, float(radians(Lrotate.x)), vec3{ 0,0,1 });
			pb4 = pb4 * glm::rotate(lazer, float(radians(Lrotate.y)), vec3{ 0,1,0 });
			line.a = vtv(pa4); line.b = vtv(pb4);
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
		}

	}

	// ray tracer
	{
		for (int t = 0; t < reflections; t++) {
			float l = MAX;
			for (auto f : faces) {
				rayTRIANGLEintersect(line, intersect, normal, f, &last, l);
			}

			lines.push_back({ line.a,intersect });

			if (last == Ray::_INtriangle) {
				vec3 direction = reflect(normal[0], intersect - line.a);
				line.b = intersect + direction;
				line.a = intersect;
				//drawLINE2(line, { 1,1,1,1 });
			}
			else if (last == Ray::_Onedge) {
				// double hit scenario:
				// onley works when planes are <= 90 degrees to each other.
				vec3 direction = reflect(normal[0], intersect - line.a);
				line.b = intersect + direction;
				line.a = intersect;
				direction = reflect(normal[1], direction);
				line.b = intersect + direction;
				line.a = intersect;
				//drawLINE2(line, { 1,1,1,1 });
			}
		}
	}

	// draw laser
	{
		linkPROGRAM(ProgramID, SHAD_vertex, SHAD_uniCOLORfragment);
		sendUNIFORMdata(ProgramID, camera);
		set_uniCOLOR(ProgramID, Lcolour);

		drawLINEs(lines);
		lines.clear();
	}

	// swap buffor, cleanup
	{
		glFlush();
		SwapBuffers();

		GLerror();
		delete context;

		frame->Lrotation[0]->SetValue(to_string(Lrotate.x));
		frame->Lrotation[1]->SetValue(to_string(Lrotate.y));
		frame->Lrotation[2]->SetValue(to_string(Lrotate.z));

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		frame->freeTEXT->SetValue(std::to_string(elapsed_secs));
		str = "";
	}

}


void GLcanvas::OnKeyDown(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_RETURN:
		frame->Destroy();

	case WXK_LEFT:
		Crotate.x += Cspeed;
		break;
	case WXK_RIGHT:
		Crotate.x -= Cspeed;
		break;
	case WXK_UP:
		Crotate.y += Cspeed;
		break;
	case WXK_DOWN:
		Crotate.y -= Cspeed;
		break;

	case 'A':
		Lrotate.x -= Lspeed;
		break;
	case 'D':
		Lrotate.x += Lspeed;
		break;
	case 'W':
		Lrotate.y += Lspeed;
		break;
	case 'S':
		Lrotate.y -= Lspeed;
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

