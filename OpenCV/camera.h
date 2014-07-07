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
	double a;
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
	double GetAngleXOZ()
	{
		return a;
	}
	void Rotate(float angleZ)
	{
		a += angleZ;
		if(a > 360) a = 0;
		if(a < 0) a = 360;
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
};

Camera::Camera(float x , float y, float z, float angle, double velocity)
{
	camPos[0] = x; 
	camPos[1] = y;
	camPos[2] = z;
	a = angle;
	v = velocity;
	for(int i = Forward; i < RotateLeft; i++)
	{
		move[i] = false;
	}
}

#endif


