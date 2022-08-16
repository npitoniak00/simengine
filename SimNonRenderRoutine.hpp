#ifndef SIMNONRENDERROUTINE_H_
#define SIMNONRENDERROUTINE_H_

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

#include "Enviroment.hpp"
#include "SimState.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class SimNonRenderRoutine {
private:
  int priority = -1;
  string instructions_slug;
  int minimum_ms_interval = -1;
  Enviroment *E;
  SimState *S;
  long int last_exec_ms;
public:
  SimNonRenderRoutine();
  SimNonRenderRoutine(Enviroment *E,SimState *Sd);
  SimNonRenderRoutine(Enviroment *Env,SimState *Sd,int priority_input,int min_ms_interval,string instructions_slug);
  bool lazy_gt_overload(SimNonRenderRoutine *s);
  long int get_last_exec_ms();
  long int get_last_exec_ms_epsilon();
  bool is_interval_sleep_expired();
  int get_priority();
  void set_minimum_time_interval(int min_ms_interval);
  void set_priority(int priority_input);
  void execute_instructions();
  void update_frame_rate_calc();
  void process_antagonist_attacks_on_protagonist();
  void compute_VarPosFigure_paths();
  void enviroment_react_to_PlotContext();
  void increment_VarPosFigure_positions();
  void update_protagonist_figure_position();
  void update_reticle_position();
  ~SimNonRenderRoutine();
};

#endif
