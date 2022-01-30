#include "Main.h"
#include <math.h>
float poop = 0;

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

	// setup box object
	{
		o = object();
		o.shader(file::vertex, file::unicolor_fragment);
		o.scale = glm::mat3(.3);
		o.setMesh(&box.Vertices[0], box.Vertices.size(), &box.Indices[0], box.Indices.size(), 8);
		auto Size = GetSize(); size = Size;
		updateResolution(o.program, size.x, size.y);
		set_uniColor(o.program, &glm::vec4(.5,0,0,1));
		o.update();
	}
	
	timer.Start(100/6); // 1000 = 1 second
} 
void Canvas::OnResize(wxSizeEvent& event)
{
	auto Size = event.GetSize(); size = Size;	
	updateResolution(o.program, size.x, size.y);
	event.Skip();
}
void Canvas::OnTimer(wxTimerEvent& event)
{
	o.rotate(0, poop);
	o.rotate(1, poop);
	o.rotate(2, poop);
	o.update();
	
	const wxPoint pt = wxGetMousePosition();
	float mouseX = 2*(float(pt.x - this->GetScreenPosition().x) / size.x) - 1.;
	float mouseY = 2*(float(pt.y - this->GetScreenPosition().y) / size.y) - 1.;
	GLint uniform = glGetUniformLocation(o.program, "iMouse");
	glUniform2f(uniform, mouseX, -mouseY); // raw matrix data in rows
	
	poop += .01F;
	Refresh();
}
void Canvas::render(wxPaintEvent& evt)
{
	// problems might arise because this is commented
	//SetCurrent(*context);
    //wxPaintDC(this);     

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	o.draw();

	GLerror();
	glFlush();
    SwapBuffers();
}



