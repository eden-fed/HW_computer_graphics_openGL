#version 150 //compatible with OpenGL version 3.2


struct stLightProperties {
bool isEnabled;
bool isDirectional;
vec4 position;
vec4 direction;
vec4 intensity;
vec4 color;
};

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;
uniform stLightProperties Lights[2];         //array of cameras

in mat4 VertexNormal;
in vec4 VertexPosition;

out vec4 P;
out vec4 N;
out vec4 L[2];
out vec4 R[2];

void main()
{
	N = normalize(NormalMatrix * VertexNormal);
	P = MVMatrix * VertexPosition;

	for (int lNum = 0; lNum < 2; ++lNum) {
			L[lNum]=vec4(0.0)
			R[lNum]=vec4(0.0)
			if (Lights[lNum].isEnabled){
				if (Lights[lNum].isDirectional)
					L[lNum] = Lights[lNum].position - Lights[lNum].direction;
				else
					L[lNum] = Lights[lNum].position - P;

				float NL = max( dot(L[lNum], N), 0.0 );
				R[lNum] = (N * (NL * 2)) - L[lNum];
			}
	}

	gl_Position = MVPMatrix * VertexPosition;
}
