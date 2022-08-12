#ifndef DAMAGERECORD_H_
#define DAMAGERECORD_H_

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class DamageRecord {
private:
  float damage;
public:
  DamageRecord();
  DamageRecord(float damage_init);
  void increment_damage_record(float damage);
  float get_damage_record();
  ~DamageRecord();
};

#endif
