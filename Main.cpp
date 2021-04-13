#include "Main.h"
#include "GLfunctions.h"
using namespace std;

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
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
			DB::Log(ssm.str());
			
			GL::CheckError();
		}
		
		// shders, programs
		{
			SH::Vertex = GL::CompileShader("shaders/Vertex.shader", GL_VERTEX_SHADER);
			SH::Fragment = GL::CompileShader("shaders/Fragment.shader", GL_FRAGMENT_SHADER);
			SH::UniColor = GL::CompileShader("shaders/UniColor.shader", GL_FRAGMENT_SHADER);
			
			PR::Program = glCreateProgram();
			GL::LinkProgram(PR::Program, SH::Vertex, SH::Fragment);
			
			cam = glm::mat4(1);
			GL::Scale(PR::Program, cam, {1,1,1});
			
			GL::CheckError();
		}
	
	}

	topsizer = new wxBoxSizer( wxVERTICAL );
	
	topsizer->Add(
		canvas,
        1,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        10 ); 
	
	topsizer->Add(
        new wxTextCtrl( this, -1, "My text.", wxDefaultPosition, wxSize(100,60), wxTE_MULTILINE),
        0,            // make vertically stretchable
        wxEXPAND |    // make horizontally stretchable
        wxALL,        //   and make border all around
        10 ); 
	
	SetSizerAndFit(topsizer);
	
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
			if(size.x > size.y) glViewport(0,0,size.y, size.y);
			if(size.x < size.y) glViewport(0,0,size.x, size.x);			
			Resized = false;
		}
	}
	
	{
		glClearColor(0, 0, 0, 1 );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		GL::drawLINE(glm::vec3(0,0,0), glm::vec3(1,1,0), glm::vec4(0,1,0,1));
	}
	
	glFlush();
    canvas->SwapBuffers();
}
void MyFrame::OnSize(wxSizeEvent& evt)
{
	Resized = true;
	Refresh();
	evt.Skip();
}



