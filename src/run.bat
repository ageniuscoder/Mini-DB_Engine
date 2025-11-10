@echo off
g++ engine.cpp -o engine 
if %errorlevel%==0 (
    engine
) else (
    echo Compilation failed!
)
