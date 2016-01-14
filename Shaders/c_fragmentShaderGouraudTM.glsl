#version 150 //compatible with OpenGL version 3.2

in vec4 color;
in vec2 texCoord;

out vec4 fColor;

uniform sampler texture;

void main() 
{ 
  fColor = color * texture( texture, texCoord );
} 
