#ifndef CAMERA_H
#define CAMERA_H
#include "MeshModel.h"
#include "Matrix4x4.h"
#include "Vector4.h"

typedef enum {
	PERSPECTIVE,
	ORTHOGRAPHIC,
} eProjectionType;

class Camera
{
	Matrix4x4 viewMtrx;
	Matrix4x4 projectionMtrx;
	eProjectionType pType;

	double flFovy, flNear, flFar, flAspectRatio;
	Vector4 vPosition, vDirection, vUp;
public:
	//constructors
	Camera(Vector4 vPosition, Vector4 vDirection, Vector4 vUp);
	Camera();

	//destructor
	~Camera();

	//get functions
	Matrix4x4 getViewMtrx() { return viewMtrx; };
	Matrix4x4 getProjectionMtrx() { return projectionMtrx; };

	//set functions
	void setCamPosition(Vector4& vPosition);
	void setCamDirection(Vector4& vDirection);
	void setCamUp(Vector4& vUp);

	void setNear(double flNear);
	void setFar(double flFar);
	void setFovy(double flFovy);
	void setProjectionType(eProjectionType pType);


	void setViewMtrx(Vector4& vPosition, Vector4& vDirection, Vector4& vUp);
	void setProjectionMatrix(double flFovy, double flNear, double flFar, eProjectionType pType, double fAspectRatio);

	//operators



};

#endif