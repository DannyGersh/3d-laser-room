#include "Main.h"
using namespace std;

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}


MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
	// init canvas\context
	{
		int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
		canvas = new BasicGLPane(this, args);
		canvas->context = new wxGLContext(canvas);
		canvas->context->SetCurrent(*canvas);
	}
	
	// init glew
	{
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

	// init shders, programs
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
		
		VAR::cam = glm::mat4(1);
		GL::Scale(PR::UniColorLaser, VAR::cam, {.5,.5,.5}, "trans");
		GL::Rotate(PR::UniColorLaser, VAR::cam, 45, glm::vec3(1,1,1), "trans");
		GL::SetUniform(PR::UniColorMesh, VAR::cam, "trans");
		
		glEnable (GL_BLEND); 
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		GL::CheckError();
	}
	

	// init gui
	{
		wxBoxSizer* SizerMain = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* SizerGui = new wxBoxSizer(wxVERTICAL);
		
		// lazer colorPick
		{
			SizerGui->AddSpacer(10);
			CTL::LaserColour = new wxColourPickerCtrl(this, ID_LASER_COLOUR);
			CTL::LaserColour->SetColour(wxColour(81,192,191));
			
			GL::SetUniform(PR::UniColorLaser, glm::vec4(81./255., 192./255., 191./255., 1.), "inCOLOR");
			
			SizerGui->Add(new wxStaticText(this, wxID_ANY, "Lazer colour:"));
			SizerGui->Add(CTL::LaserColour);
			SizerGui->AddSpacer(10);
			
			CTL::LaserSlider = new wxSlider(this, ID_LASER_SLIDER, 255, 0, 255);
			SizerGui->Add(CTL::LaserSlider);
			SizerGui->AddSpacer(10);
		
			Bind(wxEVT_COLOURPICKER_CHANGED, &MyFrame::OnLaserColour, this, ID_LASER_COLOUR);
			Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::OnLaserSlider, this, ID_LASER_SLIDER);
		}
		
		// mesh colorPick
		{
			CTL::MeshColour = new wxColourPickerCtrl(this, ID_MESH_COLOUR);
			CTL::MeshColour->SetColour(wxColour(201, 147, 212, 255));
			
			GL::SetUniform(PR::UniColorMesh, glm::vec4(201./255., 147./255., 212./255., 1.), "inCOLOR");
		
			SizerGui->Add(new wxStaticText(this, wxID_ANY, "Mesh colour:"));
			SizerGui->Add(CTL::MeshColour);
			SizerGui->AddSpacer(10);
		
			CTL::MeshSlider = new wxSlider(this, ID_MESH_SLIDER, 255, 0, 255);
			SizerGui->Add(CTL::MeshSlider);
			SizerGui->AddSpacer(10);
		
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
			CTL::LaserRotation[0] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			CTL::LaserRotation[1] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			CTL::LaserRotation[2] = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
			s[0]->Add(CTL::LaserRotation[0]); s[0]->AddSpacer(10); s[0]->Add(new wxStaticText(this, wxID_ANY, "x"));
			s[1]->Add(CTL::LaserRotation[1]); s[1]->AddSpacer(10); s[1]->Add(new wxStaticText(this, wxID_ANY, "y"));
			s[2]->Add(CTL::LaserRotation[2]); s[2]->AddSpacer(10); s[2]->Add(new wxStaticText(this, wxID_ANY, "z"));
			SizerGui->Add(new wxStaticText(this, wxID_ANY, "Lazer rotation:"));
			SizerGui->Add(s[0]);
			SizerGui->Add(s[1]);
			SizerGui->Add(s[2]);
			SizerGui->AddSpacer(10);
		}
		
		// speed and Reflections
		{
			wxTextValidator* validator = new wxTextValidator(wxFILTER_NUMERIC);
			CTL::LaserSPEED = new wxTextCtrl(this, ID_LASER_SPEED, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *validator);
			SizerGui->Add(new wxStaticText(this, wxID_ANY, "laser speed:"));
			SizerGui->Add(CTL::LaserSPEED);

			CTL::MeshSPEED = new wxTextCtrl(this, ID_LASER_SPEED, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *validator);
			SizerGui->Add(new wxStaticText(this, wxID_ANY, "Mesh speed:"));
			SizerGui->Add(CTL::MeshSPEED);
			SizerGui->AddSpacer(10);

			CTL::Reflections = new wxTextCtrl(this, ID_REFLECTIONS, "POOP_1", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, *validator);
			SizerGui->Add(new wxStaticText(this, wxID_ANY, "CTL::Reflections:"));
			SizerGui->Add(CTL::Reflections);
			SizerGui->AddSpacer(10);
		
			Bind(wxEVT_TEXT_ENTER, &MyFrame::OnLaserSPEED, this, ID_LASER_SPEED);
			Bind(wxEVT_TEXT_ENTER, &MyFrame::OnMeshSPEED, this, ID_MESH_SPEED);
			Bind(wxEVT_TEXT_ENTER, &MyFrame::OnReflections, this, ID_REFLECTIONS);
		}
	
		SizerMain->Add(canvas, 1, wxEXPAND);
		SizerMain->Add(SizerGui, 0, wxEXPAND);
		SetSizerAndFit(SizerMain);
	}

	Show();
}

void BasicGLPane::OnRender(wxPaintEvent& evt)
{
	// init paint event_callback
	{
		SetCurrent(*context);
		wxPaintDC(this); 
		
		if(Resized == true)
		{
			wxSize size = GetSize();
			glViewport(0,0,size.x, size.y);
			//if(size.x > size.y) glViewport(0,0,size.y, size.y);
			//if(size.x < size.y) glViewport(0,0,size.x, size.x);			
			Resized = false;
		}
	}
	
	// Main
	{
		VAR::cam = glm::mat4(1);
		GL::Scale(PR::UniColorLaser, VAR::cam, {.5,.5,.5}, "trans");
		GL::Rotate(PR::UniColorMesh, VAR::cam, VAR::Rotation.x, glm::vec3(1,0,0), "trans");
		GL::Rotate(PR::UniColorMesh, VAR::cam, VAR::Rotation.y, glm::vec3(0,1,0), "trans");
		GL::Rotate(PR::UniColorMesh, VAR::cam, VAR::Rotation.z, glm::vec3(0,0,1), "trans");
		GL::SetUniform(PR::UniColorLaser, VAR::cam, "trans");
					
		glClearColor(0, 0, 0, 1 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(PR::UniColorLaser);
		GL::drawLINE(glm::vec3(0,0,0), glm::vec3(1,1,0), glm::vec4(0,1,0,1));
		glUseProgram(PR::UniColorMesh);
		GL::drawLINEsI(box, boxIndex, GL_LINE_STRIP);
	}
	
	glFlush();
    SwapBuffers();
	
	GL::CheckError();
	evt.Skip();
};
void BasicGLPane::OnSize(wxSizeEvent& evt)
{
	Resized = true;
	Refresh();
	evt.Skip();
}
void BasicGLPane::OnKeyDown(wxKeyEvent& evt)
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
					VAR::Rotation.y += VAR::Mspeed;
					Refresh();
					evt.Skip();
					break;
				};
				case 'D': 
				{
					VAR::Rotation.y -= VAR::Mspeed;
					Refresh();
					evt.Skip();
					break;
				};
				case 'W': 
				{
					VAR::Rotation.x += VAR::Mspeed;
					Refresh();
					evt.Skip();
					break;
				};
				case 'S': 
				{
					VAR::Rotation.x -= VAR::Mspeed;
					Refresh();
					evt.Skip();
					break;
				};
				default:
					evt.Skip();
					return;
			}
		}
		else
		{
			switch (evt.GetKeyCode())
			{
				//case WXK_LEFT:
				//case WXK_RIGHT:
				//	break;
				
				//case WXK_RETURN:
				//	exit(0);
				default:
					evt.Skip();
					return;
			}
		}
	}

}



void MyFrame::OnLaserColour(wxColourPickerEvent& evt) 
{
	wxColour c = CTL::LaserColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL::LaserSlider->GetValue() / 255.;
	GL::SetUniform(PR::UniColorLaser, glm::vec4(r,g,b,a), "inCOLOR");
	Refresh();
	evt.Skip();
}; 
void MyFrame::OnLaserSlider(wxScrollEvent& evt)
{
	wxColour c = CTL::LaserColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL::LaserSlider->GetValue() / 255.;
	GL::SetUniform(PR::UniColorLaser, glm::vec4(r,g,b,a), "inCOLOR");
	canvas->Refresh();
	evt.Skip();
};
void MyFrame::OnMeshColour(wxColourPickerEvent& evt) 
{
	wxColour c = CTL::MeshColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL::LaserSlider->GetValue() / 255.;
	CTL::MeshColour->SetColour(wxColour(c.Red(),c.Green(),c.Blue(),a*255));
	GL::SetUniform(PR::UniColorMesh, glm::vec4(r,g,b,a), "inCOLOR");
	canvas->Refresh();
	evt.Skip();
};
void MyFrame::OnMeshSlider(wxScrollEvent& evt)
{
	wxColour c = CTL::MeshColour->GetColour();
	float r = (float)c.Red() / 255.;
	float g = (float)c.Green() / 255.;
	float b = (float)c.Blue() / 255.;
	float a = CTL::MeshSlider->GetValue() / 255.;
	GL::SetUniform(PR::UniColorMesh, glm::vec4(r,g,b,a), "inCOLOR");
	canvas->Refresh();
	evt.Skip();
};
void MyFrame::OnLaserRotation(wxCommandEvent& evt)
{
	evt.Skip();
};
void MyFrame::OnLaserSPEED(wxCommandEvent& evt)
{
	VAR::Lspeed = atof(CTL::LaserSPEED->GetValue().c_str());
	evt.Skip();
};
void MyFrame::OnMeshSPEED(wxCommandEvent& evt)
{
	VAR::Mspeed = atof(CTL::MeshSPEED->GetValue().c_str());
	evt.Skip();
};
void MyFrame::OnReflections(wxCommandEvent& evt)
{
	DB::LogF(DB::wxSimple, "POOP");
	VAR::Reflections = atof(CTL::MeshSPEED->GetValue().c_str());
	canvas->Refresh();
	evt.Skip();
};




