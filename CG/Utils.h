#pragma once

#include <Glew/include/gl/glew.h>
#include <string>
#include "Matrix4x4.h"

//Macro to make code more readable
#define BUFFER_OFFSET(offset)   ((GLvoid*) (offset))

class point4
{
public:
	point4(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) : x(_x), y(_y), z(_z), w(_w) {};
	point4() : x(0.0), y(0.0), z(0.0), w(1.0) {};

protected:
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
	GLfloat  w;
};


class color4
{
public:
	color4(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) : r(_r), g(_g), b(_b), a(_a) {};
	color4() : r(0.0), g(0.0), b(0.0), a(1.0) {};

protected:
	GLfloat  r;
	GLfloat  g;
	GLfloat  b;
	GLfloat  a;
};



struct Shader
{
	std::string filename;
	GLenum      type;
	std::string source;
};


void ConvertQuaternionToMatrix(const float quat[4], float mat[16]);
void createTranslationMatrix(float tx, float ty, float tz, float mat[16]);
void ConvertMat4x4ToArray(Matrix4x4& objMAt, float mat[16]);
void createPerspectiveProjectionMatrix(float nearPlane, float farPlane, float right, float top, float mat[16]);
void createOrthographicProjectionMatrix(float nearPlane, float farPlane, float right, float top, float mat[16]);
void displayMessage(const std::string& str);
std::wstring getOpenFileName();
GLuint initShader(std::string vertexShaderFileName, std::string fragmentShaderFileName);
std::string readShaderSource(const std::string& shaderFile);
void quad(int a, int b, int c, int d, int& globalIndex, point4* vertex_positions, color4* vertex_colors, point4* positions, color4* colors);
void createCube(point4* positions, color4* colors);

