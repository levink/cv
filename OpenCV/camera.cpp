#include "scene.h"
#include <cmath>

const float PI =3.1415926535897932384626433832795;

Camera::Camera(float x, float y, float z, float angle)
	{
		camPos[0] = x; a = angle;
		camPos[1] = y;
		camPos[2] = z;
	}
	float Camera::GetX()
	{
		return camPos[0];
	}
	float Camera::GetY()
	{
		return camPos[1];
	}
	float Camera::GetZ()
	{
		return camPos[2];
	}
	float Camera::GetAngleXOZ()
	{
		return a;
	}
	bool Camera::Rotate(float angleZ)
	{
		a+=angleZ;
		if(a>359) a-=360;
		if(a<-359)a+=360;
		return true;
	}
	bool Camera::MoveForward(float step)
	{
		camPos[0]-=step*sin(a*PI/180);
        camPos[2]+=step*cos(a*PI/180);
		return true;
	}
	bool Camera::MoveBack(float step)
	{
		camPos[0]+=step*sin(a*PI/180);
		camPos[2]-=step*cos(a*PI/180);
		return true;
	}
	bool Camera::MoveUp(float step)
	{
		camPos[1]+=step;
		return true;
	}
	bool Camera::MoveDown(float step)
	{
		camPos[1]-=step;
		return true;
	}

