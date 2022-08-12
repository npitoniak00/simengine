#include <cmath>
#include <vector>
#include <iostream>
#include "GraphNode.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GraphNode.hpp"

GraphNode::GraphNode() {
  cout << "graphnode init" << endl;
  GraphNode::visited = false;
}
GraphNode::GraphNode(glm::vec3 vec3data) {
  GraphNode::vec3data = vec3data;
  GraphNode::visited = false;
}
void GraphNode::add_edge(GraphNode* N) {
  GraphNode::edges.push_back(N);
}
void GraphNode::to_string() {
  cout << "NODE @ " << GraphNode::vec3data[0] << "," << GraphNode::vec3data[1] << "," << GraphNode::vec3data[2] << " edges: " << GraphNode::edges.size() << endl;
}
vector<GraphNode*> GraphNode::get_edges() {
  return GraphNode::edges;
}
vector<GraphNode*> GraphNode::get_edges_unvisited() {
  vector<GraphNode*> edges = GraphNode::get_edges();
  vector<GraphNode*> unvisited_edges;
  for(int i = 0; i < edges.size(); i++) {
    if(!edges[i]->visited) { unvisited_edges.push_back(edges[i]); }
  }
  return unvisited_edges;
}
float GraphNode::compute_simple_dist_from_target(GraphNode *target_node) {
  glm::vec3 self_pos = GraphNode::get_vec3data();
  glm::vec3 target_pos = target_node->get_vec3data();
  return sqrt((pow(target_pos[0]-self_pos[0],2))+(pow(target_pos[1]-self_pos[1],2))+(pow(target_pos[2]-self_pos[2],2)));
}
glm::vec3 GraphNode::get_vec3data() {
  return GraphNode::vec3data;
}
void GraphNode::set_visit_status(bool visit_status) {
  GraphNode::visited = visit_status;
}
bool GraphNode::get_visit_status() {
  return GraphNode::visited;
}
float GraphNode::get_distance() {
  return GraphNode::distance;
}
void GraphNode::set_distance(float distance) {
  if(distance == -1.0) {
    GraphNode::distance = INFINITY;
  } else {
    GraphNode::distance = distance;
  }
}
