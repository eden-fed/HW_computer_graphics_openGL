#version 150 //compatible with OpenGL version 3.2

in vec4 color;
in vec2 texCoord;

out vec4 fColor;

uniform sampler2D texMapHandle;

void main() 
{ 
  fColor = color + texture( texMapHandle, texCoord);
  //fColor = texture( texMapHandle, texCoord);
} 
