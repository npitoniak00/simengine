#pragma once
#ifndef VCCONTROLLER_H_
#define VCCONTROLLER_H_
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimState.hpp"
#include "VisualComponent.hpp"
#include "controllers/VarPosFigure.hpp"

class VisualComponent;
class SimState;
class VarPosFigure;

using namespace std;

class VCController {
private:
  unordered_map<string,VisualComponent*> VisualComponents;
public:
  VCController();
  template < typename T> void generic_add_VisualComponent(SimState *SimData, glm::vec3 pos, int vbo_index, int vbo_index_vertex_count, string name, T t);
  void remove_VisualComponent(string name,SimState *SimData);
  unordered_map<string,VisualComponent*> get_VisualComponents();
  ~VCController();
};

#endif
