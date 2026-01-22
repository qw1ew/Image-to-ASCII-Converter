cmake_minimum_required(VERSION 3.10)
project(AsciiArtConverter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(include/libwebp)
include_directories(include/libwebp/src)

file(GLOB_RECURSE WEBP_SOURCES "include/libwebp/src/*.c" "include/libwebp/sharpyuv/*.c")

include_directories(include/imgui)
include_directories(include/imgui/backends)
include_directories(include/glfw/include)
link_directories(include/glfw/lib)

# ImGui sources
set(IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
    include/imgui/imgui_demo.cpp
)

add_executable(ascii_converter
    src/main.cpp
    src/GuiApp.cpp
    src/ImageLoader.cpp
    src/AsciiConverter.cpp
    ${WEBP_SOURCES}
    ${IMGUI_SOURCES}
)

# Link GLFW and OpenGL
# "glfw3" if static, or "glfw3dll" if dynamic. We downloaded binaries which usually have both.
# The script put "libglfw3.a" (MinGW) or similar in lib. 
# CMake might need to just find it.
target_link_libraries(ascii_converter glfw3 opengl32 gdi32 user32 shell32)
