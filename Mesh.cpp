#define STB_IMAGE_IMPLEMENTATION
#include "Mesh.hpp"

#include <string>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
           std::vector<Texture> &textures) {


  Mesh::vertices = vertices;
  Mesh::indices = indices;
  Mesh::textures = textures;

  VAOi.Bind();

  // Generate VBO and link it to vertices
  VBO VBO1(vertices);
  // Generate an EBO and link it to indices
  EBO EBO1(indices);

  // Link VBO attributes such as coordinates and colors to VAO
  VAOi.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
  VAOi.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(Vertex),(void *)(3 * sizeof(float)));
  VAOi.LinkAttrib(VBO1, 2, 3, GL_FLOAT, sizeof(Vertex),(void *)(6 * sizeof(float)));
  VAOi.LinkAttrib(VBO1, 3, 2, GL_FLOAT, sizeof(Vertex),(void *)(9 * sizeof(float)));

  // unbind to each object to avoid changing them
  VAOi.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
    shader.use();
    VAOi.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;
        if(type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }
        else if(type == "specular")
        {
            num = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, 
            camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
