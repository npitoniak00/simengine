#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Utils {
public:
  string readShaderSource(const char *filePath);
  GLuint createShaderProgram();
  void printShaderLog(GLuint shader);
  void printProgramLog(int prog);
  bool checkOpenGLError();
  void print_system_data();
};

#endif
