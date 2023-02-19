#include "perlin2D.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <system_error>
#define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
#include "Mesh.hpp"
#include "perlin.hpp"

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_glfw.h"
#include "imGUI/imgui_impl_opengl3.h"

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
Perlin2D noise;

float FBM(float x, float y, float scale, float persistence, float lacrunarity, int octaves)
{
    const float xs = x / scale;
    const float ys = y / scale;
    float amp = 1.0f, freq = 1.0f, total = 0.0f, normalisation = 0.0f;

    for(int i = 0; i < octaves; i++)
    {
        float noiseVal = noise.perlin(xs * freq, ys * freq);
        total += noiseVal * amp;
        normalisation += amp;
        amp *= persistence;
        freq *= lacrunarity;
    }

    total /= normalisation;

    return total;
}

std::vector<glm::vec3> gen_coords(int dVertices, float width) {
  //std::vector<float> noiseMap = noise.generate_noise_map();
  //std::cout << noiseMap.size() << std::endl;
  std::vector<glm::vec3> coords;
  coords.reserve(dVertices * dVertices);
  // coords normals colours
  // gen coords,
  // gen Normals,
  // gen colours

  float triangle_length = width / dVertices;
  for (int col = 0; col < dVertices; col++) {
    for (int row = 0; row < dVertices; row++) {
      //float noise = noiseMap[row + col * dVertices];
      //std::cout << noise << std::endl;
      float x = row * triangle_length;
      float z = col * triangle_length;
      //float y = FBM(x, z, 5, 0.5, 2, 4);
      coords.push_back(glm::vec3(x, 0.0, z));
    }
  }
  return coords;
}

std::vector<glm::vec3> gen_normals(std::vector<glm::vec3> coords,
                                   std::vector<GLuint> indices) {
  std::vector<glm::vec3> normals;
  // normal.reserve()
  for (int i = 0; i < indices.size(); i += 3) {
    glm::vec3 U = coords[i + 1] - coords[i];
    glm::vec3 V = coords[i + 2] - coords[i];
    glm::vec3 normal = glm::normalize(glm::cross(U, V));
    normals.push_back(normal);
  }
  return normals;
}

// std::vector<glm::vec3> gen_colors()
//{

//}

std::vector<Vertex> gen_vertex()
// void gen_vertex()
{
  std::vector<Vertex> Vertices;
  std::vector<GLuint> indices = gen_indices(200);
  std::vector<glm::vec3> coords = gen_coords(200, 30.0f);
  //    std::vector<glm::vec3> normals = gen_normals(coords, indices);
  for (int i = 0; i < coords.size(); i++) {
    Vertices.push_back(
        Vertex{coords[i], glm::vec3(0.0f, 1.0f, 0.0f),
               glm::vec3(coords[i].y, coords[i].y, coords[i].y)});
  }
  return Vertices;
}

int main() {

  // if (init() != 0) {
  // std::cerr << "INIT FAILED" << std::endl;
  // return -1;
  //}
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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");


  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("Shaders/perlin.vs", "Shaders/shader.fs");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  // Gernerate Vertices
  // Generate Indices
  // TexCoords
  // Generate Normals
  //
  //
  // Vertex structure (coords, normals, color);
  //
  std::vector<Vertex> Vertices = gen_vertex();
  std::vector<GLuint> Indices = gen_indices(200);

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

  Mesh floor(Vertices, Indices);

  // Lighting

  Shader lightShader("Shaders/light.vs", "Shaders/light.fs");
  std::vector<Vertex> lightVerts(
      lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
  std::vector<GLuint> lightInd(
      lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
  Mesh light(lightVerts, lightInd);

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
  //glUniform1f(glGetUniformLocation(ourShader.ID, "persistence"))
  //
  //
  

 float persistence = 0.5f;
 float lacrunarity = 2.0f;
 int scale = 2; 
 float yOffset = 0.0f;
 int height = 1;




 //V     uniform float persistence; 
//unitform float lacunarity;
//uniform int scale; 
//Vuniform int octaves; 



  // load and create a texture
  // -------------------------

  glEnable(GL_DEPTH_TEST);

  Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 1.0f, 2.0f));



  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


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
    
  ImGui::Begin("ImGui Window");
  ImGui::Text("Hello, World");
  ImGui::SliderFloat("Persistence", &persistence, 0.1f, 1.0f);
  ImGui::SliderFloat("Lacrunarity", &lacrunarity, 0.5f, 5.0f);
  ImGui::SliderInt("Scale", &scale, 1, 10);
  ImGui::SliderFloat("yOffset", &yOffset, -2.0f, 2.0f);
  ImGui::SliderInt("height", &height, 1, 50);
  ImGui::End();
    
  ourShader.use();
  ourShader.setFloat("persistence", persistence);
  ourShader.setFloat("lacunarity", lacrunarity);
  ourShader.setInt("scale", scale);
  ourShader.setFloat("yOffset", yOffset);
  ourShader.setInt("height", height);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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
