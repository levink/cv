#include <cv.h>

class Camera
{
private:
	float camPos[3], a;
public:
	Camera(float x , float y, float z, float angle);
	float GetX();
	float GetY();
	float GetZ();
	float GetAngleXOZ();
	bool Rotate(float angleZ);
	bool MoveForward(float step);
	bool MoveBack(float step);
	bool MoveUp(float step);
	bool MoveDown(float step);
};

