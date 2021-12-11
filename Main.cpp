#include "Main.h"


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



Canvas::Canvas(wxFrame* frame):  wxGLCanvas(frame)
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
	
	// shaders, programs
	{
		shad::vertex = shad::compile(file::vertex, GL_VERTEX_SHADER);
		shad::unicolorFrag = shad::compile(file::unicolor_fragment, GL_FRAGMENT_SHADER);
		
		if( debug::get() ) 
		{ 
			#ifdef NDEBUG 
			debug::showLastError({DBINFO}); 
			exit(-1);
			#else
			debug::showLastError({DBINFO}); 
			#endif
		}
		
		prog::unicolor = glCreateProgram();
		prog::link(prog::unicolor, shad::vertex, shad::unicolorFrag);
		glUseProgram(prog::unicolor);
	}
	
	// load obj file, openGL state
	{
		// not using paths from dir:: because OBJ_Loader.h does not use wstring, and might not find the file.
		file.LoadFile("res/box.obj");
		
		glClearColor(0, 0, 0, 1.0f );
		set_uniColor(prog::unicolor, glm::vec4(1,0,0,1));
	}
	
} 
void Canvas::OnResize(wxSizeEvent& event)
{
	auto size = event.GetSize();
	if( size.x > size.y) glViewport( size.x/2-size.y/2, 0, size.y, size.y );
	else glViewport( 0, size.y/2-size.x/2, size.x, size.x );
	event.Skip();
}
void Canvas::render(wxPaintEvent& evt)
{
	SetCurrent(*context);
    wxPaintDC(this);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(prog::unicolor);
	
	GLuint buffer;
	glGenBuffers(1, &buffer);
	std::vector<glm::vec3> data{{-.7, -.7, 0}, {.7,-.7,0}, {.7, .7, 0}};
	draw::triangle({0,0,0},{-1,0,0},{-1,-1,0}, buffer);
	glDeleteBuffers(1, &buffer);
	
	GLerror();
	glFlush();
    SwapBuffers();
}


