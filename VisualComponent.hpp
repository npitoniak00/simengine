#pragma once

#ifndef VISUALCOMPONENT_H_
#define VISUALCOMPONENT_H_

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

using namespace std;

class VisualComponent {
  private:
    glm::vec3 pos;
    int vbo_index;
    int vertex_count;
    Reticle *parent_reticle = NULL;
    VarPosFigure *parent_vpf = NULL;
    StaticFigure *parent_sf = NULL;
    Floor *parent_floor = NULL;
    HUD *parent_hud = NULL;
  public:
    ~VisualComponent();
    VisualComponent(glm::vec3 input_pos);
    VisualComponent(glm::vec3 input_pos, VarPosFigure *vpf);
    VisualComponent(glm::vec3 input_pos, Floor *parent_floor);
    VisualComponent(glm::vec3 input_pos, Reticle *parent_reticle);
    VisualComponent(glm::vec3 input_pos, StaticFigure *parent_sf);
    VisualComponent(glm::vec3 input_pos, HUD *parent_hud);
    int get_vbo_index();
    void set_vbo_index(int vbo_index_input);
    int get_vertex_count();
    void set_vertex_count(int vertex_count_input);
    glm::vec3 get_position();
    VarPosFigure* get_parent();
    void set_position(glm::vec3 pos_input);
    VarPosFigure* get_VarPosFigure_parent();
    Floor* get_Floor_parent();
    HUD* get_hud_parent();
    Reticle* get_Reticle_parent();
    StaticFigure* get_StaticFigure_parent();
    string get_parent_type_str();
};

#endif
