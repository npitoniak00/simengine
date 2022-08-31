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
#include "HUD.hpp"

class VisualComponent;
class VCController;
class SimState;
class Bound;

using namespace std;

//  private
//  glm::vec3 pos;
//  SimState *SimData;

HUD::HUD(SimState *SimData_input) {
  SimData = SimData_input;
}
glm::vec3 HUD::get_position() {
  return pos;
}
void HUD::update_VisualComponent_positions() {

  glm::vec3 protagonist_position = SimData->get_protagonist_position();

  float camera_y_rot_radians = (float)(SimData->get_camera_y_rot());
  float camera_x_rot_radians = (float)(SimData->get_camera_x_rot());

  glm::vec3 hudpos = glm::vec3(protagonist_position[0]+(sin(camera_y_rot_radians)*cos(camera_x_rot_radians)),protagonist_position[1]-sin(camera_x_rot_radians),protagonist_position[2]-(cos(camera_y_rot_radians)*cos(camera_x_rot_radians)));
  unordered_map<std::string,VisualComponent*> vcs = VCController::get_VisualComponents();
  vcs["protagonist_HUD"]->set_position(pos);
}
void HUD::add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, HUD *parent_hud) {
  VCController::generic_add_VisualComponent(SimData,pos,vbo_index,vbo_index_vertex_count,name,parent_hud);
}
void HUD::remove_VisualComponent(string name,SimState *SimData) {
  VCController::remove_VisualComponent(name,SimData);
}
vector<glm::mat4> HUD::position_computation(SimState *SimData, VisualComponent *Vc) {

  glm::vec3 protagonist_position = SimData->get_protagonist_position();

  float camera_y_rot_radians = (float)(SimData->get_camera_y_rot());
  float camera_x_rot_radians = (float)(SimData->get_camera_x_rot());

  glm::vec3 hudpos = glm::vec3(protagonist_position[0]+(sin(camera_y_rot_radians)*cos(camera_x_rot_radians)),protagonist_position[1]-sin(camera_x_rot_radians),protagonist_position[2]-(cos(camera_y_rot_radians)*cos(camera_x_rot_radians)));
  glm::mat4 pMat = glm::perspective(1.0472f, 1.0f, 0.1f, 1000.0f);
  glm::mat4 pMatRotX = glm::rotate(pMat,SimData->get_camera_x_rot(),glm::vec3(1,0,0));
  glm::mat4 pMatRotY = glm::rotate(pMatRotX,SimData->get_camera_y_rot(),glm::vec3(0,1,0));

  glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(SimData->get_camera_position()[0],SimData->get_camera_position()[1],SimData->get_camera_position()[2]));
  glm::mat4 mMat = glm::translate(glm::mat4(1.0f), hudpos);

  mMat = glm::rotate(mMat,-camera_y_rot_radians,glm::vec3(0,1,0));
  mMat = glm::rotate(mMat,-camera_x_rot_radians,glm::vec3(1,0,0));
  mMat = glm::scale(mMat,glm::vec3(0.25f,0.25f,0.25f));

  vector<glm::mat4> ret;
  ret.push_back(pMatRotY);
  ret.push_back(vMat);
  ret.push_back(mMat);

  return ret;
}
HUD::~HUD() {
  cout << "destroying HUD" << endl;
}
