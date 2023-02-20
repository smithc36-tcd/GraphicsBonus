#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 currPos;
out vec3 Normal;
out vec4 Color;

// uniforms needed for 3d viewing 
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    // Current position used to calcuate the direction of light 
    currPos = vec3(model * vec4(aPos, 1.0f));
    Normal = aNormal;
    Color = vec4(aColor, 1.0);

    gl_Position = camMatrix * vec4(currPos, 1.0f);
}
