#include "glut.h"
#include "camera.h"

#ifndef __MASTER_H
#define __MASTER_H

struct Frame
{
	float *depth;
	double camOffset[3];
};

class Master
{
	int _length;
	double _zNear;
	double _zFar;
	void RestoreDepthFromBuffer(float* buf, int length)
	{
		// http://steps3d.narod.ru/tutorials/depth-to-eyez-tutorial.html
		for(int i=0; i < length; i++)
		{
			buf[i] = (_zFar * _zNear) / (buf[i] * (_zFar - _zNear) - _zFar);
		}	
	}

public:
	int fCount;
	Frame *f;
	Master(int maxFrames, double zNear, double zFar)
	{
		_length = maxFrames;
		f = new Frame[maxFrames];
		fCount = 0;
		_zNear = zNear;
		_zFar = zFar;
	}
	~Master()
	{
		for(int i = 0; i < fCount; i++)
		{
			delete[] f[i].depth;
		}
		delete[] f;
	}
	
	void CreateFrame(GLint startX, GLint startY, GLsizei w, GLsizei h, Camera *c);
	const int Capacity()
	{
		return _length;
	}
};

void Master::CreateFrame(GLint startX, GLint startY, GLsizei w, GLsizei h, Camera *c)
{
	if (fCount == _length) return;
		
	Frame* fr = &f[fCount];
		
	// 1. Reading to the p->depth at once!
	// 2. There are static frame size here
	fr->depth = new float[w * h];
	glReadPixels(startX, startY, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, fr->depth);
	RestoreDepthFromBuffer(fr->depth, w * h);
		
	fr->camOffset[0] = c->X();
	fr->camOffset[1] = c->Y();
	fr->camOffset[2] = c->Z();

	fCount++;
}


#endif
