#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.hpp"
#include "EBO.hpp"
#include "camera.hpp"
#include "Texture.hpp"

class Mesh
{
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        VAO VAOi;

        Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, 
                std::vector<Texture> &textures);

        void Draw(Shader& shader, Camera& camera);

};



#endif // !DEBUG
