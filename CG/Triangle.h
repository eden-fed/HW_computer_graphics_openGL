#pragma once
#include "Vector4.h"
//#include "Color.h"
typedef struct {
	float x;
	float y;
}TextureCrd;

typedef struct {
	Vector4 vertex;
	Vector4 normal;
	TextureCrd texCoordinate;
}vertexInfo;

class Triangle
{
	vertexInfo vertices[3];
	double area;
	Vector4 normal;

public:
	double gradient[7];
	//constructors
	Triangle();
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3);
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& n1, Vector4& n2, Vector4& n3);
	Triangle(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& n1, Vector4& n2, Vector4& n3, Vector4& t1, Vector4& t2, Vector4& t3);

	const Triangle & operator=(const Triangle & V);

	//destructor
	~Triangle();

	//get functions
	double getArea();
	Vector4 getNormal();
	Vector4 getCrossNormal();
	vertexInfo& getVertexInfo(unsigned int index);


	inline Vector4& operator[](int index) { return vertices[index].vertex; }
	inline const Vector4& operator[](int index) const { return vertices[index].vertex; }
	inline Vector4& getNormal(int index) { return vertices[index].normal; }
	inline TextureCrd& getTC(int index) { return vertices[index].texCoordinate; }
	void setNormal(Vector4 n);

	//class methods
	void projectTriangle(Matrix4x4& pMtrx);
	Vector4 calcNormal();
	const int isVertexInTriangle(Vector4& V);
	//Color getColorOfPoint(int x, int y);
	void calcBoundingRectangle(double& minX, double& maxX, double& minY, double& maxY);

};