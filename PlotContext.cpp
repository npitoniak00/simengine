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
#include "PlotContext.hpp"

using namespace std;

PlotContext::PlotContext() {
  cout << "init plot context" << endl;
}
void PlotContext::increment_round() {
  round_count++;
  round_antagonist_terminations = 0;
}
void PlotContext::increment_terminated_antagonists() {
  round_antagonist_terminations++;
  total_antagonist_terminations++;
  if(round_antagonist_terminations >= get_total_round_antagonists()) {
    increment_round();
  }
}
int PlotContext::get_total_antagonist_terminations() {
  return total_antagonist_terminations;
}
bool PlotContext::should_render_additional_antagonist(int active_antagonist_count) {
  // active_antagonist_count = Enviroment.varposfigures.size()
  if(active_antagonist_count >= get_max_active_antagonists()) { return false; }
  int remaining_round_figures = get_total_round_antagonists() - round_antagonist_terminations;
  if(remaining_round_figures - active_antagonist_count <= 0) { return false; }
  return true;
}
int PlotContext::get_max_active_antagonists() {
  return 5;
}
int PlotContext::get_total_round_antagonists() {
  return (pow(round_count,2) + 2);
}
void PlotContext::to_string() {
  cout << "\n========= PLOT CONTEXT =======" << endl;
  cout << "round: " << round_count << endl;
  cout << "smoked antagonists: " << round_antagonist_terminations << "/" << total_antagonist_terminations << endl;
  cout << "max active antagonists: " << get_max_active_antagonists() << endl;
  cout << "total round antagonists: " << get_total_round_antagonists() << endl;
}
PlotContext::~PlotContext() {
  cout << "destroying plot context" << endl;
}
