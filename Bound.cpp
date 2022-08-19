#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Bound.hpp"

using namespace std;

Bound::Bound() {
  cout << "Bound init'd" << endl;
}
Bound::Bound(glm::vec3 max_positive_corner,glm::vec3 dim_deltas,bool is_horizontal) {
  Bound::generate_rectangular_bounds(max_positive_corner,dim_deltas,is_horizontal);

}
vector<vector<int>> Bound::get_dim_bounds() {
  return bounds;
}
bool Bound::is_vec3_within_bounds(glm::vec3 input_vec3) {
  if(bounds.size() < 1) { return false; }
  if(input_vec3[0] <= bounds[0][1] && input_vec3[0] >= bounds[0][0]) {
    if(1 == 1 || (input_vec3[1] <= bounds[1][1] && input_vec3[1] >= bounds[1][0])) {
      if(input_vec3[2] <= bounds[2][1] && input_vec3[2] >= bounds[2][0]) {
        return true;
      }
    }
  }
  return false;
}
void Bound::generate_rectangular_bounds(glm::vec3 max_positive_corner,glm::vec3 dim_deltas,bool is_horizontal) {
  bounds.clear();
  is_horizontal = is_horizontal;
  if(dim_deltas[0] > 0) { dim_deltas[0] -= 1; }
  if(dim_deltas[1] > 0) { dim_deltas[1] -= 1; }
  if(dim_deltas[2] > 0) { dim_deltas[2] -= 1; }
  rectangular_bounds.clear();
  if(is_horizontal) { // horizontal rectangular bounds (ex: floor)
    rectangular_bounds.push_back(max_positive_corner);
    rectangular_bounds.push_back(glm::vec3((float)max_positive_corner[0]-dim_deltas[0],(float)max_positive_corner[1],(float)max_positive_corner[2]));
    rectangular_bounds.push_back(glm::vec3((float)max_positive_corner[0]-dim_deltas[0],(float)max_positive_corner[1],(float)max_positive_corner[2]-dim_deltas[2]));
    rectangular_bounds.push_back(glm::vec3((float)max_positive_corner[0],(float)max_positive_corner[1],(float)max_positive_corner[2]-dim_deltas[2]));
  } else { // vertical rectangular bounds (ex: wall)
    rectangular_bounds.push_back(max_positive_corner);
    rectangular_bounds.push_back(glm::vec3((float)max_positive_corner[0],(float)max_positive_corner[1],(float)max_positive_corner[2]-dim_deltas[2]));
    rectangular_bounds.push_back(glm::vec3((float)max_positive_corner[0],(float)max_positive_corner[1]-dim_deltas[1],(float)max_positive_corner[2]-dim_deltas[2]));
    rectangular_bounds.push_back(glm::vec3((float)max_positive_corner[0],(float)max_positive_corner[1]-dim_deltas[1],(float)max_positive_corner[2]));
  }
  Bound::compute_limits();
}
void Bound::compute_limits() {
  if(is_horizontal) {
    vector<int> x_limits;
    x_limits.push_back(rectangular_bounds[1][0]);
    x_limits.push_back(rectangular_bounds[0][0]);
    bounds.push_back(x_limits);
    vector<int> y_limits;
    y_limits.push_back(rectangular_bounds[0][1]);
    y_limits.push_back(rectangular_bounds[0][1]);
    bounds.push_back(y_limits);
    vector<int> z_limits;
    z_limits.push_back(rectangular_bounds[3][2]);
    z_limits.push_back(rectangular_bounds[0][2]);
    bounds.push_back(z_limits);
  } else {
    vector<int> x_limits;
    x_limits.push_back(rectangular_bounds[3][0]);
    x_limits.push_back(rectangular_bounds[0][0]);
    bounds.push_back(x_limits);
    vector<int> y_limits;
    y_limits.push_back(rectangular_bounds[0][1]);
    y_limits.push_back(rectangular_bounds[3][1]);
    bounds.push_back(y_limits);
    vector<int> z_limits;
    z_limits.push_back(rectangular_bounds[1][2]);
    z_limits.push_back(rectangular_bounds[0][2]);
    bounds.push_back(z_limits);
  }
}
glm::vec3 Bound::generate_random_inclusive_position(float padding) {
  int xlo = (int)(bounds[0][0]);
  int xhi = (int)(bounds[0][1]);
  int ylo = (int)(bounds[1][0]);
  int yhi = (int)(bounds[1][1]);
  int zlo = (int)(bounds[2][0]);
  int zhi = (int)(bounds[2][1]);

  if(xlo != xhi) {
    xlo = (xlo > 0) ? xlo -= padding : xlo += padding;
    xhi = (xhi > 0) ? xhi -= padding : xhi += padding;
  }
  if(ylo != yhi) {
    ylo = (ylo > 0) ? ylo -= padding : ylo += padding;
    yhi = (yhi > 0) ? yhi -= padding : yhi += padding;
  }
  if(zlo != zhi) {
    zlo = (zlo > 0) ? zlo -= padding : zlo += padding;
    zhi = (zhi > 0) ? zhi -= padding : zhi += padding;
  }

  int xx,yy,zz;
  if(xhi != xlo) { xx = rand() % (xhi-xlo) + xlo; } else { xx = xhi; }
  if(yhi != ylo) { yy = rand() % (yhi-ylo) + ylo; } else { yy = yhi; }
  if(zhi != zlo) { zz = rand() % (zhi-zlo) + zlo; } else { zz = zhi; }
  return glm::vec3(xx,yy,zz);
}
vector<glm::vec3> Bound::get_rectangular_bounds() {
  return rectangular_bounds;
}
Bound::~Bound() {
  cout << "Bound destruction" << endl;
}
