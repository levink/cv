#include <cv.h>

#ifndef __CAMERA_H
#define __CAMERA_H

class Camera
{
private:
	float camPos[3], a;
public:
	Camera(float x , float y, float z, float angle);
	float GetX(){
		return camPos[0];
	}
	float GetY(){
		return camPos[1];
	}
	float GetZ(){
		return camPos[2];
	}
	float GetAngleXOZ();
	bool Rotate(float angleZ);
	bool MoveForward(float step);
	bool MoveBack(float step);
	bool MoveUp(float step);
	bool MoveDown(float step);
};

#endif


