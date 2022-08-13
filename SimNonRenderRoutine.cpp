#include <iostream>
#include <thread>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>

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
SimNonRenderRoutine::SimNonRenderRoutine(Enviroment *Env,SimState *Sd,int priority_input,int min_ms_interval,routine_functionality rfp) {
  cout << "init simnonrenderroutine(*Enviroment,*SimState,priority,ms_interval,instructions)" << endl;
  S = Sd;
  E = Env;
  priority = priority_input;
  minimum_ms_interval = min_ms_interval;
  instructions = rfp;
}
bool SimNonRenderRoutine::lazy_gt_overload(SimNonRenderRoutine *s) {
  // 1. compare priority
  if(priority < s->get_priority()) { return false; }
  // 2. compare last exec ms deltas
  if(last_instruction_execution > s->get_last_instruction_execution_ts()) { return false; }
  return true;
}
void SimNonRenderRoutine::set_routine_instructions(routine_functionality rfp) {
  instructions = rfp;
}
int SimNonRenderRoutine::get_last_exec_ms_epsilon() {
  chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
  auto milliseconds = chrono::duration_cast<std::chrono::milliseconds>(last_instruction_execution - now);
  return (int)milliseconds.count();
}
chrono::high_resolution_clock::time_point SimNonRenderRoutine::get_last_instruction_execution_ts() {
  return last_instruction_execution;
}
bool SimNonRenderRoutine::is_interval_sleep_expired() {
  cout << "exec delta: " << get_last_exec_ms_epsilon() << endl;
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
  last_instruction_execution = chrono::system_clock::now();
  instructions(E,S);
}
SimNonRenderRoutine::~SimNonRenderRoutine() {
  cout << "destory simnonrenderroutine" << endl;
}
