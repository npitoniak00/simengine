#define GLM_ENABLE_EXPERIMENTAL
#include <stack>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/sysinfo.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Utils.hpp"
#include "SimState.hpp"
#include "VertexLoader.hpp"
#include "VisualComponent.hpp"
#include "VCController.hpp"
#include "Bound.hpp"
#include "Graph.hpp"
#include "GraphNode.hpp"
#include "PlotContext.hpp"
#include "Enviroment.hpp"
#include "controllers/VarPosFigure.hpp"
#include "controllers/Floor.hpp"

using namespace std;
#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
int display_inc = 0;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mvLoc, projLoc, vLoc;
GLfloat tfLoc, timeFactor;

float forward_facing_dir = 1.57;
float backward_facing_dir = 4.71;
float right_facing_dir = 3.14;
float left_facing_dir = 0.0;

int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

SimState SimData;
VertexLoader Verticies;
PlotContext GameContext;
Enviroment E(&SimData,&GameContext);

void setup_vertices(void) {
  float cube[Verticies.get_cube_verticies_len()];
  float *cube_ptr = Verticies.get_cube_verticies();
  for(int i = 0; i < Verticies.get_cube_verticies_len(); i++) { cube[i] = *cube_ptr; cube_ptr++; }
  float pyr[Verticies.get_pyramid_verticies_len()];
  float *pyr_ptr = Verticies.get_pyramid_verticies();
  for(int i = 0; i < Verticies.get_pyramid_verticies_len(); i++) { pyr[i] = *pyr_ptr; pyr_ptr++; }

  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
  glGenBuffers(numVBOs, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyr), pyr, GL_STATIC_DRAW);
}

void generate_figure_graph() {
  // generate bound for figure movement graph
  Bound* FigureMovementGraphBoundMem = (Bound*) malloc(sizeof(Bound));
  FigureMovementGraphBoundMem = new (FigureMovementGraphBoundMem) Bound();
  glm::vec3 figure_graph_max_pos_vert = glm::vec3(-500.0f,0.5f,-500.0f);
  glm::vec3 figure_graph_dimensions = glm::vec3(50.0,-0.5,50.0);
  FigureMovementGraphBoundMem->generate_rectangular_bounds(figure_graph_max_pos_vert,figure_graph_dimensions,true);
  // generate figure movement graph
  Graph* GraphMem = (Graph*) malloc(sizeof(Graph));
  GraphMem = new (GraphMem) Graph();
  GraphMem->compute_cartesian_nodes_from_Bound(1,FigureMovementGraphBoundMem);
  // save pointers to SimState
  SimData.set_figure_movement_graph_Bound(FigureMovementGraphBoundMem);
  SimData.set_enviroment_figure_pos_graph(GraphMem);
  // save pointers to Enviroment
  E.set_enviroment_figure_pos_graph(GraphMem);
}
void init(GLFWwindow* window) {
  Utils U;
  U.print_system_data();
  renderingProgram = U.createShaderProgram();
  cameraX = -0.0f; cameraY = 2.0f; cameraZ = 0.0f;
  SimData.set_camera_position(glm::vec3(-cameraX,-cameraY,-cameraZ));
  setup_vertices();

  generate_figure_graph();

  E.init_figure_Floor();
  E.init_protagonist();
  E.init_Reticle();
}
void draw_VisualComponent(VisualComponent *Vc,SimState *SimData) {
  // allocate gl uniform space
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
  vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
  // compute VC pos mat
  string vc_type = Vc->get_parent_type_str();
  vector<glm::mat4> VcMats;
  if(vc_type == "Floor") {
    VcMats = Vc->get_Floor_parent()->position_computation(SimData,Vc);
  } else if(vc_type == "Reticle") {
    glm::vec3 reticlepos = Vc->get_position();
    VcMats = Vc->get_Reticle_parent()->position_computation(SimData,Vc);
  } else {
    VcMats = Vc->get_VarPosFigure_parent()->position_computation(SimData,Vc);
    if(Vc->get_VarPosFigure_parent() == E.get_protagonist()) {
      glm::vec3 protpos = Vc->get_position();
      return;
    }
  }
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(VcMats[1] * VcMats[2]));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(VcMats[0]));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[Vc->get_vbo_index()]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, Vc->get_vertex_count());
}
void listenToKeyboard(GLFWwindow* w,SimState *SimData,Enviroment *Env) {
  if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS && SimData->is_shot_available()) {
    // if(!SimData->is_shot_available()) { return; }
    SimData->increment_shot_count();
    SimData->set_last_shot_timestamp();
    glm::vec3 reticle_position = E.get_reticle()->get_position();
    glm::vec3 cameraview_position = glm::vec3(-SimData->get_camera_position()[0],-SimData->get_camera_position()[1],-SimData->get_camera_position()[2]);
    E.execute_shot(cameraview_position,reticle_position);
  }
  if (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS) {
    float dir = forward_facing_dir + SimData->get_camera_y_rot();
    glm::vec3 protagonist_pos = SimData->get_protagonist_position();
    protagonist_pos[0] += (float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir));
    protagonist_pos[2] += (float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir));
    Bound *enclosure = SimData->get_figure_movement_graph_Bound();
    if(!enclosure->is_vec3_within_bounds(protagonist_pos)) { return; }
    SimData->increment_protagonist_position_from_input(glm::vec3((float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir)),0.0f,(float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir))));
  }
  if (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS) {
    float dir = backward_facing_dir + SimData->get_camera_y_rot();
    glm::vec3 protagonist_pos = SimData->get_protagonist_position();
    protagonist_pos[0] += (float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir));
    protagonist_pos[2] += (float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir));
    Bound *enclosure = SimData->get_figure_movement_graph_Bound();
    if(!enclosure->is_vec3_within_bounds(protagonist_pos)) { return; }
    SimData->increment_protagonist_position_from_input(glm::vec3((float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir)),0.0f,(float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir))));
  }
  if (glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS) {
    float dir = left_facing_dir + SimData->get_camera_y_rot();
    glm::vec3 protagonist_pos = SimData->get_protagonist_position();
    protagonist_pos[0] += (float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir));
    protagonist_pos[2] += (float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir));
    Bound *enclosure = SimData->get_figure_movement_graph_Bound();
    if(!enclosure->is_vec3_within_bounds(protagonist_pos)) { return; }
    SimData->increment_protagonist_position_from_input(glm::vec3((float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir)),0.0f,(float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir))));
  }
  if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    float dir = right_facing_dir + SimData->get_camera_y_rot();
    glm::vec3 protagonist_pos = SimData->get_protagonist_position();
    protagonist_pos[0] += (float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir));
    protagonist_pos[2] += (float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir));
    Bound *enclosure = SimData->get_figure_movement_graph_Bound();
    if(!enclosure->is_vec3_within_bounds(protagonist_pos)) { return; }
    SimData->increment_protagonist_position_from_input(glm::vec3((float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir)),0.0f,(float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir))));
  }
  if (glfwGetKey(w, GLFW_KEY_G) == GLFW_PRESS) {
    float dir = right_facing_dir + SimData->get_camera_y_rot();
    glm::vec3 protagonist_pos = SimData->get_protagonist_position();
    protagonist_pos[0] += (float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir));
    protagonist_pos[2] += (float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir));
    Bound *enclosure = SimData->get_figure_movement_graph_Bound();
    if(!enclosure->is_vec3_within_bounds(protagonist_pos)) { return; }
    SimData->increment_protagonist_position_from_input(glm::vec3(0.0f,0.1f,0.0f));
  }
  if (glfwGetKey(w, GLFW_KEY_B) == GLFW_PRESS) {
    float dir = right_facing_dir + SimData->get_camera_y_rot();
    glm::vec3 protagonist_pos = SimData->get_protagonist_position();
    protagonist_pos[0] += (float)(-1*SimData->config.mov_key_react_sensitivity*cos(dir));
    protagonist_pos[2] += (float)(-1*SimData->config.mov_key_react_sensitivity*sin(dir));
    Bound *enclosure = SimData->get_figure_movement_graph_Bound();
    if(!enclosure->is_vec3_within_bounds(protagonist_pos)) { return; }
    SimData->increment_protagonist_position_from_input(glm::vec3(0.0f,-0.1f,0.0f));
  }
  if (glfwGetKey(w, GLFW_KEY_R) == GLFW_PRESS) { E.compute_VarPosFigure_paths(); }
  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) { SimData->increment_camera_y_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) { SimData->increment_camera_y_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) { SimData->increment_camera_x_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) { SimData->increment_camera_x_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }

  if (glfwGetKey(w, GLFW_KEY_I) == GLFW_PRESS) { SimData->increment_reticle_x_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_J) == GLFW_PRESS) { SimData->increment_reticle_y_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_K) == GLFW_PRESS) { SimData->increment_reticle_x_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_L) == GLFW_PRESS) { SimData->increment_reticle_y_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }

  if (glfwGetKey(w, GLFW_KEY_T) == GLFW_PRESS) {
    cout << SimData->get_ms_since_init() << endl;
  }
  if (glfwGetKey(w, GLFW_KEY_C) == GLFW_PRESS) {
    SimData->get_enviroment_figure_pos_graph()->free_node_mem();
    GameContext.to_string();
  }
}
void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(renderingProgram);
  listenToKeyboard(window,&SimData,&E);
  if(display_inc % 10 == 0 && display_inc != 0) {
    SimData.update_frame_rate_calculations();
  }
  E.process_antagonist_attacks_on_protagonist();
  if(display_inc % 50 == 0) {
    E.compute_VarPosFigure_paths();
    E.react_to_PlotContext();
  }
  E.iterate_VarPosFigure_positions();
  E.update_protagonist_figure_pos();
  E.update_reticle_pos();
  vector<VisualComponent*> objs = SimData.get_VisualComponents();
  for(int i = 0; i < objs.size(); i++) {
    draw_VisualComponent(objs[i],&SimData);
  }
  display_inc++;
}
int main(void) { // main() is unchanged from before
  if (!glfwInit()) { exit(EXIT_FAILURE); }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  GLFWwindow* window = glfwCreateWindow(600, 600, "i", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
  glfwSwapInterval(1);
  init(window);
  while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
