#include <cv.h>

class Camera
{
private:
	double camPos[3], a;
public:
	Camera();
	double GetX();
	double GetY();
	double GetZ();
	double GetAngleXOZ();
	bool Rotate(int angleZ);
	bool MoveForward(int step);
	bool MoveBack(int step);
};

class Image
{
public:
	char * color;
	float * depth;
	IplImage * img;
	Image();
};