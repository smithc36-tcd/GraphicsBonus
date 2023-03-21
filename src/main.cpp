#include <glm/detail/qualifier.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <vector>

//define STB_IMAGE_IMPLEMENTATION
// #include "include/stb_image.h"
#include "Mesh.hpp"
#include "perlin.hpp"
#include "perlin2D.hpp"

#include "../include/imGUI/imgui.h"
#include "../include/imGUI/imgui_impl_glfw.h"
#include "../include/imGUI/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//colours 
glm::vec3 SEA_BLUE =            glm::vec3(0.0/255.0,    33.0/255.0,     115.0/255.0);
glm::vec3 SAND =                glm::vec3(194.0/255.0,  178.0/255.0,    128.0/255.0);
glm::vec3 GRASSLAND =           glm::vec3(128.0/255.0,  177.0/255.0,     69.0/255.0);
glm::vec3 UPPER_GRASSLAND =     glm::vec3(89.0/255.0,   149.0/255.0,    74.0/255.0);
glm::vec3 LOWER_MOUNTAIN =      glm::vec3(105.0/255.0,  105.0/255.0,    105.0/255.0);
glm::vec3 UPPER_MOUNTAIN =      glm::vec3(122.0/255.0,  122.0/255.0,    122.0/255.0);
glm::vec3 SNOW =                glm::vec3(1,1,1);

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

const float MapWidth = 30.0f;
const float SEA_LEVEL = 1.0;

Perlin2D noise;
float persistence = 0.5f;
float lacrunarity = 2.0f;
int scale = 5;
int octaves = 3;
float yOffset = 0.0f;
float xOffset = 0.0f;
float zOffset = 0.0f;
int height = 1;
int seed = 0;

float Prev_persistence = persistence;
float Prev_lacrunarity = lacrunarity;
int Prev_scale = scale;
int Prev_octaves = octaves;
float Prev_yOffset = yOffset;
float Prev_xOffset = xOffset;
float Prev_zOffset = zOffset;
int Prev_height = height;
int Prev_seed = seed;

bool CheckGUIstates() {
  if (Prev_persistence != persistence || Prev_lacrunarity != lacrunarity ||
      Prev_scale != scale || Prev_octaves != octaves ||
      Prev_yOffset != yOffset || Prev_xOffset != xOffset ||
      Prev_zOffset != zOffset || Prev_height != height || Prev_seed !=seed) {

    Prev_persistence = persistence;
    Prev_lacrunarity = lacrunarity;
    Prev_scale = scale;
    Prev_octaves = octaves;
    Prev_yOffset = yOffset;
    Prev_xOffset = xOffset;
    Prev_zOffset = zOffset;
    Prev_height = height;
    Prev_seed = seed;
    return true;
  } else {
    return false;
  }
}

float FBM(float x, float y, float scale, float persistence, float lacrunarity,
          int octaves) {
  const float xs = (x - MapWidth/2) / scale;
  const float ys = (y - MapWidth/2) / scale;
  float amp = 1.0f, freq = 1.0f, total = 0.0f, normalisation = 0.0f;

  for (int i = 0; i < octaves; i++) {
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
  std::vector<glm::vec3> coords;
  srand(abs(seed));
  int random = rand() % 200;
  coords.reserve(dVertices * dVertices);

  float triangle_length = width / dVertices;
  for (int col = 0; col < dVertices; col++) {
    for (int row = 0; row < dVertices; row++) {
      float x = row * triangle_length;
      float z = col * triangle_length;
      float noise = FBM(x + xOffset + random, z + zOffset + random, scale, persistence, lacrunarity, octaves);
      float y = fmax(0.0, (noise) * height);
      coords.push_back(glm::vec3(x, y, z));
    }
  }
  return coords;
}

std::vector<glm::vec3> gen_normals(std::vector<glm::vec3> coords,
                                   std::vector<GLuint> indices) {
  // std::cout << "Beginning of gen normals" << std::endl;
  std::vector<glm::vec3> normals;
  normals.reserve(coords.size());
  for (int i = 0; i < coords.size(); i++) {
    normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  }
  // std::vector<glm::vec3> normals(coords.size(), glm::vec3(0.0f, 0.0f, 0.0f));
  // std::cout << "Beginning of gen normals" << std::endl;
  //  normal.reserve()
  for (int i = 0; i < indices.size(); i += 3) {
    // std::cout << i << std::endl;
    glm::vec3 U = coords[indices[i + 1]] - coords[indices[i]];
    glm::vec3 V = coords[indices[i + 2]] - coords[indices[i]];
    glm::vec3 normal = glm::cross(U, V);
    normals[indices[i]] += normal;
    normals[indices[i + 1]] += normal;
    normals[indices[i + 2]] += normal;
    // std::cout << i << std::endl;
  }
  // std::cout << normals.size() << std::endl;
  return normals;
}

std::vector<glm::vec3> gen_colors(const std::vector<glm::vec3> &coords)
{
    std::vector<glm::vec3> colors; 
    colors.reserve(coords.size());
   for(glm::vec3 coord : coords)
   {
        if(coord.y < 0.6) 
               colors.push_back(SEA_BLUE);
        else if(coord.y < 2.0)
               colors.push_back(SAND);
        else if(coord.y < 4.0)
               colors.push_back(GRASSLAND);
        else if(coord.y < 6.0)
               colors.push_back(UPPER_GRASSLAND);
        else if(coord.y < 10)
               colors.push_back(LOWER_MOUNTAIN);
        else if(coord.y < 20)
               colors.push_back(UPPER_MOUNTAIN);
        else
               colors.push_back(SNOW);
    }
   return colors; 
}

std::vector<Vertex> gen_vertex(std::vector<GLuint> indices)
// void gen_vertex()
{
  std::vector<Vertex> Vertices;
  std::vector<glm::vec3> coords = gen_coords(200, MapWidth);
  std::vector<glm::vec3> normals = gen_normals(coords, indices);
  std::vector<glm::vec3> colors = gen_colors(coords);



  for (int i = 0; i < coords.size(); i++) {
    Vertices.push_back(Vertex{coords[i], normals[i], colors[i]});
  }
  return Vertices;
}

std::vector<glm::vec3> gen_water(int dVertices, float width) {
  std::vector<glm::vec3> coords;
  coords.reserve(dVertices * dVertices);

  float triangle_length = width / dVertices;
  for (int col = 0; col < dVertices; col++) {
    for (int row = 0; row < dVertices; row++) {
      float x = row * triangle_length;
      float z = col * triangle_length;
      float noise = FBM(x, z, 3, 1, 5, 2);
      float y = (noise) + SEA_LEVEL;
      coords.push_back(glm::vec3(x, y, z));
    }
  }
  return coords;
}
std::vector<Vertex> gen_water_vertex(std::vector<GLuint> indices)
// void gen_vertex()
{
  std::vector<Vertex> Vertices;
  std::vector<glm::vec3> watercoords = gen_water(200, MapWidth);
  //std::cout << "Coords generated" << std::endl;
  std::vector<glm::vec3> normals = gen_normals(watercoords, indices);
  //std::cout << "Normals Generated" << std::endl;

  for (int i = 0; i < watercoords.size(); i++) {
    Vertices.push_back(Vertex{watercoords[i], normals[i], glm::vec3(21.0f/255.0f, 49.0f/255.0f, 126.0f/255.0f)});
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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  std::vector<GLuint> Indices = gen_indices(200);
  std::cout << Indices.size() << std::endl;
  // for(GLuint x : Indices){
  // std::cout << x << ", ";
  //}
  std::vector<Vertex> Vertices = gen_vertex(Indices);

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


  Shader waterShader("Shaders/perlin.vs", "Shaders/shader.fs");
  std::vector<Vertex> waterV = gen_water_vertex(Indices);
  Mesh water( waterV ,Indices);

  // Lighting

  Shader lightShader("Shaders/light.vs", "Shaders/light.fs");
  
  std::vector<Vertex> lightVerts(
      lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
  std::vector<GLuint> lightInd(
      lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
  Mesh light(lightVerts, lightInd);

  glm::vec4 lightColor = glm::vec4(255.0/255.0, 171.0f/255.0f, 55.0f/255.0f, 1.0f);

  glm::vec3 lightPos = glm::vec3(60.0f, 100.0f, 60.0f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 pyramidModel = glm::mat4(1.0f);
  pyramidModel = glm::translate(pyramidModel, pyramidPos);

  lightShader.use();
  // lightShader.

  glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(lightModel));
  glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x,
              lightColor.y, lightColor.z, lightColor.w);
  ourShader.use();
  glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(pyramidModel));
  glUniform4f(glGetUniformLocation(ourShader.ID, "lightColor"), lightColor.x,
              lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(ourShader.ID, "lightPos"), lightPos.x,
              lightPos.y, lightPos.z);
  // glUniform1f(glGetUniformLocation(ourShader.ID, "persistence"))
  //
  //

  // V     uniform float persistence;
  // unitform float lacunarity;
  // uniform int scale;
  // Vuniform int octaves;

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

    if (CheckGUIstates()) {
      std::vector<Vertex> newVertices = gen_vertex(Indices);
      std::vector<Vertex> waterV = gen_water_vertex(Indices);
      floor.Redraw(newVertices);
      water.Redraw(waterV);
    }
    ourShader.use();
    floor.Draw(ourShader, camera);
    light.Draw(lightShader, camera);

    waterShader.use();
    float time = glfwGetTime();
    float val = sin(time);
    waterShader.setFloat("time", time);
    water.Draw(waterShader, camera);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------

    ImGui::Begin("ImGui Window");
    ImGui::Text("Hello, World");
    ImGui::SliderFloat("Persistence", &persistence, 0.1f, 1.0f);
    ImGui::SliderFloat("Lacrunarity", &lacrunarity, 0.5f, 5.0f);
    ImGui::SliderInt("Scale", &scale, 1, 25);
    ImGui::SliderInt("Octaves", &octaves, 1, 8);
    ImGui::SliderFloat("yOffset", &yOffset, -2.0f, 2.0f);
    ImGui::SliderFloat("xOffset", &xOffset, -20.0f, 20.0f);
    ImGui::SliderFloat("zOffset", &zOffset, -20.0f, 20.0f);
    ImGui::SliderInt("height", &height, 1, 50);
    ImGui::InputInt("Seed", &seed);
    ImGui::End();

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
