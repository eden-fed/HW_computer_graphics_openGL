
#ifndef VECTOR4_H
#define VECTOR4_H
#include <string>
#include "Matrix4x4.h"

//row vector
class Vector4
{
private:
	//assighning coordinate variables
	double vectorValues[4];

public:
	//constructors
	Vector4();
	Vector4(const double valX, const double valY, const double valZ, const double valW);

	//copy constructor
	Vector4(const Vector4& V);

	//destructor
	~Vector4();

	//set
	void setVlaues(const double valX, const double valY, const double valZ, const double valW);

	//get functions
	const double getSize()const;

	//operators
	double &operator[](int i);
	const double &operator[](int i) const;
	const Vector4& operator = (const Vector4& V);
	const bool operator == (const Vector4& V)const;
	const bool operator != (const Vector4& V)const;
	const Vector4& operator += (const Vector4& V);
	 Vector4 operator + (const Vector4& V)const;
	const Vector4& operator -= (const Vector4& V);
	const Vector4 operator - (const Vector4& V)const;
	const Vector4 operator / (const double num)const;

	const double operator*(const Vector4& V)const; //Dot product 
	const Vector4 operator^(const Vector4& V)const; //Cross product
	const Vector4 operator*(const double num)const; //scalar*vector

	const Vector4& operator *= (const Matrix4x4& M) ;//vector*matrix
	const Vector4 operator * (const Matrix4x4& M);

	//class methods
	Vector4 roundVectorValues();
	Vector4 normalize() const;

	//helper functions
	int helpRoundNum(double F);

};

#endif
