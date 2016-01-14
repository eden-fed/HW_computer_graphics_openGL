#include "Triangle.h"

#define B0DX 0
#define B0DY 1
#define B1DX 2
#define B1DY 3
#define B2DX 4
#define B2DY 5
#define IS_FILLED 6

#define X 0
#define Y 1
#define Z 3


Triangle::Triangle()
{
}

Triangle::Triangle(Vector4 & v1, Vector4 & v2, Vector4 & v3)
{
	this->vertices[0].vertex = v1;
	this->vertices[1].vertex = v2;
	this->vertices[2].vertex = v3;
	this->normal = getCrossNormal();
	this->area = 0;
	memset(gradient, 0.0, sizeof(gradient));
}

Triangle::Triangle(Vector4 & v1, Vector4 & v2, Vector4 & v3, Vector4 & n1, Vector4 & n2, Vector4 & n3)
{
	this->vertices[0].vertex = v1;
	this->vertices[1].vertex = v2;
	this->vertices[2].vertex = v3;
	this->vertices[0].normal = n1;
	this->vertices[1].normal = n2;
	this->vertices[2].normal = n3;
	this->normal = calcNormal();
	this->area = 0;
	memset(gradient, 0.0, sizeof(gradient));
}

Triangle::~Triangle()
{
}


const Triangle & Triangle::operator=(const Triangle & T)
{
	memcpy(this, &T, sizeof(Triangle));
	return *this;
}

double Triangle::getArea()
{
	if (this->area == 0) {
		this->area = 0.5*((vertices[0].vertex - vertices[1].vertex).getSize())*((vertices[0].vertex - vertices[2].vertex).getSize());
	}
	return this->area;
}

Vector4 Triangle::getNormal()
{
	return this->normal;
}

Vector4 Triangle::getCrossNormal()
{
	return ((vertices[1].vertex - vertices[0].vertex) ^ (vertices[2].vertex - vertices[0].vertex)).normalize();
}

vertexInfo& Triangle::getVertexInfo(unsigned int index)
{
	return this->vertices[index];
}

void Triangle::setNormal(Vector4 n)
{
	this->normal = n;
}

void Triangle::projectTriangle(Matrix4x4 & pMtrx)
{
		for (int i = 0; i < 3; i++) {
			vertices[i].normal = (vertices[i].vertex + vertices[i].normal)*pMtrx;
			vertices[i].vertex *= pMtrx;
			vertices[i].normal = (vertices[i].normal - vertices[i].vertex).normalize();
		}
}

Vector4 Triangle::calcNormal()
{
	return (vertices[0].normal + vertices[1].normal + vertices[2].normal) / 3;
}

const int Triangle::isVertexInTriangle(Vector4 & V)
{
	for (int i = 0; i < 3; i++) {
		if (V == vertices[i].vertex) {
			return i;
		}
	}
	return -1;
}
/*
Color Triangle::getColorOfPoint(int x, int y)
{
	return Color(0x00ff00);
}*/
void Triangle::calcBoundingRectangle(double & minX, double & maxX, double & minY, double & maxY)
{
	minX = maxX = (*this)[0][0];
	minY = maxY = (*this)[0][1];
	for (int i = 1; i < 3; i++) {
		if ((*this)[i][0] < minX) {
			minX = (*this)[i][0];
		}
		if ((*this)[i][0] > maxX) {
			maxX = (*this)[i][0];
		}
		if ((*this)[i][1] < minY) {
			minY = (*this)[i][1];
		}
		if ((*this)[i][1] > maxY) {
			maxY = (*this)[i][1];
		}
	}
	minX--;
	minY--;
	maxX++;
	maxY++;
}
