#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

#include "SimState.hpp"
#include "PlotContext.hpp"
#include "Enviroment.hpp"
#include "controllers/VarPosFigure.hpp"
#include "controllers/StaticFigure.hpp"
#include "controllers/Floor.hpp"
#include "GraphNode.hpp"
#include "Graph.hpp"
#include "Bound.hpp"

class Floor;
class VarPosFigure;
class SimState;
class GraphNode;
class PlotContext;
class Graph;
class Bound;

Enviroment::Enviroment(SimState *SimData_input) {
  SimData = SimData_input;
  enviroment_figure_pos_graph = SimData->get_enviroment_figure_pos_graph();
}
Enviroment::Enviroment(SimState *SimData_input,PlotContext *PlotContext_input) {
  GameContext = PlotContext_input;

  SimData = SimData_input;
  enviroment_figure_pos_graph = SimData->get_enviroment_figure_pos_graph();
}
void Enviroment::react_to_PlotContext() {
  if(GameContext == NULL) { return; }
  int rendered_antagonists = varposfigures.size();
  if(GameContext->should_render_additional_antagonist(rendered_antagonists) && rendered_antagonists == varposfigures.size()) {
    init_VarPosFigure();
  }
}
void Enviroment::remove_VarPosFigure(VarPosFigure* fp) {
  vector<VarPosFigure*> vpfs;
  for(int i = 0; i < varposfigures.size(); i++) {
    if(varposfigures[i] != fp) { vpfs.push_back(varposfigures[i]); }
  }
  Enviroment::varposfigures = vpfs;
}
void Enviroment::remove_StaticFigure(StaticFigure* sf) {
  vector<StaticFigure*> sfs;
  for(int i = 0; i < staticfigures.size(); i++) {
    if(staticfigures[i] != sf) { sfs.push_back(staticfigures[i]); }
  }
  Enviroment::staticfigures = sfs;
}
VarPosFigure* Enviroment::get_protagonist() {
  return protagonist;
}
Reticle* Enviroment::get_reticle() {
  return reticle;
}
void Enviroment::process_antagonist_attacks_on_protagonist() {
  if(protagonist == NULL) { return; }
  glm::vec3 proto_pos = protagonist->get_position();
  for(int i = 0; i < varposfigures.size(); i++) { varposfigures[i]->attack_vpf_target(); }
}
void Enviroment::free_staticfigure(StaticFigure *sf) {
  remove_StaticFigure(sf);
  free(sf);
}
void Enviroment::simulate_varposfigure_death(VarPosFigure *vpf) {
  free_varposfigure_mem(vpf);
  GameContext->increment_terminated_antagonists();
}
void Enviroment::free_varposfigure_mem(VarPosFigure *vpf) {
  vpf->remove_VisualComponent(string("sprite"),SimData);
  remove_VarPosFigure(vpf);
  free(vpf);
}
glm::vec3 Enviroment::compute_next_point(glm::vec3 p1,glm::vec3 p2,float dist) {
  glm::vec3 v1 = glm::vec3(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
  float vlen = sqrt(pow(v1[0],2) + pow(v1[1],2) + pow(v1[2],2));
  glm::vec3 v2 = glm::vec3(v1[0]/vlen,v1[1]/vlen,v1[2]/vlen);
  glm::vec3 vd = glm::vec3(dist*v1[0],dist*v1[1],dist*v1[2]);
  glm::vec3 v3 = glm::vec3(p2[0]+vd[0],p2[1]+vd[1],p2[2]+vd[2]);
  return v3;
}
float Enviroment::get_delta_distance(glm::vec3 p1,glm::vec3 p2) {
  return sqrt(pow((p2[0]-p1[0]),2) + pow((p2[1]-p1[1]),2) + pow((p2[2]-p1[2]),2));
}
vector<VarPosFigure*> Enviroment::get_shot_collisions(vector<glm::vec3> shot_indecies) {
  vector<vector<vector<int>>> figure_bounds;
  vector<VarPosFigure*> hits;
  for(int fi = 0; fi < varposfigures.size(); fi++) {
    figure_bounds.push_back(varposfigures[fi]->get_figure_hitbox_bounds());
  }
  int xpass = 0;
  int zpass = 0;
  for(int si = 0; si < shot_indecies.size(); si++) {
    for(int fi = 0; fi < figure_bounds.size(); fi++) {
      if(figure_bounds[fi][0][0] > shot_indecies[si][0] || figure_bounds[fi][0][1] < shot_indecies[si][0]) { continue; }
      xpass++;
      if(figure_bounds[fi][2][0] > shot_indecies[si][2] || figure_bounds[fi][2][1] < shot_indecies[si][2]) { continue; }
      zpass++;
      int exists = 0;
      for(int i = 0; i < hits.size(); i++) { if(hits[i] == varposfigures[fi]) { exists = 1; } }
      if(!exists) { hits.push_back(varposfigures[fi]); }
    }
  }
  return hits;
}
vector<glm::vec3> Enviroment::compute_linear_shot_indecies(glm::vec3 camera_pos,glm::vec3 reticle_pos,int shot_vector_analysis_index_count,float index_interval_dist) {
  vector<glm::vec3> shot_vector_indecies;
  // cout << "reticle pos from env: " << glm::to_string(reticle->get_position()) << endl;
  shot_vector_indecies.push_back(camera_pos);
  shot_vector_indecies.push_back(reticle_pos);
  // forecast points ahead of reticle
  for(int index_inc = 0; index_inc < shot_vector_analysis_index_count; index_inc++) {
    int svs = shot_vector_indecies.size();
    glm::vec3 tmp = Enviroment::compute_next_point(shot_vector_indecies[svs-2],shot_vector_indecies[svs-1],index_interval_dist);
    shot_vector_indecies.push_back(tmp);
  }
  return shot_vector_indecies;
}
void Enviroment::execute_shot(glm::vec3 camera_pos,glm::vec3 reticle_pos) {
  SimData->set_last_shot_timestamp();
  vector<glm::vec3> shot_points = Enviroment::compute_linear_shot_indecies(camera_pos,reticle_pos,100,1.0);
  // for(int i = 0; i < shot_points.size(); i++) { Enviroment::init_StaticFigure(shot_points[i]); }
  vector<VarPosFigure*> shot_figure_collisions = Enviroment::get_shot_collisions(shot_points);
  if(shot_figure_collisions.size() > 0) { cout << "FIGURE SHOT" << endl; }
  for(int i = 0; i < shot_figure_collisions.size(); i++) {
    shot_figure_collisions[i]->inflict_damage(-30.0);
    if(shot_figure_collisions[i]->is_dead()) {
      simulate_varposfigure_death(shot_figure_collisions[i]);
    }
  }
}
void Enviroment::update_protagonist_figure_pos() {
  if(protagonist == NULL) { return; }
  glm::vec3 pos = SimData->get_protagonist_position();
  protagonist->increment_vc_pos(pos);
  SimData->set_camera_position(glm::vec3(-pos[0],-pos[1],-pos[2]));
}
void Enviroment::update_reticle_pos() {
  if(reticle == NULL) { return; }
  reticle->update_VisualComponent_positions();
}
void Enviroment::init_figure_Floor() {
  Floor* FloorMem = (Floor*) malloc(sizeof(Floor));
  FloorMem = new (FloorMem) Floor(SimData);
  FloorMem->add_VisualComponent(SimData,0,36,string("figure_floor"),FloorMem);
  floors.push_back(FloorMem);
}
void Enviroment::init_Reticle() {
  Reticle* ReticleMem = (Reticle*) malloc(sizeof(Reticle));
  ReticleMem = new (ReticleMem) Reticle(SimData);
  ReticleMem->add_VisualComponent(SimData,0,36,string("target_indicator"),ReticleMem);
  ReticleMem->update_VisualComponent_positions();
  reticle = ReticleMem;
}
void Enviroment::free_floor(Floor *floor_input) {
  vector<Floor*> floor_renderings;
  for(int i = 0; i < floors.size(); i++) {
    if(floor_input != floors[i]) { floor_renderings.push_back(floors[i]); }
  }
  floors = floor_renderings;
  free(floor_input);
}
void Enviroment::free_reticle() {
  if(reticle == NULL) { return; }
  free(reticle);
}
void Enviroment::init_protagonist() {
  VarPosFigure* FigureMem = (VarPosFigure*) malloc(sizeof(VarPosFigure));
  FigureMem = new (FigureMem) VarPosFigure(SimData);

  string sc("sprite");
  FigureMem->add_VisualComponent(SimData,0,36,sc,FigureMem);
  protagonist = FigureMem;

  glm::vec3 random_target = SimData->get_figure_movement_graph_Bound()->generate_random_inclusive_position(); 
  SimData->set_protagonist_position(random_target);
}
void Enviroment::init_VarPosFigure() {
  VarPosFigure* FigureMem = (VarPosFigure*) malloc(sizeof(VarPosFigure));
  FigureMem = new (FigureMem) VarPosFigure(SimData);

  string sc("sprite");
  FigureMem->add_VisualComponent(SimData,0,36,sc,FigureMem);
  if(protagonist != NULL) { FigureMem->set_vpf_target(protagonist); }
  varposfigures.push_back(FigureMem);
}
void Enviroment::init_StaticFigure(glm::vec3 input_pos) {
  glm::vec3 pos = input_pos;
  StaticFigure* FigureMem = (StaticFigure*) malloc(sizeof(StaticFigure));
  FigureMem = new (FigureMem) StaticFigure(SimData,pos);

  string sc("static_object");
  FigureMem->add_VisualComponent(SimData,0,36,sc,FigureMem);
  staticfigures.push_back(FigureMem);
}
void Enviroment::set_enviroment_figure_pos_graph(Graph *fig_movement_graph) {
  enviroment_figure_pos_graph = fig_movement_graph;
}
void Enviroment::compute_VarPosFigure_paths() {
  for(int i = 0; i < varposfigures.size(); i++) {
    varposfigures[i]->compute_path_to_target(SimData);
  }
}
void Enviroment::iterate_VarPosFigure_positions() {
  for(int i = 0; i < varposfigures.size(); i++) {
    if(!varposfigures[i]->is_pos_seq_path_exhausted()) {
      varposfigures[i]->increment_vc_pos();
    }
  }
}
Enviroment::~Enviroment() {
  cout << "destroying enviroment obj" << endl;
}
