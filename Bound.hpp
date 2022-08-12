#ifndef BOUND_H_
#define BOUND_H_

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

using namespace std;

class Bound {
private:
  bool is_horizontal;
  vector<glm::vec3> rectangular_bounds; // clockwise corner-wise starting with most positive corner
  vector<vector<int>> bounds;
public:
  Bound();
  void generate_rectangular_bounds(glm::vec3 max_positive_corner,glm::vec3 dim_deltas,bool is_horizontal);
  bool is_vec3_within_bounds(glm::vec3 input_vec3);
  vector<glm::vec3> get_rectangular_bounds();
  vector<vector<int>> get_dim_bounds();
  glm::vec3 generate_random_inclusive_position();
  ~Bound();
};

#endif
