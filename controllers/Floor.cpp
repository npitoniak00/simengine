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
#include "../VCController.hpp"
#include "../Bound.hpp"
#include "Floor.hpp"

class VCController;
class SimState;
class Bound;

using namespace std;

Floor::Floor(SimState *SimData_input) {
  SimData = SimData_input;
  Bound *graph_bounds = SimData->get_figure_movement_graph_Bound();
  vector<glm::vec3> rect_verticies = graph_bounds->get_rectangular_bounds();
  scale_dims = glm::vec3(rect_verticies[0][0]-rect_verticies[1][0],rect_verticies[0][1]-2.0f,rect_verticies[0][2]-rect_verticies[3][2]);
  glm::vec3 floor_rect_center = glm::vec3(rect_verticies[0][0]-((scale_dims[0])/(2.0f)),-0.5f,rect_verticies[0][2]-((scale_dims[2])/(2.0f)));
  pos = floor_rect_center;
}
void Floor::add_VisualComponent(SimState *SimData, int vbo_index, int vbo_index_vertex_count, string name, Floor *parent_floor) {
  VCController::generic_add_VisualComponent(SimData,pos,vbo_index,vbo_index_vertex_count,name,parent_floor);
}
void Floor::remove_VisualComponent(string name,SimState *SimData) {
  VCController::remove_VisualComponent(name,SimData);
}
vector<glm::mat4> Floor::position_computation(SimState *SimData,VisualComponent *Vc) {
  glm::vec3 vcpos = Vc->get_position();
  glm::mat4 pMat = glm::perspective(1.0472f, 1.0f, 0.1f, 1000.0f);
  glm::mat4 pMatRotX = glm::rotate(pMat,SimData->get_camera_x_rot(),glm::vec3(1,0,0));
  glm::mat4 pMatRotY = glm::rotate(pMatRotX,SimData->get_camera_y_rot(),glm::vec3(0,1,0));
  glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(SimData->get_camera_position()[0],SimData->get_camera_position()[1],SimData->get_camera_position()[2]));
  glm::mat4 mMat = glm::translate(glm::mat4(1.0f), glm::vec3(vcpos[0],vcpos[1],vcpos[2]));
  mMat = glm::scale(mMat,glm::vec3(scale_dims[0]/2.0f,0.5f,scale_dims[2]/2.0f));
  vector<glm::mat4> ret;
  ret.push_back(pMatRotY);
  ret.push_back(vMat);
  ret.push_back(mMat);
  return ret;
}
Floor::~Floor() {
  cout << "destroying floor" << endl;
}
