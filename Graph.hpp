#ifndef GRAPH_H_
#define GRAPH_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Bound.hpp"
#include "GraphNode.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GraphNode;
class Bound;

using namespace std;

class Graph {
private:
  vector<GraphNode*> nodes;
  unordered_map<string, GraphNode*> node_map;
  int node_count = 0;
public:
  Graph();
  void add_node(GraphNode* N);
  void tag_nodes_unvisited();
  void init_node_distances();
  bool is_unvisited_nodes_present();
  vector<GraphNode*> get_nodes();
  void free_node_mem();
  vector<GraphNode*> compute_cartesian_nodes_from_Bound(int prec,Bound *input_Bound);
  string float_to_string(int prec, float f);
  void add_nodes(vector<GraphNode*> new_nodes);
  void add_edges_to_cartesian_nodes(int prec);
  GraphNode* get_node_approximating_position(glm::vec3 position);
  void dfs(GraphNode *start_node, GraphNode *target_node);
  vector<GraphNode*> get_shortest_path_node_sequence(GraphNode* destination);
  vector<glm::vec3> compute_precision_path_between_positions(int prec, glm::vec3 init_pos, glm::vec3 dest_pos);
  vector<glm::vec3> compute_precision_path_between_positions_for_nodeset(int prec, vector<GraphNode*> gn_pos);
  ~Graph();
};

#endif
