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
#include "SimRoutineScheduler.hpp"
#include "SimNonRenderRoutine.hpp"

using namespace std;

SimRoutineScheduler::SimRoutineScheduler() {
  cout << "simroutinescheduler init" << endl;
}
void SimRoutineScheduler::add_routine(SimNonRenderRoutine *snrr) {
  routines.push_back(snrr);
}
void SimRoutineScheduler::add_routine(Enviroment *Env,SimState *SimData,int priority,int min_ms_interval,routine_functionality rfp) {
  SimNonRenderRoutine* SnrrMem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  SnrrMem = new (SnrrMem) SimNonRenderRoutine(Env,SimData,priority,min_ms_interval,rfp);
  routines.push_back(SnrrMem);
}
void SimRoutineScheduler::free_routines() {
  cout << "free'ing routines" << endl;
}
int SimRoutineScheduler::get_routine_count() {
  return routines.size();
}
vector<SimNonRenderRoutine*> SimRoutineScheduler::get_expired_routines() {
  vector<SimNonRenderRoutine*> expired_routines;
  for(int i = 0; i < routines.size(); i++) {
    if(routines[i]->is_interval_sleep_expired()) { expired_routines.push_back(routines[i]); }
  }
  return expired_routines;
}
vector<SimNonRenderRoutine*> SimRoutineScheduler::sort_routines() {
  vector<SimNonRenderRoutine*> sorted_routines;
  if(routines.size() < 2) { return sorted_routines; }
  bool compare = routines[0]->lazy_gt_overload(routines[1]);
  // mergesort using lazy_gt_overload()
  return sorted_routines;
}
SimRoutineScheduler::~SimRoutineScheduler() {
  cout << "destroying simroutinescheduler" << endl;
}
