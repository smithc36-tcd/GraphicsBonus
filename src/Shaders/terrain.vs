#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 currPos;

// uniforms needed for 3d viewing 
uniform mat4 camMatrix;

void main()
{
    // Current position used to calcuate the direction of light 
    currPos = vec3(vec4(aPos, 1.0f));
    //currPos.y = currPos.y + 0.1 * sin(time * currPos.y);
    //Normal = aNormal;
    //Color = vec4(aColor, 1.0);

    //gl_Position = camMatrix * vec4(currPos, 1.0f);
    gl_Position = vec4(currPos, 1.0f);
}

