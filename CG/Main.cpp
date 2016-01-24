#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>
#include "SOIL.h"

#include <vector>
#include <Windows.h>
#include <assert.h>

#include "Obj Parser/wavefront_obj.h"
#include "Utils.h"
#include "MeshModel.h"
#include "Camera.h"

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE MAX_PATH
#define PI 3.14159265358979323846 

typedef enum {
	GOURAUD,
	PHONG
} eShadingType;

TCHAR g_Buffer[BUFSIZE];


float g_scale = 1.0f;
float g_quaternion[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
bool g_drawWireframe = false;
//bool g_drawWireframe = true;


unsigned int g_Color = 0xff0000ff;

float g_Swidth = 1366;
float g_Sheight = 768;

float g_near = 0.01;
float g_far = 10000;
float g_fovy = 60;

float g_translationX = 0.0;
float g_translationY = 0.0;
float g_translationZ = 0.0;

float g_xRotation = 0.0;
float g_yRotation = 0.0;
float g_zRotation = 0.0;

//global veriables for glut functions
bool g_reset = false;
bool g_centerCam = false;
bool g_showCrdSystem = false;
bool g_normals = false;
bool g_bbox = false;
bool g_projectionType = true;
bool g_space = true;//initialize to world space
double g_normals_size = 1;


float g_ambient = 0.1;
float g_diffuse = 0.6;
float g_specular = 0.7;
float g_specularExp = 32;
float g_xLightPosition = 0.0;
float g_yLightPosition = 0.0;
float g_zLightPosition = 0.0;
float g_xLightDirection = 0.0;
float g_yLightDirection = 0.0;
float g_zLightDirection = -5;
bool g_lightType = false;
//unsigned int g_lightIntensity = 0xffffff;
float g_lightIntensity[4] = { 1, 0, 0 ,1 }; // red								 

float g_ambientLight[4] = { 1, 0, 0 ,1 }; // red								 

TwType shadingType;
bool g_mesh = false;
eShadingType g_shadingType = GOURAUD;

MeshModel model;
int numV;//number of vertices

GLuint g_tex[2] = {0,0};
bool g_useTM = false;
bool g_useNM = false;

GLuint g_vertexArrayID = 0;
GLuint g_vertexBufferObjectID = 0;
GLuint g_activeProgramID = 0;
GLuint g_programID1 = 0;
GLuint g_programID2 = 0;
GLuint g_programID3 = 0;
GLuint g_programID4 = 0;
GLuint g_programID5 = 0;


Matrix4x4 transformations_matrix;
Matrix4x4 rotations_matrix;
Matrix4x4 saveCamera(1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

bool g_light1Enable=false;
bool g_light2Enable=false;


void TW_CALL loadOBJModel(void* clientData);
void TW_CALL loadTMFile(void* clientData);
void TW_CALL loadNMFile(void* clientData);
void TW_CALL Start(void* clientData);

void TW_CALL applyTranslation(void* clientData);
void TW_CALL applyScale(void* clientData);
void TW_CALL applyXrotation(void* clientData);
void TW_CALL applyYrotation(void* clientData);
void TW_CALL applyZrotation(void* clientData);
void TW_CALL applyLight1(void* clientData);
void TW_CALL applyLight2(void* clientData);


void initScene();
void initGraphics(int argc, char *argv[]);
void drawScene();
void Display();
void Reshape(int width, int height);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void Special(int k, int x, int y);
void Terminate(void);







int main(int argc, char *argv[])
{
	// Initialize openGL, glut, glew
	initGraphics(argc, argv);
	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	// Set GLUT callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	//initScene();

	atexit(Terminate);  // Called after glutMainLoop ends

						// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 600' color='96 216 224' "); // change default tweak bar size and color

	TwAddButton(bar, "LoadOBJ", loadOBJModel, NULL, "help='button to load obj file'");
	TwAddButton(bar, "load_TM", loadTMFile, NULL, "help='button to load PNG file'");
	TwAddButton(bar, "load_NM", loadNMFile, NULL, "help='button to load PNG file'");
	TwAddButton(bar, "Start", Start, NULL, "help='button to start file'");
	TwAddSeparator(bar, NULL, NULL);

	TwAddVarRW(bar, "reset", TW_TYPE_BOOLCPP, &g_reset, "help='reset everything'");
	TwAddVarRW(bar, "OW space", TW_TYPE_BOOLCPP, &g_space, " help='true=transform in world space ,false=transform in object space' ");
	TwAddVarRW(bar, "OW Crd System", TW_TYPE_BOOLCPP, &g_showCrdSystem, " help='boolean variable to indicate if to show WO coordinate system or not.' ");
	TwAddVarRW(bar, "showBbox", TW_TYPE_BOOLCPP, &g_bbox, " help='boolean variable to indicate if to show the bbox or not.' ");

	TwAddVarRW(bar, "showNormals", TW_TYPE_BOOLCPP, &g_normals, " help='boolean variable to indicate if to show normals or not.' group='normals'");
	TwAddVarRW(bar, "normalsSize", TW_TYPE_FLOAT, &g_normals_size, " min=0.1 max=100 step=0.1 keyIncr=t keyDecr=T help='Change notmals size (20=original size).' group='normals'");
	TwAddVarRW(bar, "projectionType", TW_TYPE_BOOLCPP, &g_projectionType, " help='true = orthographic, false = perspective.' group='camera'");
	TwAddVarRW(bar, "near", TW_TYPE_FLOAT, &g_near, "step=0.01 keyIncr=n keyDecr=N  group='camera'");
	TwAddVarRW(bar, "far", TW_TYPE_FLOAT, &g_far, "step=0.1 keyIncr=f keyDecr=F  group='camera'");
	TwAddVarRW(bar, "fovy", TW_TYPE_FLOAT, &g_fovy, "step=0.1 keyIncr=v keyDecr=V  group='camera'");

	TwAddVarRW(bar, "centerCamera", TW_TYPE_BOOLCPP, &g_centerCam, "help='point the camera to the center of the model'  group='camera'");

	TwAddVarRW(bar, "translate X", TW_TYPE_FLOAT, &g_translationX, "min=-30 max=30 step=1 keyIncr=right keyDecr=left   group='tranfromations' ");
	TwAddVarRW(bar, "translate Y", TW_TYPE_FLOAT, &g_translationY, "min=-30 max=30 step=1 keyIncr=up keyDecr=down   group='tranfromations' ");
	TwAddVarRW(bar, "translate Z", TW_TYPE_FLOAT, &g_translationZ, "min=-30 max=30 step=1 keyIncr=> keyDecr=<   group='tranfromations' ");
	TwAddButton(bar, "apply translation", applyTranslation, NULL, "help='apply translation' group='tranfromations' ");

	TwAddVarRW(bar, "scale", TW_TYPE_FLOAT, &g_scale, " min=0.01 max=2.5 step=0.01 keyIncr=+ keyDecr=-   group='tranfromations' ");
	TwAddButton(bar, "apply scale", &applyScale, NULL, "help='apply scale' group='tranfromations' ");

	TwAddVarRW(bar, "x-rotation", TW_TYPE_FLOAT, &g_xRotation, "min = -360 max = 360 step=1 keyIncr=x keyDecr=X   group='tranfromations' ");
	TwAddButton(bar, "apply x rotation", &applyXrotation, NULL, " help='apply scale' group='tranfromations' ");

	TwAddVarRW(bar, "y-rotation", TW_TYPE_FLOAT, &g_yRotation, "min = -360 max = 360 step=1 keyIncr=y keyDecr=Y   group='tranfromations' ");
	TwAddButton(bar, "apply y rotation", &applyYrotation, NULL, " help='apply scale'  group='tranfromations' ");

	TwAddVarRW(bar, "z-rotation", TW_TYPE_FLOAT, &g_zRotation, "min = -360 max = 360 step=1 keyIncr=z keyDecr=Z   group='tranfromations' ");
	TwAddButton(bar, "apply z rotation", &applyZrotation, NULL, " help='apply scale' group='tranfromations' ");

	//add 'g_quaternion' to 'bar': this is a variable of type TW_TYPE_QUAT4D which defines the object's orientation using quaternions
	//TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_quaternion, " label='Object rotation' opened=true help='This is object rotation' group=tranfromations ");

	TwAddVarRW(bar, "ambient", TW_TYPE_FLOAT, &g_ambient, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "diffuse", TW_TYPE_FLOAT, &g_diffuse, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "specular", TW_TYPE_FLOAT, &g_specular, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "specularExp", TW_TYPE_FLOAT, &g_specularExp, "min = 0 max = 1000 step=1 keyIncr=z keyDecr=Z   group='material' ");

	TwAddVarRW(bar, "x-position", TW_TYPE_FLOAT, &g_xLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "y-position", TW_TYPE_FLOAT, &g_yLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "z-position", TW_TYPE_FLOAT, &g_zLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "x-direction", TW_TYPE_FLOAT, &g_xLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "y-direction", TW_TYPE_FLOAT, &g_yLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "z-direction", TW_TYPE_FLOAT, &g_zLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "point/directional", TW_TYPE_BOOLCPP, &g_lightType, "help='false=point, true=directional'  group='light'");
	//TwAddVarRW(bar, "light intensity", TW_TYPE_COLOR32, &g_lightIntensity, " coloralpha=true colormode=rgb group='light'");
	TwAddVarRW(bar, "light intensity", TW_TYPE_COLOR4F, &g_lightIntensity, " colormode=rgb group='light'");

	TwAddVarRW(bar, "on/off light 1", TW_TYPE_BOOLCPP, &g_light1Enable, "group='light'");
	TwAddVarRW(bar, "on/off light 2", TW_TYPE_BOOLCPP, &g_light2Enable, "group='light'");

	TwAddVarRW(bar, "ambient light intensity", TW_TYPE_COLOR4F, &g_ambientLight, " colormode=rgb group='light'");

	shadingType = TwDefineEnum("ShadingType", NULL, 0);
	TwAddVarRW(bar, "shading type", shadingType, &g_shadingType, " enum=' 0 {GOURAUD}, 1 {PHONG}' group='shading'");
	TwAddVarRW(bar, "show mesh model", TW_TYPE_BOOLCPP, &g_mesh, "help='false=shading, true=meshmodel'  group='shading'");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}


void initScene_helper(GLuint programID)
{
	if (!programID)
	{
		std::cout << "\nFatal Error in shader creation!\n\a\a\a";
		return;
	}

	//get the identifier of the attribute "vPosition" in the active gl program
	GLint vPosition_id = glGetAttribLocation(programID, "VertexPosition");
	//this enables the generic vertex attribute array such that the values in the generic vertex attribute array
	//will be accessed and used for rendering when calls are made to vertex array commands such as glDrawArrays or glDrawElements.
	glEnableVertexAttribArray(vPosition_id);
	//specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(vPosition_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));


	GLuint vNormal_id = glGetAttribLocation(programID, "VertexNormal");
	glEnableVertexAttribArray(vNormal_id);
	//note that the pointer offset is not 0, indicating that the normal data in the vertex array buffer starts right after the geometry data.
	glVertexAttribPointer(vNormal_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV*sizeof(point4)));


	GLuint vTexCoord_id = glGetAttribLocation(programID, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord_id);
	//note that the pointer offset is not 0, indicating that the normal data in the vertex array buffer starts right after the geometry data.
	glVertexAttribPointer(vTexCoord_id, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(2*numV*sizeof(point4)));

}

void initTextureObject(GLuint programID)
{
	if (g_useTM) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_tex[0]);
		GLuint TM = glGetUniformLocation(programID, "texMapHandle");
		glUniform1i(TM, 0);
	}

	if (g_useNM) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_tex[1]);
		GLuint NM = glGetUniformLocation(programID, "normalMapHandle");
		glUniform1i(NM, 1);
	}
}

void initScene()
{

	std::vector<point4>  positions;
	std::vector<point4>  normals;
	std::vector<point2> texCoordinates;
	model.getAllVerticesOfInTriangles(positions, normals, texCoordinates);

	//create a vertex array object
	glGenVertexArrays(1, &g_vertexArrayID);
	glBindVertexArray(g_vertexArrayID);


	//create and initialize a buffer object
	glGenBuffers(1, &g_vertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObjectID);

	numV = positions.size();

	if (g_useTM || g_useNM) {
		//this will allocate memory for the buffer object on the GPU
		glBufferData(GL_ARRAY_BUFFER, numV * 3 * sizeof(point4), NULL, GL_STATIC_DRAW);
	}
	else {
		glBufferData(GL_ARRAY_BUFFER, numV * 2 * sizeof(point4), NULL, GL_STATIC_DRAW);
	}


	//this will copy the data from CPU memory to GPU memory
	//glBufferSubData redefines the data store for the buffer object currently bound to target
	glBufferSubData(GL_ARRAY_BUFFER, 0, numV*sizeof(point4), &positions[0]);
	//the colors are appended to the buffer right after the positions
	glBufferSubData(GL_ARRAY_BUFFER, numV*sizeof(point4), numV*sizeof(color4), &normals[0]);
	if (g_useTM || g_useNM) {
		//the TC are appended to the buffer right after the colors
		glBufferSubData(GL_ARRAY_BUFFER, numV * 2 * sizeof(point4), numV*sizeof(point2), &texCoordinates[0]);
	}


	if (!SetCurrentDirectory(g_Buffer))
	{
		printf("SetCurrentDirectory failed (%d)\n", GetLastError());
		return;
	}

	//create, load, compile, attach vertex and fragment shader

	g_programID1 = initShader("..\\Shaders\\a_vertexShaderGouraud.glsl", "..\\Shaders\\a_fragmentShaderGouraud.glsl");
	g_programID2 = initShader("..\\Shaders\\b_vertexShaderPhong.glsl", "..\\Shaders\\b_fragmentShaderPhong.glsl");
	g_programID3 = initShader("..\\Shaders\\c_vertexShaderGouraudTM.glsl", "..\\Shaders\\c_fragmentShaderGouraudTM.glsl");
	g_programID4 = initShader("..\\Shaders\\d_vertexShaderPhongTM.glsl", "..\\Shaders\\d_fragmentShaderPhongTM.glsl");
	g_programID5 = initShader("..\\Shaders\\f_vertexShaderPhongNM.glsl", "..\\Shaders\\f_fragmentShaderPhongNM.glsl");

	initScene_helper(g_programID1);
	initScene_helper(g_programID2);
	initScene_helper(g_programID3);
	initScene_helper(g_programID4);
	initScene_helper(g_programID5);

	initTextureObject(g_programID3);
	initTextureObject(g_programID4);
	initTextureObject(g_programID5);
}


void TW_CALL loadOBJModel(void *data)
{
	//**********************************************************
	DWORD dwRet;
	dwRet = GetCurrentDirectory(BUFSIZE, g_Buffer);
	if (dwRet == 0)
	{
		printf("GetCurrentDirectory failed (%d)\n", GetLastError());
		return;
	}
	if (dwRet > BUFSIZE)
	{
		printf("Buffer too small; need %d characters\n", dwRet);
		return;
	}
	//******************************************************
	std::wstring str = getOpenFileName();

	Wavefront_obj objScene;
	bool result = objScene.load_file(str);

	if (result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
		//store the values in Object, MeshModel...
		//draw the object for the first time
		model.setAll(objScene);
		transformations_matrix.setAllValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//this is the model matrix
		rotations_matrix.setAllValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//this is the rotations
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
	//clear = false;
	//glutPostRedisplay();
}

void TW_CALL loadTMFile(void *data)
{
	g_useTM = true;
	std::wstring str = getOpenFileName();
	std::wcout << str << "\n";
	char fileName[150];
	std::wcstombs(fileName, str.c_str(), 150);

	g_tex[0] = SOIL_load_OGL_texture
		(
			fileName,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

}

void TW_CALL loadNMFile(void *data)
{
	g_useNM = true;
	std::wstring str = getOpenFileName();
	std::wcout << str << "\n";
	char fileName[150];
	std::wcstombs(fileName, str.c_str(), 150);

	g_tex[1] = SOIL_load_OGL_texture
		(
			fileName,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

}

void TW_CALL Start(void *data)
{
	std::cout << "Started" << std::endl;
	initScene();
}

void initGraphics(int argc, char *argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_CORE_PROFILE);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	//glutInitContextVersion(2, 0);
	//glutInitContextVersion(3, 0);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //use a framebuffer with double buffering and RGBA format. Use a z-buffer

	glutInitWindowSize(g_Swidth, g_Sheight);
	glutCreateWindow("Computer Graphics Skeleton using: OpenGL, AntTweakBar, freeGLUT, glew");
	glutCreateMenu(NULL);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		assert(0);
		return;
	}
	else
	{
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}
}


void TW_CALL applyTranslation(void* clientData) {
	if (g_translationX != 0.0 || g_translationY != 0.0 || g_translationZ != 0.0) {
		Matrix4x4 mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, g_translationX, g_translationY, g_translationZ, 1);
		if (g_space) {//world space
			transformations_matrix *= mat;
		}
		else {//object space
			transformations_matrix = mat*transformations_matrix;
		}
		glutPostRedisplay();
	}
}
void TW_CALL applyScale(void* clientData) {

	if (g_scale != 1.0) {
		Matrix4x4 mat(g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, 1);
		if (g_space) {//world space
			transformations_matrix *= mat;
		}
		else {//object space
			transformations_matrix = mat*transformations_matrix;
		}

		glutPostRedisplay();
	}
}

void TW_CALL applyXrotation(void* clientData) {
	double teta = g_xRotation*PI / 180.0;
	Matrix4x4 mat(1, 0, 0, 0,
		0, cos(teta), sin(teta), 0,
		0, -sin(teta), cos(teta), 0,
		0, 0, 0, 1);
	if (g_xRotation != 0.0) {
		if (!g_space)//object space
		{
			transformations_matrix *= mat;
			rotations_matrix = mat*rotations_matrix;
		}
		else {//world space
			transformations_matrix = mat*transformations_matrix;
			rotations_matrix *= mat;

		}
		glutPostRedisplay();
	}
}
void TW_CALL applyYrotation(void* clientData) {
	double teta = g_yRotation*PI / 180.0;
	Matrix4x4 mat(cos(teta), 0, -sin(teta), 0,
		0, 1, 0, 0,
		sin(teta), 0, cos(teta), 0,
		0, 0, 0, 1);
	if (g_yRotation != 0.0) {
		if (!g_space) {
			transformations_matrix *= mat;
			rotations_matrix = mat*rotations_matrix;
		}
		else {
			transformations_matrix = mat*transformations_matrix;
			rotations_matrix *= mat;
		}
		glutPostRedisplay();
	}
}
void TW_CALL applyZrotation(void* clientData) {
	double teta = g_zRotation*PI / 180.0;
	Matrix4x4 mat(cos(teta), sin(teta), 0, 0,
		-sin(teta), cos(teta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	if (g_zRotation != 0.0) {
		if (!g_space) {
			transformations_matrix *= mat;
			rotations_matrix = mat*rotations_matrix;
		}
		else {
			transformations_matrix = mat*transformations_matrix;
			rotations_matrix *= mat;
		}
		glutPostRedisplay();
	}
}

/*
void transformation() {
	if (g_scale != 1.0) {
		Matrix4x4 mat(g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, 1);
		if (g_space) {//world space
			transformations_matrix *= mat;
		}
		else {//object space
			transformations_matrix = mat*transformations_matrix;
		}
		g_scale = 1.0;
	}
	if (g_translationX != 0.0 || g_translationY != 0.0 || g_translationZ != 0.0) {
	//	Matrix4x4 mat(1, 0, 0, g_translationX, 0, 1, 0, g_translationY, 0, 0, 1, g_translationZ,0 ,0 ,0, 1);
		Matrix4x4 mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, g_translationX, g_translationY, g_translationZ, 1);
		if (g_space) {//world space
			transformations_matrix *= mat;
		}
		else {//object space
			transformations_matrix = mat*transformations_matrix;
		}
		g_translationX = g_translationY = g_translationZ = 0;
	}

	if (g_quaternion[0] != 0.0f || g_quaternion[1] != 0.0f || g_quaternion[2] != 0.0f || g_quaternion[3] != 1.0f) {
		GLfloat mat_rotation[16];
		ConvertQuaternionToMatrix(g_quaternion, mat_rotation);
		Matrix4x4 mat(mat_rotation);
		mat=mat.transpose();
		if (g_space) {//world space
			transformations_matrix *= mat;
			rotations_matrix *= mat;
		}
		else {//object space
			transformations_matrix = mat*transformations_matrix;
			rotations_matrix = mat*rotations_matrix;
		}
		g_quaternion[0] = 0.0f; g_quaternion[1] = 0.0f; g_quaternion[2] = 0.0f; g_quaternion[3] = 1.0f;
	}

}
void transformation() {
		Matrix4x4 matS(g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, g_scale, 0, 0, 0, 0, 1);
	
		Matrix4x4 matT(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, g_translationX, g_translationY, g_translationZ, 1);
	
		GLfloat mat_rotation[16];
		ConvertQuaternionToMatrix(g_quaternion, mat_rotation);
		Matrix4x4 matR(mat_rotation);
		matR=matR.transpose();

	transformations_matrix = matS*matR*matT;
}*/

void applyLights() {

	GLuint l1id = glGetUniformLocation(g_activeProgramID, "Lights[0].isEnabled");
	glUniform1i(l1id, g_light1Enable);

	if (g_light1Enable) {

		GLuint l1t = glGetUniformLocation(g_activeProgramID, "Lights[0].isDirectional");
		glUniform1i(l1t, g_lightType);

		GLuint l1p = glGetUniformLocation(g_activeProgramID, "Lights[0].position");
		glUniform4f(l1p, g_xLightPosition, g_yLightPosition, g_zLightPosition,1);

		GLuint l1d = glGetUniformLocation(g_activeProgramID, "Lights[0].direction");
		glUniform4f(l1d, g_xLightDirection, g_yLightDirection, g_zLightDirection, 1);

		GLuint l1i = glGetUniformLocation(g_activeProgramID, "Lights[0].intensity");
		glUniform4f(l1i, g_lightIntensity[0], g_lightIntensity[1], g_lightIntensity[2], g_lightIntensity[3]);
	}

	GLuint l2id = glGetUniformLocation(g_activeProgramID, "Lights[1].isEnabled");
	glUniform1i(l2id, g_light2Enable);

	if (g_light2Enable) {

		GLuint l2t = glGetUniformLocation(g_activeProgramID, "Lights[1].isDirectional");
		glUniform1i(l2t, g_lightType);

		GLuint l2p = glGetUniformLocation(g_activeProgramID, "Lights[1].position");
		glUniform4f(l2p, g_xLightPosition, g_yLightPosition, g_zLightPosition, 1);

		GLuint l2d = glGetUniformLocation(g_activeProgramID, "Lights[1].direction");
		glUniform4f(l2d, g_xLightDirection, g_yLightDirection, g_zLightDirection, 1);

		GLuint l2i = glGetUniformLocation(g_activeProgramID, "Lights[1].intensity");
		glUniform4f(l2i, g_lightIntensity[0], g_lightIntensity[1], g_lightIntensity[2], g_lightIntensity[3]);
	}

	GLuint amb = glGetUniformLocation(g_activeProgramID, "AmbientProduct");
	glUniform4f(amb, g_ambientLight[0], g_ambientLight[1], g_ambientLight[2], g_ambientLight[3]);
	//glUniform4f(amb,1,0,0, 1);



}
void applyMaterial() {
	GLuint ma = glGetUniformLocation(g_activeProgramID, "material.ambient");
	glUniform1f(ma, g_ambient);

	GLuint md = glGetUniformLocation(g_activeProgramID, "material.diffuse");
	glUniform1f(md, g_diffuse);

	GLuint ms = glGetUniformLocation(g_activeProgramID, "material.specular");
	glUniform1f(ms, g_specular);

	GLuint mse = glGetUniformLocation(g_activeProgramID, "material.specularExp");
	glUniform1f(mse, g_specularExp);
}
void drawScene()
{
	if (g_activeProgramID == 0)
	{
		return;
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//transformation();
	glUseProgram(g_activeProgramID);

	Matrix4x4 MV(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, -5, 1);
	MV = transformations_matrix*MV;

	Camera cam({ 0, 0, 0, 1 }, (model.getCentroid())*MV, { 0,1,0,1 });
	//pointing camera at object
	if (g_centerCam) {
		saveCamera.setAllValues(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		saveCamera *= cam.getViewMtrx();
		g_centerCam = false;
	}
	MV *= saveCamera;

	cam.setProjectionMatrix(g_fovy, g_near, g_far, (eProjectionType)g_projectionType, 1);
	Matrix4x4 projectionMtrx(cam.getProjectionMtrx());

	Matrix4x4 MVP = MV*projectionMtrx;


	GLfloat MVMatrix[16];
	ConvertMat4x4ToArray(MV, MVMatrix);
	//GLfloat MVMatrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	GLuint mat_MV_id = glGetUniformLocation(g_activeProgramID, "MVMatrix");
	glUniformMatrix4fv(mat_MV_id, 1, false, MVMatrix); //glUniformMatrix4fv assumes that the matrix is given in column major order. i.e, the first four elements in "mat" array corresponds to the first column of the matrix

	GLfloat MVPMatrix[16];
	ConvertMat4x4ToArray(MVP, MVPMatrix);
	//GLfloat MVPMatrix[16] = { 0.1,0,0,0.0,0,0.1,0,0,0,0,0.1,-5,0,0,0,1 };
	GLuint mat_MVP_id = glGetUniformLocation(g_activeProgramID, "MVPMatrix");
	glUniformMatrix4fv(mat_MVP_id, 1, false, MVPMatrix);

	GLfloat NormalMatrix[16];
	ConvertMat4x4ToArray(rotations_matrix, NormalMatrix);
	//GLfloat NormalMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	GLuint mat_NM_id = glGetUniformLocation(g_activeProgramID, "NormalMatrix");
	glUniformMatrix4fv(mat_NM_id, 1, false, NormalMatrix);

	applyLights();
	applyMaterial();

	if (g_drawWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//this will invoke the rendering of the model.
	//GL_TRIANGLES means that each three consecutive vertices in the array are connected and treated as a single triangle.
	//this means that vertices in our meshes have to be duplicated. to avoid duplication and render triangles with additional indices information, one can use glDrawElements
	glDrawArrays(GL_TRIANGLES, 0, numV);

	glPopAttrib();
}

//callback function called by GLUT to render screen
void Display()
{
	if (g_useNM) {
		g_activeProgramID = g_programID5;
	}
	else if (g_shadingType==GOURAUD) {
		if (g_useTM) {
			g_activeProgramID = g_programID3;
		}
		else {
			g_activeProgramID = g_programID1;
		}
	}
	else {//phong
		if (g_useTM) {
			g_activeProgramID = g_programID4;
		}
		else {
			g_activeProgramID = g_programID2;
		}
	}

	glClearColor(0.0, 0.0, 0.0, 1.0); //set the background color to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //init z-buffer and framebuffer

	drawScene();

	//draw tweak bars
	TwDraw();

	// Present frame buffer
	glutSwapBuffers();
}




// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	int WIDTH = 300;
	int HEIGHT = 300;
	//this needed to be changed in order to have isotropic scale in x and y axis
	const float ar_origin = (float)WIDTH / (float)HEIGHT;
	const float ar_new = (float)width / (float)height;

	float scale_w = (float)width / (float)WIDTH;
	float scale_h = (float)height / (float)HEIGHT;
	if (ar_new > ar_origin) {
		scale_w = scale_h;
	}
	else {
		scale_h = scale_w;
	}

	float margin_x = (width - WIDTH * scale_w) / 2;
	float margin_y = (height - HEIGHT * scale_h) / 2;

	glViewport(margin_x, margin_y, WIDTH * scale_w, HEIGHT * scale_h);


	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);
	glutPostRedisplay();
}


void MouseButton(int button, int state, int x, int y)
{
	TwEventMouseButtonGLUT(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	glutPostRedisplay();
}

void PassiveMouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	glutPostRedisplay();
}


void Keyboard(unsigned char k, int x, int y)
{
	TwEventKeyboardGLUT(k, x, y);
	glutPostRedisplay();
}

void Special(int k, int x, int y)
{
	TwEventSpecialGLUT(k, x, y);
	glutPostRedisplay();
}



// Function called at exit
void Terminate(void)
{
	TwTerminate();

	//to do: add code for releasing opengl data such as textures and buffers

}



