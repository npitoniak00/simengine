#ifndef VARPOSFIGURE_H_
#define VARPOSFIGURE_H_
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
#include "../DamageRecord.hpp"
#include "../VisualComponent.hpp"
#include "../VCController.hpp"
#include "../Graph.hpp"
#include "../GraphNode.hpp"
#include "../Bound.hpp"

class VisualComponent;
class VCController;
class DamageRecord;
class SimState;
class Bound;

using namespace std;
class VarPosFigure: public VCController {
private:
  glm::vec3 pos;
  vector<glm::vec3> pos_seq;
  int pos_it;
  SimState *SimData;
  DamageRecord *DamageData;
  VarPosFigure *vpf_target = NULL;
public:
  VarPosFigure(SimState *SimData_input);
  VarPosFigure(SimState *SimData_input,VarPosFigure *vpf_target);
  void inflict_damage(float damage_delta);
  void set_vpf_target(VarPosFigure *vpf_target);
  void attack_vpf_target();
  float get_damage();
  bool is_dead();
  bool is_pos_seq_path_exhausted();
  glm::vec3 get_position();
  vector<vector<int>> get_figure_hitbox_bounds();
  void add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, VarPosFigure *vpf);
  void remove_VisualComponent(string name,SimState *SimData);
  void increment_vc_pos();
  void increment_vc_pos(glm::vec3 input_pos);
  void compute_path_to_target(SimState *SimDataInput);
  vector<glm::mat4> position_computation(SimState *SimData,VisualComponent *Vc);
  ~VarPosFigure();
};

#endif
