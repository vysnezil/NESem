cmake_minimum_required(VERSION 3.10)

project(NESem)
#set(CMAKE_BUILD_TYPE "Release")

add_executable(NESem src/main.cpp src/Display.cpp src/Input.cpp src/Bus.cpp src/Cartridge.cpp src/CPU.cpp src/PPU.cpp src/Mapper/Mapper.cpp src/Mapper/Mapper000.cpp src/Color.cpp src/Dialog.cpp src/Menu.cpp src/Menu.h "src/glHelper.cpp" "src/glHelper.h" src/imgui_demo.cpp "src/Save.h")

#add_subdirectory(lib/glfw-3.3.6)
#target_link_libraries(NESem glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(NESem OpenGL::GL)

add_subdirectory(lib/glad)
target_link_libraries(NESem glad)

add_subdirectory(lib/nativefiledialog-extended)
target_link_libraries(NESem nfd)

add_subdirectory(lib/imgui)
target_link_libraries(NESem imgui)