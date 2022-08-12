#ifndef VERTEXLOADER_H_
#define VERTEXLOADER_H_

#include <iostream>
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

class VertexLoader {
  private:
    float *cube_verticies;
    float *pyramid_verticies;
    int cube_verticies_len = 108;
    int pyr_verticies_len = 54;
  public:
    VertexLoader();
    vector<glm::vec3> generate_simple_bezier(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,int prec);
    float *get_pyramid_verticies();
    int get_pyramid_verticies_len();
    float *get_cube_verticies();
    int get_cube_verticies_len();
    void load_pyramid_verticies();
    void load_cube_verticies();
};

#endif
