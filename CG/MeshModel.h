#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include "Obj Parser\wavefront_obj.h"
#include "Utils.h"


class MeshModel
{


	//Wavefront_obj obj;
	std::vector<Vector4>  vertices;
	std::vector<Vector4>  normals;
	std::vector<Triangle> faces;
	Vector4 centroid;//****temp**** may not need it 	

public:
//	Material material;

	//constructors
	MeshModel();
	MeshModel(Wavefront_obj& J);

	//destructor
	~MeshModel();

	void setAll(Wavefront_obj& J);
	//get functions
	Vector4 getCentroid();
	inline int getNumberOfFaces() { return faces.size(); };//get number of triangles
	inline int getNumberOfVertices() { return vertices.size(); };//get number of vertices
	inline int getNumberOfNormals() { return vertices.size(); };//get number of normals
	inline Vector4 getVertex(int index) { return vertices[index]; };//get vertex by index
	//inline Vector4 getNormal(int index) { return normals[index]; }; // get normal by index
	inline Triangle getFace(int index) { return faces[index]; }; // get normal by index
	inline std::vector<Triangle>& getAllFaces() { return faces; }; // get normal by index


	//class methods 
	void calcNormals();
	void calcCentroid();
	void transformMshMdl(Matrix4x4 &M);
	void transformMshMdlonlyVertices(Matrix4x4 &M);
	void getMinMaxValues(double& maxXpos, double& maxXneg, double& maxYpos, double& maxYneg, double& maxZpos, double& maxZneg);
	void scale10units();
	void moveCentroidToOrigin();

	void getAllVerticesOfInTriangles(std::vector<point4>& positions, std::vector<point4>& normals); // get normal by index
	

};