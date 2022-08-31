#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <stack>
#include <thread> // lazy sleep functionality - remove as possibly exploitable
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

#include "stb_image.h"
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
#include "SimRoutineScheduler.hpp"
#include "SimNonRenderRoutine.hpp"
#include "controllers/VarPosFigure.hpp"
#include "controllers/Floor.hpp"

using namespace std;
#define numVAOs 1
#define numVBOs 7

float cameraX, cameraY, cameraZ;
int display_inc = 0;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint norms[1];
GLuint mvLoc, projLoc, vLoc, light_loc, random_data_loc;
GLfloat tfLoc, timeFactor;

float forward_facing_dir = 1.57;
float backward_facing_dir = 4.71;
float right_facing_dir = 3.14;
float left_facing_dir = 0.0;

int width, height;
float aspect;

unsigned int texture;

glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

glm::vec3 lightloc = glm::vec3(0.0f,5.0f,0.0f);

SimState *SimData;
VertexLoader *Verticies;
PlotContext *GameContext;
Enviroment *E;
SimRoutineScheduler *Srs;

void loadTexture(const char *texImagePath) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char *data = stbi_load(texImagePath, &width, &height, &nrChannels, 0);
  if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "\n\nTEX FAIL\n Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}
void allocate_tool_mem() {
  SimState* SimDataMem = (SimState*) malloc(sizeof(SimState));
  SimDataMem = new (SimDataMem) SimState();
  SimData = SimDataMem;

  PlotContext* PlotContextMem = (PlotContext*) malloc(sizeof(PlotContext));
  PlotContextMem = new (PlotContextMem) PlotContext();
  GameContext = PlotContextMem;

  Enviroment* EnviromentMem = (Enviroment*) malloc(sizeof(Enviroment));
  EnviromentMem = new (EnviromentMem) Enviroment(SimDataMem,PlotContextMem);
  E = EnviromentMem;

  VertexLoader* VertexLoaderMem = (VertexLoader*) malloc(sizeof(VertexLoader));
  VertexLoaderMem = new (VertexLoaderMem) VertexLoader();
  Verticies = VertexLoaderMem;

  SimRoutineScheduler* SimRoutineSchedulerMem = (SimRoutineScheduler*) malloc(sizeof(SimRoutineScheduler));
  SimRoutineSchedulerMem = new (SimRoutineSchedulerMem) SimRoutineScheduler();
  Srs = SimRoutineSchedulerMem;
}
void setup_vertices(void) {
  float cube[Verticies->get_cube_verticies_len()];
  float *cube_ptr = Verticies->get_cube_verticies();
  for(int i = 0; i < Verticies->get_cube_verticies_len(); i++) { cube[i] = *cube_ptr; cube_ptr++; }
  float pyr[Verticies->get_pyramid_verticies_len()];
  float *pyr_ptr = Verticies->get_pyramid_verticies();
  for(int i = 0; i < Verticies->get_pyramid_verticies_len(); i++) { pyr[i] = *pyr_ptr; pyr_ptr++; }
  float sphere[Verticies->get_sphere_verticies_len()];
  float *sphere_ptr = Verticies->get_sphere_verticies();
  for(int i = 0; i < Verticies->get_sphere_verticies_len(); i++) { sphere[i] = *sphere_ptr; sphere_ptr++; }
  float sphere_norms[Verticies->get_sphere_verticies_len()];
  float *spheren_ptr = Verticies->get_sphere_normals();
  for(int i = 0; i < Verticies->get_sphere_verticies_len(); i++) { sphere_norms[i] = *spheren_ptr; spheren_ptr++; }
  float cube_norms[Verticies->get_cube_normals_len()];
  float *cuben_ptr = Verticies->get_cube_normals();
  for(int i = 0; i < Verticies->get_cube_normals_len(); i++) { cube_norms[i] = *cuben_ptr; cuben_ptr++; }
  float cube_texts[2*(Verticies->get_cube_normals_len()/3)];
  float *cubet_ptr = Verticies->get_cube_texts();
  int cc = 0;
  for(int i = 0; i < 2*(Verticies->get_cube_normals_len()/3); i++) { cube_texts[i] = *cubet_ptr; cubet_ptr++; cc++; }
  float sphere_texts[2*(Verticies->get_sphere_normals_len()/3)];
  float *spheret_ptr = Verticies->get_sphere_texts();
  int sc = 0;
  for(int i = 0; i < 2*(Verticies->get_sphere_normals_len()/3); i++) { sphere_texts[i] = *spheret_ptr; spheret_ptr++; sc++;}

  cout << "sc: " << sc << " cc: " << cc << endl;

  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
  glGenBuffers(numVBOs, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyr), pyr, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_norms), sphere_norms, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_norms), cube_norms, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texts), cube_texts, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_texts), sphere_texts, GL_STATIC_DRAW);
  for(int i = 0; i < 100; i++) { cout << ":::ct>" << cube_texts[i] << endl; }
  for(int i = 0; i < 100; i++) { cout << ":::st>" << sphere_texts[i] << endl; }
}
void init_routine_scheduler() {
  SimNonRenderRoutine* SnrrMem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  SnrrMem = new (SnrrMem) SimNonRenderRoutine(E,SimData,2,1000,string("frame rate calculation"));

  SimNonRenderRoutine* Snrr1Mem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  Snrr1Mem = new (Snrr1Mem) SimNonRenderRoutine(E,SimData,2,500,string("process antagonist attacks"));

  SimNonRenderRoutine* Snrr2Mem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  Snrr2Mem = new (Snrr2Mem) SimNonRenderRoutine(E,SimData,2,1500,string("compute figure paths"));

  SimNonRenderRoutine* Snrr3Mem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  Snrr3Mem = new (Snrr3Mem) SimNonRenderRoutine(E,SimData,3,3000,string("enviroment plot reaction"));

  SimNonRenderRoutine* Snrr4Mem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  Snrr4Mem = new (Snrr4Mem) SimNonRenderRoutine(E,SimData,1,20,string("inc VarPosFigure positions"));

  SimNonRenderRoutine* Snrr5Mem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  Snrr5Mem = new (Snrr5Mem) SimNonRenderRoutine(E,SimData,1,5,string("update protagonist position"));

  SimNonRenderRoutine* Snrr6Mem = (SimNonRenderRoutine*) malloc(sizeof(SimNonRenderRoutine));
  Snrr6Mem = new (Snrr6Mem) SimNonRenderRoutine(E,SimData,1,5,string("update reticle position"));

  Srs->add_routine(SnrrMem);
  Srs->add_routine(Snrr1Mem);
  Srs->add_routine(Snrr2Mem);
  Srs->add_routine(Snrr3Mem);
  Srs->add_routine(Snrr4Mem);
  Srs->add_routine(Snrr5Mem);
  Srs->add_routine(Snrr6Mem);
}
void generate_figure_graph() {
  glm::vec3 figure_graph_max_pos_vert = glm::vec3(25.0f,0.5f,25.0f);
  glm::vec3 figure_graph_dimensions = glm::vec3(50.0,-0.5,50.0);
  // generate bound for figure movement graph
  Bound* FigureMovementGraphBoundMem = (Bound*) malloc(sizeof(Bound));
  FigureMovementGraphBoundMem = new (FigureMovementGraphBoundMem) Bound(figure_graph_max_pos_vert,figure_graph_dimensions,true);
  // FigureMovementGraphBoundMem->generate_rectangular_bounds(figure_graph_max_pos_vert,figure_graph_dimensions,true);
  // generate figure movement graph
  Graph* GraphMem = (Graph*) malloc(sizeof(Graph));
  GraphMem = new (GraphMem) Graph();
  GraphMem->compute_cartesian_nodes_from_Bound(1,FigureMovementGraphBoundMem);
  // save pointers to SimState
  SimData->set_figure_movement_graph_Bound(FigureMovementGraphBoundMem);
  SimData->set_enviroment_figure_pos_graph(GraphMem);
  // save pointers to Enviroment
  E->set_enviroment_figure_pos_graph(GraphMem);
}
void init(GLFWwindow* window) {
  allocate_tool_mem();

  Utils U;
  U.print_system_data();
  renderingProgram = U.createShaderProgram();
  cameraX = -0.0f; cameraY = 2.0f; cameraZ = 0.0f;
  SimData->set_camera_position(glm::vec3(-cameraX,-cameraY,-cameraZ));
  setup_vertices();

  loadTexture("fries.jpg");

  generate_figure_graph();

  E->init_figure_Floor();
  E->init_protagonist();
  E->init_Reticle();

  init_routine_scheduler();
  // SimData->init_camera_rots();
}
void draw_VisualComponent(VisualComponent *Vc,SimState *SimData) {
  // allocate gl uniform space
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
  vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
  light_loc = glGetUniformLocation(renderingProgram,"light_pos");
  random_data_loc = glGetUniformLocation(renderingProgram,"random_data");
  // compute VC pos mat
  string vc_type = Vc->get_parent_type_str();
  vector<glm::mat4> VcMats;
  if(vc_type == "Floor") {
    VcMats = Vc->get_Floor_parent()->position_computation(SimData,Vc);
  } else if(vc_type == "Reticle") {
    glm::vec3 reticlepos = Vc->get_position();
    VcMats = Vc->get_Reticle_parent()->position_computation(SimData,Vc);
  } else if(vc_type == "StaticFigure") {
    VcMats = Vc->get_StaticFigure_parent()->position_computation(SimData,Vc);
  } else if(vc_type == "HUD") {
    VcMats = Vc->get_hud_parent()->position_computation(SimData,Vc);
  } else {
    VcMats = Vc->get_VarPosFigure_parent()->position_computation(SimData,Vc);
    if(Vc->get_VarPosFigure_parent() == E->get_protagonist()) {
      glm::vec3 protpos = Vc->get_position();
      return;
    }
  }
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(VcMats[1] * VcMats[2]));
  glUniform3f(light_loc, 0.0f, 5.0f, 0.0f);
  glUniform3f(random_data_loc, 1.0, 1.0, 1.0);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(VcMats[0]));

  glBindTexture(GL_TEXTURE_2D, texture);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[Vc->get_vbo_index()]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  if(vc_type == "Floor") {
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
  } else if(vc_type == "VarPosFigure") {
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, Vc->get_vertex_count());
}
void listenToKeyboard(GLFWwindow* w,SimState *SimData,Enviroment *Env) {
  if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS && SimData->is_shot_available()) {
    // if(!SimData->is_shot_available()) { return; }
    SimData->increment_shot_count();
    SimData->set_last_shot_timestamp();
    glm::vec3 reticle_position = E->get_reticle()->get_position();
    glm::vec3 cameraview_position = glm::vec3(-SimData->get_camera_position()[0],-SimData->get_camera_position()[1],-SimData->get_camera_position()[2]);
    E->execute_shot(cameraview_position,reticle_position);
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
  if (glfwGetKey(w, GLFW_KEY_R) == GLFW_PRESS) { E->compute_VarPosFigure_paths(); }
  if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) { SimData->increment_camera_y_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) { SimData->increment_camera_y_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) { SimData->increment_camera_x_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) { SimData->increment_camera_x_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }

  if (glfwGetKey(w, GLFW_KEY_I) == GLFW_PRESS) { SimData->increment_reticle_x_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_J) == GLFW_PRESS) { cout << SimData->config.reticle_rotation_sensitivity << endl; SimData->increment_reticle_y_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_K) == GLFW_PRESS) { SimData->increment_reticle_x_rot(1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_L) == GLFW_PRESS) { SimData->increment_reticle_y_rot(-1.0f*SimData->config.reticle_rotation_sensitivity); }
  if (glfwGetKey(w, GLFW_KEY_H) == GLFW_PRESS) { E->draw_hud(); }

  if (glfwGetKey(w, GLFW_KEY_T) == GLFW_PRESS) {
    cout << SimData->get_ms_since_init() << endl;
  }
  if (glfwGetKey(w, GLFW_KEY_C) == GLFW_PRESS) {
    SimData->get_enviroment_figure_pos_graph()->free_node_mem();
    GameContext->to_string();
  }
}
void display(GLFWwindow* window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(renderingProgram);
  listenToKeyboard(window,SimData,E);

  if(display_inc == 200) { cout << "\ninit HUD\n" << endl; E->init_HUD(); }

  //cout << "prot pos: " << glm::to_string(E->get_protagonist()->get_position()) << endl;

  vector<SimNonRenderRoutine*> queued_routines = Srs->sort_routines();
  for(int i = 0; i < queued_routines.size(); i++) {
    queued_routines[i]->execute_instructions();
  }

  vector<VisualComponent*> objs = SimData->get_VisualComponents();
  for(int i = 0; i < objs.size(); i++) {
    draw_VisualComponent(objs[i],SimData);
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
