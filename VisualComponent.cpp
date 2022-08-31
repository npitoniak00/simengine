#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VisualComponent.hpp"
#include "SimState.hpp"
#include "controllers/VarPosFigure.hpp"
#include "controllers/StaticFigure.hpp"
#include "controllers/Floor.hpp"
#include "controllers/Reticle.hpp"
#include "controllers/HUD.hpp"

class SimState;
class VarPosFigure;
class Reticle;
class HUD;

VisualComponent::VisualComponent(glm::vec3 input_pos) {
  pos = input_pos;
}
VisualComponent::VisualComponent(glm::vec3 input_pos, VarPosFigure *vpf) {
  cout << "visualcomponent() vpf " << vpf << endl;
  pos = input_pos;
  parent_vpf = vpf;
}
VisualComponent::VisualComponent(glm::vec3 input_pos, Floor *pf) {
  cout << "visualcomponent() floor " << pf << endl;
  pos = input_pos;
  parent_floor = pf;
}
VisualComponent::VisualComponent(glm::vec3 input_pos, Reticle *pr) {
  cout << "visualcomponent() reticle " << pr << endl;
  pos = input_pos;
  parent_reticle = pr;
}
VisualComponent::VisualComponent(glm::vec3 input_pos, StaticFigure *psf) {
  cout << "visualcomponent() staticfigure " << psf << endl;
  pos = input_pos;
  parent_sf = psf;
}
VisualComponent::VisualComponent(glm::vec3 input_pos, HUD *phud) {
  cout << "visualcomponent() HUD " << phud << endl;
  pos = input_pos;
  parent_hud = phud;
}
VisualComponent::~VisualComponent() {
  cout << "destroying VisualComponent" << endl;
}
string VisualComponent::get_parent_type_str() {
  if(parent_floor != NULL) { return string("Floor"); }
  if(parent_vpf != NULL) { return string("VarPosFigure"); }
  if(parent_reticle != NULL) { return string("Reticle"); }
  if(parent_sf != NULL) { return string("StaticFigure"); }
  if(parent_hud != NULL) { return string("HUD"); }
  return string("VarPosFigure");
}
Floor* VisualComponent::get_Floor_parent() {
  return parent_floor;
}
VarPosFigure* VisualComponent::get_VarPosFigure_parent() {
  return parent_vpf;
}
Reticle* VisualComponent::get_Reticle_parent() {
  return parent_reticle;
}
StaticFigure* VisualComponent::get_StaticFigure_parent() {
  return parent_sf;
}
VarPosFigure* VisualComponent::get_parent() {
  return parent_vpf;
}
HUD* VisualComponent::get_hud_parent() {
  return parent_hud;
}
void VisualComponent::set_vertex_count(int vertex_count_input) {
  vertex_count = vertex_count_input;
}
int VisualComponent::get_vertex_count() {
  return vertex_count;
}
int VisualComponent::get_normal_count() {
  return normal_count;
}
void VisualComponent::set_normal_count(int normal_count_input) {
  normal_count = normal_count_input;
}
void VisualComponent::set_vbo_index(int vbo_index_input) {
  vbo_index = vbo_index_input;
}
int VisualComponent::get_vbo_index() {
  return vbo_index;
}
void VisualComponent::set_position(glm::vec3 pos_input) {
  pos = pos_input;
}
glm::vec3 VisualComponent::get_position() {
  return pos;
}
