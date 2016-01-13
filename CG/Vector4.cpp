#include "Vector4.h"

Vector4::Vector4() :vectorValues{0,0,0,1}
{
}

Vector4::Vector4(const double valX, const double valY, const double valZ, const double valW)
{
	this->vectorValues[0] = valX;
	this->vectorValues[1] = valY;
	this->vectorValues[2] = valZ;
	this->vectorValues[3] = valW;
}

Vector4::Vector4(const Vector4 & V)
{
	//void * memcpy ( void * destination, const void * source, size_t num );
	memcpy(this, &V,sizeof(Vector4));
	//for (int i = 0; i <= 3; i++) {
	//	this->vectorValues[i] = V.vectorValues[i];
	//}
}

Vector4::~Vector4()
{
}

void Vector4::setVlaues(const double valX, const double valY, const double valZ, const double valW)
{
	this->vectorValues[0] = valX;
	this->vectorValues[1] = valY;
	this->vectorValues[2] = valZ;
	this->vectorValues[3] = valW;
}


const double Vector4::getSize() const
{
	return (sqrt(pow((*this)[0], 2) + pow((*this)[1], 2) + pow((*this)[2], 2)));
}

double & Vector4::operator[](int i)
{
	return this->vectorValues[i];
}

const double & Vector4::operator[](int i) const
{
	return this->vectorValues[i];
}

const Vector4 & Vector4::operator=(const Vector4 & V)
{
	memcpy(this, &V, sizeof(Vector4));
	return *this;
}

const bool Vector4::operator==(const Vector4 & V) const
{
	if (memcmp(this, &V, sizeof(Vector4)) == 0)
		return true;
	else
		return false;
}

const bool Vector4::operator!=(const Vector4 & V) const
{
	return !(*this==V);
}

const Vector4 & Vector4::operator+=(const Vector4 & V)
{
		for (int i = 0; i <= 2; i++) {
			this->vectorValues[i] += V.vectorValues[i];
		}
	return *this;

}

Vector4 Vector4::operator+(const Vector4 & V)const
{
	Vector4 retV = *this;
		for (int i = 0; i <= 2; i++) {
			retV.vectorValues[i] += V.vectorValues[i];
		}
	
	return retV;
}

const Vector4 & Vector4::operator-=(const Vector4 & V)
{	
		for (int i = 0; i <= 2; i++) {
			this->vectorValues[i] -= V.vectorValues[i];
		}
	return *this;
}

const Vector4 Vector4::operator-(const Vector4 & V) const
{
	Vector4 retV = *this;
		for (int i = 0; i <= 2; i++) {
			retV.vectorValues[i] -= V.vectorValues[i];
		}
	return retV;
}

const Vector4 Vector4::operator/(const double num) const
{
	Vector4 retVal;
	for (int i = 0; i < 3; i++) {
		retVal[i] = (*this)[i] * (1/num);
	}

	return retVal;
}

const double Vector4::operator*(const Vector4 & V)const
{
	double retVal=0;
		for (int i = 0; i < 3; i++) {
			retVal += (*this)[i] * V[i];
			//retVal += (*this)[0] * V[0];
		}
	

	return retVal;
}

const Vector4 Vector4::operator^(const Vector4 & V)const //RH coordinate system
{
	Vector4 retV;
	retV[0] = (*this)[1] * V[2] - (*this)[2] * V[1];
	retV[1] = (*this)[2] * V[0] - (*this)[0] * V[2];
	retV[2] = (*this)[0] * V[1] - (*this)[1] * V[0];
	return retV;
}

const Vector4 Vector4::operator*(const double num)const
{
	Vector4 retV;
	for (int i = 0; i <= 2; i++) {
		retV.vectorValues[i] = num*this->vectorValues[i];
	}
	return retV;
}

const Vector4 & Vector4::operator*=(const Matrix4x4 & M)
{
	//	Vector4 V = *this;
	//	memset(this, 0, sizeof(Vector4));
	//	for (int i = 0; i <= 3; i++) {
	//		for (int j = 0; j <= 3; j++) {
	//			(*this)[i] += V[j] * M[j][i];
	//		}
	//	}

	//return *this*(1 / (*this)[3]);
	(*this) = (*this)*M;
	return (*this);
}

const Vector4 Vector4::operator*(const Matrix4x4 & M)
{
	Vector4 V(0,0,0,0);
	
		for (int i = 0; i <= 3; i++) {
			for (int j = 0; j <= 3; j++) {
				V[i] += (*this)[j] * M[j][i];
			}
		}
	return V*(1/V[3]);
}

Vector4 Vector4::roundVectorValues()
{
	Vector4 retVal;
	for (int i = 0; i < 3; i++) {
		retVal[i] = helpRoundNum((*this)[i]);
	}
	return retVal;
}

int Vector4::helpRoundNum(double f)
{
	int i = f;
	if (f - i <= 0.5) {
		return i;
	}
	else {
		return i + 1;
	}
}

Vector4 Vector4::normalize() const
{
	Vector4 ret;
	double size = this->getSize();
	if (size != 0) {
		for (int i = 0; i < 3; i++) {
			ret[i] = (*this)[i] / size;
		}
	}
	return ret;
}

/*Vector4 Vector4::convertToEuclideanCoordinate()
{
	Vector4 retVal;
	for (int i = 0; i < 3; i++) {
		retVal[i] = (*this)[i];
	}
	for (int i = 0; i < 3; i++) {
		retVal[i] /= (*this)[3];
	}
	return retVal;
}*/

