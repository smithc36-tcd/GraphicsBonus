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

// Perlin
uniform float persistence; 
uniform float lacunarity;
uniform int scale; 
uniform int octaves; 
uniform float yOffset;
uniform int height; 

float _lerp(float a0, float a1, float t)
{
    return (a1-a0) * t + a0;
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float Perlin(float x, float y)
{
    float interger_x = x - fract(x);
    float fractional_x = x - interger_x;

    float interger_y = y - fract(y);
    float fractional_y = y - interger_y;

    float v1 = rand(vec2(interger_x, interger_y));
    float v2 = rand(vec2(interger_x + 1.0, interger_y));
    float v3 = rand(vec2(interger_x, interger_y + 1.0));
    float v4 = rand(vec2(interger_x + 1.0, interger_y + 1.0));

    float i1 = _lerp(v1, v2, fractional_x);
    float i2 = _lerp(v3, v4, fractional_x);

    return _lerp(i1, i2, fractional_y) * 0.5 + 0.5;
}

float FBM(float x, float z)
{
    float scalef = float(scale);

    float xs = x / scale; 
    float zs = z /  scale; 
    
    float amp = 1.0;
    float freq = 1.0;
    float total = 0.0; 
    float normalisation = 0.0;

    const int octaves = 3;
    for(int i = 0; i < octaves; i++)
    {
        float noiseVal = Perlin(xs * freq, zs * freq);
        total += noiseVal * amp;
        normalisation += amp;
        amp *= persistence;
        freq *= lacunarity;
    }
    total /= normalisation;

    return total;
}

void main()
{
    // Current position used to calcuate the direction of light 
    currPos = vec3(model * vec4(aPos, 1.0f));
    currPos.y = height * FBM(currPos.x, currPos.z) + yOffset;
    //currPos.y = Perlin(currPos.x, currPos.z);
    Normal = aNormal;
    Color = vec4(aColor, 1.0);

    gl_Position = camMatrix * vec4(currPos, 1.0f);
}
