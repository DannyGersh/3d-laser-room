cd %~dp0
cmake -B result
cmake --build result --config debug
pause