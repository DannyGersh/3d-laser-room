#pragma once
#include <string>
#include <vector>

namespace debug
{
	struct Info
	{
		int line;
		std::string file;
		std::string function;
		
		Info() = default;
		Info(int Line, std::string File, std::string Function)
		{
			line		= Line;
			file		= File;
			function	= Function;
		}
	};
	
	struct Data
	{
		std::wstring msg;
		Info info;
		int level;
		
		Data() = default;
		Data( std::wstring msg_in, Info info_in, int level_in )
		{
			msg  	=	msg_in;
			info 	=	info_in;
			level	=	level_in;
		}
	};
	Data nulld{L"", {0,"",""}, 0};
	
	void(*db)(Data) = [](Data d) { std::wcout<<d.msg<<"\n"; };
	
	const int kritical = 1;
	const int warning = 2;
	const int message = 3;
	
	std::vector<Data> raised;
	bool empty() { return !raised.size(); };
	void raise(Data d) { raised.push_back(d); }

	Data last = nulld;
	
	bool get()
	{ 
		if( !empty() ) 
		{ 
			last = raised.back(); 
			raised.pop_back(); 
			return true; 
		}
		else return false;
	};
	void showLastError(Info i)
	{
		db({ last.msg , i, last.level });
	}
};

#define DBINFO debug::Info(__LINE__, __FILE__, __FUNCTION__)
