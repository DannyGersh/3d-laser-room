maxWnd
:: name of project without extension
set name=Main
:: cd to project directorie
cd %~dp0../



set inc=
set lib=

set inc=%inc%;C:\danny\c++\pacages\wxWidgets\lib\gcc_dll\mswu
set inc=%inc%;C:\danny\c++\pacages\wxWidgets\include
set lib=%lib%;C:\danny\c++\pacages\wxWidgets\lib
set lib=%lib%;C:\danny\c++\pacages\wxWidgets\lib\gcc_dll

set inc=%inc%;C:\danny\c++\pacages\glew\include
set lib=%lib%;C:\danny\c++\pacages\glew\lib

set inc=%inc%;C:\danny\c++\pacages\glm-0.9.9.8

set CPLUS_INCLUDE_PATH=%inc%
set LIBRARY_PATH=%lib%

:: compile and run
cls
@echo off
g++ --version
::g++ %name%.cpp -g -lDbghelp -lglew32 -lopengl32  -lwxbase315u_gcc_custom -lwxbase315u_net_gcc_custom -lwxbase315u_xml_gcc_custom -lwxmsw315u_adv_gcc_custom -lwxmsw315u_aui_gcc_custom -lwxmsw315u_core_gcc_custom -lwxmsw315u_gl_gcc_custom -lwxmsw315u_html_gcc_custom -lwxmsw315u_media_gcc_custom -lwxmsw315u_propgrid_gcc_custom -lwxmsw315u_ribbon_gcc_custom -lwxmsw315u_richtext_gcc_custom -lwxmsw315u_stc_gcc_custom -lwxmsw315u_webview_gcc_custom -lwxmsw315u_xrc_gcc_custom -o exe\%name%.exe
g++ %name%.cpp -g -lDbghelp -lglew32 -lopengl32  -lwxbase315u_gcc_custom -lwxmsw315u_core_gcc_custom -lwxmsw315u_gl_gcc_custom -o exe\%name%.exe

::IF %ERRORLEVEL% EQU -300 ( 
::	exit
::)

if %ERRORLEVEL% NEQ 0 ( 
	echo program return code: %ERRORLEVEL%
	pause
	exit
)


cv2pdb exe\%name%.exe
exe\%name%.exe

echo program return code: %ERRORLEVEL%
pause



:: adding libraries:
::g++ %name%.cpp -o exe\%name%.exe -lopengl32 -lglew32 -lGLU32 -lGdi32