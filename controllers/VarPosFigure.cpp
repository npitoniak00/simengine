#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <typeinfo>
#include <string>
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
#include "../Bound.hpp"
#include "../GraphNode.hpp"
#include "../DamageRecord.hpp"
#include "VarPosFigure.hpp"

class VisualComponent;
class VCController;
class DamageRecord;
class SimState;
class Bound;

using namespace std;

VarPosFigure::VarPosFigure(SimState *SimData_input) {
  SimData = SimData_input;
  Bound *h = SimData->get_figure_movement_graph_Bound();
  glm::vec3 rp = h->generate_random_inclusive_position(0);
  pos = rp;

  DamageRecord* DamageMem = (DamageRecord*) malloc(sizeof(DamageRecord));
  DamageMem = new (DamageMem) DamageRecord(50.0);
  DamageData = DamageMem;
}
VarPosFigure::VarPosFigure(SimState *SimData_input, VarPosFigure *vpf_input) {
  vpf_target = vpf_input;

  SimData = SimData_input;
  Bound *floor = SimData->get_figure_movement_graph_Bound();
  glm::vec3 rp = floor->generate_random_inclusive_position(0);
  pos = rp;

  DamageRecord* DamageMem = (DamageRecord*) malloc(sizeof(DamageRecord));
  DamageMem = new (DamageMem) DamageRecord(50.0);
  DamageData = DamageMem;
}
void VarPosFigure::inflict_damage(float damage_delta) {
  if(DamageData == NULL) { return; }
  DamageData->increment_damage_record(damage_delta);
}
void VarPosFigure::set_vpf_target(VarPosFigure *input_vpf) {
  vpf_target = input_vpf;
}
void VarPosFigure::attack_vpf_target() {
  if(vpf_target == NULL) { return; }
  glm::vec3 target_pos = vpf_target->get_position();
  float dist = sqrt(pow((pos[0]-target_pos[0]),2) + pow((pos[1]-target_pos[1]),2) + pow((pos[2]-target_pos[2]),2));
  if(dist < 2.0) { vpf_target->inflict_damage(-1.0); }
}
bool VarPosFigure::is_dead() {
  if(DamageData == NULL) { return false; }
  if(DamageData->get_damage_record() <= 0) { return true; }
  return false;
}
float VarPosFigure::get_damage() {
  if(DamageData == NULL) { return -1.0; }
  return DamageData->get_damage_record();
}
vector<vector<int>> VarPosFigure::get_figure_hitbox_bounds() {
  vector<vector<int>> ret_bounds;
  vector<int> x;
  vector<int> y;
  vector<int> z;
  x.push_back(pos[0]-1.0);
  x.push_back(pos[0]+1.0);
  ret_bounds.push_back(x);
  y.push_back(pos[1]-1.0);
  y.push_back(pos[1]+1.0);
  ret_bounds.push_back(y);
  z.push_back(pos[2]-1.0);
  z.push_back(pos[2]+1.0);
  ret_bounds.push_back(z);
  return ret_bounds;
}
glm::vec3 VarPosFigure::get_position() {
  return pos;
}
void VarPosFigure::compute_path_to_target(SimState *SimDataInput) {
  Graph *pos_graph = SimDataInput->get_enviroment_figure_pos_graph();
  GraphNode *u = pos_graph->get_node_approximating_position(glm::vec3((float)pos[0],(float)pos[1],(float)pos[2]));
  Bound *h = SimData->get_figure_movement_graph_Bound();
  glm::vec3 cam_pos = SimDataInput->get_camera_position();
  glm::vec3 protag_loc = SimDataInput->get_protagonist_position();
  GraphNode *t = pos_graph->get_node_approximating_position(protag_loc);
  pos_graph->dfs(u,t);
  vector<GraphNode*> gn_path = pos_graph->get_shortest_path_node_sequence(t);
  pos_seq.clear();
  pos_seq = pos_graph->compute_precision_path_between_positions_for_nodeset(SimData->config.movement_path_node_precision,gn_path);
  pos_it = 0;
}
bool VarPosFigure::is_pos_seq_path_exhausted() {
  if(pos_seq.size() - 1 == pos_it) { return 1; }
  return 0;
}
void VarPosFigure::increment_vc_pos() {
  if(pos_seq.size() == 0 || pos_it >= pos_seq.size() - 2) { return; }
  unordered_map<std::string,VisualComponent*> vcs = VCController::get_VisualComponents();
  vcs["sprite"]->set_position(pos_seq[pos_it]);
  pos = pos_seq[pos_it];
  pos_it++;
}
void VarPosFigure::increment_vc_pos(glm::vec3 input_pos) {
  unordered_map<std::string,VisualComponent*> vcs = VCController::get_VisualComponents();
  vcs["sprite"]->set_position(input_pos);
  pos = input_pos;
}
void VarPosFigure::add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, VarPosFigure *parent_vpf) {
  VCController::generic_add_VisualComponent(SimData,pos,vbo_index,vbo_index_vertex_count,name,parent_vpf);
}
void VarPosFigure::remove_VisualComponent(string name,SimState *SimData) {
  VCController::remove_VisualComponent(name,SimData);
}
VarPosFigure::~VarPosFigure() {
  if(DamageData != NULL) { free(DamageData); }
}
vector<glm::mat4> VarPosFigure::position_computation(SimState *SimData,VisualComponent *Vc) {
  glm::vec3 vcpos = Vc->get_position();
  glm::mat4 pMat = glm::perspective(1.0472f, 1.0f, 0.1f, 1000.0f);
  glm::mat4 pMatRotX = glm::rotate(pMat,SimData->get_camera_x_rot(),glm::vec3(1,0,0));
  glm::mat4 pMatRotY = glm::rotate(pMatRotX,SimData->get_camera_y_rot(),glm::vec3(0,1,0));
  glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(SimData->get_camera_position()[0],SimData->get_camera_position()[1],SimData->get_camera_position()[2]));
  glm::mat4 mMat = glm::translate(glm::mat4(1.0f), glm::vec3(vcpos[0],vcpos[1],vcpos[2]));
  mMat = glm::scale(mMat,glm::vec3(1.0,1.0f,1.0f));
  vector<glm::mat4> ret;
  ret.push_back(pMatRotY);
  ret.push_back(vMat);
  ret.push_back(mMat);
  return ret;
}
