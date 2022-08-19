#ifndef ENVIROMENT_H_
#define ENVIROMENT_H_

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
#include "PlotContext.hpp"
#include "controllers/VarPosFigure.hpp"
#include "controllers/StaticFigure.hpp"
#include "controllers/Reticle.hpp"
#include "controllers/Floor.hpp"
#include "controllers/HUD.hpp"

class Floor;
class HUD;
class VarPosFigure;
class VisualComponent;
class SimState;
class Reticle;
class PlotContext;

class Enviroment {
private:
  Graph *enviroment_figure_pos_graph = NULL;
  vector<VarPosFigure*> varposfigures;
  vector<StaticFigure*> staticfigures;
  vector<Floor*> floors;
  VarPosFigure *protagonist = NULL;
  Reticle *reticle = NULL;
  HUD *protagonist_HUD = NULL;
  SimState *SimData;
  PlotContext *GameContext;
public:
  void draw_hud();
  Enviroment(SimState *SimData_input);
  Enviroment(SimState *SimData_input,PlotContext *GameContext);
  VarPosFigure* get_protagonist();
  void react_to_PlotContext();
  void execute_shot(glm::vec3 camera_pos,glm::vec3 reticle_pos);
  void free_varposfigure_mem(VarPosFigure *vpf);
  void free_reticle();
  void free_floor(Floor *floor_input);
  void free_staticfigure(StaticFigure *sf);
  vector<glm::vec3> compute_linear_shot_indecies(glm::vec3 camera_pos,glm::vec3 reticle_pos,int shot_vector_analysis_index_count,float index_interval_dist);
  void simulate_varposfigure_death(VarPosFigure *vpf);
  glm::vec3 compute_next_point(glm::vec3 p1,glm::vec3 p2,float dist);
  vector<VarPosFigure*> get_shot_collisions(vector<glm::vec3> shot_indecies);
  float get_delxta_distance(glm::vec3 p1,glm::vec3 p2);
  void set_enviroment_figure_pos_graph(Graph *fig_movement_graph);
  void init_VarPosFigure();
  void init_StaticFigure(glm::vec3 input_pos,glm::vec3 scale_vec);
  void init_figure_Floor();
  void init_protagonist();
  void init_Reticle();
  void init_HUD();
  float get_delta_distance(glm::vec3 p1,glm::vec3 p2);
  void process_antagonist_attacks_on_protagonist();
  Reticle* get_reticle();
  void update_reticle_pos();
  void remove_VarPosFigure(VarPosFigure *fp);
  void remove_StaticFigure(StaticFigure *sf);
  void update_protagonist_figure_pos();
  void compute_VarPosFigure_paths();
  void iterate_VarPosFigure_positions();
  ~Enviroment();
};

#endif
