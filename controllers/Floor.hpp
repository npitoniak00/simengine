#ifndef FLOOR_H_
#define FLOOR_H_

#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../SimState.hpp"
#include "../VisualComponent.hpp"
#include "../VCController.hpp"
#include "../Graph.hpp"
#include "../GraphNode.hpp"
#include "../Bound.hpp"

class VisualComponent;
class VCController;
class SimState;
class Bound;

using namespace std;

class Floor: public VCController {
private:
  glm::vec3 pos;
  SimState *SimData;
  glm::vec3 scale_dims;
public:
  Floor(SimState *SimData_input);
  void add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, Floor *parent_floor);
  void remove_VisualComponent(string name,SimState *SimData);
  vector<glm::mat4> position_computation(SimState *SimData,VisualComponent *Vc);
  ~Floor();
};

#endif
