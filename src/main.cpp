#include <filesystem>
#include <iostream>
#include <vector>

#include "Mesh.hpp"
#include "gui.hpp"
// #include "perlin.hpp"
// #include "perlin2D.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// colours
// glm::vec3 SEA_BLUE = glm::vec3(0.0 / 255.0, 33.0 / 255.0, 115.0 / 255.0);
// glm::vec3 SAND = glm::vec3(194.0 / 255.0, 178.0 / 255.0, 128.0 / 255.0);
// glm::vec3 GRASSLAND = glm::vec3(128.0 / 255.0, 177.0 / 255.0, 69.0 / 255.0);
// glm::vec3 UPPER_GRASSLAND =
// glm::vec3(89.0 / 255.0, 149.0 / 255.0, 74.0 / 255.0);
// glm::vec3 LOWER_MOUNTAIN =
// glm::vec3(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);
// glm::vec3 UPPER_MOUNTAIN =
// glm::vec3(122.0 / 255.0, 122.0 / 255.0, 122.0 / 255.0);
// glm::vec3 SNOW = glm::vec3(1, 1, 1);

std::vector<GLuint> gen_indices(int div) {
  std::vector<GLuint> indices;
  indices.reserve(div * div * 6);

  for (int col = 0; col < div - 1; col++) {
    for (int row = 0; row < div - 1; row++) {
      int index = row + div * col;

      // top triangle
      indices.push_back(index);
      indices.push_back(index + div + 1);
      indices.push_back(index + div);

      // bottom triangle
      indices.push_back(index);
      indices.push_back(index + 1);
      indices.push_back(index + div + 1);
    }
  }
  return indices;
}

const float MAP_WIDTH = 100.0f;
const int NUM_VERTICES = 500;
const float SEA_LEVEL = 1.0;

// Perlin2D noise;
float persistence = 0.5f;
float lacunarity = 2.0f;
int scale = 5;
int octaves = 3;
float yOffset = 0.0f;
float xOffset = 0.0f;
float zOffset = 0.0f;
int height = 1;
int seed = 0;

std::vector<glm::vec3> gen_coords(int dVertices, float width) {
  std::vector<glm::vec3> coords;
  coords.reserve(dVertices * dVertices);

  float triangle_length = width / dVertices;
  for (int col = 0; col < dVertices; col++) {
    for (int row = 0; row < dVertices; row++) {
      float x = row * triangle_length;
      float z = col * triangle_length;
      coords.push_back(glm::vec3(x, 0.0f, z));
    }
  }
  return coords;
}

std::vector<Vertex> gen_vertex(std::vector<GLuint> indices) {
  std::vector<Vertex> Vertices;
  std::vector<glm::vec3> coords = gen_coords(NUM_VERTICES, MAP_WIDTH);

  for (int i = 0; i < coords.size(); i++) {
    Vertices.push_back(Vertex{coords[i]});
  }
  return Vertices;
}

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glad: load all OpenGL function pointers
  // ---------------------------------------

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // init imGUI
  GUI myGUI(window);

  // build and compile our shader zprogram
  // ------------------------------------

  // Shader ourShader("../src/Shaders/shader.vs", "../src/Shaders/shader.fs");
  Shader terrainShader("../src/Shaders/terrain.vs", "../src/Shaders/terrain.gs",
                       "../src/Shaders/terrain.fs");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  std::vector<GLuint> Indices = gen_indices(NUM_VERTICES);
  std::vector<Vertex> Vertices = gen_vertex(Indices);

  Mesh floor(Vertices, Indices);

  glEnable(GL_DEPTH_TEST);

  Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 1.0f, 2.0f));

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    myGUI.newFrame();

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    terrainShader.use();
    float timeValue = sin(glfwGetTime());
    terrainShader.setFloat("time", timeValue);
    terrainShader.setFloat("lacunarity", lacunarity);
    terrainShader.setFloat("persistence", persistence);
    terrainShader.setInt("scale", scale);

    floor.Draw(terrainShader, camera);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------

    // Render GUI elements
    myGUI.render(persistence, lacunarity, scale, octaves, xOffset, zOffset,
                 height);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  terrainShader.Delete();

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
