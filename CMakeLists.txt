cmake_minimum_required(VERSION 3.20)
project(terrain_gen)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES 
    src/*.cpp
    include/imGUI/*.cpp)

set(HEADER_FILES
    src/*.hpp)



add_executable(MyExecutable ${SOURCE_FILES})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(MyExecutable glfw OpenGL::GL X11 pthread Xrandr Xi dl)

