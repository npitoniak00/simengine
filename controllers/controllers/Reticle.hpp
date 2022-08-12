#ifndef RETICLE_H_
#define RETICLE_H_
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

class Reticle: public VCController {
private:
  glm::vec3 pos;
  SimState *SimData;
public:
  Reticle(SimState *SimData_input);
  glm::vec3 get_position();
  void update_VisualComponent_positions();
  void add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, Reticle *parent_reticle);
  void remove_VisualComponent(string name,SimState *SimData);
  vector<glm::mat4> position_computation(SimState *SimData, VisualComponent *Vc);
  ~Reticle();
};

#endif
