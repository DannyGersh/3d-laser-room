#include "Main.h"
using namespace std;

#ifdef NDEBUG 
wxIMPLEMENT_APP(MyApp);
#else
wxIMPLEMENT_APP_CONSOLE(MyApp);
#endif

bool MyApp::OnInit()
{
	debug::db = [](debug::Data d) 
	{ 
		std::wcout<<d.msg<<'\n';
		if( d.level == debug::kritical ) exit(-1); 
	};


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
    // initialization - glCanvas, glContext, glew
	{
		int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
		canvas = new wxGLCanvas(this, wxID_ANY,  args, wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"));
		context = new wxGLContext(canvas);	
		context->SetCurrent(*canvas);
		
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::wstring errStr1 = L"Could not initialize glew:\n";
			std::wstring errStr2 = errStr1 + reinterpret_cast<const wchar_t*>(glewGetErrorString(err)) + L"\n";
			//wxMessageBox(errStr2 , "Error", wxICON_ERROR | wxOK);
			debug::db({ std::wstring(errStr2), {DBINFO}, debug::kritical });

			exit(-1);
		}
	
	}
	
	// load obj file
	// not using paths from dir:: because OBJ_Loader.h does not use wstring, and might not find the file.
	objl::Loader file;
	file.LoadFile("res/box.obj");
	
	// openGL
	{
		shad::vertex = shad::compile(file::vertex, GL_VERTEX_SHADER);
		shad::unicolorFrag = shad::compile(file::unicolor_fragment, GL_FRAGMENT_SHADER);
		
		//debug::Data a = debug::get();
		
		if( debug::get() ) std::wcout<<debug::last.msg;
		
		prog::unicolor = glCreateProgram();
		prog::link(prog::unicolor, shad::vertex, shad::unicolorFrag);
		glUseProgram(prog::unicolor);
	}

	Gui* gui = new Gui(this, canvas);

}
	
void MyFrame::render(wxPaintEvent& evt)
{
    canvas->SetCurrent(*context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
	
	// randomly change color of backgrownd via glClear
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		glClearColor(r, g, b, 1.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glFlush();
    canvas->SwapBuffers();
}


