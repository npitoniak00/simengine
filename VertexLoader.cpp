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
float *VertexLoader::get_sphere_texts() { return &sphere_textures[0]; }
float *VertexLoader::get_cube_texts() { return &cube_textures[0]; }
float *VertexLoader::get_pyramid_verticies() { return &pyramid_verticies[0]; }
float *VertexLoader::get_cube_verticies() { return &cube_verticies[0]; }
float *VertexLoader::get_cube_normals() { return &cube_normals[0]; }
float *VertexLoader::get_sphere_verticies() { return &sphere_verticies[0]; }
float *VertexLoader::get_sphere_normals() { return &sphere_normals[0]; }
int VertexLoader::get_pyramid_verticies_len() { return 54; }
int VertexLoader::get_cube_verticies_len() { return 108; }
int VertexLoader::get_cube_normals_len() { return 108*2; }
int VertexLoader::get_sphere_verticies_len() { return sphere_verticies_len; }
int VertexLoader::get_sphere_normals_len() { return sphere_normals_len; }
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
  float *cnormal_malloc = (float*) malloc(cube_normals_len * sizeof(float));
  float *ctex_malloc = (float*) malloc((2*(cube_verticies_len / 3)) * sizeof(float));
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
  vector<glm::vec3> norms;
  for(int i = 0; i < 108; i++) { cube_malloc[i] = cube[i]; }
  for(int i = 0; i < 108; i += 9) {
    glm::vec3 v1 = glm::vec3(cube[i],cube[i+1],cube[i+2]);
    glm::vec3 v2 = glm::vec3(cube[i+3],cube[i+4],cube[i+5]);
    glm::vec3 v3 = glm::vec3(cube[i+6],cube[i+7],cube[i+8]);
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v1;
    //glm::vec3 cross = glm::normalize(glm::cross(e1,e2));
    glm::vec3 cross = glm::vec3(1.0,0.0,0.0);
    for(int j = 0; j < 3; j++) { norms.push_back(cross); norms.push_back(cross); }
  }
  int ni = 0;
  for(int i = 0; i < norms.size(); i++) {
    cnormal_malloc[ni] = norms[i][0]; ni++;
    cnormal_malloc[ni] = norms[i][1]; ni++;
    cnormal_malloc[ni] = norms[i][2]; ni++;
    cout << "cube normal: ("<< norms[i][0] <<","<< norms[i][1] <<","<< norms[i][2] <<")" << endl;
  }
cout << "post cube-normal" << endl;
 float triTexCords[] = {
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
  };
  int ctci = 0;
  for(int i = 0; i < (cube_verticies_len / 3); i++) {
    cout << "about to set ctxmalloc @  " << ctci << endl;
/*    if(i % 3 == 0) {
      ctex_malloc[ctci] = -1.0f; ctci++;
      ctex_malloc[ctci] = 0.0f; ctci++;
    } else if(i % 3 == 1) {
      ctex_malloc[ctci] = -1.0f; ctci++;
      ctex_malloc[ctci] = 1.0f; ctci++;
    } else {
      ctex_malloc[ctci] = 0.0f; ctci++;
      ctex_malloc[ctci] = 0.0f; ctci++;
    }*/
  }
  //for(int j = 0; j < 50; j++) {
  //  cout << "cube text post-load: " << ctex_malloc[j] << endl;
  //}
  cube_normals = cnormal_malloc;
  cube_verticies = cube_malloc;
  cube_textures = ctex_malloc;
}
void VertexLoader::load_pyramid_verticies() {
  // pyramid_verticies = (float*) malloc(pyr_verticies_len * sizeof(float));
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
  vector<glm::vec3> norms;
  int y = 0;
  for(int i = 0; i < datums.size(); i += 9) {
    glm::vec3 v1 = glm::vec3(datums[i],datums[i+1],datums[i+2]);
    glm::vec3 v2 = glm::vec3(datums[i+3],datums[i+4],datums[i+5]);
    glm::vec3 v3 = glm::vec3(datums[i+6],datums[i+7],datums[i+8]);
    glm::vec3 e1;
    glm::vec3 e2;
    if(y % 2 == 0) {
      e1 = v2 - v1;
      e2 = v3 - v1;
    } else {
      e2 = v2 - v1;
      e1 = v3 - v1;
    }
    glm::vec3 cross = glm::normalize(glm::cross(e1,e2));
    for(int j = 0; j < 3; j++) { norms.push_back(cross); }
    y += 1;
  }
  float *sphere_malloc = (float*) malloc((sphere_verticies_len) * sizeof(float));
  float *normal_malloc = (float*) malloc((sphere_verticies_len) * sizeof(float));
  float *stex_malloc = (float*) malloc(((sphere_verticies_len / 3)*2) * sizeof(float));
  int ni = 0;
  for(int i = 0; i < norms.size(); i++) {
    normal_malloc[ni] = norms[i][0]; ni++;
    normal_malloc[ni] = norms[i][1]; ni++;
    normal_malloc[ni] = norms[i][2]; ni++;
  }
  for(int i = 0; i < datums.size(); i++) {
    sphere_malloc[i] = datums[i];
  }
  int ctci = 0;
  int d = 0;
  for(int i = 0; i < (sphere_verticies_len / 3); i++) {
    if(d % 2 == 0) {
      if(i % 3 == 0) {
      stex_malloc[ctci] = 0.0f; ctci++;
      stex_malloc[ctci] = 0.0f; ctci++;
      } else if(i % 3 == 1) {
      stex_malloc[ctci] = 1.0f; ctci++;
      stex_malloc[ctci] = 0.0f; ctci++;
      } else {
      stex_malloc[ctci] = 1.0f; ctci++;
      stex_malloc[ctci] = 1.0f; ctci++;
      }
    } else {
      if(i % 3 == 0) {
      stex_malloc[ctci] = 0.0f; ctci++;
      stex_malloc[ctci] = 0.0f; ctci++;
      } else if(i % 3 == 1) {
      stex_malloc[ctci] = 0.0f; ctci++;
      stex_malloc[ctci] = 1.0f; ctci++;
      } else {
      stex_malloc[ctci] = 1.0f; ctci++;
      stex_malloc[ctci] = 1.0f; ctci++;
      }
    }
    d++;
  }
  sphere_verticies_len = datums.size();
  sphere_normals_len = datums.size();
  sphere_normals = &normal_malloc[0];
  sphere_verticies = &sphere_malloc[0];
  sphere_textures = &stex_malloc[0];

  cout << "loading sphere verts: " << datums.size() << endl;
  cout << "loading sphere vert tris: " << (datums.size() % 9) << endl;
}
