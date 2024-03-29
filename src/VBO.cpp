#include "VBO.hpp"

VBO::VBO(std::vector<Vertex> &vertices) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_DYNAMIC_DRAW);
}

void VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }

void VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::Delete() { glDeleteBuffers(1, &ID); }

void VBO::SubData(std::vector<Vertex> &newVertices) {
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(Vertex),
                  newVertices.data());
}
