#define GLM_ENABLE_EXPERIMENTAL
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
#include "glm/gtx/string_cast.hpp"

#include "../SimState.hpp"
#include "../VisualComponent.hpp"
#include "../VCController.hpp"
#include "../Graph.hpp"
#include "../Bound.hpp"
#include "../GraphNode.hpp"
#include "Reticle.hpp"

class VisualComponent;
class VCController;
class SimState;
class Bound;

using namespace std;

Reticle::Reticle(SimState *SimData_input) {
  SimData = SimData_input;
}
glm::vec3 Reticle::get_position() {
  glm::vec3 protagonist_position = SimData->get_protagonist_position();

  float reticle_y_rot_radians = SimData->get_reticle_y_rot();
  float reticle_x_rot_radians = SimData->get_reticle_x_rot();
  float camera_y_rot_radians = SimData->get_camera_y_rot();
  float camera_x_rot_radians = SimData->get_camera_x_rot();

  float zcalc = (sin(reticle_x_rot_radians) * cos(reticle_y_rot_radians-camera_y_rot_radians)) + protagonist_position[2];
  float xcalc = (sin(reticle_x_rot_radians) * sin(reticle_y_rot_radians-camera_y_rot_radians)) + protagonist_position[0];
  float ycalc = cos(reticle_x_rot_radians) + protagonist_position[1];

  return glm::vec3(xcalc,ycalc,zcalc);
}
void Reticle::add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, Reticle *parent_reticle) {
  VCController::generic_add_VisualComponent(SimData,pos,vbo_index,vbo_index_vertex_count,name,parent_reticle);
}
void Reticle::remove_VisualComponent(string name,SimState *SimData) {
  VCController::remove_VisualComponent(name,SimData);
}
void Reticle::update_VisualComponent_positions() {
  glm::vec3 prot_pos = SimData->get_protagonist_position();
  pos = glm::vec3(prot_pos[0],prot_pos[1],prot_pos[2]-1.0f);
  //cout << "updating visualcomponent position reticle: " << glm::to_string(pos) << endl;
  unordered_map<std::string,VisualComponent*> vcs = VCController::get_VisualComponents();
  vcs["target_indicator"]->set_position(pos);
}
Reticle::~Reticle() {
  cout << "destroying Reticle" << endl;
}
vector<glm::mat4> Reticle::position_computation(SimState *SimData,VisualComponent *Vc) {

  glm::vec3 protagonist_position = SimData->get_protagonist_position();

  float reticle_y_rot_radians = SimData->get_reticle_y_rot();
  float reticle_x_rot_radians = SimData->get_reticle_x_rot();
  float camera_y_rot_radians = SimData->get_camera_y_rot();
  float camera_x_rot_radians = SimData->get_camera_x_rot();

  float zcalc = (sin(reticle_x_rot_radians) * cos(reticle_y_rot_radians)) + protagonist_position[2];
  float xcalc = (sin(reticle_x_rot_radians) * sin(reticle_y_rot_radians)) + protagonist_position[0];
  float ycalc = cos(reticle_x_rot_radians) + protagonist_position[1];
  pos = glm::vec3(xcalc,ycalc,zcalc);

  glm::mat4 pMat = glm::perspective(1.0472f, 1.0f, 0.1f, 1000.0f);

  glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(SimData->get_camera_position()[0],SimData->get_camera_position()[1],SimData->get_camera_position()[2]));
  glm::mat4 mMat = glm::translate(glm::mat4(1.0f), glm::vec3(xcalc,ycalc,zcalc));
  mMat = glm::scale(mMat,glm::vec3(0.01,0.01f,0.01f));

  vector<glm::mat4> ret;
  ret.push_back(pMat);
  ret.push_back(vMat);
  ret.push_back(mMat);

  return ret;
}
