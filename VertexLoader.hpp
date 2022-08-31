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
    float *cube_normals;
    float *cube_textures;
    float *pyramid_verticies;
    float *sphere_verticies;
    float *sphere_normals;
    float *sphere_textures;
    int cube_verticies_len = 108;
    int cube_normals_len = 108*2;
    int pyr_verticies_len = 54;
    int sphere_verticies_len = 6840*3;
    int sphere_normals_len = 1254;
  public:
    VertexLoader();
    float toRadians(float degrees);
    vector<glm::vec3> generate_simple_bezier(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,int prec);
    float *get_sphere_texts();
    float *get_cube_texts();
    float *get_sphere_verticies();
    float *get_pyramid_verticies();
    int get_sphere_verticies_len();
    int get_pyramid_verticies_len();
    float *get_cube_verticies();
    float *get_cube_normals();
    int get_cube_verticies_len();
    int get_cube_normals_len();
    float *get_sphere_normals();
    int get_sphere_normals_len();
    void load_pyramid_verticies();
    void load_cube_verticies();
    void load_sphere_verticies();
};

#endif
