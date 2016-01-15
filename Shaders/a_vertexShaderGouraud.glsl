#version 150 //compatible with OpenGL version 3.2
struct stLightProperties {
bool isEnabled;
bool isDirectional;
vec4 position;
vec4 direction;
vec4 intensity;
};

struct stMaterialProperties {
float ambient;
float diffuse;
float specular;
float specularExp;
};

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat4 NormalMatrix;
uniform stLightProperties Lights[2];         //array of cameras
uniform stMaterialProperties material;       //material attributes
uniform vec4 AmbientProduct;  


in mat4 VertexNormal;
in vec4 VertexPosition;

out vec4 color;


void main()
{
vec4 P;
vec4 L[2];
vec4 R[2];
vec4 N=vec4(0.0,0.0,0.0,1.0);
vec4 eyePosition=vec4(0.0,0.0,0.0,1.0);

N.xyz = normalize(vec3(NormalMatrix * VertexNormal));
P = MVMatrix * VertexPosition;

for (int lNum = 0; lNum < 2; ++lNum) {
		L[lNum]=vec4(0.0,0.0,0.0,1.0);
		R[lNum]=vec4(0.0,0.0,0.0,1.0);
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

	L[0].xyz = normalize(vec3(L[0]));
	L[1].xyz = normalize(vec3(L[1]));
	R[0].xyz = normalize(vec3(R[0]));
	R[1].xyz = normalize(vec3(R[1]));

	//calculate ambiant lighting
	vec4 ambient = material.ambient * AmbientProduct;

	//init vectors
	vec4 diffuse  = vec4(0.0,0.0,0.0,1.0);
	vec4 specular = vec4(0.0,0.0,0.0,1.0) ;
	


	vec4 V=vec4(0.0,0.0,0.0,1.0);
	V.xyz = normalize(vec3(eyePosition - P));					
	for (int lNum = 0; lNum < 2; ++lNum) {
		if (Lights[lNum].isEnabled){
					//calculate diffuse lighting
					float NL = max( dot(L[lNum], N), 0.0 );
					diffuse += material.diffuse*NL*Lights[lNum].intensity;
					
					//calculate specular lighting
					float RV = pow(dot(R[lNum], V),material.specularExp);
					specular += material.specular*RV*Lights[lNum].intensity;
		}

	}

    color = ambient + diffuse + specular;
    color.a = 1.0;

}

