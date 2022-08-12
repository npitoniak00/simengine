#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimState.hpp"
#include "VisualComponent.hpp"
#include "VCController.hpp"
#include "controllers/VarPosFigure.hpp"
#include "controllers/StaticFigure.hpp"
#include "controllers/Floor.hpp"
#include "controllers/Reticle.hpp"

class VisualComponent;
class SimState;
class VarPosFigure;
class StaticFigure;
class Floor;
class Reticle;

using namespace std;

VCController::VCController() {
  cout << "VCController init'd()" << endl;
}
void VCController::remove_VisualComponent(string name,SimState *SimData) {
  if(VisualComponents.find(name) == VisualComponents.end()) { return; }
  std::unordered_map<std::string,VisualComponent*>::const_iterator result = VisualComponents.find(name);
  VisualComponent *Vc = result->second;
  SimData->remove_VisualComponent(Vc);
  //Vc->~VisualComponent();
  //free(Vc);
}
unordered_map<string,VisualComponent*> VCController::get_VisualComponents() {
  return VisualComponents;
}
VCController::~VCController() {
  cout << "destroying VCController" << endl;
}
template <typename T> void VCController::generic_add_VisualComponent(SimState *SimData,glm::vec3 pos,int vbo_index,int vbo_index_vertex_count,string name,T t) {
  //cout << "reached template method" << endl;
  //cout << "vbo index: " << vbo_index << endl;
  //out << "vbo index vertex count: " << vbo_index_vertex_count << endl;
  //cout << "generic template param type: " << typeid(t).name() << endl;
  VisualComponent* Vc = (VisualComponent*) malloc(sizeof(VisualComponent));
  Vc = new (Vc) VisualComponent(pos,t);
  Vc->set_vbo_index(vbo_index);
  Vc->set_vertex_count(vbo_index_vertex_count);
  SimData->add_VisualComponent(Vc);
  VisualComponents.insert({name,Vc});
}
template void VCController::generic_add_VisualComponent<VarPosFigure*>(SimState *SimData,glm::vec3 pos,int vbo_index,int vbo_index_vertex_count,string name,VarPosFigure*);
template void VCController::generic_add_VisualComponent<Floor*>(SimState *SimData,glm::vec3 pos,int vbo_index,int vbo_index_vertex_count,string name,Floor*);
template void VCController::generic_add_VisualComponent<Reticle*>(SimState *SimData,glm::vec3 pos,int vbo_index,int vbo_index_vertex_count,string name,Reticle*);
template void VCController::generic_add_VisualComponent<StaticFigure*>(SimState *SimData,glm::vec3 pos,int vbo_index,int vbo_index_vertex_count,string name,StaticFigure*);
