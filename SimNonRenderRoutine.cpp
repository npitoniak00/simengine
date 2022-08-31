#include <iostream>
#include <thread>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>
#include <unordered_map>
#include <typeinfo>

#include "Enviroment.hpp"
#include "SimState.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimNonRenderRoutine.hpp"

using namespace std;

SimNonRenderRoutine::SimNonRenderRoutine() {
  cout << "init simnonrenderroutine ()" << endl;
}
SimNonRenderRoutine::SimNonRenderRoutine(Enviroment *Env,SimState *Sd) {
  cout << "init simnonrenderroutine(*Enviroment,*SimState)" << endl;
  S = Sd;
  E = Env;
}
SimNonRenderRoutine::SimNonRenderRoutine(Enviroment *Env,SimState *Sd,int priority_input,int min_ms_interval,string instructions_slug_input) {
  cout << "init simnonrenderroutine(*Enviroment,*SimState,priority,ms_interval,string instructions_slug)" << endl;
  S = Sd;
  E = Env;
  priority = priority_input;
  minimum_ms_interval = min_ms_interval;
  instructions_slug = instructions_slug_input;
  SimNonRenderRoutine::last_exec_ms = S->get_ms_since_init();
}
bool SimNonRenderRoutine::lazy_gt_overload(SimNonRenderRoutine *s) {
  // 1. compare priority
  if(priority < s->get_priority()) { return false; }
  // 2. compare last exec ms deltas
  if(SimNonRenderRoutine::last_exec_ms > s->get_last_exec_ms()) { return false; }
  return true;
}
long int SimNonRenderRoutine::get_last_exec_ms_epsilon() {
  long int ms_now = S->get_ms_since_init();
  long int delta = ms_now - last_exec_ms;
  return delta;
}
long int SimNonRenderRoutine::get_last_exec_ms() {
  return last_exec_ms;
}
bool SimNonRenderRoutine::is_interval_sleep_expired() {
  long int e = get_last_exec_ms_epsilon();
  if(minimum_ms_interval != -1 && abs(get_last_exec_ms_epsilon()) >= minimum_ms_interval) { return true; }
  return false;
}
void SimNonRenderRoutine::set_priority(int priority_input) {
  priority = priority_input;
}
int SimNonRenderRoutine::get_priority() {
  return priority;
}
void SimNonRenderRoutine::set_minimum_time_interval(int min_ms_interval) {
  minimum_ms_interval = min_ms_interval;
}
void SimNonRenderRoutine::execute_instructions() {
  SimNonRenderRoutine::last_exec_ms = S->get_ms_since_init();
  if(instructions_slug == "frame rate calculation") {
    update_frame_rate_calc();
    return;
  }
  if(instructions_slug == "process antagonist attacks") {
    process_antagonist_attacks_on_protagonist();
    return;
  }
  if(instructions_slug == "compute figure paths") {
    compute_VarPosFigure_paths();
    return;
  }
  if(instructions_slug == "enviroment plot reaction") {
    enviroment_react_to_PlotContext();
    return;
  }
  if(instructions_slug == "inc VarPosFigure positions") {
    increment_VarPosFigure_positions();
    return;
  }
  if(instructions_slug == "update protagonist position") {
    update_protagonist_figure_position();
    return;
  }
  if(instructions_slug == "update reticle position") {
    update_reticle_position();
    return;
  }
}
void SimNonRenderRoutine::update_frame_rate_calc() {
  S->update_frame_rate_calculations();
}
void SimNonRenderRoutine::process_antagonist_attacks_on_protagonist() {
  E->process_antagonist_attacks_on_protagonist();
}
void SimNonRenderRoutine::compute_VarPosFigure_paths() {
  E->compute_VarPosFigure_paths();
}
void SimNonRenderRoutine::enviroment_react_to_PlotContext() {
  E->react_to_PlotContext();
}
void SimNonRenderRoutine::increment_VarPosFigure_positions() {
  E->iterate_VarPosFigure_positions();
}
void SimNonRenderRoutine::update_protagonist_figure_position() {
  E->update_protagonist_figure_pos();
}
void SimNonRenderRoutine::update_reticle_position() {
  E->update_reticle_pos();
}
SimNonRenderRoutine::~SimNonRenderRoutine() {
  cout << "destory simnonrenderroutine" << endl;
}
