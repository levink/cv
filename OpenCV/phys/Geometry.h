#include<iostream> // ÓÄÀËÈÒÜ!!!!!
#ifndef __GEOMETRY_H
#define __GEOMETRY_H

using namespace std;

class Vector
{
private:
	double x;
	double y;
	double z;
public:
	Vector();
	Vector(double _x, double _y, double _z);
	Vector(double v[3]);

	double GetX();
	double GetY();
	double GetZ();
	void SetX(double newx);
	void SetY(double newy);
	void SetZ(double newz);
	
	double length();
	double length2();

	Vector operator+(Vector& const right);
	Vector operator-(Vector& const right);
	Vector operator-();
	Vector operator*(double scale);
	Vector operator/(double u);
	
	double operator&(Vector& const right);
	double operator^(Vector& const right);
	Vector operator*(Vector& const right);
	Vector operator>(Vector& const right);
	Vector& operator= (Vector val);

	bool operator<(double value);
	bool operator==(Vector& const right);
	bool operator!=(Vector& const right);
};
Vector sqrt_ve(Vector& const value);
Vector Vector_norm(Vector value);

class Matrix
{
	private:
		double mat [3][3];
	public:
		Matrix();

		Matrix(double count[3][3]);

		double GetM(int a,int b);
		
		void SetM(double value,int i,int j);
		
		const double* Values();

		Matrix Transpose();

		Matrix operator* (double t);

		Vector operator* (Vector vector);

		Matrix operator* (Matrix& const right);

		Matrix operator/ (double t);

		Matrix operator= (Matrix& const right);

		Matrix Invert();
};

class Plane
{
private:
	double tmp [3][3];
	Matrix Mat;
	double equa [4];
public:
	Plane();
	void PlaneSetEquation(double eq[4]);
	Matrix GetBathis ();
	Plane(double Ctmp[3][3]);

	Plane(Vector x1,Vector x2, Vector x3);

	Plane(double eq [4]);
	Matrix Plane::GetInvertMat();

	Matrix GetMat();
	Vector GetN();
	double GetA();
	double GetB();
	double GetC();
	double GetD();
};
#endif