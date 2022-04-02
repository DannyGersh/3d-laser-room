#include "Main.h"
#include <math.h>
float poop = 0;
float poop2 = 0;

#ifdef NDEBUG 
wxIMPLEMENT_APP(MyApp);
#else
wxIMPLEMENT_APP_CONSOLE(MyApp);
#endif

bool MyApp::OnInit()
{	
	// debug
	{
		
	#ifdef NDEBUG 
	debug::db = [](debug::Data d) 
	{ 
		wxMessageBox(d.msg, d.line, d.file);
		if( d.level == debug::kritical ) exit(-1); 
	};	
	#else
	debug::db = [](debug::Data d) 
	{ 
		#ifdef USE_STACKTRACE
		std::string a = boost::stacktrace::to_string(boost::stacktrace::stacktrace());
		std::wcout<<d.msg<<'\n'<<a<<'\n';
		#else
		std::wcout<<d.msg<<"\n("<<d.info.line<<") "<<d.info.file<<"\n";
		#endif
	};
	#endif
	
	}
	
	
	// get paths
	{
		
	std::wstring argv0 = static_cast<std::wstring>(this->argv[0]);
	
	dir::exe = fs::directory_entry( fs::path(argv0).parent_path() );
	if( ! dir::exe.exists() )
		debug::db({ std::wstring(L"Cant find root directorie: ") + dir::exe.path().generic_wstring() + L"\n", {DBINFO}, debug::kritical });
	
	dir::res = fs::directory_entry( dir::exe.path() / L"res" );
	if( !dir::res.exists() )
		debug::db({ std::wstring(L"Cant find directorie: " + dir::res.path().generic_wstring() + L"\n"), {DBINFO}, debug::kritical });
	
	dir::shaders = fs::directory_entry(dir::exe.path() / L"res/shaders");
	if( ! dir::shaders.exists() )
		debug::db({ std::wstring(L"Cant find directorie: " + dir::shaders.path().generic_wstring() + L"\n"), {DBINFO}, debug::kritical });
	
	file::box = (dir::res.path() / L"box.obj").generic_wstring();
	if( ! (fs::directory_entry(file::box).exists() ) )
		debug::db({ std::wstring(L"Cant find file: " + file::box + L"\n"), {DBINFO}, debug::kritical });
	
	file::vertex = (dir::shaders.path() / L"vertex.shader").generic_wstring();
	file::unicolor_fragment = (dir::shaders.path() / L"unicolor_fragment.shader").generic_wstring();
	
	}

	
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "wx_gl")
{
	canvas = new Canvas(this);
	Gui gui = Gui(this, canvas);
}



Canvas::Canvas(wxFrame* frame):  wxGLCanvas(frame), timer(this, TIMER_ID)
{ 
	context = new wxGLContext(this); 
	context->SetCurrent(*this); 
	
	// glew
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::wstring errStr1 = L"Could not initialize glew:\n";
			std::wstring errStr2 = errStr1 + reinterpret_cast<const wchar_t*>(glewGetErrorString(err)) + L"\n";
			debug::db({ std::wstring(errStr2), {DBINFO}, debug::kritical });
		}
	}
	
	// load obj file, openGL state
	{
		// not using paths from dir:: because OBJ_Loader.h does not use wstring, and might not find the file.
		if( ! objFile.LoadFile("res/box.obj") ) debug::db({ L"Cant find obj file", {DBINFO}, debug::kritical });
		box = objFile.LoadedMeshes[0];
		
		glClearColor(0, 0, 0, 1.0f );
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}

	// setup camera and box object
	{
		o.shader(file::vertex, file::unicolor_fragment);
		o.scale = glm::mat3(.5);
		o.setMesh(&box.Vertices[0], box.Vertices.size(), &box.Indices[0], box.Indices.size(), 8);
		
		auto Size = GetSize(); 
		size.x = Size.x;	
		size.y = Size.y;
		set_uniColor(o.program, &glm::vec4(.5,0,0,1));
		o.update();
		
		d.shader(file::vertex, file::unicolor_fragment);
		set_uniColor(d.program, &glm::vec4(.5,.5,0,1));
		d.update();
	}

	// ubo
	{
		GLuint qq[2] = {o.program, d.program};
		cam.setup(qq, 2, 0);
	}
	
	timer.Start(100/6); // 1000 = 1 second
} 
void Canvas::OnResize(wxSizeEvent& event)
{
	auto Size = event.GetSize(); 
	size.x = Size.x;	
	size.y = Size.y;
	cam.updateResolution(Size.x, Size.y);
	
	event.Skip();
}
void Canvas::OnTimer(wxTimerEvent& event)
{
	float speed = .05;

	if (keyStatePos.y == 1) { cam.translate.z -= .01; }
	if (keyStatePos.y == -1) { cam.translate.z += .01; }
	
	if (keyStatePos.z == 1) { poop += speed; }
	if (keyStatePos.z == -1) { poop -= speed; };
	cam.rotate(1, poop);
	
	if(keyState[0] == 1) { poop2 += speed; }
	if(keyState[1] == 1) { poop2 -= speed; }
	cam.rotate(0, poop2);

	cam.update();
	o.update();
	
	const wxPoint pt = wxGetMousePosition();
	float mouseX = 1. - 2*(float(pt.x - this->GetScreenPosition().x) / size.x);
	float mouseY = 2*(float(pt.y - this->GetScreenPosition().y) / size.y) - 1.;
	cam.updateMouse(mouseX, mouseY);
	
	//poop += .01F;
	GLerror(DBINFO);
	Refresh();
}
void Canvas::render(wxPaintEvent& evt)
{
	// problems might arise because this is commented
	// DO NOT DELETE THIS COMMENTED SECTION
	//SetCurrent(*context);
    //wxPaintDC(this);     

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(o.program);
	o.draw();
	
	glUseProgram(d.program);
	glm::vec3 d1(0, 0, 0); glm::vec3 d2(0,0,0.5);
	draw::line(&d1, &d2);
	
	GLerror(DBINFO);
	glFlush();
    SwapBuffers();
}

void Canvas::OnKeyDown(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	switch(key)
	{
		case 65: { keyStatePos.x = 1; break; }
		case 68: { keyStatePos.x = -1; break; }
		case 87: { keyStatePos.y = 1; break; }
		case 83: { keyStatePos.y = -1; break; }
		case 81: { keyStatePos.z = 1; break; } // q
		case 69: { keyStatePos.z = -1; break; } // e
		
		case 49: { keyState[0] = 1; break; } // 1
		case 50: { keyState[1] = 1; break; } // 2
	}
	std::cout<<key<<'\n';
    event.Skip();
}
void Canvas::OnKeyUp(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	switch(key)
	{
		case 65: { keyStatePos.x = 0; break; }
		case 68: { keyStatePos.x = 0; break; }
		case 87: { keyStatePos.y = 0; break; }
		case 83: { keyStatePos.y = 0; break; }
		case 81: { keyStatePos.z = 0; break; } // q
		case 69: { keyStatePos.z = 0; break; } // e
		
		case 49: { keyState[0] = 0; break; } // 1
		case 50: { keyState[1] = 0; break; } // 2
	}
	event.Skip();
}

