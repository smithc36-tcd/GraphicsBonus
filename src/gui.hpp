#ifndef GUI_H
#define GUI_H

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_glfw.h"
#include "imGUI/imgui_impl_opengl3.h"

class GUI{
   public:
    GUI(GLFWwindow *window);

    void newFrame();
    void render(float &persistence, 
            float &lacunarity, 
            int &scale, 
            int &octaves, 
            float &xOffset,
            float &zOffset, 
            int & height);
    void Delete();

};


#endif // !DEBUG
       // 
