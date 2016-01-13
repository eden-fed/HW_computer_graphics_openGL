#include "Matrix4x4.h"
#include "Vector4.h"

Matrix4x4::Matrix4x4():v1{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1} }
{

}

Matrix4x4::Matrix4x4(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double z1, double z2, double z3, double z4, double w1, double w2, double w3, double w4=1)
{
	v1[0][0] = x1;
	v1[0][1] = x2;
	v1[0][2] = x3;
	v1[0][3] = x4;
	v1[1][0] = y1;
	v1[1][1] = y2;
	v1[1][2] = y3;
	v1[1][3] = y4;
	v1[2][0] = z1;
	v1[2][1] = z2;
	v1[2][2] = z3;
	v1[2][3] = z4;
	v1[3][0] = w1;
	v1[3][1] = w2;
	v1[3][2] = w3;
	v1[3][3] = w4;
}

Matrix4x4::Matrix4x4(const Matrix4x4 & m)
{
	memcpy(this, &m, sizeof(Matrix4x4));
}

Matrix4x4::Matrix4x4(float mat[16])
{
	setColumn(mat[0 * 4 + 0], mat[0 * 4 + 1], mat[0 * 4 + 2], mat[0 * 4 + 3], 0);
	setColumn(mat[1 * 4 + 0], mat[1 * 4 + 1], mat[1 * 4 + 2], mat[1 * 4 + 3], 1);
	setColumn(mat[2 * 4 + 0], mat[2 * 4 + 1], mat[2 * 4 + 2], mat[2 * 4 + 3], 2);
	setColumn(mat[3 * 4 + 0], mat[3 * 4 + 1], mat[3 * 4 + 2], mat[3 * 4 + 3], 3);
}

void Matrix4x4::setRow(double x, double y, double z, double w, unsigned int rowNum)
{
	v1[rowNum][0] = x;
	v1[rowNum][1] = y;
	v1[rowNum][2] = z;
	v1[rowNum][3] = w;
}

void Matrix4x4::setColumn(double x, double y, double z, double w, unsigned int colNum)
{
	v1[0][colNum] = x;
	v1[1][colNum] = y;
	v1[2][colNum] = z;
	v1[3][colNum] = w;
}

const double*  Matrix4x4::operator[](int i) const
{
	return this->v1[i];
}

double * Matrix4x4::operator[](int i)
{
	return this->v1[i];
}

const Matrix4x4 & Matrix4x4::operator=(const Matrix4x4 & m)
{
	memcpy(this, &m, sizeof(Matrix4x4));
	return *this;
}

const bool Matrix4x4::operator==(const Matrix4x4 & m) const
{
	if (memcmp(this, &m, sizeof(Matrix4x4)) == 0)
		return true;
	else
		return false;
}

const bool Matrix4x4::operator!=(const Matrix4x4 & m) const
{
	return (!(*this == m));
}

const Matrix4x4  Matrix4x4::operator+(const Matrix4x4 & m) const
{
	Matrix4x4 newMat;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMat[i][j] = v1[i][j] + m[i][j];
	return newMat;
}

const Matrix4x4 Matrix4x4::operator-(const Matrix4x4 & m) const
{
	Matrix4x4 newMat;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMat[i][j] = v1[i][j] - m[i][j];
	return newMat;
}

const Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & m) const
{//return this*m
	Matrix4x4 newMat;
	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
		{
			newMat[i][j] = 0;
			for (int k = 0; k<4; k++)
				newMat[i][j] = newMat[i][j] + (*this)[i][k] * m[k][j];
		}
	return newMat;
}

const Matrix4x4& Matrix4x4::operator*=(const Matrix4x4 & m) 
{//this=this*m
/*	Matrix4x4 newMat;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			newMat[i][j] = v1[i][0] * m[0][j] + v1[i][1] * m[1][j] + v1[i][2] * m[2][j] + v1[i][3] * m[3][j];*/
	*this = (*this)*m;
	return *this;
}
const Matrix4x4 Matrix4x4::transpose()
{
	Matrix4x4 ret;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ret[i][j] = *this[j][i];
		}
	}
	return ret;
}

void Matrix4x4::setAllValues(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4, double z1, double z2, double z3, double z4, double w1, double w2, double w3, double w4)
{
	v1[0][0] = x1;
	v1[0][1] = x2;
	v1[0][2] = x3;
	v1[0][3] = x4;
	v1[1][0] = y1;
	v1[1][1] = y2;
	v1[1][2] = y3;
	v1[1][3] = y4;
	v1[2][0] = z1;
	v1[2][1] = z2;
	v1[2][2] = z3;
	v1[2][3] = z4;
	v1[3][0] = w1;
	v1[3][1] = w2;
	v1[3][2] = w3;
	v1[3][3] = w4;
}

/*const Vector4 Matrix4x4::operator*(const Vector4 & v) const
{
	Vector4 newV;
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			newV[i] += v1[i][j] * v[j];
		}
	}
	return newV;
}*/


