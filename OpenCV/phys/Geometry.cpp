#include <cmath>
#include "Objects.h"

//Vector::
	Vector::Vector()
	{
		x=0;
		y=0;
		z=0;
	}
	Vector::Vector(double _x, double _y, double _z)
	{
		x=_x;
		y=_y;
		z=_z;
	}
	Vector::Vector(double v[3])
	{
		x=v[0];
		y=v[1];
		z=v[2];
	}

	double Vector::GetX()
	{
		return x;
	}
	double Vector::GetY()
	{
		return y;
	}
	double Vector::GetZ()
	{
		return z;
	}
	void Vector::SetX(double newx)
	{
		x=newx;
	}
	void Vector::SetY(double newy)
	{
		y=newy;
	}
	void Vector::SetZ(double newz)
	{
		z=newz;
	}
	
	double Vector::length()
	{
		return sqrt( x*x + y*y + z*z);
	}
	double Vector::length2()
	{
		return x*x + y*y + z*z;
	}

	Vector Vector::operator+(Vector& const right)
	{
		return Vector(x+right.GetX(), y+right.GetY(), z+right.GetZ());
	}
	Vector Vector::operator-(Vector& const right)
	{
		return Vector(x - right.GetX(), y - right.GetY(), z - right.GetZ());
	}
	Vector Vector::operator-()
	{
		return Vector(-x, -y, -z); 
	}
	Vector Vector::operator*(double scale)
	{
		return Vector(x * scale, y * scale, z * scale);
	}
	Vector Vector::operator/(double u)
	{
		return Vector(x,y,z)*(1/u);
	}
	
	double Vector::operator&(Vector& const right)
	{
		//scalar multiply
		return x * right.GetX() + y * right.GetY() + z * right.GetZ(); 
	}
	double Vector::operator^(Vector& const right)
	{
		return( (x * right.GetX() + y * right.GetY() + z * right.GetZ() ) / (sqrt( (x * x + y * y + z * z) * right.length())) );
	}
	Vector Vector::operator*(Vector& const right)
	{
		//vector multiply
		return Vector(
			y * right.z - z * right.y,
			z * right.x - x * right.z,
			x * right.y - y * right.x);
	}
	Vector sqrt_ve(Vector& const value)
	{
		return Vector(	
			sqrt(value.GetX()), 
			sqrt(value.GetY()), 
			sqrt(value.GetZ()) );
	}
	Vector Vector::operator>(Vector& const right)
	{
		return Vector(
			x * right.x,
			y * right.y,
			z * right.z);
	}
	Vector& Vector::operator= (Vector val)
	{
		x = val.GetX();
		y = val.GetY();
		z = val.GetZ();
		return *this;
	}

	bool Vector::operator<(double value)
	{	
		return value <= 0 || this->length2() < value*value;
	}
	bool Vector::operator==(Vector& const right)
	{
		return x == right.GetX() && y == right.GetY() && z == right.GetZ();
	}
	bool Vector::operator!=(Vector& const right)
	{
		return x != right.GetX() || y != right.GetY() || z != right.GetZ();	
	}
	Vector Vector_norm(Vector value)
	{
		double longg  = sqrt(value.GetX()* value.GetX() + value.GetY() * value.GetY() + value.GetZ() * value.GetZ()); 
		return Vector(
			value.GetX() / longg,
			value.GetY() / longg,
			value.GetZ() / longg );
	}
//Matrix::
		Matrix::Matrix()
		{
			for(int i=0;i<3;i++)
				for(int e=0;e<3;e++)
					mat[i][e] = 0;
		}

		Matrix::Matrix(double count[3][3])
		{
			for(int i=0;i<3;i++)
				for(int e=0;e<3;e++)
					mat[i][e] = count[i][e];
		}

		double Matrix::GetM(int a,int b)
		{
			return mat[a][b];
		}
		
		void Matrix::SetM(double value,int i,int j)
		{
			mat[i][j] = value;
		}
		
		const double* Matrix::Values()
		{
			return &(mat[0][0]);
		}

		Matrix Matrix::Transpose()
		{
			double tr[3][3];
			for(int i=0;i<3;i++)
				for(int e=0;e<3;e++)
					tr[e][i] = mat[i][e];
			return Matrix(tr);
		}

		Matrix Matrix::operator* (double t)
		{
			double _mat[3][3];
			for(int i=0;i<3;i++)
				for(int e=0;e<3;e++)
					_mat[i][e] = mat[i][e] * t;
			return Matrix(_mat);
		}

		Vector Matrix::operator* (Vector vector)
		{
			double vx = vector.GetX();
			double vy = vector.GetY();
			double vz = vector.GetZ();
			return Vector(
				(mat[0][0] * vx +  mat[0][1] * vy + mat[0][2] * vz),
				(mat[1][0] * vx +  mat[1][1] * vy + mat[1][2] * vz),
				(mat[2][0] * vx +  mat[2][1] * vy + mat[2][2] * vz));
		}

		Matrix Matrix::operator* (Matrix& const right)
		{
			double _mat[3][3];
			const double* m = right.Values();

			/*for(int i=0;i<3;i++)
				for(int j=0;j<3;j++)
					_mat[i][j] = m1[i*3+0] * m2[0*3 + j] + m1[i*3+1] * m2[1*3 + j] + m1[i*3+2] * m2[2*3 + j];*/
			
			// more quickly
			_mat[0][0] = mat[0][0] * m[0*3 + 0] + mat[0][1] * m[1*3 + 0] + mat[0][2] * m[2*3 + 0];
			_mat[0][1] = mat[0][0] * m[0*3 + 1] + mat[0][1] * m[1*3 + 1] + mat[0][2] * m[2*3 + 1];
			_mat[0][2] = mat[0][0] * m[0*3 + 2] + mat[0][1] * m[1*3 + 2] + mat[0][2] * m[2*3 + 2];
			_mat[1][0] = mat[1][0] * m[0*3 + 0] + mat[1][1] * m[1*3 + 0] + mat[1][2] * m[2*3 + 0];
			_mat[1][1] = mat[1][0] * m[0*3 + 1] + mat[1][1] * m[1*3 + 1] + mat[1][2] * m[2*3 + 1];
			_mat[1][2] = mat[1][0] * m[0*3 + 2] + mat[1][1] * m[1*3 + 2] + mat[1][2] * m[2*3 + 2];
			_mat[2][0] = mat[2][0] * m[0*3 + 0] + mat[2][1] * m[1*3 + 0] + mat[2][2] * m[2*3 + 0];
			_mat[2][1] = mat[2][0] * m[0*3 + 1] + mat[2][1] * m[1*3 + 1] + mat[2][2] * m[2*3 + 1];
			_mat[2][2] = mat[2][0] * m[0*3 + 2] + mat[2][1] * m[1*3 + 2] + mat[2][2] * m[2*3 + 2];
			
			return Matrix(_mat);
		}

		Matrix Matrix::operator/ (double t)
		{
			double _mat[3][3];
			for(int i=0;i<3;i++)
				for(int e=0;e<3;e++)
					_mat[i][e] = mat[i][e] / t;
			return Matrix(mat);
		}

		Matrix Matrix::operator= (Matrix& const right)
		{
			//TODO: test this method
			const double *items = right.Values();
			for(int i=0;i<3;i++)
				for(int j=0;j<3;j++)
					mat[i][j] = items[i*3 + j];
			return *this;
		}

		Matrix Matrix::Invert()
		{
			//Determinant calculating
			double d1 = mat[0][0] * ( mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]);
			double d2 = mat[0][1] * ( mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]);
			double d3 = mat[0][2] * ( mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]);
			double det = d1 - d2 + d3;
			if(det == 0) det = 1;
			else det = 1 / det; //(!)
			
			//Mat_alg_aff + Transposition + " scale on (1/det)" in one step
			double matA [3][3];
			matA[0][0] =  (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) * det;
			matA[1][0] = -(mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) * det;
			matA[2][0] =  (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) * det;
			matA[0][1] = -(mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]) * det; 
			matA[1][1] =  (mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2]) * det; 
			matA[2][1] = -(mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1]) * det;
			matA[0][2] =  (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]) * det;
			matA[1][2] = -(mat[0][0] * mat[1][2] - mat[1][0] * mat[0][2]) * det;
			matA[2][2] =  (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) * det;
			
			return Matrix(matA);
		}
//Plane
	Plane::Plane()
	{
		equa[0] = 0;
		equa[1] = 0;
		equa[2] = 0;
		equa[3] = 0; 
		tr[0] = NULL;
		tr[1] = NULL;
		tr[2] = NULL;
		nor[0] = NULL;
		nor[1] = NULL;
		nor[2] = NULL;
		tr_num = 0;
		tes[0] = 0;
		tes[1] = 0;
		tes[2] = 0;
		tes[3] = 0;
		tes[4] = 0;
		tes[5] = 0;
		li = NULL;
		li_num = 0;
	}

	void Plane::PlaneSetEquation(double eq[4])
	{
		tr[0] = NULL;
		tr[1] = NULL;
		tr[2] = NULL;
		tes[0] = 0;
		tes[1] = 0;
		tes[2] = 0;
		tes[3] = 0;
		tes[4] = 0;
		tes[5] = 0;
		equa[0] = eq[0];
		equa[1] = eq[1];
		equa[2] = eq[2];
		equa[3] = eq[3];
		li = NULL;
		li_num = 0;
	}

	
	Plane::Plane(double Ctmp[3][3])
	{
		equa[0] = Ctmp[1][0]*(Ctmp[2][1] - Ctmp[2][2]) + Ctmp[1][1]*(Ctmp[2][2] - Ctmp[2][0]) + Ctmp[1][2]*(Ctmp[2][0] - Ctmp[2][1]);
		equa[1] = Ctmp[2][0]*(Ctmp[0][1] - Ctmp[0][2]) + Ctmp[2][1]*(Ctmp[0][2] - Ctmp[0][0]) + Ctmp[2][2]*(Ctmp[0][0] - Ctmp[0][1]);
		equa[2] = Ctmp[0][0]*(Ctmp[1][1] - Ctmp[1][2]) + Ctmp[0][1]*(Ctmp[1][2] - Ctmp[1][0]) + Ctmp[0][2]*(Ctmp[1][0] - Ctmp[1][1]);
		equa[3] = - (equa[0]*Ctmp[0][0] + equa[1] * Ctmp[0][1] + equa[2] * Ctmp[0][2]);

		double longg  = sqrt(equa[0] * equa[0] + equa[1] * equa[1] + equa[2] * equa[2]); 
		//double eq[4] = {equa[0],equa[1],equa[2],equa[3]};
		equa[0] = equa[0] / longg;
		equa[1] = equa[1] / longg;
		equa[2] = equa[2] / longg;
		equa[3] = equa[3] / longg;
		/*equa[0] = eq[0];
		equa[1] = eq[1];
		equa[2] = eq[2];
		equa[3] = eq[3];*/
	}

	Plane::Plane(Vector O,Vector A, Vector B)
	{
		equa[0] = ((A.GetY() - O.GetY()) * (B.GetZ() - O.GetZ())) - ((A.GetZ() - O.GetZ()) * (B.GetY() - O.GetY()));
		equa[1] = -((A.GetX() - O.GetX()) * (B.GetZ() - O.GetZ())) - ((A.GetZ() - O.GetZ()) * (B.GetX() - O.GetX()));
		equa[2] = ((A.GetX() - O.GetX()) * (B.GetY() - O.GetY())) - ((A.GetY() - O.GetY()) * (B.GetX() - O.GetX()));
		equa[3] = -(equa[0] * O.GetX() + equa[1] * O.GetY() + equa[2] * O.GetZ());

		double longg  = sqrt(equa[0] * equa[0] + equa[1] * equa[1] + equa[2] * equa[2]); 
		equa[0] = equa[0] / longg;
		equa[1] = equa[1] / longg;
		equa[2] = equa[2] / longg;
		equa[3] = equa[3] / longg;
	}

	Plane::Plane(double eq [4])
	{
		tr[0] = NULL;
		tr[1] = NULL;
		tr[2] = NULL;
		tes[0] = 0;
		tes[1] = 0;
		tes[2] = 0;
		tes[3] = 0;
		tes[4] = 0;
		tes[5] = 0;
		li = NULL;
		li_num = 0;
		for(int i=0;i<4;i++)
		{
			equa[i] = eq[i];
		}
		double length = sqrt(eq[0] * eq[0] + eq[1] * eq[1] + eq[2] * eq[2]); 
		equa[0] = equa[0] / length;
		equa[1] = equa[1] / length;
		equa[2] = equa[2] / length;
		equa[3] = equa[3] / length;
	}

	

	Vector Plane::GetN()
	{
		return Vector(equa[0],equa[1],equa[2]);
	}
	double Plane::GetA()
	{
		return equa[0];
	}
	double Plane::GetB()
	{
		return equa[1];
	}
	double Plane::GetC()
	{
		return equa[2];
	}
	double Plane::GetD()
	{
		return equa[3];
	}
	Vector Plane::project(Vector* point)
 	{
		double test = equa[0] * point->GetX() + equa[1] * point->GetY() + equa[2] * point->GetZ() + equa[3];
		if(test >=0.00001 || test <=-0.00001)
		{
			double _x = point->GetX();
			double _y = point->GetY();
			double _z = point->GetZ();
			if(equa[0] != 0)
			{

				double x = (_x * ( equa[1] * equa[1] + equa[2] * equa[2]) - equa[0] * (equa[1] * _y + equa[2] * _z + equa[3])) / Vector(equa[0],equa[1],equa[2]).length2();
				return(Vector(x,(equa[1] * (x - _x))/equa[0] + _y,(equa[2] * (x - _x))/equa[0] + _z));
			}
			if(equa[1] != 0)
			{

				double y = (_y * ( equa[0] * equa[0] + equa[2] * equa[2]) - equa[1] * (equa[0] * _x + equa[2] * _z + equa[3])) / Vector(equa[0],equa[1],equa[2]).length2();
				return(Vector((equa[0] * (y - _y))/equa[1] + _x,y,(equa[2] * (y - _y))/equa[1] + _z));
			}
			if(equa[2] != 0)
			{

				double z = (_z * ( equa[0] * equa[0] + equa[1] * equa[1]) - equa[2] * (equa[0] * _x + equa[1] * _y + equa[3])) / Vector(equa[0],equa[1],equa[2]).length2();
				return(Vector((equa[0] * (z - _z))/equa[2] + _x,(equa[1] * (z - _z))/equa[2] + _y,z));
			}
		}
		return (*point);
	}
	void Plane::SetPoints(Vector* point, int l)
	{
		num = l;
		for(int i= 0; i < l; i++)
		{
			Vector t = point[i];
			tmp.push_back(project(&t));
		}
		this->triangulation();
 	}
	void Plane::triangulation() // триангуляция !!!!!!!(не работает вообще 06.01.2015)работает только для четырёхугольников! + определение ограничивающего куба + определение нормалей к контуру
	{
		int current = 0;
		int maximum = num;
		tr_num = 0;
		tr[0] = new int[num];
		tr[1] = new int[num];
		tr[2] = new int[num];
		nor[0] = new Vector[num];
		nor[1] = new Vector[num];
		nor[2] = new Vector[num];
		int cur_rej = 0;
		int max_rej = 4;
		int * rejected = new int[4];
		int signal = num;

		li_num = num;
		li = new Line[num];
		int n = 0;
		for(int i =0;i<num;i++)
		{
			n = i + 1;
			if(n >= num)
				n  = n - (num);
			li[i].vec = Vector(tmp[n].GetX() - tmp[i].GetX(),tmp[n].GetY() - tmp[i].GetY(),tmp[n].GetZ() - tmp[i].GetZ());
			li[i].tmp = tmp[i];
			if(tmp[i].GetX() > tmp[n].GetX())
			{
				li[i].limit[0] = tmp[i].GetX();
				li[i].limit[1] = tmp[n].GetX();
			}	
			else
			{	
				li[i].limit[0] = tmp[n].GetX();
				li[i].limit[1] = tmp[i].GetX();
			}
			if(tmp[i].GetY() > tmp[n].GetY())
			{
				li[i].limit[2] = tmp[i].GetY();
				li[i].limit[3] = tmp[n].GetY();
			}	
			else
			{	
				li[i].limit[2] = tmp[n].GetY();
				li[i].limit[3] = tmp[i].GetY();
			}
			if(tmp[i].GetZ() > tmp[n].GetZ())
			{
				li[i].limit[4] = tmp[i].GetZ();
				li[i].limit[5] = tmp[n].GetZ();
			}						 
			else					 
			{						 
				li[i].limit[4] = tmp[n].GetZ();
				li[i].limit[5] = tmp[i].GetZ();
			}
			
		}

		Vector testing = Vector();
		n = 0;
		int  i = 0;
		int e  = 0;
		while(signal >= 3) // начало триангуляции код не тестировался ПОСЛЕ ТЕСТА УДАЛИТЬ. Эту запись, а не код
		{
			bool et = true;
			bool nt = true;
			bool it = true;
			e = i + 1; 
			n = i - 1; 
			while(et || nt || it)
			{
				if(et)
				{
					bool erej = true;
					if(e >= num)
					{
						e  = e - (num);
					}
					for(int i = 0;i<cur_rej;i++)
					{
						if(rejected[i] == e)
						{
							erej = false;
							break;
						}
					}
					if(!erej)
					{
						e++;
						erej = true;
					}
					else
					{
						et = false;
					}
				}
				if(nt)
				{
					bool nrej = true;
					if(n < 0)
					{
						n = num + n;
					}
					for(int i = 0;i<cur_rej;i++)
					{
						if(rejected[i] == n)
						{
							nrej = false;
							break;
						}
					}
					if(!nrej)
					{
						n--;
						nrej = true;
					}
					else
					{
						nt = false;
					}
				}
				if(it)
				{
					bool irej = true;
					if(i >= num)
					{
						i = 0;
					}
					for(int c = 0;c<cur_rej;c++)
					{
						if(rejected[c] == i)
						{
							irej = false;
							break;
						}
					}
					if(!irej)
					{
						i++;
						irej = true;
					}
					else
					{
						it = false;
					}
				}
			}
			
			Vector normal[3];
			Vector edge = Vector(tmp[e].GetX() - tmp[i].GetX(),0, tmp[e].GetZ() - tmp[i].GetZ());
			normal[0] = Vector(edge.GetZ(), 0, -edge.GetX()); // Всё правильно, не надо пугаться 
			testing = Vector(tmp[n].GetX() - tmp[e].GetX(), 0, tmp[n].GetZ() - tmp[e].GetZ());
			if((normal[0] & testing) <= 0)
			{
				if((normal[0] & testing) == 0)
				{
					Vector tes = Vector(tmp[n].GetX() - tmp[i].GetX(),0, tmp[n].GetZ() - tmp[i].GetZ());
					if((normal[0] & tes) < 0)
						normal[0] = Vector(-edge.GetZ(), 0, edge.GetX());
				}
				else
					normal[0] = Vector(-edge.GetZ(), 0, edge.GetX());
			}
			testing = - edge;
			edge = Vector(tmp[n].GetX() - tmp[e].GetX(),0, tmp[n].GetZ() - tmp[e].GetZ());
			normal[1] = Vector(edge.GetZ(), 0, -edge.GetX());
			if((normal[1] & testing) <= 0)
			{
				if((normal[1] & testing) == 0)
				{
					Vector tes = Vector(tmp[i].GetX() - tmp[n].GetX(),0, tmp[i].GetZ() - tmp[n].GetZ());
					if((normal[i] & tes) < 0)
						normal[1] = Vector(-edge.GetZ(), 0, edge.GetX());
				}
				else
					normal[1] = Vector(-edge.GetZ(), 0, edge.GetX());
			}
			testing = - edge;
			edge = Vector(tmp[i].GetX() - tmp[n].GetX(),0, tmp[i].GetZ() - tmp[n].GetZ());
			normal[2] = Vector(edge.GetZ(), 0, -edge.GetX());
			if((normal[2] & testing) <= 0)
			{
				if((normal[2] & testing) == 0)
				{
					Vector tes = Vector(tmp[e].GetX() - tmp[i].GetX(),0, tmp[e].GetZ() - tmp[i].GetZ());
					if((normal[2] & tes) < 0)
						normal[2] = Vector(-edge.GetZ(), 0, edge.GetX());
				}
				else
					normal[2] = Vector(-edge.GetZ(), 0, edge.GetX());
			}
			bool flag = 1;
			for(int c = 0;c<num;c++)
			{
				if(c!=i)
				{
					testing = Vector(tmp[c].GetX() - tmp[i].GetX(),0, tmp[c].GetZ() - tmp[i].GetZ());
					if((normal[0] & testing) > 0 && (normal[1] & testing) > 0 && (normal[2] & testing) > 0)
					{
						flag = 0;
						break;
					}
				}
			}
			if(flag)
			{
				tr[0][current] = i;
				tr[1][current] = e;
				tr[2][current] = n;
				nor[0][current] = normal[0];
				nor[1][current] = normal[1];
				nor[2][current] = normal[2];
				tr_num += 1;
				current += 1;
				signal -= 1;
				rejected[cur_rej] = i;
				cur_rej += 1;
				if(cur_rej >=max_rej)
				{
					int * copy = new int[cur_rej];
					for(int i = 0;i<cur_rej;i++)
					{
						copy[i] = rejected[i];
					}
					delete rejected;
					rejected = new int [cur_rej + 4];
					for(int i = 0 ;i<cur_rej;i++)
					{
						rejected[i] = copy[i];
					}
					delete copy;
					max_rej +=4;
				}
				if(current >= maximum)
				{
					int * cop1 = new int[current];
					int * cop2 = new int[current];
					int * cop3 = new int[current];
					Vector * co_n1 = new Vector[current];
					Vector * co_n2 = new Vector[current];
					Vector * co_n3 = new Vector[current];
					for(int i = 0;i<num;i++)
					{
						cop1[i] = tr[0][i];
						cop2[i] = tr[1][i];
						cop3[i] = tr[2][i];
						co_n1[i] = nor[0][i];
						co_n2[i] = nor[1][i];
						co_n3[i] = nor[2][i];
					}
					delete tr[0];
					delete tr[1];
					delete tr[2];
					delete nor[0];
					delete nor[1];
					delete nor[2];
					tr[0] = new int[current + 10];
					tr[1] = new int[current + 10];
					tr[2] = new int[current + 10];
					nor[0] = new Vector[current + 10];
					nor[1] = new Vector[current + 10];
					nor[2] = new Vector[current + 10];
					for(int i = 0; i < num;i++)
					{
						tr[0][i] = cop1[i];
						tr[1][i] = cop2[i];
						tr[2][i] = cop3[i];
						nor[0][i] = co_n1[i];
						nor[1][i] = co_n2[i];
						nor[2][i] = co_n3[i];
					}
					delete cop1;
					delete cop2;
					delete cop3;
					delete co_n1;
					delete co_n2;
					delete co_n3;
					maximum += 10;
				}
			
			}// конец триангуляции, конец света, конец добра и зла, чёрная дыра без массы и тому подобные прелести ( Конец шуту и королю, и глупости, и уму. Исполняли Никитины, Автора стихов не помню)
			i++;
		}
		delete rejected;
		// Конец поиска.
		//Определение ограничивающего куба.
		double maxX = tmp[num-1].GetX();
		double maxY = tmp[num-1].GetY();
		double maxZ = tmp[num-1].GetZ();
		for(int i = 0;i<num;i++) 
		{
			if(tmp[i].GetX() > maxX)
			{
				maxX = tmp[i].GetX();
			}
			if(tmp[i].GetY() > maxY)
			{
				maxY = tmp[i].GetY();
			}
			if(tmp[i].GetZ() > maxZ)
			{
				maxZ = tmp[i].GetZ();
			}
		}
		tes[0] = maxX + 2;
		tes[2] = maxY + 2;
		tes[4] = maxZ + 2;
		maxX = tmp[num-1].GetX();
		maxY = tmp[num-1].GetY();
		maxZ = tmp[num-1].GetZ();
		for(int i = 0;i<num;i++) 
		{
			if(tmp[i].GetX() < maxX)
			{
				maxX = tmp[i].GetX();
			}
			if(tmp[i].GetY() < maxY)
			{
				maxY = tmp[i].GetY();
			}
			if(tmp[i].GetZ() < maxZ)
			{
				maxZ = tmp[i].GetZ();
			}
		}
		tes[1] = maxX - 2;
		tes[3] = maxY - 2;
		tes[5] = maxZ - 2;
		//Конец определения куба.
	}

	Line::Line ()
	{
		vec = Vector();
		tmp = Vector();
	}
	Line::Line(Vector v, Vector t)
	{
		vec = v;
		tmp = t;
	}
	Vector Line::projection(Vector t) 
	{
		double x,y,z;
		double p[3] = {vec.GetX(),vec.GetY(),vec.GetZ()};
		bool flag = true;
		if(p[0] != 0)
		{
			x = (p[0] * ( p[0] * t.GetX() + p[1] * ( t.GetY() - tmp.GetY()) + p[2]*(t.GetZ() - tmp.GetZ())) + tmp.GetX() * (p[1] * p[1] + p[2] * p[2])) / (p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
			y = tmp.GetY() - (p[1] * (tmp.GetX() - x)) / (p[0]); 
			z = tmp.GetZ() - (p[2] * (tmp.GetX() - x)) / (p[0]);
			flag = false;
		}
		if(p[1] != 0 && flag)
		{
			y = (p[1] * (p[1] * t.GetY() + p[0] * (t.GetX() - tmp.GetX()) + p[2]*(t.GetZ() - tmp.GetZ())) + tmp.GetY() * (p[0] * p[0] + p[2] * p[2])) / (p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
			x = tmp.GetX() - (p[0] * (tmp.GetY() - y )) / (p[1]);
			z = tmp.GetZ() - (p[2] * (tmp.GetY() - y )) / (p[1]);
			flag = false;
		}
		if(p[2] != 0 && flag)
		{
			z = (p[2] * (p[2] * t.GetZ() + p[0] * (t.GetX() + tmp.GetX()) + p[1]*(t.GetY() + tmp.GetY())) + tmp.GetZ() * (p[0] * p[0] + p[1] * p[1])) / (p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
			x = tmp.GetX() - (p[0] * (tmp.GetZ() - z)) / (p[2]);
			y = tmp.GetY() - (p[1] * (tmp.GetZ() - z)) / (p[2]);
		}
		return Vector(x,y,z);
	}
	Vector Line::lineXYZ(double x)
	{
		Vector t = Vector(x,vec.GetY() * (x - tmp.GetX())/vec.GetX() + tmp.GetY(), vec.GetZ() * (x - tmp.GetX())/vec.GetX() + tmp.GetZ());
		return t ;
	}