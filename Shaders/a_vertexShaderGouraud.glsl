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
	vec4 N;
	vec4 eyePosition=vec4(0.0,0.0,0.0,1.0);

	N.xyz = normalize(vec3(NormalMatrix * VertexNormal)); N.w=1;

	P = MVMatrix * VertexPosition;


	//calculate L and R
	for (int lNum = 0; lNum < 2; ++lNum) {
			L[lNum]=vec4(0.0,0.0,0.0,1.0);
			R[lNum]=vec4(0.0,0.0,0.0,1.0);
			if (Lights[lNum].isEnabled){
				if (Lights[lNum].isDirectional)
				{
					L[lNum] = Lights[lNum].position - Lights[lNum].direction;
					color = vec4(1.0, 0.0, 0.0, 1.0);
					return;
				}
				else
				{
					L[lNum] = Lights[lNum].position - P;
					color = vec4(1.0, 0.0, 0.0, 1.0);
					return;
				}

				float D=dot(vec3(L[lNum]), vec3(N));
				float NL = max(D, 0.0 );
				R[lNum] = ((NL * 2)*N) - L[lNum];

				L[lNum].xyz = normalize(vec3(L[lNum]));
				R[lNum].xyz = normalize(vec3(R[lNum]));
			}
	}

	gl_Position = MVPMatrix * VertexPosition;

	//calculate ambiant lighting
	vec4 ambient = material.ambient * AmbientProduct;

	//init vectors
	vec4 diffuse  = vec4(0.0,0.0,0.0,1.0);
	vec4 specular = vec4(0.0,0.0,0.0,1.0) ;

	vec4 V;
	V.xyz = normalize(vec3(eyePosition - P)); V.w=1;		
			
	for (int lNum = 0; lNum < 2; ++lNum) {
		if (Lights[lNum].isEnabled){
					float D;
					//calculate diffuse lighting
					D=dot(vec3(L[lNum]), vec3(N));	
					float NL = max( D, 0.0 );
					diffuse += material.diffuse*NL*Lights[lNum].intensity;
					
					if( D < 0.0 ){
					    specular = vec4(0.0, 0.0, 0.0, 1.0);
						continue;
					}

					//calculate specular lighting
					D=dot(vec3(R[lNum]), vec3(V));
					float RV = pow(D,material.specularExp);
					specular += material.specular*RV*Lights[lNum].intensity;
		}

	}

    color = ambient + diffuse + specular;

    color.a = 1.0;
}

