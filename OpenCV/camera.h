#include <cv.h>

#ifndef __CAMERA_H
#define __CAMERA_H

const double PI = 3.1415926535897932384626433832795;
const double D2R = PI / 180; //degrees to radians

enum Move {
	Forward = 0, 
	Back,
	Left,
	Right,
	Up, 
	Down,
	RotateLeft,
	RotateRight
};

class Camera
{
private:
	double camPos[3];
	double aZ, a;
	void Move(double dx, double dy, double dz)
	{
		camPos[0] += dx;
		camPos[1] += dy;
		camPos[2] += dz;
	}
public:
	double v;
	bool move[8];
	Camera(float x , float y, float z, float angle, double velocity);	
	double X(){
		return camPos[0];
	}
	double Y(){
		return camPos[1];
	}
	double Z(){
		return camPos[2];
	}
	double GetAngleZ()
	{
		return aZ;
	}
	double GetAngleY()
	{
		return a;
	}
	void Rotate(float horizontal, float vertical)
	{
		aZ += vertical;
		a += horizontal;
		if(aZ >= 360) aZ = aZ-360;
		if(aZ < 0) aZ = aZ + 360;
		if(a >= 360) a -= 360;
		if(a < 0) a += 360;
	}
	void MoveForward()
	{
		Move(v * sin(a * D2R), 0, -v * cos(a * D2R));
	}
	void MoveBack()
	{
		Move(-v * sin(a * D2R), 0, v * cos(a * D2R));
	}
	void MoveRight()
	{
		Move(v * cos(a * D2R), 0, v * sin(a * D2R));
	}
	void MoveLeft()
	{
		Move(-v * cos(a * D2R), 0, -v * sin(a * D2R));
	}
	void MoveUp(float step)
	{
		Move(0, step, 0);
	}
	void MoveDown(float step)
	{
		Move(0, -step, 0);
	}

	void MoveForward(double dist)
	{
		Move(dist * sin(a * D2R), 0, -dist * cos(a * D2R));
	}
	void MoveBack(double dist)
	{
		Move(-dist * sin(a * D2R), 0, dist * cos(a * D2R));
	}
	void MoveRight(double dist)
	{
		Move(dist * cos(a * D2R), 0, dist * sin(a * D2R));
	}
	void MoveLeft(double dist)
	{
		Move(-dist * cos(a * D2R), 0, -dist * sin(a * D2R));
	}
};

Camera::Camera(float x , float y, float z, float angle, double velocity)
{
	camPos[0] = x; 
	camPos[1] = y;
	camPos[2] = z;
	a = angle;
	aZ = 0;
	v = velocity;
	for(int i = Forward; i < RotateLeft; i++)
	{
		move[i] = false;
	}
}

#endif


