#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>


#include <vector>
#include <Windows.h>
#include <assert.h>


#include "Utils.h"




float g_scale = 1.0f;
float g_quaternion[4] = {0.0f, 0.0f, 0.0f, 1.0f};
bool g_drawWireframe = false;
//bool g_drawWireframe = true;


GLuint g_vertexArrayID = 0;
GLuint g_vertexBufferObjectID = 0;
GLuint g_programID = 0;




void TW_CALL loadOBJModel(void* clientData);
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

	initScene();

	atexit(Terminate);  // Called after glutMainLoop ends

	// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color

	//add 'g_Zoom' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_FLOAT. Its key shortcuts are [z] and [Z].
	TwAddVarRW(bar, "Scale", TW_TYPE_FLOAT, &g_scale, " label='Scale' min=-10.0 max=10.0 step=0.01 group=Engine ");

	//add 'g_quaternion' to 'bar': this is a variable of type TW_TYPE_QUAT4D which defines the object's orientation using quaternions
	TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_quaternion, " label='Object rotation' opened=true help='This is object rotation' ");

	TwAddButton(bar, "open", loadOBJModel, NULL, " label='Open OBJ File...' ");

	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}



void initScene()
{
	point4 positions[36];
	color4 colors[36];
	createCube(positions, colors);


	//create a vertex array object
	glGenVertexArrays(1, &g_vertexArrayID);
	glBindVertexArray(g_vertexArrayID);


	//create and initialize a buffer object
	glGenBuffers(1, &g_vertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObjectID);

	int numV = 36;

	//this will allocate memory for the buffer object on the GPU
	glBufferData(GL_ARRAY_BUFFER, numV*(sizeof(point4) + sizeof(color4)), NULL, GL_STATIC_DRAW);
	//this will copy the data from CPU memory to GPU memory
	//glBufferSubData redefines the data store for the buffer object currently bound to target
	glBufferSubData(GL_ARRAY_BUFFER, 0, numV*sizeof(point4), &positions[0]);
	//the colors are appended to the buffer right after the positions
	glBufferSubData(GL_ARRAY_BUFFER, numV*sizeof(point4), numV*sizeof(color4), &colors[0]);


	//create, load, compile, attach vertex and fragment shader
	g_programID = initShader("..\\Shaders\\vertexShader.glsl", "..\\Shaders\\fragmentShader.glsl");
	if(!g_programID)
	{
		std::cout << "\nFatal Error in shader creation!\n\a\a\a";
		return;
	}

	//get the identifier of the attribute "vPosition" in the active gl program
	GLint vPosition_id = glGetAttribLocation(g_programID, "vPosition");
	//this enables the generic vertex attribute array such that the values in the generic vertex attribute array
	//will be accessed and used for rendering when calls are made to vertex array commands such as glDrawArrays or glDrawElements.
	glEnableVertexAttribArray(vPosition_id);
	//specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(vPosition_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));


	GLuint vColor_id = glGetAttribLocation(g_programID, "vColor" );
	glEnableVertexAttribArray(vColor_id);
	//note that the pointer offset is not 0, indicating that the color data in the vertex array buffer starts right after the geometry data.
	glVertexAttribPointer(vColor_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV*sizeof(point4)));
}



void TW_CALL loadOBJModel(void* clientData)
{
	std::wstring str = getOpenFileName();
	std::wcout << str << "\n";
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
	
	glutInitWindowSize(800, 800);
	glutCreateWindow("Computer Graphics Skeleton using: OpenGL, AntTweakBar, freeGLUT, glew");
	glutCreateMenu(NULL);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
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
	if(g_programID == 0)
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
	createTranslationMatrix(0.0f, 0.0f, -5.0f, mat_translation);

	GLuint mat_translation_id = glGetUniformLocation(g_programID, "translation");
	glUniformMatrix4fv(mat_translation_id, 1, false, mat_translation);

	GLfloat mat_projection[16];
	createPerspectiveProjectionMatrix(1.0f, 10.0f, 0.25f, 0.25f, mat_projection);

	GLuint mat_projection_id = glGetUniformLocation(g_programID, "projection");
	glUniformMatrix4fv(mat_projection_id, 1, false, mat_projection);

	GLuint scale_id = glGetUniformLocation(g_programID, "scale");
	glUniform1f(scale_id, g_scale);

	if(g_drawWireframe)
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
	glDrawArrays(GL_TRIANGLES, 0, 36);

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



