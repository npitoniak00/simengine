#ifndef PLOTCONTEXT_H_
#define PLOTCONTEXT_H_

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

using namespace std;

class PlotContext {
private:
  int round_count = 1;
  int round_antagonist_terminations = 0;
  int total_antagonist_terminations = 0;
public:
  PlotContext();
  int get_max_active_antagonists();
  int get_total_round_antagonists();
  int get_total_antagonist_terminations();
  bool should_render_additional_antagonist(int active_antagonist_count);
  void to_string();
  void increment_round();
  void increment_terminated_antagonists();
  ~PlotContext();
};

#endif
