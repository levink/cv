#include <cmath>

#include "camera.h"

const float PI =3.1415926535897932384626433832795;

Camera::Camera(float x, float y, float z, float angle)
{
	camPos[0] = x; 
	camPos[1] = y;
	camPos[2] = z;
	a = angle;
};
float Camera::GetAngleXOZ()
{
	return a;
};
bool Camera::Rotate(float angleZ)
{
	a+=angleZ;
	if(a>359) a-=360;
	if(a<-359)a+=360;
	return true;
};
bool Camera::MoveForward(float step)
{
	camPos[0]-=step*sin(a*PI/180);
    camPos[2]+=step*cos(a*PI/180);
	return true;
};
bool Camera::MoveBack(float step)
{
	camPos[0]+=step*sin(a*PI/180);
	camPos[2]-=step*cos(a*PI/180);
	return true;
};
bool Camera::MoveUp(float step)
{
	camPos[1]+=step;
	return true;
};
bool Camera::MoveDown(float step)
{
	camPos[1]-=step;
	return true;
};
