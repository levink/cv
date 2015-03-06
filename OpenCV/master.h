#include <vector>
#include "glut.h"
#include "camera.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"

#ifndef __MASTER_H
#define __MASTER_H

const double TAN_30 = 0.5773502692;

struct Pix
{
	float x,y,z;
};


class Master
{
private:
	int FramesCount;
	int W_WIDTH, W_HEIGHT;
	double Z_NEAR, Z_FAR;
	std::vector<Pix>cloud;
	void RestoreDepthFromBuffer(float* buf, int length, float _zFar, float _zNear)
	{
		for(int i=0; i < length; i++)
		{
			buf[i] = (_zFar * _zNear) / (buf[i] * (_zFar - _zNear) - _zFar);
		}	
	}
	void SetProjectionParams(double *top, double *left, double *aspect = NULL)
	{
		double tmp_a = 0;
		if (!aspect) aspect = &tmp_a;
		*aspect = ((double)W_WIDTH) / W_HEIGHT;
		if (top) * top = Z_NEAR * TAN_30;
		if (top && left) *left = -(*top) * (*aspect);
	}
public:
	Master(int w, int h, float zNear, int zFar)
	{
		W_WIDTH = w;
		W_HEIGHT = h;
		Z_NEAR = zNear;
		Z_FAR = zFar;
		FramesCount = 0;
	}
	~Master()
	{

	}
	void AddFrame(int startX, int startY, int w, int h, float _zFar, float _zNear, Camera *c)
	{
		Pix mas;
		float * depth = new float[w*h];
		
		double _h = 1/(double)W_HEIGHT, _w = 1/(double)W_WIDTH, _z = 1/(double)Z_NEAR, top, left;

		SetProjectionParams(&top, &left);

		glReadPixels(startX, startY, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
		RestoreDepthFromBuffer(depth, w * h, _zFar, _zNear);
	
		double cx = c->X();
		double cy = c->Y();
		double cz = c->Z();

	     glm::vec4 Position;
	     glm::vec4 Transformed;

		int n = 0;
		for(int y = 0; y < W_HEIGHT; y++)
		{
			double _y = -top + 2 * top * _h * y; //[-top; top]
			for(int x = 0; x < W_WIDTH; x++)
			{
				double z_real = depth[W_WIDTH * y + x];
				if (z_real == Z_FAR) continue;
				double zz = abs(z_real * _z);
				double _x = left - 2 * left * _w * x; //[left; -left]
				double x_real = _x * zz;
				double y_real = _y * zz; 
				 
				Position = glm::vec4(x_real, y_real, z_real, 1.0f);
				Transformed = glm::rotateY(Position, (float)(-c->GetAngleY()*D2R));
				Transformed = glm::vec4(Transformed.data[0]+cx, Transformed.data[1]+cy, Transformed.data[2]+cz, 1.0f);
				mas.x = Transformed.data[0];
				mas.y = Transformed.data[1];
				mas.z = Transformed.data[2];
				cloud.push_back(mas);
				n++;
			}
		}
		FramesCount++;

	}
	void DrawFrames()
	{
		glColor3d(1, 1, 1);
		glPointSize(1);
		glPushMatrix();
		glBegin(GL_POINTS);
		for(int i = 0; i<(int)cloud.size(); i++)
		{
			glVertex3d(cloud[i].x, cloud[i].y, cloud[i].z);
		}
		glEnd();
		glPopMatrix();
	}
	int GetFramesCount()
	{
		return FramesCount;
	}
};



#endif
