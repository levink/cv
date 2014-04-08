#include "scene.h"
#include <cmath>

const float PI =3.141592653;

Camera::Camera()
	{
		camPos[0] = 30; a = 0;
		camPos[1] = 10;
		camPos[2] = 30;
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
		if(a==360) a = 0;
		a+=angleZ;
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
		camPos[1]+=step;
		return true;
	}