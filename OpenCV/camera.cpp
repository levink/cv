#include "scene.h"
#include <cmath>

const float PI =3.141592653;

Camera::Camera()
	{
		camPos[0] = 30; a = 0;
		camPos[1] = 10;
		camPos[2] = 30;
	}
	double Camera::GetX()
	{
		return camPos[0];
	}
	double Camera::GetY()
	{
		return camPos[1];
	}
	double Camera::GetZ()
	{
		return camPos[2];
	}
	double Camera::GetAngleXOZ()
	{
		return a;
	}
	bool Camera::Rotate(int angleZ)
	{
		if(a==360) a = 0;
		a+=angleZ;
		return true;
	}
	bool Camera::MoveForward(int step)
	{
		camPos[0]-=step*sin(a*PI/180);
        camPos[2]+=step*cos(a*PI/180);
		return true;
	}
	bool Camera::MoveBack(int step)
	{
		camPos[0]+=step*sin(a*PI/180);
		camPos[2]-=step*cos(a*PI/180);
		return true;
	}