#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>


#include <vector>
#include <Windows.h>
#include <assert.h>

#include "Obj Parser/wavefront_obj.h"
#include "Utils.h"
#include "MeshModel.h"

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE MAX_PATH
#define PI 3.14159265358979323846 
TCHAR g_Buffer[BUFSIZE];


float g_scale = 1.0f;
float g_quaternion[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
bool g_drawWireframe = false;
//bool g_drawWireframe = true;


unsigned int g_Color = 0xff0000ff;

double g_Swidth = 1366;
double g_Sheight = 768;

double g_near = 0.01;
double g_far = 10000;
double g_fovy = 60;

float g_translationX = 0.0;
float g_translationY = 0.0;
float g_translationZ = 0.0;


//global veriables for glut functions
bool g_reset = false;
bool g_centerCam = false;
bool g_showCrdSystem = false;
bool g_normals = false;
bool g_bbox = false;
bool g_projectionType = true;
bool g_space = true;//initialize to world space
double g_normals_size = 1;

double g_ambient = 0.1;
double g_diffuse = 0.6;
double g_specular = 0.7;
double g_specularExp = 32;
double g_xLightPosition = 0.0;
double g_yLightPosition = 0.0;
double g_zLightPosition = 0.0;
double g_xLightDirection = 0.0;
double g_yLightDirection = 0.0;
double g_zLightDirection = -5;
bool g_lightType = false;
unsigned int g_lightIntensity = 0xffffff;
unsigned int g_ambientLight = 0xffffff;
TwType shadingType;
bool g_mesh = false;

MeshModel model;
int numV;//number of vertices

GLuint g_vertexArrayID = 0;
GLuint g_vertexBufferObjectID = 0;
GLuint g_programID = 0;
GLuint g_programIDG = 0;
GLuint g_programIDP = 0;

Matrix4x4 translation_matrix;


void TW_CALL loadOBJModel(void* clientData);
void TW_CALL applyTranslation(void* clientData);
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

	TwAddButton(bar, "LoadOBJ", loadOBJModel, NULL, "help='button to load obf file'");
	TwAddSeparator(bar, NULL, NULL);

	TwAddVarRW(bar, "reset", TW_TYPE_BOOLCPP, &g_reset, "help='reset everything'");
	TwAddVarRW(bar, "OW space", TW_TYPE_BOOLCPP, &g_space, " help='true=transform in world space ,false=transform in object space' ");
	TwAddVarRW(bar, "OW Crd System", TW_TYPE_BOOLCPP, &g_showCrdSystem, " help='boolean variable to indicate if to show WO coordinate system or not.' ");
	TwAddVarRW(bar, "showBbox", TW_TYPE_BOOLCPP, &g_bbox, " help='boolean variable to indicate if to show the bbox or not.' ");

	TwAddVarRW(bar, "showNormals", TW_TYPE_BOOLCPP, &g_normals, " help='boolean variable to indicate if to show normals or not.' group='normals'");
	TwAddVarRW(bar, "normalsSize", TW_TYPE_DOUBLE, &g_normals_size, " min=0.1 max=100 step=0.1 keyIncr=t keyDecr=T help='Change notmals size (20=original size).' group='normals'");
	TwAddVarRW(bar, "projectionType", TW_TYPE_BOOLCPP, &g_projectionType, " help='true = orthographic, false = perspective.' group='camera'");
	TwAddVarRW(bar, "near", TW_TYPE_DOUBLE, &g_near, "step=0.01 keyIncr=n keyDecr=N  group='camera'");
	TwAddVarRW(bar, "far", TW_TYPE_DOUBLE, &g_far, "step=0.1 keyIncr=f keyDecr=F  group='camera'");
	TwAddVarRW(bar, "fovy", TW_TYPE_DOUBLE, &g_fovy, "step=0.1 keyIncr=v keyDecr=V  group='camera'");

	TwAddVarRW(bar, "centerCamera", TW_TYPE_BOOLCPP, &g_centerCam, "help='point the camera to the center of the model'  group='camera'");

	TwAddVarRW(bar, "translate X", TW_TYPE_FLOAT, &g_translationX, "min=-30 max=30 step=1 keyIncr=right keyDecr=left   group='tranfromations' ");
	TwAddVarRW(bar, "translate Y", TW_TYPE_FLOAT, &g_translationY, "min=-30 max=30 step=1 keyIncr=up keyDecr=down   group='tranfromations' ");
	TwAddVarRW(bar, "translate Z", TW_TYPE_FLOAT, &g_translationZ, "min=-30 max=30 step=1 keyIncr=> keyDecr=<   group='tranfromations' ");
	//TwAddButton(bar, "apply translation", applyTranslation, NULL, "help='apply translation' group='tranfromations' ");

	//add 'g_Zoom' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &g_scale, " label='Scale' min=-10.0 max=10.0 step=0.01 group=tranfromations ");

	//add 'g_quaternion' to 'bar': this is a variable of type TW_TYPE_QUAT4D which defines the object's orientation using quaternions
	TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_quaternion, " label='Object rotation' opened=true help='This is object rotation' group=tranfromations ");

	TwAddVarRW(bar, "ambient", TW_TYPE_DOUBLE, &g_ambient, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "diffuse", TW_TYPE_DOUBLE, &g_diffuse, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "specular", TW_TYPE_DOUBLE, &g_specular, "min = 0 max = 1000 step=0.1 keyIncr=z keyDecr=Z   group='material' ");
	TwAddVarRW(bar, "specularExp", TW_TYPE_DOUBLE, &g_specularExp, "min = 0 max = 1000 step=1 keyIncr=z keyDecr=Z   group='material' ");

	TwAddVarRW(bar, "x-position", TW_TYPE_DOUBLE, &g_xLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "y-position", TW_TYPE_DOUBLE, &g_yLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "z-position", TW_TYPE_DOUBLE, &g_zLightPosition, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "x-direction", TW_TYPE_DOUBLE, &g_xLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "y-direction", TW_TYPE_DOUBLE, &g_yLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "z-direction", TW_TYPE_DOUBLE, &g_zLightDirection, "min = -1000 max = 1000 step=1 keyIncr=z keyDecr=Z   group='light' ");
	TwAddVarRW(bar, "point/directional", TW_TYPE_BOOLCPP, &g_lightType, "help='false=point, true=directional'  group='light'");
	TwAddVarRW(bar, "light intensity", TW_TYPE_COLOR32, &g_lightIntensity, " coloralpha=true colormode=rgb group='light'");
	TwAddButton(bar, "apply on light 1", &applyLight1, NULL, " help='apply scale' group='light' ");
	TwAddButton(bar, "apply on light 2", &applyLight2, NULL, " help='apply scale' group='light' ");
	TwAddVarRW(bar, "ambient light intensity", TW_TYPE_COLOR32, &g_ambientLight, "coloralpha=true colormode=rgb group='light' ");

	shadingType = TwDefineEnum("ShadingType", NULL, 0);
	//TwAddVarRW(bar, "shading type", shadingType, &g_shadingType, " enum='0 {FLAT}, 1 {GOURAUD}, 2 {PHONG}' group='shading'");
	TwAddVarRW(bar, "show mesh model", TW_TYPE_BOOLCPP, &g_mesh, "help='false=shading, true=meshmodel'  group='shading'");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}



void initScene()
{

	std::vector<point4>  positions;
	std::vector<point4>  normals;
	model.getAllVerticesOfInTriangles(positions, normals);

	//create a vertex array object
	glGenVertexArrays(1, &g_vertexArrayID);
	glBindVertexArray(g_vertexArrayID);


	//create and initialize a buffer object
	glGenBuffers(1, &g_vertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObjectID);

	numV = positions.size();

	//this will allocate memory for the buffer object on the GPU
	glBufferData(GL_ARRAY_BUFFER, numV*2*sizeof(point4), NULL, GL_STATIC_DRAW);

	//this will copy the data from CPU memory to GPU memory
	//glBufferSubData redefines the data store for the buffer object currently bound to target
	glBufferSubData(GL_ARRAY_BUFFER, 0, numV*sizeof(point4), &positions[0]);
	//the colors are appended to the buffer right after the positions
	glBufferSubData(GL_ARRAY_BUFFER, numV*sizeof(point4), numV*sizeof(color4), &normals[0]);



	if (!SetCurrentDirectory(g_Buffer))
	{
		printf("SetCurrentDirectory failed (%d)\n", GetLastError());
		return;
	}

	//create, load, compile, attach vertex and fragment shader
	g_programID = initShader("..\\Shaders\\vertexShader.glsl", "..\\Shaders\\fragmentShader.glsl");
	//g_programIDG = initShader("..\\Shaders\\vertexShaderGouraud.glsl", "..\\Shaders\\fragmentShaderGouraud.glsl");
	//g_programIDP = initShader("..\\Shaders\\vertexShaderPhong.glsl", "..\\Shaders\\fragmentShaderPhong.glsl");

	if (!g_programID)
	{
		std::cout << "\nFatal Error in shader creation!\n\a\a\a";
		return;
	}
	/*if (!g_programIDP)
	{
		std::cout << "\nFatal Error in shader creation!\n\a\a\a";
		return;
	}*/
	//get the identifier of the attribute "vPosition" in the active gl program
	GLint vPosition_id = glGetAttribLocation(g_programID, "vPosition");
	//this enables the generic vertex attribute array such that the values in the generic vertex attribute array
	//will be accessed and used for rendering when calls are made to vertex array commands such as glDrawArrays or glDrawElements.
	glEnableVertexAttribArray(vPosition_id);
	//specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(vPosition_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));


	GLuint vNormal_id = glGetAttribLocation(g_programID, "vNormal");
	glEnableVertexAttribArray(vNormal_id);
	//note that the pointer offset is not 0, indicating that the normal data in the vertex array buffer starts right after the geometry data.
	glVertexAttribPointer(vNormal_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV*sizeof(point4)));
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
	/*	transform.setAllValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//this is the model matrix
		axisTransform.setAllValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//this is the model matrix
		box.setVertices(m);
		sceneObject.setModel(m, transform);*/

	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
	//clear = false;
	//glutPostRedisplay();
	initScene();
}
/*void TW_CALL applyTranslation(void* clientData) {
	Matrix4x4 temp(1,0,0,g_translationX,0,1,0,g_translationY,0,0,1,g_translationZ,0,0,0,1);
	if (g_space) {
		translation_matrix *= temp;
	}
	else translation_matrix = temp*translation_matrix;

	g_translationX = 0;
	g_translationY = 0;
	g_translationZ=0;
}*/
void TW_CALL applyLight1(void* clientData) {

}
void TW_CALL applyLight2(void* clientData) {

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



void drawScene()
{
	if (g_programID == 0)
	{
		return;
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glUseProgram(g_programID);

	GLfloat mat_rotation[16];
	ConvertQuaternionToMatrix(g_quaternion, mat_rotation);

	GLuint mat_rotation_id = glGetUniformLocation(g_programID, "rotation");
	glUniformMatrix4fv(mat_rotation_id, 1, false, mat_rotation); //glUniformMatrix4fv assumes that the matrix is given in column major order. i.e, the first four elements in "mat" array corresponds to the first column of the matrix

	GLfloat mat_translation[16];
	//createTranslationMatrix(translation_matrix[0][3], translation_matrix[1][3], translation_matrix[2][3] , mat_translation);
	createTranslationMatrix(g_translationX, g_translationY, g_translationZ-5.0f, mat_translation);

	GLuint mat_translation_id = glGetUniformLocation(g_programID, "translation");
	glUniformMatrix4fv(mat_translation_id, 1, false, mat_translation);

	GLfloat mat_projection[16];
	createPerspectiveProjectionMatrix(1.0f, 10.0f, 0.25f, 0.25f, mat_projection);

	GLuint mat_projection_id = glGetUniformLocation(g_programID, "projection");
	glUniformMatrix4fv(mat_projection_id, 1, false, mat_projection);

	GLuint scale_id = glGetUniformLocation(g_programID, "scale");
	glUniform1f(scale_id, g_scale);

	//GLuint space_id = glGetUniformLocation(g_programID, "space");
	//glUniform1i(space_id, g_space);

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
	//this needed to be changed in order to have isotropic scale in x and y axis
	glViewport(0, 0, width, height);
	//glViewport(0, 0, 800, 800);

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



