cmake_minimum_required(VERSION 3.10)

project(imgui)

add_library(imgui include/imgui.h include/imconfig.h include/imgui_impl_glfw.h include/imgui_impl_opengl2.h include/imgui_internal.h include/imstb_rectpack.h include/imstb_textedit.h include/imstb_truetype.h src/imgui.cpp src/imgui_draw.cpp src/imgui_impl_glfw.cpp src/imgui_impl_opengl2.cpp src/imgui_tables.cpp src/imgui_widgets.cpp src/imgui_demo.cpp)

target_include_directories(imgui PUBLIC include/)

add_subdirectory(../glfw-3.3.6 ../sub)
target_link_libraries(imgui glfw)