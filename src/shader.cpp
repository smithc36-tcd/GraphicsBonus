#include "shader.hpp"
#include <glm/glm.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. retrieve the vertex / fragment source code from filepath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // Objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    // read files buffer contents to stream
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // Convert steam into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. Compile headers

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  // Print any errors
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  // Print any errors
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // Print any errors
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(const char *vertexPath, const char *geometryPath,
               const char *fragmentPath) {
  // 1. retrieve the vertex / fragment source code from filepath
  std::string vertexCode;
  std::string geometryCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream gShaderFile;
  std::ifstream fShaderFile;

  // Objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    gShaderFile.open(geometryPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, gShaderStream, fShaderStream;

    // read files buffer contents to stream
    vShaderStream << vShaderFile.rdbuf();
    gShaderStream << gShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    gShaderFile.close();
    fShaderFile.close();

    // Convert steam into string
    vertexCode = vShaderStream.str();
    geometryCode = gShaderStream.str();
    fragmentCode = fShaderStream.str();

  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *gShaderCode = geometryCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. Compile headers

  unsigned int vertex, geometry, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  // Print any errors
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  geometry = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometry, 1, &gShaderCode, NULL);
  glCompileShader(geometry);

  // Print any errors
  glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(geometry, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  // Print any errors
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, geometry);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // Print any errors
  glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(geometry);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z,
             float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::Delete() { glDeleteProgram(ID); }
