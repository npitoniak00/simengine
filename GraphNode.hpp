#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class GraphNode {
private:
  glm::vec3 vec3data;
  vector<GraphNode*> edges;
  float distance;
  bool visited;
public:
  GraphNode();
  GraphNode(glm::vec3 vec3data);

  glm::vec3 get_vec3data();
  void add_edge(GraphNode* N);
  vector<GraphNode*> get_edges();
  vector<GraphNode*> get_edges_unvisited();

  bool get_visit_status();
  void set_visit_status(bool visited);
  float get_distance();
  void set_distance(float distance);
  float compute_simple_dist_from_target(GraphNode *target_node);

  void to_string();
};

#endif
