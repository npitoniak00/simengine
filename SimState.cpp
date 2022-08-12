#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <chrono>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimState.hpp"
#include "VisualComponent.hpp"
#include "Graph.hpp"
#include "GraphNode.hpp"

class VisualComponent;
class Graph;
class GraphNode;

SimState::SimState() {
  cout << "gamestate init'd()" << endl;
  timed_activities.sim_init = chrono::system_clock::now();
  timed_activities.last_fr_calc = chrono::system_clock::now();
}
int SimState::get_ms_since_init() {
  chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
  auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(timed_activities.sim_init - now);
  return (int)(milliseconds.count());
}
void SimState::update_frame_rate_calculations() {
  chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
  auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(timed_activities.last_fr_calc - now);
  float ms = (float)abs(milliseconds.count());
  timed_activities.last_fr_calc = now;
}
bool SimState::is_shot_available() {
  chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
  auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(timed_activities.last_shot - now);
  int msdelta = (int)milliseconds.count();
  if(abs(msdelta) < 100) { return false; }
  return true;
}
void SimState::set_last_shot_timestamp() {
  timed_activities.last_shot = chrono::system_clock::now();
}
void SimState::increment_shot_count() {
  SimState::shot_count++;
}
int SimState::get_shot_count() {
  return SimState::shot_count;
}
void SimState::test() {
  cout << "sin: " << sin(camera_y_rot) << endl;
  cout << "cos: " << cos(camera_y_rot) << endl;
}
void SimState::set_camera_position(glm::vec3 input_camera_pos) {
  camera_pos = input_camera_pos;
}
glm::vec3 SimState::get_camera_position() {
  return camera_pos;
}
void SimState::set_protagonist_position(glm::vec3 input_protagonist_pos) {
  protagonist_pos = input_protagonist_pos;
}
glm::vec3 SimState::get_protagonist_position() {
  return protagonist_pos;
}
void SimState::increment_position_from_input(glm::vec3 delta_pos) {
  camera_pos[0] += -delta_pos[0];
  camera_pos[1] += -delta_pos[1];
  camera_pos[2] += -delta_pos[2];
}
void SimState::increment_protagonist_position_from_input(glm::vec3 delta_pos) {
  protagonist_pos[0] += delta_pos[0];
  protagonist_pos[1] += delta_pos[1];
  protagonist_pos[2] += delta_pos[2];
}
void SimState::add_VisualComponent(VisualComponent *Vc) {
  VisualComponents.push_back(Vc);
}
void SimState::increment_camera_y_rot(float rot_inc) {
  camera_y_rot += rot_inc;
}
void SimState::increment_camera_x_rot(float rot_inc) {
  camera_x_rot += rot_inc;
}
void SimState::increment_reticle_y_rot(float rot_inc) {
  if((reticle_y_rot+rot_inc) > (init_reticle_y_rot + 0.4)) { SimState::increment_camera_y_rot(-rot_inc); return; }
  if((reticle_y_rot+rot_inc) < (init_reticle_y_rot - 0.4)) { SimState::increment_camera_y_rot(-rot_inc); return; }
  reticle_y_rot += rot_inc;
}
void SimState::increment_reticle_x_rot(float rot_inc) {
  if((reticle_x_rot + rot_inc) < (init_reticle_x_rot - 0.4)) { SimState::increment_camera_x_rot(rot_inc); return; }
  if((reticle_x_rot + rot_inc) > (init_reticle_x_rot + 0.4)) { SimState::increment_camera_x_rot(rot_inc); return; }
  reticle_x_rot += rot_inc;
}
float SimState::get_reticle_figure_y_rot() {
  return reticle_figure_y_rot;
}
float SimState::get_reticle_figure_x_rot() {
  return reticle_figure_x_rot;
}
float SimState::get_camera_y_rot() {
  return camera_y_rot;
}
float SimState::get_camera_x_rot() {
  return camera_x_rot;
}
float SimState::get_reticle_y_rot() {
  return reticle_y_rot;
}
float SimState::get_reticle_x_rot() {
  return reticle_x_rot;
}
void SimState::set_camera_y_rot(float camera_y_rot_input) {
  camera_y_rot = camera_y_rot_input;
}
void SimState::set_camera_x_rot(float camera_x_rot_input) {
  camera_x_rot = camera_x_rot_input;
}
void SimState::remove_VisualComponent(VisualComponent *Vc) {
  int match_index = -1;
  for(int i = 0; i < VisualComponents.size(); i++) {
    if(VisualComponents[i] == Vc) { match_index = i; }
  }
  if(match_index != -1) { VisualComponents.erase(std::next(VisualComponents.begin(),match_index)); }
}
void SimState::set_enviroment_figure_pos_graph(Graph *input_graph) {
  enviroment_figure_pos_graph = input_graph;
}
void SimState::set_figure_movement_graph_Bound(Bound *bound) {
  figure_movement_Bound = bound;
}
Bound* SimState::get_figure_movement_graph_Bound() {
  return figure_movement_Bound;
}
Graph* SimState::get_enviroment_figure_pos_graph() {
  return enviroment_figure_pos_graph;
}
vector<VisualComponent*> SimState::get_VisualComponents() {
  return VisualComponents;
}
void SimState::print_visual_components() {
  cout << "VISUAL COMPONENTS (len=" << VisualComponents.size() << ") ---" << endl;
  for(int i = 0; i < VisualComponents.size(); i++) {
    cout << VisualComponents[i] << endl;
  }
  cout << "--- END VISUAL COMPONENTS" << endl;
}
