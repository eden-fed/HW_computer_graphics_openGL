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
uniform sampler2D normalMapHandle;
uniform sampler2D texMapHandle;
uniform vec4 AmbientProduct;                
uniform vec4 eyePosition;                     //cammera position
uniform stLightProperties Lights[2];         //array of cameras
uniform stMaterialProperties material;       //material attributes


out vec4 outFragColor;

void main()
{
	vec4 NM=texture( normalMapHandle, texCoord );
	// Expand the range of the normal value from (0, +1) to (-1, +1).
	NM.x = (2.0 * NM.x) - 1.0;
	NM.y = (2.0 * NM.y) - 1.0;
	NM.z = (2.0 * NM.z) - 1.0;
	NM.r*=N.x;
	NM.g*=N.y;
	NM.b*=N.z;
	vec4 normal;
	normal.xyz = normalize(vec3(NM)); normal.w=1;
	
	
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
					D=dot(vec3(L[lNum]), vec3(normal));	
					float NL = max( D, 0.0 );
					diffuse += material.diffuse*NL*Lights[lNum].intensity;

					//calculate specular lighting
					D=dot(vec3(R[lNum]), vec3(V));
					D=max( D, 0.0 );
					float RV = pow(D,material.specularExp);
					specular += material.specular*RV*Lights[lNum].intensity;
		}

	}

    color = ambient + diffuse + specular;
    color.a = 1.0;
	
	//output color
	outFragColor = color ;//+ texture( texMapHandle, texCoord );
}
