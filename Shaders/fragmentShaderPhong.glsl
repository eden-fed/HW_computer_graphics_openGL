#version 150 //compatible with OpenGL version 3.2

struct stLightProperties {
bool isEnabled;
bool isDirectional;
vec3 position;
vec3 direction;
vec3 intensity;
vec3 color;
};

struct stMaterialProperties {
float ambient;
float diffuse;
float specular;
float specularExp;
};


//interpolated inputes
in vec4 P;
in vec4 N;
in vec4 L[2];
in vec4 R[2];

//regular inputes
uniform vec4 AmbientProduct;                
uniform vec4 eyePosition                     //cammera position
uniform stLightProperties Lights[2];         //array of cameras
uniform stMaterialProperties material;       //material attributes


out vec4 outFragColor;

void main()
{
	vec4 color;

	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	R[1] = normalize(R[1]);
	R[2] = normalize(R[2]);

	//calculate ambiant lighting
	vec4 ambient = material.ambient * AmbientProduct;

	//init vectors
	vec4 diffuse  = vec4(0.0,0.0,0.0);
	vec4 specular = vec4(0.0,0.0,0.0) ;
	

	vec4 V = normalize(eyePosition - P);					
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
	
	//output color
	outFragColor = color;
}
