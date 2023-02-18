#ifndef PERLIN2D_H
#define PERLIN2D_H

#include <glm/glm.hpp>

class Perlin2D
{


    public:
        Perlin2D();
        float perlin(float x, float y);
        
    private:
        float interpolate(float a0, float a1, float t);
        glm::vec2 randGrad(int xi, int yi);
        float dotGrad(int xi, int yi, float x, float y);
};

#endif // !PERLIN2D_H

