cd %~dp0
cmake -B result
cmake --build result --config debug

echo off
IF NOT EXIST result\Release mkdir result\Release
IF NOT EXIST result\Release\wxbase315u_vc_custom.dll copy "C:\Program Files (x86)\wxWidgets\lib\vc_x64_dll\wxbase315u_vc_custom.dll" result\Release
IF NOT EXIST result\Release\wxmsw315u_core_vc_custom.dll copy "C:\Program Files (x86)\wxWidgets\lib\vc_x64_dll\wxmsw315u_core_vc_custom.dll" result\Release
IF NOT EXIST result\Release\wxmsw315u_gl_vc_custom.dll copy "C:\Program Files (x86)\wxWidgets\lib\vc_x64_dll\wxmsw315u_gl_vc_custom.dll" result\Release

IF NOT EXIST result\Debug mkdir result\Debug
IF NOT EXIST result\Debug\wxbase315ud_vc_custom.dll copy "C:\Program Files (x86)\wxWidgets\lib\vc_x64_dll\wxbase315ud_vc_custom.dll" result\Debug
IF NOT EXIST result\Debug\wxmsw315ud_core_vc_custom.dll copy "C:\Program Files (x86)\wxWidgets\lib\vc_x64_dll\wxmsw315ud_core_vc_custom.dll" result\Debug
IF NOT EXIST result\Debug\wxmsw315ud_gl_vc_custom.dll copy "C:\Program Files (x86)\wxWidgets\lib\vc_x64_dll\wxmsw315ud_gl_vc_custom.dll" result\Debug

pause