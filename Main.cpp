#include "Main.h"
#include "GLfunctions.h"
#include <wx/clrpicker.h>
using namespace std;

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "wx_gl")
{
    // init 
	{
		// glCanvas, glContext, glew
		{
			int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
			canvas = new wxGLCanvas(this, wxID_ANY,  args, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"));
			context = new wxGLContext(canvas);	
			context->SetCurrent(*canvas);
			
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				stringstream ssm; ssm<<"ERROR: glewInit() faild: "<<glewGetErrorString(err);
				DB::LogT(ssm.str());
				assert(GLEW_OK == err);
			}
			else fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
			
			stringstream ssm;
			ssm<<
				"GL_VENDOR: "<<
				glGetString(GL_VENDOR)<<"\n"<<
				"GL_RENDERER: "<<
				glGetString(GL_RENDERER)<<"\n"<<
				"GL_VERSION: "<<
				glGetString(GL_VERSION)<<"\n";
			DB::LogF(DB::Simple, ssm.str());

			GL::CheckError();
		}
		
		// shders, programs
		{
			SH::Vertex = GL::CompileShader("shaders/Vertex.shader", GL_VERTEX_SHADER);
			SH::Fragment = GL::CompileShader("shaders/Fragment.shader", GL_FRAGMENT_SHADER);
			SH::UniColor = GL::CompileShader("shaders/UniColor.shader", GL_FRAGMENT_SHADER);
			
			PR::UniColorLaser = glCreateProgram();
			PR::UniColorMesh = glCreateProgram();
			GL::LinkProgram(PR::UniColorLaser, SH::Vertex, SH::UniColor);
			GL::LinkProgram(PR::UniColorMesh, SH::Vertex, SH::UniColor);
			glUseProgram(PR::UniColorLaser);
			
			glDeleteShader(SH::Vertex);
			glDeleteShader(SH::Fragment);
			glDeleteShader(SH::UniColor);
			
			cam = glm::mat4(1);
			cam2 = glm::mat4(1);
			GL::Scale(PR::UniColorLaser, cam, {.5,.5,.5}, "trans");
			GL::Rotate(PR::UniColorLaser, cam, 45, glm::vec3(1,1,1));
			GL::SetUniform(PR::UniColorMesh, cam, "trans");
			
			glEnable (GL_BLEND); 
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			GL::CheckError();
		}
	
	}

	// init ctrls
	{
		// lazer colorPick
		{
			SIZER.Gui->AddSpacer(10);
			CTL.LaserColour = new wxColourPickerCtrl(this, ID_LASER_COLOUR);
			CTL.LaserColour->SetColour(wxColour(81,192,191));
			
			GL::SetUniform(PR::UniColorLaser, glm::vec4(81./255., 192./255., 191./255., 1.), "inCOLOR");
			
			SIZER.Gui->Add(new wxStaticText(this, wxID_ANY, "Lazer colour:"));
			SIZER.Gui->Add(CTL.LaserColour);
			SIZER.Gui->AddSpacer(10);

			CTL.LaserSlider = new wxSlider(this, ID_LASER_SLIDER, 255, 0, 255);
			SIZER.Gui->Add(CTL.LaserSlider);
			SIZER.Gui->AddSpacer(10);

			Bind(wxEVT_COLOURPICKER_CHANGED, &MyFrame::OnLaserColour, this, ID_LASER_COLOUR);
			Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::OnLaserSlider, this, ID_LASER_SLIDER);
		}

		// mesh colorPick
		{
			CTL.MeshColour = new wxColourPickerCtrl(this, ID_MESH_COLOUR);
			CTL.MeshColour->SetColour(wxColour(201, 147, 212, 255));
			
			GL::SetUniform(PR::UniColorMesh, glm::vec4(201./255., 147./255., 212./255., 1.), "inCOLOR");

			SIZER.Gui->Add(new wxStaticText(this, wxID_ANY, "Mesh colour:"));
			SIZER.Gui->Add(CTL.MeshColour);
			SIZER.Gui->AddSpacer(10);

			CTL.MeshSlider = new wxSlider(this, ID_MESH_SLIDER, 255, 0, 255);
			SIZER.Gui->Add(CTL.MeshSlider);
			SIZER.Gui->AddSpacer(10);

			Bind(wxEVT_COLOURPICKER_CHANGED, &MyFrame::OnMeshColour, this, ID_MESH_COLOUR);
			Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::OnMeshSlider, this, ID_MESH_SLIDER);
		}

		// lazer rotation
		{
			wxBoxSizer* s[3] = {
				new wxBoxSizer(wxHORIZONTAL),
				new wxBoxSizer(wxHORIZONTAL),
				new wxBoxSizer(wxHORIZONTAL)
			};
			CTL.LaserRotation[0] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			CTL.LaserRotation[1] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			CTL.LaserRotation[2] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			s[0]->Add(CTL.LaserRotation[0]); s[0]->AddSpacer(10); s[0]->Add(new wxStaticText(this, wxID_ANY, "x"));
			s[1]->Add(CTL.LaserRotation[1]); s[1]->AddSpacer(10); s[1]->Add(new wxStaticText(this, wxID_ANY, "y"));
			s[2]->Add(CTL.LaserRotation[2]); s[2]->AddSpacer(10); s[2]->Add(new wxStaticText(this, wxID_ANY, "z"));
			SIZER.Gui->Add(new wxStaticText(this, wxID_ANY, "Lazer rotation:"));
			SIZER.Gui->Add(s[0]);
			SIZER.Gui->Add(s[1]);
			SIZER.Gui->Add(s[2]);
			SIZER.Gui->AddSpacer(10);
		}

		// speed and Reflections
		{
			wxTextValidator* LaserSPEED_validator = new wxTextValidator(wxFILTER_NUMERIC);
			CTL.LaserSPEED = new wxTextCtrl(this, ID_LASER_SPEED, "POOP_2", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *LaserSPEED_validator);
			SIZER.Gui->Add(new wxStaticText(this, wxID_ANY, "laser speed:"));
			SIZER.Gui->Add(CTL.LaserSPEED);
			SIZER.Gui->AddSpacer(10);

			wxTextValidator* reflection_validator = new wxTextValidator(wxFILTER_DIGITS);

			CTL.Reflections = new wxTextCtrl(this, ID_REFLECTIONS, "POOP_1", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *reflection_validator);
			SIZER.Gui->Add(new wxStaticText(this, wxID_ANY, "CTL.Reflections:"));
			SIZER.Gui->Add(CTL.Reflections);
			SIZER.Gui->AddSpacer(10);

			Bind(wxEVT_TEXT_ENTER, &MyFrame::OnLaserSPEED, this, ID_LASER_SPEED);
			Bind(wxEVT_TEXT_ENTER, &MyFrame::OnReflections, this, ID_REFLECTIONS);
		}

	}

	// init sizers
	{
		SIZER.Main->Add(canvas, 1, wxEXPAND);
		SIZER.Main->Add(SIZER.Gui, 0, wxEXPAND);
		SetSizerAndFit(SIZER.Main);
	}

	SetSizerAndFit(SIZER.Main);
	Maximize();
	Show();
}
	
void MyFrame::OnRender(wxPaintEvent& evt)
{
    // init paint event_callback
	{
		canvas->SetCurrent(*context);
		wxPaintDC(this); 
		
		if(Resized == true)
		{
			wxSize size = canvas->GetSize();
			glViewport(0,0,size.x, size.y);
			//if(size.x > size.y) glViewport(0,0,size.y, size.y);
			//if(size.x < size.y) glViewport(0,0,size.x, size.x);			
			Resized = false;
		}
	}
	
	// Main
	{
		glClearColor(0, 0, 0, 1 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(PR::UniColorLaser);
		GL::drawLINE(glm::vec3(0,0,0), glm::vec3(1,1,0), glm::vec4(0,1,0,1));
		glUseProgram(PR::UniColorMesh);
		GL::drawLINEsI(box, boxIndex, GL_LINE_STRIP);
	}
	
	glFlush();
    canvas->SwapBuffers();
	
	GL::CheckError();
}
void MyFrame::OnSize(wxSizeEvent& evt)
{
	Resized = true;
	Refresh();
	evt.Skip();
}
void MyFrame::OnLaserColour(wxColourPickerEvent& evt) 
{
	wxColour c = CTL.LaserColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL.LaserSlider->GetValue() / 255.;
	GL::SetUniform(PR::UniColorLaser, glm::vec4(r,g,b,a), "inCOLOR");
	Refresh();
}; 
void MyFrame::OnLaserSlider(wxScrollEvent& evt)
{
	wxColour c = CTL.LaserColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL.LaserSlider->GetValue() / 255.;
	GL::SetUniform(PR::UniColorLaser, glm::vec4(r,g,b,a), "inCOLOR");
	Refresh();
};
void MyFrame::OnMeshColour(wxColourPickerEvent& evt) 
{
	wxColour c = CTL.MeshColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL.LaserSlider->GetValue() / 255.;
	CTL.MeshColour->SetColour(wxColour(c.Red(),c.Green(),c.Blue(),a*255));
	GL::SetUniform(PR::UniColorMesh, glm::vec4(r,g,b,a), "inCOLOR");
	Refresh();
};
void MyFrame::OnMeshSlider(wxScrollEvent& evt)
{
	wxColour c = CTL.MeshColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL.MeshSlider->GetValue() / 255.;
	GL::SetUniform(PR::UniColorMesh, glm::vec4(r,g,b,a), "inCOLOR");
	Refresh();
};
void MyFrame::OnLaserRotation(wxCommandEvent& evt)
{
	
};
void MyFrame::OnLaserSPEED(wxCommandEvent& evt) {};
void MyFrame::OnReflections(wxCommandEvent& evt) {};



void MyApp::OnKey(wxKeyEvent& evt)
{
		
	wxChar uc = evt.GetUnicodeKey();
	if (uc != WXK_NONE)
	{
		if ( uc >= 32 )
		{
			switch(uc)
			{
				case 'A': 
				{
					GL::Rotate(PR::UniColorMesh, frame->cam, 20, glm::vec3(1,1,1));
					GL::SetUniform(PR::UniColorLaser, frame->cam, "trans");
					frame->Refresh();
					evt.Skip();
				};
			}
		}
		else
		{
			switch (evt.GetKeyCode())
			{
				//case WXK_LEFT:
				//case WXK_RIGHT:
				//	break;
				
				case WXK_RETURN:
					exit(0);
			}
		}
	}

}

