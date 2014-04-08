#include <cv.h>

class Camera
{
private:
	float camPos[3], a;
public:
	Camera();
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