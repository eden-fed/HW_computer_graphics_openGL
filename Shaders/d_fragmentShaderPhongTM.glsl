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


//interpolated inputes
in vec4 P;
in vec4 N;
in vec4 L[2];
in vec4 R[2];
in vec2 texCoord;

//regular inputes
uniform sampler texture;
uniform vec4 AmbientProduct;                
uniform vec4 eyePosition;                     //cammera position
uniform stLightProperties Lights[2];         //array of cameras
uniform stMaterialProperties material;       //material attributes


out vec4 outFragColor;

void main()
{
	vec4 color;
	vec4 eyePosition=vec4(0.0,0.0,0.0,1.0);

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

					//calculate specular lighting
					D=dot(vec3(R[lNum]), vec3(V));
					float RV = pow(D,material.specularExp);
					specular += material.specular*RV*Lights[lNum].intensity;
		}

	}

    color = ambient + diffuse + specular;
    color.a = 1.0;
	
	//output color
	outFragColor = color* texture( texture, texCoord );
}
