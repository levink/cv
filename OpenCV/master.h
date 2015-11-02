#include <vector>
#include <math.h>
#include "glut.h"
#include "camera.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include <opencv2\legacy\legacy.hpp>

#ifndef __MASTER_H
#define __MASTER_H

struct Pix
{
	float x,y,z,r,g,b;
	Pix operator =(Pix &a)
	{
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		this->r = a.r;
		this->g = a.g;
		this->b = a.b;
	}
};


class Cloud
{
private:
public:
	int x1, y1, z1, x2, y2, z2;
	std::vector<Pix>el;
	Cloud()
	{

	}

};


bool operator ==(Pix &a, Pix &b)
{
	if ((a.x == b.x) && (a.y == b.y) && (a.z == b.z))
		return true;
	else
		return false;
}


class Master
{
private:
	int FramesCount, W_WIDTH, W_HEIGHT, pointSize;
	float memory;
	double Z_NEAR, Z_FAR, viewAngle;
	Cloud stor[1000];


	void RestoreDepthFromBuffer(float* buf, int length, float _zFar, float _zNear)
	{
		for(int i=0; i < length; i++)
		{
			buf[i] = (_zFar * _zNear) / (buf[i] * (_zFar - _zNear) - _zFar);
		}	
	}
	void SetProjectionParams(double *top, double *left, double viewAngle, double *aspect = NULL){
		double tmp_a = 0;
		if (!aspect) aspect = &tmp_a;
		*aspect = ((double)W_WIDTH) / W_HEIGHT;
		if (top) * top = Z_NEAR * tan((viewAngle/2)*D2R);
		if (top && left) *left = -(*top) * (*aspect);
	}
public:
	Master(int w, int h, float zNear, int zFar, double viewAng)
	{
		W_WIDTH = w;
		W_HEIGHT = h;
		Z_NEAR = zNear;
		Z_FAR = zFar;
		viewAngle = viewAng;
		FramesCount = 0;
		pointSize = 2;
		memory = 0;
		int i = 0;
		for(int z = -50; z<=40; z+=10)
		{
			for(int y = -50; y<=40; y+=10)
			{
				for(int x = -50; x<=40; x+=10)
				{
					stor[i].x1 = x;
					stor[i].y1 = y;
					stor[i].z1 = z;
					stor[i].x2 = x+10;
					stor[i].y2 = y+10;
					stor[i].z2 = z+10;
					i++;
				}
			}
		}
	}
	~Master()
	{

	}
	void AddFrameGLDepth(int startX, int startY, int w, int h, float _zFar, float _zNear, Camera *c)
	{
		Pix mas;
		float * depth = new float[w*h];
		char * color = new char[w*h*3];
		
		double _h = 1/(double)W_HEIGHT, _w = 1/(double)W_WIDTH, _z = 1/(double)Z_NEAR, top, left;

		SetProjectionParams(&top, &left, viewAngle);

		glReadPixels(startX, startY, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
		glReadPixels(startX, startY, w, h, GL_RGB, GL_BYTE, color);
		RestoreDepthFromBuffer(depth, w * h, _zFar, _zNear);
	
		double cx = c->X();
		double cy = c->Y();
		double cz = c->Z();

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
				 
				Transformed = glm::vec4(x_real, y_real, z_real, 1.0f);
				Transformed = glm::rotateX(Transformed, (float)(-c->GetAngleZ()*D2R));
				Transformed = glm::rotateY(Transformed, (float)(-c->GetAngleY()*D2R));
				Transformed = glm::vec4(Transformed[0]+cx, Transformed[1]+cy, Transformed[2]+cz, 1.0f);

				float X = Transformed[0] , Y = Transformed[1], Z = Transformed[2];
				int f = 0;
				for(int i = 0; i<1000; i++)
				{
					int a = stor[i].el.size();

					if ((((stor[i].x1 <= (int)X) && (stor[i].x2 >= (int)X)) && ((stor[i].y1 <= (int)Y) && (stor[i].y2 >= (int)Y)) && ((stor[i].z1 <= (int)Z) && (stor[i].z2 >= (int)Z))))
					{
						bool pl = true;
						for(int ii = 0; ii<a; ii++)
						{
							if(((float)(((stor[i].el[ii].x-X)*(stor[i].el[ii].x-X)))+(float)(((stor[i].el[ii].y-Y)*(stor[i].el[ii].y-Y)))+(float)(((stor[i].el[ii].z-Z)*(stor[i].el[ii].z-Z)))<=(float)(0.1))  )
							{
								pl = false;
								break;
							}

						}
						if(pl)
						{
							mas.x = X; mas.y = Y; mas.z = Z;
							mas.r = color[n*3]; mas.g = color[n*3+1]; mas.b = color[n*3+2];
							stor[i].el.push_back(mas);
							f++;
						}
					}
				//	memory += (float)((sizeof(Pix)*f / 1048576));
				}
				n++;
			}
		}


		FramesCount++;
		std::cout << "Кадр занесён на карту (" << FramesCount << ")" << std::endl;
	}
	void AddFrameCameraDepth(bool fastState = 1)
	{
		CvSize size; size.width=W_WIDTH; size.height=W_HEIGHT;
		IplImage* srcLeft  = cvCreateImage(size, IPL_DEPTH_8U, 3); //srcLeft->origin = IPL_ORIGIN_BL; 
		IplImage* srcRight = cvCreateImage(size, IPL_DEPTH_8U, 3); //srcRight->origin = IPL_ORIGIN_BL;
		IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1); //leftImage->origin = IPL_ORIGIN_BL;
		IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1); //rightImage->origin = IPL_ORIGIN_BL;
		IplImage *img = cvCreateImage(size, IPL_DEPTH_8U, 1); //img->origin = IPL_ORIGIN_BL; 
		glReadPixels(0, W_HEIGHT, W_WIDTH, W_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, srcLeft->imageData);
		glReadPixels(W_WIDTH, W_HEIGHT, W_WIDTH, W_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, srcRight->imageData);
		cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
		cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);
		CvMat* disparity_left = cvCreateMat( size.height, size.width, CV_16S );
		CvMat* disparity_right = cvCreateMat( size.height, size.width, CV_16S );
		CvStereoGCState* state = cvCreateStereoGCState( 64, 2 );
		cvFindStereoCorrespondenceGC( leftImage, rightImage, disparity_left, disparity_right, state, 0 );
		cvReleaseStereoGCState( &state );
		/*CvStereoBMState* state = cvCreateStereoBMState(0, 32);
		cvFindStereoCorrespondenceBM(leftImage, rightImage, disparity_left, state);
		cvReleaseStereoBMState(&state);*/
		CvMat* disparity_left_visual = cvCreateMat( size.height, size.width, CV_8U );
		cvConvertScale( disparity_left, disparity_left_visual, -16 );
		cvGetImage(disparity_left_visual, img);

		cvShowImage("1", srcLeft);
		cvShowImage("2", img);

	}
	void DrawFrames()
	{
		glPointSize(pointSize);
		glPushMatrix();
		glBegin(GL_POINTS);
		for(int i = 0; i<1000; i++)
		{
			for(int ii = 0; ii<(int)stor[i].el.size(); ii++)
			{
				glColor3b(stor[i].el[ii].r, stor[i].el[ii].g, stor[i].el[ii].b); 
				glVertex3d(stor[i].el[ii].x, stor[i].el[ii].y, stor[i].el[ii].z);
			}
		}
		glEnd();
		glPopMatrix();
	}
	float GetUsedMemoryMB()
	{
		return memory;
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
