#include "MeshModel.h"
#include "Matrix4x4.h"


MeshModel::MeshModel()
{
}
MeshModel::MeshModel(Wavefront_obj & J)
{

	bool has_normals=false;
	if (J.m_normals.size() != 0) {
		has_normals = true;
	}
	for (int i = 0; i < J.m_points.size(); i++) {
		vertices.push_back(J.m_points[i]);
	}

	this->moveCentroidToOrigin();
	this->scale10units();

	if (has_normals) {
		for (int i = 0; i < J.m_normals.size(); i++) {
			normals.push_back(J.m_normals[i]);
		}
		for (int i = 0; i < J.m_faces.size(); i++) {
			Triangle t(vertices[J.m_faces[i].v[0]], vertices[J.m_faces[i].v[1]], vertices[J.m_faces[i].v[2]], normals[J.m_faces[i].n[0]], normals[J.m_faces[i].n[1]], normals[J.m_faces[i].n[2]]);
			faces.push_back(t);
		}
	}
	else {
		for (int i = 0; i < J.m_faces.size(); i++) {
			Triangle t(vertices[J.m_faces[i].v[0]], vertices[J.m_faces[i].v[1]], vertices[J.m_faces[i].v[2]]);
			faces.push_back(t);
		}
		this->calcNormals();
	}



}

MeshModel::~MeshModel()
{
}

void MeshModel::setAll(Wavefront_obj & J)
{

	bool has_normals = false;
	if (J.m_normals.size() != 0) {
		has_normals = true;
	}
	for (int i = 0; i < J.m_points.size(); i++) {
		vertices.push_back(J.m_points[i]);
	}

	this->moveCentroidToOrigin();
	this->scale10units();

	if (has_normals) {
		for (int i = 0; i < J.m_normals.size(); i++) {
			normals.push_back(J.m_normals[i]);
		}
		for (int i = 0; i < J.m_faces.size(); i++) {
			Triangle t(vertices[J.m_faces[i].v[0]], vertices[J.m_faces[i].v[1]], vertices[J.m_faces[i].v[2]], normals[J.m_faces[i].n[0]], normals[J.m_faces[i].n[1]], normals[J.m_faces[i].n[2]]);
			faces.push_back(t);
		}
	}
	else {
		for (int i = 0; i < J.m_faces.size(); i++) {
			Triangle t(vertices[J.m_faces[i].v[0]], vertices[J.m_faces[i].v[1]], vertices[J.m_faces[i].v[2]]);
			faces.push_back(t);
		}
		this->calcNormals();
	}


}

Vector4 MeshModel::getCentroid()
{
	return this->centroid;
}

void MeshModel::calcNormals()
{
	Vector4 sumVector;
	double sumArea;
	for (int i = 0; i < vertices.size(); i++) {
		sumVector.setVlaues(0, 0, 0, 1);
		sumArea = 0;
		for (int j = 0; j < faces.size(); j++) {
			if (faces[j].isVertexInTriangle(vertices[i])!=-1) {
				sumVector += faces[j].getCrossNormal()*faces[j].getArea();
				sumArea += faces[j].getArea();
			}
		}
		Vector4 normal = sumVector * (1.0 / sumArea);

		for (int j = 0; j < faces.size(); j++) {
			int x = faces[j].isVertexInTriangle(vertices[i]);
			if (x!=-1) {
				faces[j].getNormal(x) = normal;
			}
		}
	}
}

void MeshModel::calcCentroid()
{
	Vector4 sum;
	for (int i = 0; i < vertices.size(); i++) {
		sum += vertices[i];
	}
	this->centroid = sum * (1.0 / vertices.size());
}

void MeshModel::transformMshMdl(Matrix4x4 &M)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] = vertices[i]*M;
	}
	for (int j = 0; j < faces.size(); j++) {
		for (int k = 0; k < 3; k++) {
		//	faces[j].getNormal(k) = faces[j].getNormal(k).normalize();
			faces[j].getNormal(k) = (faces[j][k] + faces[j].getNormal(k))*M;
			faces[j][k] = faces[j][k] * M;
			faces[j].getNormal(k) = (faces[j].getNormal(k) - faces[j][k]).normalize();
		}
		faces[j].setNormal(faces[j].calcNormal());
	}
	calcCentroid();
}

void MeshModel::transformMshMdlonlyVertices(Matrix4x4 &M)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] = vertices[i]*M;
		//vertices[i].normal = vertices[i].normal*M;
	}
	calcCentroid();
}
/*
void MeshModel::transformNormals(Matrix4x4 & M, double normSize)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].normal = vertices[i].vertex + vertices[i].normal.normalize();
		vertices[i].normal = vertices[i].normal * M;
	}
}*/



void MeshModel::moveCentroidToOrigin()
{
	this->calcCentroid();
	Matrix4x4 M(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, (-1 * this->centroid[0]), (-1 * this->centroid[1]), (-1 * this->centroid[2]), 1);
	transformMshMdlonlyVertices(M);
}

void MeshModel::getAllVerticesOfInTriangles(std::vector<point4>& positions, std::vector<point4>& normals)
{
	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			point4 p(faces[i][j][0], faces[i][j][1], faces[i][j][2], 1.0);
			positions.push_back(p);
			point4 n(faces[i].getNormal(j)[0], faces[i].getNormal(j)[1], faces[i].getNormal(j)[2], 1.0);
			normals.push_back(n);
		}
	}
}

void MeshModel::getMinMaxValues(double& maxX, double& minX, double& maxY, double& minY, double& maxZ, double& minZ) {
	minX = maxX = vertices[0][0];
	minY = maxY = vertices[0][1];
	minZ = maxZ = vertices[0][2];
	for (int i = 1; i < vertices.size(); i++) {
		if (vertices[i][0] > maxX) maxX = vertices[i][0];
		else if (vertices[i][0] < minX) minX = vertices[i][0];
		if (vertices[i][1] > maxY) maxY = vertices[i][1];
		else if (vertices[i][1] < minY) minY = vertices[i][1];
		if (vertices[i][2] > maxZ) maxZ = vertices[i][2];
		else if (vertices[i][2] < minZ) minZ = vertices[i][2];
	}
}
void MeshModel::scale10units()
{
	double maxVal = 0.0;
	double maxXpos , maxXneg , maxYpos, maxYneg , maxZpos, maxZneg ;
	getMinMaxValues(maxXpos, maxXneg, maxYpos, maxYneg, maxZpos, maxZneg);
	maxXpos -= maxXneg;
	maxYpos -= maxYneg;
	maxZpos -= maxZneg;
	maxVal = maxXpos;
	if (maxYpos > maxVal) maxVal = maxYpos;
	if (maxZpos > maxVal) maxVal = maxZpos;

	double scaleValue = 10.0 / maxVal;
	Matrix4x4 M(scaleValue, 0, 0, 0, 0, scaleValue, 0, 0, 0, 0, scaleValue, 0, 0, 0, 0, 1);
	transformMshMdlonlyVertices(M);
}

