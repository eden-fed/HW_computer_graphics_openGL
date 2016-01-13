#version 150 //compatible with OpenGL version 3.2

in vec4 vPosition;
in vec4 vNormal;

//
//
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 projection;
uniform float scale;
//uniform bool space;//true=world space

out vec4 color;

void main()
{
	//gl_Position is a vec4 built-in GLSL output variable that holds the transformed vertex position
	gl_Position = vPosition;

	//uniform scaling - dividing the w coordinate is like multiplying the x, y, z coordinates
 	gl_Position.w = gl_Position.w / scale;

	//rotate then translate in world coordinates
	//if(space)
	gl_Position = gl_Position*rotation*translation;
	//else gl_Position = gl_Position*translation*rotation;
	
	//projection
	gl_Position = gl_Position*projection;

	color = vColor;
}
