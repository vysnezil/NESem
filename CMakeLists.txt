cmake_minimum_required(VERSION 3.10)

project(NESem)
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_STANDARD 17)

add_executable(NESem src/main.cpp src/Display.cpp src/Input.cpp src/Bus.cpp src/Cartridge.cpp src/CPU.cpp src/PPU.cpp src/Mapper/Mapper.cpp src/Mapper/Mapper000.cpp src/Color.cpp src/Dialog.cpp src/Menu.cpp src/Menu.h "src/glHelper.cpp" "src/glHelper.h" "src/Save.h" "src/SaveManager.cpp" "src/SaveManager.h" "src/Overlay.h" "src/Overlay.cpp")

#add_subdirectory(lib/glfw-3.3.6)
#target_link_libraries(NESem glfw)

add_subdirectory(lib/glad)
target_link_libraries(NESem glad)

add_subdirectory(lib/nativefiledialog-extended)
target_link_libraries(NESem nfd)

add_subdirectory(lib/imgui)
target_link_libraries(NESem imgui)

add_subdirectory(lib/md5)
target_link_libraries(NESem md5)

add_subdirectory(lib/json)
target_link_libraries(NESem json)

target_compile_features(NESem PRIVATE cxx_std_17)