#version 430
in vec4 varyingColor;
in vec2 TexCoord;
out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform sampler2D ourTexture;
void main(void) {
  color = texture(ourTexture,TexCoord); // * varyingColor;
  //color = varyingColor;
}
