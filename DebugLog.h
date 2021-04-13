#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#endif

// return codes on failure:
// -30 : SymGetSymFromAddr() failed
// -31 : DB::LogT() failed

// usage:
/*
	DB usage:
	Log\LogT - logs a message by using the current callback.
	LogT loggs with a StackTrace snapshot (currently onley windows).
	set the callback function to change how the message is logged.
	
	example:
	DB::callback = [](std::string str, const char* file, int line) { std::cout<<str<<"\n"; };
	DB::Log("test1");
	DB::LogT("test2");
*/

// IMPORTATN ! (windows)
/*
	StackTrace functions properly onley for debug builds
	and when PDB files are pressent.
	this file does not generate PDBs.

	If using MinGw, get a copy of cv2pdb.exe
	and use it to generate PDB.
*/



namespace DB
{
	void (*DefaultCallback)(std::string, const char*, int) = [](std::string str, const char* file, int line) { std::cout<<str<<'\n'<<"Called from "<<file<<"(line "<<line<<")"; };

	void (*callback)(std::string, const char*, int) = DefaultCallback;
	void Log(std::string str, const char* file, int line) 
	{
		try { callback(str, file, line); } 
		catch( ... ) { exit (EXIT_FAILURE); }
	}
	#define Log(str) Log(str, __FILE__, __LINE__)
	
	void LogT(std::string str, const char* file, int line);
}


#ifdef _WIN32

namespace StackTrace
{
	
	std::vector<std::string> GetStackTrace()
	{
		std::vector<std::string> entries;
		
		DWORD machine = IMAGE_FILE_MACHINE_I386;
		HANDLE process  = GetCurrentProcess();
		HANDLE thread   = GetCurrentThread();
		CONTEXT context = {};
		context.ContextFlags = CONTEXT_FULL;
	
		SymInitialize(process, NULL, TRUE);
		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
		
		RtlCaptureContext(&context);
		
		STACKFRAME frame = {};
		frame.AddrPC.Offset    = context.Eip;
		frame.AddrPC.Mode      = AddrModeFlat;
		frame.AddrFrame.Offset = context.Ebp;
		frame.AddrFrame.Mode   = AddrModeFlat;
		frame.AddrStack.Offset = context.Esp;
		frame.AddrStack.Mode   = AddrModeFlat;
	
		while (StackWalk(machine, process, thread, &frame, &context , NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
		{
			CHAR* FuncName;
			PCHAR FileName;
			int LineNumber;
			
			// get calling function
			{
				IMAGEHLP_SYMBOL symbol;
				symbol.SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
				symbol.MaxNameLength = 254;
	
				if(SymGetSymFromAddr(GetCurrentProcess(), frame.AddrPC.Offset, (DWORD*)0, &symbol))
				{	
					FuncName = &symbol.Name[0];
					
					std::string test = (std::string)FuncName;
					if (test.find("RtlGetAppContainerNamedObjectPath") != std::string::npos) continue;
					if (test.find("BaseThreadInitThunk") != std::string::npos) continue;
					if (test.find("public_all") != std::string::npos) continue;
  
				}
				else
				{
					std::string er = std::string("ERROR SymGetSymFromAddr() failed: ") + std::to_string(GetLastError()) + "\n";
					DB::Log(er);
					exit(-30);
				}
			}
			
			// get source file name, line
			{
				DWORD  dwAddress;
				DWORD  dwDisplacement;
				IMAGEHLP_LINE line; 
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE);
				
				// play with second arg of SymGetLineFromAddr if result not acurate
				if(SymGetLineFromAddr(process, frame.AddrPC.Offset-sizeof(DWORD), &dwDisplacement, &line))
				{
					FileName = line.FileName;
					LineNumber = line.LineNumber;
					
					std::string test = std::string(FileName);
					if (test.find("DebugLog.h") != std::string::npos) continue;
				}
				else
				{
					// potential BUGS hear !
					/*
						there are unwanted stackTrace frames
						of the operatin system,
						they are NOT syncronised,
						therfore they make an error of 
						ERROR_MESSAGE_SYNC_ONLY (487)
						
						we simply break on this error
						wich is bad because this error
						may pop up in other sircumstances
						
						the user may encaunter problems when 
						dealing with multiple threads
					*/
					
					//if(GetLastError() == 487) break;
				}
			}
			
			std::string t = std::string("\t") + FileName + std::string("\tFunction: ") + FuncName + std::string("\tLine: ") + std::to_string(LineNumber);
			entries.push_back(t);
		}
		
		// get executor file
		{
			//DWORD moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);
			DWORD moduleBase = SymGetModuleBase(process, 0);
			char moduleBuff[MAX_PATH];
			GetModuleFileNameA((HINSTANCE)moduleBase, moduleBuff , MAX_PATH);
		
			entries.push_back(std::string("from: ")+moduleBuff);
		}
		
		SymCleanup(process);
		
		return entries;
	}

	std::string Get() 
	{
		// color console
		//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output
		//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

		std::vector<std::string> t = GetStackTrace();
		std::string fin("StackTrace: \n");
		
		for(auto i: t) fin += i + "\n";
		return fin;
	}

}


void DB::LogT(std::string str, const char* file, int line) 
{
	
	try 
	{ 
		std::string fin = str + "\n" + StackTrace::Get();
		callback(fin, file, line); 
	} 
	catch( ... ) { exit (-31); }
}
#define LogT(str) LogT(str, __FILE__, __LINE__)



#elif
void LogT(std::string str, const char* file, int line) 
{
	try { callback(str, file, line); } 
	catch( ... ) { exit (-31); }
}
#define LogT(str) LogT(str, __FILE__, __LINE__)
#endif


