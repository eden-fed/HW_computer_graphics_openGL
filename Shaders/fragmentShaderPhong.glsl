#version 150 //compatible with OpenGL version 3.2

in vec4 P;
in vec4 N;
in vec4 L1;
in vec4 L2;
in vec4 R1;
in vec4 R2;

uniform vec4 AmbientProduct;

//cammera position
uniform vec4 eyePosition;

//light number 1
uniform vec4 light1_position;//
uniform vec4 light1_direction;//
uniform vec4 light1_intensity;
uniform bool light1_isDirectional;//
//light number 2
uniform vec4 light2_position;//
uniform vec4 light2_direction;//
uniform vec4 light2_intensity;
uniform bool light2_isDirectional;//

//material attributes
uniform double material_ambient;
uniform double material_diffuse;
uniform double material_specular;
uniform double material_specularExp;


out vec4 outFragColor;

void main()
{
	vec4 color;

	P  = normalize(P);
	N  = normalize(N);
	L  = normalize(L1);
	L2 = normalize(L2);
	R1 = normalize(R1);
	R2 = normalize(R2);

	//calculate ambiant lighting
	vec4 ambient = material_ambient * AmbientProduct;

	//calculate diffuse lighting
	double NL1 = max( dot(L1, N), 0.0 );
    vec4  diffuse1 = material_diffuse*NL1*light1_intensity;

	double NL2 = max( dot(L21, N), 0.0 );
    vec4  diffuse2 = material_diffuse*NL2*light2_intensity;

	//calculate specular lighting
	vec4 V = eyePosition - P;				//calculation for both light sources
	V  = normalize(V);						//calculation for both light sources

	double R1V = pow(dot(R1, V),material_specularExp);
	vec4  specular1 = material_specular*R1V*light1_intensity;

	double R2V = pow(dot(R2, V),material_specularExp);
	vec4  specular2 = material_specular*R2V*light2_intensity;


    //gl_Position = Projection * ModelView * vPosition;

    color = ambient + diffuse1 + diffuse2 + specular1 + specular2;
    color.a = 1.0;




	outFragColor = color;
}
