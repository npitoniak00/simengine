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
  load_sphere_verticies();
}
float *VertexLoader::get_pyramid_verticies() { return &pyramid_verticies[0]; }
float *VertexLoader::get_cube_verticies() { return &cube_verticies[0]; }
float *VertexLoader::get_sphere_verticies() { return &sphere_verticies[0]; }
int VertexLoader::get_pyramid_verticies_len() { return 54; }
int VertexLoader::get_cube_verticies_len() { return 108; }
int VertexLoader::get_sphere_verticies_len() { return sphere_verticies_len; }
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
float VertexLoader::toRadians(float degrees) {
  return (degrees * 2.0f * 3.14159f) / 360.0f;
}
void VertexLoader::load_sphere_verticies() {
  int prec = 10;
  float mag = 0.3;
  float yi = 0;

  vector<vector<glm::vec3>> lvl_verts;
  vector<vector<glm::vec3>> bottom_verts;
  for(float xi = 0.05; xi < 1.0; xi += (1.0/(float)prec)) {
    vector<glm::vec3> lvl_vert;
    vector<glm::vec3> bottom_vert;
    float yi = pow(( 1.0 - pow(xi, 2) ), 0.5);
    mag = xi;
    for(float dir = 0; dir < 6.28; dir += (6.28 / (float)prec)) {
      lvl_vert.push_back(glm::vec3((mag * cos(dir)),yi,(mag * sin(dir))));
      bottom_vert.push_back(glm::vec3((mag * cos(dir)),yi*-1,(mag * sin(dir))));
    }
    lvl_verts.push_back(lvl_vert);
    bottom_verts.push_back(bottom_vert);
  }
  for(int y = bottom_verts.size()-1; y >= 0; y--) {
    lvl_verts.push_back(bottom_verts[y]);
  }
  vector<float> datums;
  // form triangles with vert lvl above
  for(int i = lvl_verts.size()-1; i > 0; i--) {
    for(int j = 0; j < lvl_verts[i].size(); j += 1) {
      for(int ii = 0; ii < 3; ii++) { datums.push_back(lvl_verts[i][j][ii]); }
      for(int ii = 0; ii < 3; ii++) { datums.push_back(lvl_verts[i][j+1][ii]); }
      for(int ii = 0; ii < 3; ii++) { datums.push_back(lvl_verts[i-1][j][ii]); }

      for(int ii = 0; ii < 3; ii++) { datums.push_back(lvl_verts[i-1][j][ii]); }
      for(int ii = 0; ii < 3; ii++) { datums.push_back(lvl_verts[i-1][j+1][ii]); }
      for(int ii = 0; ii < 3; ii++) { datums.push_back(lvl_verts[i][j+1][ii]); }
    }
  }
  sphere_verticies_len = datums.size();
  float *sphere_malloc = (float*) malloc((sphere_verticies_len) * sizeof(float));
  for(int i = 0; i < datums.size(); i++) {
    sphere_malloc[i] = datums[i];
  }
  sphere_verticies = &sphere_malloc[0];
  cout << "loading sphere verts of count: " << datums.size() << endl;
}
