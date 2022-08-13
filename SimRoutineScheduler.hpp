#ifndef SIMROUTINESCHEDULER_H_
#define SIMROUTINESCHEDULER_H_

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

#include "SimNonRenderRoutine.hpp"

class SimNonRenderRoutine;

using namespace std;

class SimRoutineScheduler {
private:
  vector<SimNonRenderRoutine*> routines;
public:
  SimRoutineScheduler();
  void add_routine(SimNonRenderRoutine *snrr);
  void add_routine(Enviroment *Env,SimState *SimData,int priority,int min_ms_interval,routine_functionality rfp);
  void free_routines();
  int get_routine_count();
  vector<SimNonRenderRoutine*> sort_routines();
  vector<SimNonRenderRoutine*> get_expired_routines();
  ~SimRoutineScheduler();
};

#endif
