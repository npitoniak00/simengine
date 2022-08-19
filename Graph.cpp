#include "Graph.hpp"
#include <iostream>
#include <typeinfo>
#include <vector>
#include <cmath>
#include <stack>
#include <unordered_map>

#include "Bound.hpp"
#include "GraphNode.hpp"
#include "Graph.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GraphNode;
class Bound;

using namespace std;

Graph::Graph() {
  cout << "graph init" << endl;
}
void Graph::add_node(GraphNode* N) {
  Graph::nodes.push_back(N);
  node_count++;
}
void Graph::add_nodes(vector<GraphNode*> new_nodes) {
  for(int i = 0; i < new_nodes.size(); i++) {
    Graph::nodes.push_back(new_nodes[i]);
    node_count++;
  }
}
vector<GraphNode*> Graph::get_nodes() {
  return Graph::nodes;
}
void Graph::tag_nodes_unvisited() {
  for(int i = 0; i < Graph::nodes.size(); i++) {
    Graph::nodes[i]->set_visit_status(false);
  }
}
void Graph::init_node_distances() {
  for(int i = 0; i < Graph::nodes.size(); i++) {
    Graph::nodes[i]->set_distance(-1.0); // -1.0 interpreted by node as INF
  }
}
bool Graph::is_unvisited_nodes_present() {
  for(int i = 0; i < Graph::nodes.size(); i++) {
    if(Graph::nodes[i]->get_visit_status() == false) { return true; }
  }
  return false;
}
void Graph::free_node_mem() {
  for(int i = 0; i < nodes.size(); i++) { free(nodes[i]); }
  nodes.clear();
  node_map.clear();
}
vector<GraphNode*> Graph::compute_cartesian_nodes_from_Bound(int prec,Bound *input_Bound) {
  // prec:float indicates [1,10,100,1000]^2 etc 1x1 cube sub-divisions (finer prec = smoother movement)
  // boundaries:vector<float> indicates bundaries of space to model with 1x1 cubes
  float xval,yval,zval;
  vector<glm::vec3> bounds = input_Bound->get_rectangular_bounds();
  vector<GraphNode*> graph_nodes;
  for(int xi = bounds[1][0]; xi <= bounds[0][0]; xi++) { // iterate over xrange of boundaries
    for(int zi = bounds[3][2]; zi <= bounds[0][2]; zi++) { // iterate over zrange of boundaries
      // iterate precision portions of each macro-cube
      for(int pix = 0; pix < prec; pix++) {
        for(int piz = 0; piz  < prec; piz++) {
          xval = (float)xi+(((float)pix)/prec);
          yval = (float)bounds[0][1];
          zval = (float)zi+(((float)piz)/prec);

          GraphNode* GraphNodeMem = (GraphNode*) malloc(sizeof(GraphNode));
          GraphNodeMem = new (GraphNodeMem) GraphNode({xval,yval,zval});

          string node_position_id = Graph::float_to_string(prec,xval) + Graph::float_to_string(prec,yval) + Graph::float_to_string(prec,zval);
          graph_nodes.push_back(GraphNodeMem);
          Graph::node_map[node_position_id] = GraphNodeMem;
        } // end prec z loop
      } // end prec x loop
    }
  }

  add_nodes(graph_nodes);
  add_edges_to_cartesian_nodes(1);

  return graph_nodes;
}
// float_to_string() used to convert a locational dimension metric into
// a correlating name which may be used to access a node in the graph most similar to the original float value
string Graph::float_to_string(int prec, float f) {
  unordered_map<int,int> pm;
  pm[1] = 0;
  pm[10] = 1;
  pm[100] = 2;
  pm[1000] = 3;
  string retstr;
  string raw_f = to_string(f);
  int point_encountered = -1;
  for(int i = 0; i < raw_f.size(); i++) {
    if(raw_f[i] == '.') { point_encountered = i; }
    if(point_encountered == -1) { retstr += raw_f[i]; }
    if(point_encountered > -1 && (i - point_encountered) <= pm[prec]) { retstr += raw_f[i]; }
  }
  if(retstr[retstr.size() - 1] == '.') { retstr.pop_back(); }
  return retstr;
}
void Graph::add_edges_to_cartesian_nodes(int prec) {
  // record x,y,z ranges from boundaries
  float movement = 1 / prec;
  // for each node, compute 4 simple neighbors, add as neighbor if within boundaries
  int neighbor_inc = 0;
  for(int i = 0; i < Graph::nodes.size(); i++) {
    glm::vec3 node_position = Graph::nodes[i]->get_vec3data();
    // compute simple 8 directional nodes with respect to precision (left,right,top,bottom,4diagonals)
    string x_inc_node_position_id = Graph::float_to_string(prec, node_position[0] + movement) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2]);
    string x_dec_node_position_id = Graph::float_to_string(prec, node_position[0] - movement) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2]);

    string z_inc_node_position_id = Graph::float_to_string(prec, node_position[0]) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2] + movement);
    string z_dec_node_position_id = Graph::float_to_string(prec, node_position[0]) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2] - movement);

    string x_inc_top_left_node_position_id = Graph::float_to_string(prec, node_position[0] + movement) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2] - movement);
    string x_inc_bottom_left_node_position_id = Graph::float_to_string(prec, node_position[0] - movement) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2] - movement);

    string x_inc_top_right_node_position_id = Graph::float_to_string(prec, node_position[0] + movement) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2] + movement);
    string x_inc_bottom_right_node_position_id = Graph::float_to_string(prec, node_position[0] - movement) + Graph::float_to_string(prec, node_position[1]) + Graph::float_to_string(prec, node_position[2] + movement);
    // check node_map for presence of computed potential neighbor positions
    if (Graph::node_map.find(x_inc_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(x_inc_node_position_id)); }
    if (Graph::node_map.find(x_dec_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(x_dec_node_position_id)); }
    if (Graph::node_map.find(z_inc_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(z_inc_node_position_id)); }
    if (Graph::node_map.find(z_dec_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(z_dec_node_position_id)); }
    if (Graph::node_map.find(x_inc_top_left_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(x_inc_top_left_node_position_id)); }
    if (Graph::node_map.find(x_inc_bottom_left_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(x_inc_bottom_left_node_position_id)); }
    if (Graph::node_map.find(x_inc_top_right_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(x_inc_top_right_node_position_id)); }
    if (Graph::node_map.find(x_inc_bottom_right_node_position_id) != Graph::node_map.end()) { neighbor_inc++; Graph::nodes[i]->add_edge(Graph::node_map.at(x_inc_bottom_right_node_position_id)); }
  }
  // cout << "node count: " << Graph::nodes.size() << " edge count: " << neighbor_inc << endl;
}
GraphNode* Graph::get_node_approximating_position(glm::vec3 position) {
  float min_dist = INFINITY;
  float dist;
  GraphNode* ret_node;
  for(int ii = 0; ii < node_count; ii++) {
    dist = sqrt(pow((Graph::nodes[ii]->get_vec3data()[0] - position[0]),2) + pow((Graph::nodes[ii]->get_vec3data()[1] - position[1]),2) + pow((Graph::nodes[ii]->get_vec3data()[2] - position[2]),2));
    if(dist < min_dist) {
      min_dist = dist;
      ret_node = Graph::nodes[ii];
    }
  }
  return ret_node;
}
void Graph::dfs(GraphNode *start_node, GraphNode *target_node) {
  Graph::init_node_distances();
  Graph::tag_nodes_unvisited();
  start_node->set_distance(0.0);
  GraphNode *n = start_node;
  bool target_found = false;
  stack<GraphNode*> graphnodestack;
  while(!target_found) {
    glm::vec3 lol = n->get_vec3data();
    vector<GraphNode*> n_neighbors = n->get_edges_unvisited();
    if(n_neighbors.size() == 0) {
      graphnodestack.pop();
      n = graphnodestack.top();
    } else {
      GraphNode *dfheur = n_neighbors[0];
      // update edge distances if shorter path exists / calc least distance to target
      for(int i = 0; i < n_neighbors.size(); i++) {
        if(n_neighbors[i]->compute_simple_dist_from_target(target_node) < dfheur->compute_simple_dist_from_target(target_node)) { dfheur = n_neighbors[i]; }
        if(n->get_distance() + 1 < n_neighbors[i]->get_distance()) { n_neighbors[i]->set_distance(n->get_distance() + 1); }
      }
      if(dfheur == target_node) { target_found = true; }
      graphnodestack.push(n);
      n = dfheur;
    }
  }
}
vector<GraphNode*> Graph::get_shortest_path_node_sequence(GraphNode* destination) {
  // init at destination, repetitively move to smallest-distance neighbor until node distance = start (distance=0)
  vector<GraphNode*> node_seq;
  GraphNode* curr = destination;
  while(curr->get_distance() > 0) {
    // iterate neighbors, copy min distance neighbor into curr:Node*
    GraphNode* min_dist_neighbor;
    vector<GraphNode*> neighbors = curr->get_edges();
    for(int i = 0; i < neighbors.size(); i++) {
      if(i == 0 || neighbors[i]->get_distance() < min_dist_neighbor->get_distance()) {
        min_dist_neighbor = neighbors[i];
      }
    }
    node_seq.push_back(min_dist_neighbor);
    curr = min_dist_neighbor;
  }
  // reverse node_seq (start=djk origin, end=destination node
  vector<GraphNode*> node_seq_rev;
  for(int i = node_seq.size() - 1; i >= 0; i--) {
    node_seq_rev.push_back(node_seq[i]);
  }
  return node_seq_rev;
}
vector<glm::vec3> Graph::compute_precision_path_between_positions(int prec, glm::vec3 init_pos, glm::vec3 dest_pos) {
  vector<glm::vec3> prec_positions;
  // compute vector components from init_pos to dest_pos
  float x_component = dest_pos[0] - init_pos[0];
  float y_component = dest_pos[1] - init_pos[1];
  float z_component = dest_pos[2] - init_pos[2];
  // compute magnitude of vector sqrt(component_x^2 + component_y^2)
  float mag = sqrt(pow(x_component,2) + pow(y_component,2) + pow(z_component,2));
  // compute direction of vector atan(component_y / component_x)
  float dir = atan(y_component / x_component);
  // alternatively, calculate x,y,z components of init->dest, iteratively multiply by proportion 0->1 depending on precision
  vector<glm::vec3> mp;
  for(int i = 0; i < prec; i += 1) {
    float mult = ((float)i)*(1.0 / prec);
    glm::vec3 modified_position;
    prec_positions.push_back(glm::vec3((init_pos[0]+(x_component*mult)),(init_pos[1]+(y_component*mult)),(init_pos[2]+(z_component*mult))));
  }
  for(int i = 0; i < prec_positions.size(); i++) {
    mp.push_back(prec_positions[i]);
  }
  return mp;
}
vector<glm::vec3> Graph::compute_precision_path_between_positions_for_nodeset(int prec, vector<GraphNode*> gn_pos) {
  vector<glm::vec3> ret_pos;
  for(int i = 0; i < gn_pos.size(); i++) {
    if(i == 0) { continue; }
    vector<glm::vec3> sub_pos = Graph::compute_precision_path_between_positions(prec,gn_pos[i-1]->get_vec3data(),gn_pos[i]->get_vec3data());
    for(int j = 0; j < sub_pos.size(); j++) { ret_pos.push_back(sub_pos[j]); }
  }
  return ret_pos;
}
Graph::~Graph() {
  free_node_mem();
}
