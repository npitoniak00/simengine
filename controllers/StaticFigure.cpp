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
#include "StaticFigure.hpp"

class VisualComponent;
class VCController;
class SimState;
class Bound;

using namespace std;

StaticFigure::StaticFigure(SimState *SimData_input,glm::vec3 pos_input) {
  cout << "init static figure" << endl;
  SimData = SimData_input;
  pos = pos_input;
}
StaticFigure::StaticFigure(SimState *SimData_input,glm::vec3 pos_input,glm::vec3 scale_vec) {
  cout << "init static figure" << endl;
  SimData = SimData_input;
  pos = pos_input;
  figure_scale = scale_vec;
}
void StaticFigure::set_scale(glm::vec3 scale_vec) {
  figure_scale = scale_vec;
}
vector<vector<int>> StaticFigure::get_figure_hitbox_bounds() {
  vector<vector<int>> ret_bounds;
  vector<int> x;
  vector<int> y;
  vector<int> z;
  x.push_back(pos[0]+0.5);
  x.push_back(pos[0]-0.5);
  ret_bounds.push_back(x);
  y.push_back(pos[1]+0.5);
  y.push_back(pos[1]-0.5);
  ret_bounds.push_back(y);
  z.push_back(pos[2]+0.5);
  z.push_back(pos[2]-0.5);
  ret_bounds.push_back(z);
  return ret_bounds;
}
glm::vec3 StaticFigure::get_position() {
  return pos;
}
void StaticFigure::add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, StaticFigure *parent_sf) {
  VCController::generic_add_VisualComponent(SimData,pos,vbo_index,vbo_index_vertex_count,name,parent_sf);
}
void StaticFigure::remove_VisualComponent(string name,SimState *SimData) {
  VCController::remove_VisualComponent(name,SimData);
}
StaticFigure::~StaticFigure() {
  cout << "destroying staticfigure" << endl;
}
vector<glm::mat4> StaticFigure::position_computation(SimState *SimData,VisualComponent *Vc) {
  glm::vec3 vcpos = Vc->get_position();
  glm::mat4 pMat = glm::perspective(1.0472f, 1.0f, 0.1f, 1000.0f);
  glm::mat4 pMatRotX = glm::rotate(pMat,SimData->get_camera_x_rot(),glm::vec3(1,0,0));
  glm::mat4 pMatRotY = glm::rotate(pMatRotX,SimData->get_camera_y_rot(),glm::vec3(0,1,0));
  glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(SimData->get_camera_position()[0],SimData->get_camera_position()[1],SimData->get_camera_position()[2]));
  glm::mat4 mMat = glm::translate(glm::mat4(1.0f), glm::vec3(vcpos[0],vcpos[1],vcpos[2]));
  mMat = glm::scale(mMat,StaticFigure::figure_scale);
  vector<glm::mat4> ret;
  ret.push_back(pMatRotY);
  ret.push_back(vMat);
  ret.push_back(mMat);
  return ret;
}
