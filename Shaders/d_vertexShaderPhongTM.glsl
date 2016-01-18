#version 150 //compatible with OpenGL version 3.2


struct stLightProperties {
bool isEnabled;
bool isDirectional;
vec4 position;
vec4 direction;
vec4 intensity;
};

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;
uniform stLightProperties Lights[2];         //array of cameras

in vec4 VertexNormal;
in vec4 VertexPosition;
in vec2 vTexCoord;

out vec4 P;
out vec4 N;
out vec4 L[2];
out vec4 R[2];
out vec2 texCoord;

void main()
{
	N.xyz = normalize(vec3(NormalMatrix * VertexNormal)); N.w=1;
	P = MVMatrix * VertexPosition;

	for (int lNum = 0; lNum < 2; ++lNum) {
			L[lNum]=vec4(0.0,0.0,0.0,1.0);
			R[lNum]=vec4(0.0,0.0,0.0,1.0);
			if (Lights[lNum].isEnabled){
				if (Lights[lNum].isDirectional)
				{
					L[lNum] = Lights[lNum].position - Lights[lNum].direction;
				}
				else
				{
					L[lNum] = Lights[lNum].position - P;
				}

				float D=dot(vec3(L[lNum]), vec3(N));
				float NL = max(D, 0.0 );
				R[lNum] = ((NL * 2)*N) - L[lNum];

				L[lNum].xyz = normalize(vec3(L[lNum]));
				R[lNum].xyz = normalize(vec3(R[lNum]));
			}
	}

	gl_Position = MVPMatrix * VertexPosition;
	texCoord    = vTexCoord;
}
