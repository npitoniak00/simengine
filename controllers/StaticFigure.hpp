#ifndef STATICFIGURE_H_
#define STATICFIGURE_H_
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

class StaticFigure: public VCController {
private:
  glm::vec3 pos;
  glm::vec3 figure_scale = glm::vec3(1.0f,1.0f,1.0f);
  SimState *SimData;
public:
  StaticFigure(SimState *SimData_input,glm::vec3 pos);
  StaticFigure(SimState *SimData_input,glm::vec3 pos,glm::vec3 scale_vec);
  glm::vec3 get_position();
  void set_scale(glm::vec3 scale_vec);
  vector<vector<int>> get_figure_hitbox_bounds();
  void add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, StaticFigure *sf);
  void remove_VisualComponent(string name,SimState *SimData);
  vector<glm::mat4> position_computation(SimState *SimData,VisualComponent *Vc);
  ~StaticFigure();
};

#endif
