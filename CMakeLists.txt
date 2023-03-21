cmake_minimum_required(VERSION 3.20)
project(terrain_gen)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCE_FILES 
    src/main.cpp
    src/Mesh.cpp
    src/camera.cpp
    src/perlin2D.cpp
    src/perlin.cpp
    src/shader.cpp
    src/EBO.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/gui.cpp
    #include/imGUI
    include/glad.c
    #include/imGUI/imgui.cpp
    #include/imGUI/imgui_impl_opengl3.cpp
    #include/imGUI/imgui_impl_glfw.ccp
    include/imGUI/imgui.cpp
    include/imGUI/imgui_demo.cpp
    include/imGUI/imgui_draw.cpp
    include/imGUI/imgui_impl_glfw.cpp
    include/imGUI/imgui_impl_opengl3.cpp
    include/imGUI/imgui_tables.cpp
    include/imGUI/imgui_widgets.cpp
    )

#set(HEADER_FILES
     #src/*.hpp)



add_executable(terrain_gen ${SOURCE_FILES})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(terrain_gen glfw OpenGL::GL X11 pthread Xrandr Xi dl)

