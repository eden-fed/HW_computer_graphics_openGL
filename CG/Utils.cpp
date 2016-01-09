#include "Utils.h"
#include <windows.h>
#include <assert.h>
#include <iostream>
#include <fstream>



//helper function to convert a quaternion to a 4x4 rotation matrix
void ConvertQuaternionToMatrix(const float quat[4], float mat[16])
{
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];
	mat[0*4+0] = - yy2 - zz2 + 1.0f;
	mat[1*4+0] = xy2 + wz2;
	mat[2*4+0] = xz2 - wy2;
	mat[3*4+0] = 0;
	
	mat[0*4+1] = xy2 - wz2;
	mat[1*4+1] = - xx2 - zz2 + 1.0f;
	mat[2*4+1] = yz2 + wx2;
	mat[3*4+1] = 0;
	
	mat[0*4+2] = xz2 + wy2;
	mat[1*4+2] = yz2 - wx2;
	mat[2*4+2] = - xx2 - yy2 + 1.0f;
	mat[3*4+2] = 0;
	
	mat[0*4+3] = 0;
	mat[1*4+3] = 0;
	mat[2*4+3] = 0;
	mat[3*4+3] = 1;
}



void createTranslationMatrix(float tx, float ty, float tz, float mat[16])
{
	mat[0*4+0] = 1.0f;
	mat[0*4+1] = 0.0f;
	mat[0*4+2] = 0.0f;
	mat[0*4+3] = tx;

	mat[1*4+0] = 0.0f;
	mat[1*4+1] = 1.0f;
	mat[1*4+2] = 0.0f;
	mat[1*4+3] = ty;

	mat[2*4+0] = 0.0f;
	mat[2*4+1] = 0.0f;
	mat[2*4+2] = 1.0f;
	mat[2*4+3] = tz;

	mat[3*4+0] = 0.0f;
	mat[3*4+1] = 0.0f;
	mat[3*4+2] = 0.0f;
	mat[3*4+3] = 1.0f;
}



void createPerspectiveProjectionMatrix(float nearPlane, float farPlane, float right, float top, float mat[16])
{
	assert(nearPlane > 0.0f);
	assert(farPlane > nearPlane);
	assert(farPlane > nearPlane);
	assert(right > 0.0f);
	assert(top > 0.0f);

	mat[0*4+0] = nearPlane / right;
	mat[0*4+1] = 0.0f;
	mat[0*4+2] = 0.0f;
	mat[0*4+3] = 0.0f;

	mat[1*4+0] = 0.0f;
	mat[1*4+1] = nearPlane / top;
	mat[1*4+2] = 0.0f;
	mat[1*4+3] = 0.0f;
	
	mat[2*4+0] = 0.0f;
	mat[2*4+1] = 0.0f;
	mat[2*4+2] = -(farPlane + nearPlane)/(farPlane - nearPlane);
	mat[2*4+3] = -2.0f*(farPlane*nearPlane)/(farPlane - nearPlane);
	
	mat[3*4+0] = 0.0f;
	mat[3*4+1] = 0.0f;
	mat[3*4+2] = -1.0f;
	mat[3*4+3] = 0.0f;
}


void displayMessage(const std::string& str)
{
	std::wstring wStr(str.begin(), str.end());
	MessageBox(NULL, wStr.c_str(), TEXT("Renderer"), MB_OK);
}


std::wstring getOpenFileName()
{
	const int strMaxLen = 10000;
	OPENFILENAME ofn = {0};
	TCHAR fileStr[strMaxLen] = {0};

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = fileStr;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileStr)/sizeof(TCHAR) - 1;

	GetOpenFileName(&ofn);
	return fileStr;
}




std::string readShaderSource(const std::string& shaderFile)
{
	std::ifstream ifile(shaderFile);
	std::string filetext;

	while(ifile.good())
	{
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}
	ifile.close();
	return filetext;
}




void createCube(point4* positions, color4* colors)
{
	// Vertices of a unit cube centered at origin, sides aligned with axes
	point4 vertex_positions[8] =
	{
		point4( -0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5,  0.5,  0.5, 1.0 ),
		point4(  0.5,  0.5,  0.5, 1.0 ),
		point4(  0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5, -0.5, -0.5, 1.0 ),
		point4( -0.5,  0.5, -0.5, 1.0 ),
		point4(  0.5,  0.5, -0.5, 1.0 ),
		point4(  0.5, -0.5, -0.5, 1.0 )
	};

	// RGBA vertex colors
	color4 vertex_colors[8] = 
	{
		color4( 0.0, 0.0, 0.0, 1.0 ),  // black
		color4( 1.0, 0.0, 0.0, 1.0 ),  // red
		color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
		color4( 0.0, 1.0, 0.0, 1.0 ),  // green
		color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
		color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
		color4( 1.0, 1.0, 1.0, 1.0 ),  // white
		color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
	};

	int globalIndex = 0;

	// generate 12 triangles: 36 vertices and 36 colors
	quad(1, 0, 3, 2, globalIndex, vertex_positions, vertex_colors, positions, colors);
	quad(2, 3, 7, 6, globalIndex, vertex_positions, vertex_colors, positions, colors);
	quad(3, 0, 4, 7, globalIndex, vertex_positions, vertex_colors, positions, colors);
	quad(6, 5, 1, 2, globalIndex, vertex_positions, vertex_colors, positions, colors);
	quad(4, 5, 6, 7, globalIndex, vertex_positions, vertex_colors, positions, colors);
	quad(5, 4, 0, 1, globalIndex, vertex_positions, vertex_colors, positions, colors);
}



void quad(int a, int b, int c, int d, int& globalIndex, point4* vertex_positions, color4* vertex_colors, point4* positions, color4* colors)
{
	//first triangle of quad
	positions[globalIndex] = vertex_positions[a];
	colors[globalIndex] = vertex_colors[a];
	globalIndex++;

	positions[globalIndex] = vertex_positions[b];
	colors[globalIndex] = vertex_colors[b];
	globalIndex++;

	positions[globalIndex] = vertex_positions[c];
	colors[globalIndex] = vertex_colors[c];
	globalIndex++;

	//second triangle of quad
	positions[globalIndex] = vertex_positions[a];
	colors[globalIndex] = vertex_colors[a];
	globalIndex++;

	positions[globalIndex] = vertex_positions[c];
	colors[globalIndex] = vertex_colors[c];
	globalIndex++;

	positions[globalIndex] = vertex_positions[d];
	colors[globalIndex] = vertex_colors[d];
	globalIndex++;
}


//create a GLSL program object from vertex and fragment shader files
GLuint initShader(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
	Shader shaders[2] =
	{
		{vertexShaderFileName, GL_VERTEX_SHADER, std::string()},
		{fragmentShaderFileName, GL_FRAGMENT_SHADER, std::string()}
	};

	GLuint program = glCreateProgram();

	for ( int i = 0; i < 2; ++i ) {
		Shader& s = shaders[i];
		s.source = readShaderSource( s.filename );
		if (shaders[i].source.empty())
		{
			std::cerr << "Failed to read " << s.filename << std::endl;
			return 0;
		}

		GLuint shader = glCreateShader( s.type );
		const GLchar* strings[1];
		strings[0] = s.source.c_str();
		glShaderSource(shader, 1, strings, NULL);
		
		glCompileShader( shader );

		GLint  compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled ) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete [] logMsg;
			return 0;
		}

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv( program, GL_LINK_STATUS, &linked );
	if ( !linked ) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog( program, logSize, NULL, logMsg );
		std::cerr << logMsg << std::endl;
		delete [] logMsg;

		return 0;
		//exit( EXIT_FAILURE );
	}

	/* use program object */
	glUseProgram(program);

	return program;
}
