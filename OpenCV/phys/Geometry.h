#include<iostream> // УДАЛИТЬ!!!!!
#include <vector>

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
	double operator%(Vector& const right);
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

class Line 
{
public:
	Vector vec;
	Vector tmp;
	double limit[6]; // Max X -> Min X -> Max Y -> Min Y -> Max Z -> Min Z

	Line();
	Line(Vector v, Vector t);
	Vector projection(Vector t);
	Vector lineXYZ(double x);
};

class Plane
{
private:
	double equa [4]; // Нормаль к плоскости.
	//Matrix Mat; // Матрица перевода в СК, связанную с плоскостью.
	//Vector * vec; // Направляющие вектора прямых, ограничивающих контур. Точки, через которые проходят прямые - это tmp под номером = номеру вектора.
	double tes [6]; // Ограничивающий куб. MaxX->MinX->MaxY->MinY->MaxZ->MinZ
public:
	Vector * nor[3]; // Нормали к прямым, ограничивающим контур. В СК, связанной с плоскостью.
	int *  tr[3]; // Треугольники. Содержит номера вершин, принадлежащих треугольникам. Необходимо для проверки столкновения и правильной отрисовки плоскостей.
	int tr_num; // количество треугольников, нужно для того, чтобы понять, было ли столкновение. 
	vector<Vector> tmp; // Точки, определяющие ВЫПУКЛЫЙ контур.
	int num;
	Line * li;
	// Проекции точек, определяющих ВЫПУКЛЫЙ контур.
	int li_num;

	Plane();
	Plane(double eq [4]);
	Plane(double Ctmp[3][3]);
	Plane(Vector x1,Vector x2, Vector x3);

	void PlaneSetEquation(double eq[4]);

	Vector project(Vector* point);
	void SetPoints(Vector * t, int l);
	void triangulation();
	bool cubeinspection(Vector tmp)
	{
		bool a = tmp.GetX() < tes[0] && tmp.GetX() > tes[1];
		bool b = tmp.GetY() < tes[2] && tmp.GetY() > tes[3];
		bool c = tmp.GetZ() < tes[4] && tmp.GetZ() > tes[5];
		return ( a && b & c);
	}

	//Matrix GetMat();
	Vector GetN();
	double GetA();
	double GetB();
	double GetC();
	double GetD();
};

#endif