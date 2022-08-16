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
void SimRoutineScheduler::merge(int left,int middle,int right,vector<SimNonRenderRoutine*> *routines_vector) {
  vector<SimNonRenderRoutine*> tmp;
  int li = left;
  int ri = middle + 1;
  while(li <= middle && ri <= right) {
    if((*routines_vector)[li]->lazy_gt_overload((*routines_vector)[ri])) {
      tmp.push_back((*routines_vector)[li]);
      li++;
    } else {
      tmp.push_back((*routines_vector)[ri]);
      ri++;
    }
  }
  while(li <= middle) {
    tmp.push_back((*routines_vector)[li]);
    li++;
  }
  while(ri <= right) {
    tmp.push_back((*routines_vector)[ri]);
    ri++;
  }
  for(int i = 0; i < tmp.size(); i++) {
    (*routines_vector)[left+i] = tmp[i];
  }
}
void SimRoutineScheduler::mergesort(int left,int right,vector<SimNonRenderRoutine*> *routines_vector) {
  if(left >= right) { return; }
  int middle = left + (right - left) / 2;

  mergesort(left,middle,routines_vector);
  mergesort(middle+1,right,routines_vector);
  merge(left,middle,right,routines_vector);
}
void SimRoutineScheduler::add_routine(SimNonRenderRoutine *snrr) {
  routines.push_back(snrr);
}
void SimRoutineScheduler::add_routine(Enviroment *Env,SimState *SimData,int priority,int min_ms_interval,string instructions_slug) {
  SimNonRenderRoutine* SnrrMem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  SnrrMem = new (SnrrMem) SimNonRenderRoutine(Env,SimData,priority,min_ms_interval,instructions_slug);
  routines.push_back(SnrrMem);
}
void SimRoutineScheduler::free_routines() {
  cout << "free'ing routines" << endl;
}
int SimRoutineScheduler::get_routine_count() {
  return routines.size();
}
vector<SimNonRenderRoutine*> SimRoutineScheduler::get_expired_routines() {
//  cout << "computing expired routines" << endl;
  vector<SimNonRenderRoutine*> expired_routines;
  for(int i = 0; i < routines.size(); i++) {
  /*  cout << "about to test for expiration" << i << endl;
    cout << routines.size() << endl;
    cout << typeid(routines[0]).name() << endl;
    cout << routines[0]->get_priority() << endl;
    cout << routines[i]->is_interval_sleep_expired() << endl;
    cout << "is expired above indicator" << endl;*/
    if(routines[i]->is_interval_sleep_expired()) {
      //cout << "about to record routine" << endl;
      expired_routines.push_back(routines[i]);
    }
  }
  //cout << "end computing expired routines" << endl;
  return expired_routines;
}
vector<SimNonRenderRoutine*> SimRoutineScheduler::sort_routines() {
//  cout << "\na\n" << endl;
  vector<SimNonRenderRoutine*> routines_mod = get_expired_routines();
//  cout << "b" << endl;
  if(routines_mod.size() <= 1) { cout << "c" << endl; return routines_mod; }
//  cout << "d" << endl;
  mergesort(0,routines_mod.size()-1,&routines_mod);
//  cout << "e" << endl;
  return routines_mod;
}
SimRoutineScheduler::~SimRoutineScheduler() {
  cout << "destroying simroutinescheduler" << endl;
}
