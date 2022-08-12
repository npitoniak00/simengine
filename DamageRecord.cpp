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

#include "DamageRecord.hpp"

using namespace std;

DamageRecord::DamageRecord() {
  damage = 1.0;
}
DamageRecord::DamageRecord(float damage_init) {
  damage = damage_init;
}
void DamageRecord::increment_damage_record(float damage_adjustment) {
  damage += damage_adjustment;
}
float DamageRecord::get_damage_record() {
  return damage;
}
DamageRecord::~DamageRecord() {
  cout << "destroying DamageRecord " << endl;
}
