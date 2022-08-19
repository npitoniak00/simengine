#pragma once
#ifndef SIMSTATE_H_
#define SIMSTATE_H_

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include <chrono>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VisualComponent.hpp"
#include "Graph.hpp"
#include "GraphNode.hpp"

class VisualComponent;
class PlotContext;
class Graph;
class GraphNode;
using namespace std;

class SimState {
  private:
    glm::vec3 camera_pos;
    glm::vec3 protagonist_pos;

    float camera_y_rot;
    float camera_x_rot;
    float init_reticle_y_rot = -3.14;
    float init_reticle_x_rot = 1.57;
    float reticle_y_rot = -3.14;
    float reticle_x_rot = 1.57;
    float reticle_figure_y_rot = 0.0;
    float reticle_figure_x_rot = 0.0;

    vector<VisualComponent*> VisualComponents;
    Graph *enviroment_figure_pos_graph = NULL;
    Bound *figure_movement_Bound = NULL;
    int shot_count = 0;
  public:
    struct {
      float reticle_rotation_sensitivity = 0.07;
      float camera_rotation_sensitivity = 0.07;
      float mov_key_react_sensitivity = 0.35;
      int movement_path_node_precision = 12;
    } config;
    struct {
      chrono::high_resolution_clock::time_point sim_init;
      chrono::high_resolution_clock::time_point last_shot;
      chrono::high_resolution_clock::time_point last_fr_calc;
    } timed_activities;
    SimState();
    void update_frame_rate_calculations();
    int get_ms_since_init();
    bool is_shot_available();
    void increment_shot_count();
    int get_shot_count();
    void set_last_shot_timestamp();
    void set_camera_position(glm::vec3 input_camera_pos);
    void test();
    glm::vec3 get_camera_position();
    void set_protagonist_position(glm::vec3 input_protagonist_pos);
    glm::vec3 get_protagonist_position();
    float get_reticle_figure_y_rot();
    float get_reticle_figure_x_rot();
    void increment_protagonist_position_from_input(glm::vec3 delta_pos);
    float get_camera_y_rot();
    float get_camera_x_rot();
    float get_reticle_y_rot();
    float get_reticle_x_rot();
    void set_figure_movement_graph_Bound(Bound *b);
    Bound* get_figure_movement_graph_Bound();
    void set_enviroment_figure_pos_graph(Graph *input_graph);
    Graph* get_enviroment_figure_pos_graph();
    void set_camera_y_rot(float camera_r_rot_input);
    void set_camera_x_rot(float camera_r_rot_input);
    void init_camera_rots();
    void increment_camera_y_rot(float rot_inc);
    void increment_camera_x_rot(float rot_inc);
    void increment_reticle_y_rot(float rot_inc);
    void increment_reticle_x_rot(float rot_inc);
    void increment_position_from_input(glm::vec3 delta_pos);
    void add_VisualComponent(VisualComponent *Vc);
    void remove_VisualComponent(VisualComponent *Vc);
    vector<VisualComponent*> get_VisualComponents();
    void print_visual_components();
};

#endif
