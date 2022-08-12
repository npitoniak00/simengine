#include "Utils.hpp"
#include <iostream>

void Utils::printShaderLog(GLuint shader) {
  using namespace std;
  int len = 0;
  int chWrittn = 0;
  char *log;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetShaderInfoLog(shader, len, &chWrittn, log);
    cout << "Shader Info Log: " << log << endl;
    free(log);
  }
}
void Utils::print_system_data() {
  int sysinfo(struct sysinfo *info);
  struct sysinfo {
       long uptime;             /* Seconds since boot */
       unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
       unsigned long totalram;  /* Total usable main memory size */
       unsigned long freeram;   /* Available memory size */
       unsigned long sharedram; /* Amount of shared memory */
       unsigned long bufferram; /* Memory used by buffers */
       unsigned long totalswap; /* Total swap space size */
       unsigned long freeswap;  /* swap space still available */
       unsigned short procs;    /* Number of current processes */
       unsigned long totalhigh; /* Total high memory size */
       unsigned long freehigh;  /* Available high memory size */
       unsigned int mem_unit;   /* Memory unit size in bytes */
       char _f[20-2*sizeof(long)-sizeof(int)]; /* Padding for libc5 */
  } sysdata;
  cout << "\n\n================ SYSTEM DATA PRINT" << endl;
  cout << "seconds since boot: " << sysdata.uptime << endl;
  cout << "total ram: " << sysdata.totalram << endl;
  cout << "free ram: " << sysdata.freeram << endl;
  cout << "buffer ram: " << sysdata.bufferram << endl;
  cout << "mem unit: " << sysdata.mem_unit << endl;
  cout << "procs: " << sysdata.procs << endl;
  cout << "totalhigh: " << sysdata.totalhigh << endl;
  cout << "freehigh: " << sysdata.freehigh << endl;
  cout << "====================\n" << endl;
}
void Utils::printProgramLog(int prog) {
  using namespace std;
  int len = 0;
  int chWrittn = 0;
  char *log;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetProgramInfoLog(prog, len, &chWrittn, log);
    cout << "Program Info Log: " << log << endl;
    free(log);
  }
}
bool Utils::checkOpenGLError() {
  using namespace std;
  bool foundError = false;
  int glErr = glGetError();
  while (glErr != GL_NO_ERROR) {
    cout << "glError: " << glErr << endl;
    foundError = true;
    glErr = glGetError();
  }
  return foundError;
}
string Utils::readShaderSource(const char *filePath) {
  using namespace std;
  string content;
  ifstream fileStream(filePath, ios::in);
  string line = "";
  while (!fileStream.eof()) {
    getline(fileStream, line);
    content.append(line + "\n");
  }
  fileStream.close();
  return content;
}
GLuint Utils::createShaderProgram() {
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  string vertShaderStr = readShaderSource("shaders/vs.glsl");
  string fragShaderStr = readShaderSource("shaders/fs.glsl");
  const char *vertShaderSrc = vertShaderStr.c_str();
  const char *fragShaderSrc = fragShaderStr.c_str();
  glShaderSource(vShader, 1, &vertShaderSrc, NULL);
  glShaderSource(fShader, 1, &fragShaderSrc, NULL);
  glCompileShader(vShader);
  glCompileShader(fShader);
  GLuint vfProgram = glCreateProgram();
  glAttachShader(vfProgram, vShader);
  glAttachShader(vfProgram, fShader);
  glLinkProgram(vfProgram);
  return vfProgram;
}
