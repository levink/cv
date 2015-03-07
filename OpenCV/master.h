#include <vector>
#include <math.h>
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
	float x,y,z,r,g,b;
};


class Master
{
private:
	int FramesCount, W_WIDTH, W_HEIGHT, pointSize;
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
		pointSize = 2;
	}
	~Master()
	{

	}
	void AddFrame(int startX, int startY, int w, int h, float _zFar, float _zNear, Camera *c)
	{
		Pix* mas = new Pix[w*h];
		float * depth = new float[w*h];
		char * color = new char[w*h*3];
		
		double _h = 1/(double)W_HEIGHT, _w = 1/(double)W_WIDTH, _z = 1/(double)Z_NEAR, top, left;

		SetProjectionParams(&top, &left);

		glReadPixels(startX, startY, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
		glReadPixels(startX, startY, w, h, GL_RGB, GL_BYTE, color);
		RestoreDepthFromBuffer(depth, w * h, _zFar, _zNear);
	
		double cx = c->X();
		double cy = c->Y();
		double cz = c->Z();

	     glm::vec4 Position;
	     glm::vec4 Transformed;

		int n = 0;
		int count = cloud.size();
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
				mas[n].x = Transformed.data[0];
				mas[n].y = Transformed.data[1];
				mas[n].z = Transformed.data[2];
				mas[n].r = color[n*3];
				mas[n].g = color[n*3+1];
				mas[n].b = color[n*3+2];
				cloud.push_back(mas[n]);
				n++;
			}
		}

		//float x1=0,y1=0,z1=0,x2,y2,z2; bool pl = true; int ii = 0;
		//if(!cloud.empty())
		//{
		//	for(int i = 0; i<w*h; i++)
		//	{
		//		pl = true; ii = 0;
		//		for(ii = 0; ii<count; ii++)
		//		{
		//			x1 = cloud[ii].x; y1 = cloud[ii].y; z1 = cloud[ii].z;
		//			x2 = mas[i].x; y2 = mas[i].y; z2 = mas[i].z;
		//			if(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1))<1) 
		//			{
		//				pl = false;
		//				break;
		//			}
		//		}
		//		if(pl) cloud.push_back(mas[ii]);
		//	}
		//	count = cloud.size();
		//}
		//else
		//{
		//	for(int i = 0; i<w*h; i++)
		//	{
		//		cloud.push_back(mas[i]);
		//	}
		//	count = cloud.size();
		//}

		FramesCount++;
	}
	void DrawFrames()
	{
		glPointSize(pointSize);
		glPushMatrix();
		glBegin(GL_POINTS);
		for(int i = 0; i<(int)cloud.size(); i++)
		{
			glColor3b(cloud[i].r, cloud[i].g, cloud[i].b); 
			glVertex3d(cloud[i].x, cloud[i].y, cloud[i].z);
		}
		glEnd();
		glPopMatrix();
	}
	
	float GetUsedMemoryMB()
	{
		return (float)(sizeof(cloud)*(int)cloud.size())/1048576;
	}

	int GetFramesCount()
	{
		return FramesCount;
	}
	void AddPointSize(int size)
	{
		pointSize += size;
		if(pointSize<1) pointSize = 1;
	}
};



#endif
