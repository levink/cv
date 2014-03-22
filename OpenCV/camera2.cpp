#include "scene.h"
#include <cmath>

const float PI =3.141592653;

Camera2::Camera2()
	{
		camPos2[0] = -30; a = -45;
		camPos2[1] = 10;
		camPos2[2] = -30;
	}
	double Camera2::GetX()
	{
		return camPos2[0];
	}
	double Camera2::GetY()
	{
		return camPos2[1];
	}
	double Camera2::GetZ()
	{
		return camPos2[2];
	}
	double Camera2::GetAngleXOZ()
	{
		return a;
	}
	bool Camera2::Rotate(int angleZ)
	{
		if(a==360) a = 0;
		a+=angleZ;
		return true;
	}
	bool Camera2::MoveForward(int step)
	{
		camPos2[0]-=step*sin(a*PI/180);
        camPos2[2]+=step*cos(a*PI/180);
		return true;
	}
	bool Camera2::MoveBack(int step)
	{
		camPos2[0]+=step*sin(a*PI/180);
		camPos2[2]-=step*cos(a*PI/180);
		return true;
	}