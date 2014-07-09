#ifndef __MATH_DATA_H
#define __MATH_DATA_H

#define PI 3.1415926535897932384626433832795

struct Vector4d
{
	double e[4];
};
class Matrix
{
public:
	double e[4][4];
	Matrix(double values[4][4])
	{
		e[0][0] = values[0][0];
		e[0][1] = values[0][1];
		e[0][2] = values[0][2];
		e[0][3] = values[0][3];	

		e[1][0] = values[1][0];
		e[1][1] = values[1][1];
		e[1][2] = values[1][2];
		e[1][3] = values[1][3];
		
		e[2][0] = values[2][0];
		e[2][1] = values[2][1];
		e[2][2] = values[2][2];
		e[2][3] = values[2][3];
		
		e[3][0] = values[3][0];
		e[3][1] = values[3][1];
		e[3][2] = values[3][2];
		e[3][3] = values[3][3];
	}
	Vector4d operator*(const Vector4d &el)
	{
		Vector4d res;
		res.e[0] = e[0][0]*el.e[0] + e[0][1]*el.e[1] + e[0][2]*el.e[2] + e[0][3]*el.e[3];
		res.e[1] = e[1][0]*el.e[0] + e[1][1]*el.e[1] + e[1][2]*el.e[2] + e[1][3]*el.e[3];
		res.e[2] = e[2][0]*el.e[0] + e[2][1]*el.e[1] + e[2][2]*el.e[2] + e[2][3]*el.e[3];
		res.e[3] = e[3][0]*el.e[0] + e[3][1]*el.e[1] + e[3][2]*el.e[2] + e[3][3]*el.e[3];
		return res;
	};
};

#endif

