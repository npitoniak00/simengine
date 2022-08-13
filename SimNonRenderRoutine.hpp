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

// typedef void (*routine_functionality)(void);
typedef void (*routine_functionality)(Enviroment *Env,SimState *Sd);

class SimNonRenderRoutine {
private:
  int priority = -1;
  int minimum_ms_interval = -1;
  routine_functionality instructions = NULL;
  //routine_functionality1 instructions1 = NULL;
  Enviroment *E;
  SimState *S;
  chrono::high_resolution_clock::time_point last_instruction_execution;
public:
  SimNonRenderRoutine();
  SimNonRenderRoutine(Enviroment *E,SimState *Sd);
  SimNonRenderRoutine(Enviroment *Env,SimState *Sd,int priority_input,int min_ms_interval,routine_functionality rfp);
  bool lazy_gt_overload(SimNonRenderRoutine *s);
  chrono::high_resolution_clock::time_point get_last_instruction_execution_ts();
  int get_last_exec_ms_epsilon();
  bool is_interval_sleep_expired();
  int get_priority();
  void set_routine_instructions(routine_functionality rfp);
  //void set_routine_instructions(routine_functionality1 rfp);
  void set_minimum_time_interval(int min_ms_interval);
  void set_priority(int priority_input);
  void execute_instructions();
  ~SimNonRenderRoutine();
};

#endif
