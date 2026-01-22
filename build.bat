@echo off
setlocal EnableDelayedExpansion
echo Attempting to build ASCII Converter (CLI + GUI)...

WHERE g++ >nul 2>nul
IF !ERRORLEVEL! EQU 0 (
    echo Found g++. Compiling...
    
    g++ -O3 -I./include -I./include/libwebp -I./include/libwebp/src ^
    -I./include/imgui -I./include/imgui/backends -I./include/glfw/include ^
    src/main.cpp src/ImageLoader.cpp src/AsciiConverter.cpp src/GuiApp.cpp ^
    include/imgui/imgui.cpp ^
    include/imgui/imgui_draw.cpp ^
    include/imgui/imgui_tables.cpp ^
    include/imgui/imgui_widgets.cpp ^
    include/imgui/backends/imgui_impl_glfw.cpp ^
    include/imgui/backends/imgui_impl_opengl3.cpp ^
    include/libwebp/src/dec/*.c ^
    include/libwebp/src/dsp/*.c ^
    include/libwebp/src/utils/*.c ^
    include/libwebp/src/enc/*.c ^
    include/libwebp/src/mux/*.c ^
    include/libwebp/src/demux/*.c ^
    include/libwebp/sharpyuv/*.c ^
    -L./include/glfw/lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 ^
    -o ascii_converter.exe
    
    IF !ERRORLEVEL! EQU 0 (
        echo Build successful! Run ascii_converter.exe
    ) ELSE (
        echo Build failed.
    )
    exit /b
)

WHERE cmake >nul 2>nul
IF !ERRORLEVEL! EQU 0 (
    echo Found CMake. Building...
    mkdir build 2>nul
    cd build
    cmake ..
    cmake --build . --config Release
    cd ..
    exit /b
)

echo.
echo No compiler (g++, cmake) found.
pause
