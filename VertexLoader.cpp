#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexLoader.hpp"

VertexLoader::VertexLoader() {
  cout << "vertex loader init'd()" << endl;
  load_cube_verticies();
  load_pyramid_verticies();
}
float *VertexLoader::get_pyramid_verticies() { return &pyramid_verticies[0]; }
float *VertexLoader::get_cube_verticies() { return &cube_verticies[0]; }
int VertexLoader::get_pyramid_verticies_len() { return 54; }
int VertexLoader::get_cube_verticies_len() { return 108; }
// simple curve anchors from negative to positive
vector<glm::vec3> VertexLoader::generate_simple_bezier(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3, int prec) {
  vector<glm::vec3> curve_points;
  // compute anchor lines
  // build line class, init via 2 points, generates points between them

  // use prec to calculate point pairs which each point will reside
  // use lines (lines.count == prec) to generate points on curve
  return curve_points;
}
void VertexLoader::load_cube_verticies() {
  float *cube_malloc = (float*) malloc(cube_verticies_len * sizeof(float));
  float cube[108] = {
      -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
  };
  for(int i = 0; i < 108; i++) { cube_malloc[i] = cube[i]; }
  cube_verticies = cube_malloc;
}
void VertexLoader::load_pyramid_verticies() {
  //pyramid_verticies = (float*) malloc(pyr_verticies_len * sizeof(float));
  float pyr[54] = {
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
  };
  pyramid_verticies = &pyr[0];
}
