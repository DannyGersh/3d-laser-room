#ifndef finInclude
		
	#include <GL/glew.h>
	
	#include "wx/wx.h"
	#include "wx/glcanvas.h"
	#include "wx/clrpicker.h"
	#include "wx/slider.h"
	#include "wx/grid.h"

	#include <fstream>
	#include <iostream>
	#include <string>
	#include <filesystem>
	#include <sstream>
	
	#ifndef NDEBUG
		#if __has_include ("boost/stacktrace.hpp")
			#ifdef USE_STACKTRACE
				#include "boost/stacktrace.hpp"
			#endif
		#endif
	#endif
	
	#include "depend/OBJ_Loader.h"
	
	#include "depend/glm/gtc/matrix_transform.hpp"
	#include "depend/glm/gtc/type_ptr.hpp"

	
	#define finInclude
	
#endif