#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) 
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Define the view matrix 
    view = glm::lookAt(Position, Position + Orientation, Up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)width/height, 
            nearPlane, farPlane);

    cameraMatrix = projection * view;
}
void Camera::Matrix(Shader &shader, const char *uniform)
{

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, 
            glm::value_ptr(cameraMatrix));
}

glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(Position, Position + glm::vec3(0.0f, 0.0f, -1.0f), Up);
    }

void Camera::Inputs(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * Orientation;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += speed * -Orientation;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position += speed * -Up;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4;
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.05f;
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


        // Prevents the camera from jumping around
        if(firstClick)
        {
            glfwSetCursorPos(window, (width/2) , (height/2));
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // calcualte rotation from center
        float rotX = sensitivity * (float)(mouseY - ((float)height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - ((float)width / 2)) / width;

        // Prevents turning too quickly
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), 
                glm::normalize(glm::cross(Orientation, Up)));
        if(!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) ||
                    glm::angle(newOrientation, -Up) <= glm::radians(5.0)))
        {
            Orientation = newOrientation;
        }

        // rotate in the y direction
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // set curso rposition to center 
        glfwSetCursorPos(window, (width/2) , (height/2));
         
 
    }
    else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        //  on release of mouse 
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }


    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

