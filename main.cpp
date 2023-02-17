#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
#include "Mesh.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Initialisation function
int init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  return 0;
}

int main() {

  if (init() != 0) {
    std::cerr << "INIT FAILED" << std::endl;
    return -1;
  }

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

  if (window == NULL) { 
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

  Texture textures[] = {
      Texture("Textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
      Texture("Textures/planksSpec.png", "specular", 1, GL_RED,
              GL_UNSIGNED_BYTE)};

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  // Vertices coordinates
  Vertex vertices[] = {
      //               COORDINATES           /            COLORS          /
      //               TexCoord         /       NORMALS         //
      Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
      Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
      Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

  // Indices for vertices order
  GLuint indices[] = {0, 1, 2, 0, 2, 3};

  Vertex lightVertices[] = {//     COORDINATES     //
                            Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
                            Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
                            Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
                            Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
                            Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
                            Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
                            Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
                            Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

  GLuint lightIndices[] = {0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3,
                           3, 7, 6, 3, 6, 2, 2, 6, 5, 2, 5, 1,
                           1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

  std::vector<Vertex> verts(vertices,
                            vertices + sizeof(vertices) / sizeof(Vertex));
  std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
  std::vector<Texture> tex(textures,
                           textures + sizeof(textures) / sizeof(Texture));
  Mesh floor(verts, ind, tex);

  // Lighting

  Shader lightShader("Shaders/light.vs", "Shaders/light.fs");
  std::vector<Vertex> lightVerts(
      lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
  std::vector<GLuint> lightInd(
      lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
  Mesh light(lightVerts, lightInd, tex);

  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 pyramidModel = glm::mat4(1.0f);
  pyramidModel = glm::translate(pyramidModel, pyramidPos);

  lightShader.use();
  glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(lightModel));
  glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x,
              lightColor.y, lightColor.z, lightColor.w);
  ourShader.use();
  glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(pyramidModel));
  glUniform4f(glGetUniformLocation(ourShader.ID, "lightColor"), lightColor.x,
              lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(ourShader.ID, "lightPos"), lightColor.x,
              lightColor.y, lightColor.z);

  // load and create a texture
  // -------------------------
  // Texture tex("Textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA,
  // GL_UNSIGNED_BYTE);
  // tex.texUnit(ourShader, "tex0", 0);
  // Texture planksSpec("Textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED,
  // GL_UNSIGNED_BYTE);
  // tex.texUnit(ourShader, "tex1", 1);

  glEnable(GL_DEPTH_TEST);

  Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    floor.Draw(ourShader, camera);
    light.Draw(lightShader, camera);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  ourShader.Delete();
  lightShader.Delete();
  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
