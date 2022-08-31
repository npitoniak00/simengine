#version 430
layout (location=0) in vec3 position;
layout (location=1) in vec3 vertNormal;
layout (location=2) in vec2 aTexCoord;
uniform vec3 random_data;
uniform vec3 light_pos;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
out vec4 varyingColor;
out vec2 TexCoord;
void main(void) {
  vec3 modelViewNormal = vec3(mv_matrix * vec4(vertNormal, 0.0));
  vec3 modelViewVertex = vec3(mv_matrix * proj_matrix * vec4(position,1.0));
  vec3 lp = vec3(mv_matrix * vec4(vec3(0.0,2.0,0.0),1.0));
  float dist = pow( (pow(modelViewVertex[0]-lp[0],2)) + (pow(modelViewVertex[1]-lp[1],2)) + (pow(modelViewVertex[2]-lp[2],2)) ,0.5);
  vec3 lightVector = normalize(vec3(lp[0]-modelViewVertex[0],lp[1]-modelViewVertex[1],lp[2]-modelViewVertex[2]));
  float t = max(dot(lightVector,modelViewNormal),0.1);
  float diffuse = (1.0 / (1.0 + (0.1 * dist)));

  gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
  vec4 ambient = vec4(3.0,1.0,1.0,1.0);
  //varyingColor = ((vec4(light_pos,1.0) * vec4(position,1.0)) * 0.5) * (ambient * t * diffuse) + vec4(0.5, 0.5, 0.5, 0.5);
  varyingColor = vec4(0.5,0.5,0.5,1.0); // * ambient * t * diffuse;
  //varyingColor = vec4(0.3,0.5,0.3,1.0);
  TexCoord = aTexCoord;
}
