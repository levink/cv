#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <Windows.h>
#include "glut.h"    
#include "camera.h"
#include "master.h"
#include "mathdata.h"

//GL_DEPTH_BITS = 24 bit per pixel
const double Z_NEAR = 1;
const double Z_FAR = 100;
const double VIEW_ANGLE = 60; 
const double TAN_30 = 0.5773502692; // (!) a half VIEW_ANGLE

/* W_WIDTH - OpenGL&CV scene(!) horizontal size (not window) */ 
/* W_HEIGHT - OpenGL&CV scene(!) vertical size */
#define SCREEN_MODE 1

#pragma region Defines

#if SCREEN_MODE == 1
	int W_WIDTH = 640;
	int W_HEIGHT = 480;
	int CONSOLEY = 519;
#elif SCREEN_MODE == 2
	int W_WIDTH = 320;
	int W_HEIGHT = 240;
	int CONSOLEY = 279;
#endif
#ifndef SCREEN_MODE
#error
#endif

#pragma endregion

Camera cam1 = Camera(0, 10, 0, 0, 2);
Camera cam2 = Camera(0, 10, 0, 0, 1);
int activeScene = 0;

long prevTime = GetTickCount();
int fps = 0, frameCount = 0;

float arr[4] = {50.0, 10.0, 50.0, 1.0};
float arr2[3] = {50.0, -1.0, 50.0};
float colorBlack[3] = {0.5, 0.3, 0.2};
float colorX[3] = {1,0,1}, colorY[3] = {0,0,1}, colorZ[3] = {1,0,0}, 
	colorA[3] = {1,1,0}, colorTeapot[3] = {0,1,0}, amb[4] = {0,1,0,0};

int mx = 0, my = 0;

Master* master = NULL;
bool createFrame = false;

void SetProjectionParams(double *top, double *left, double *aspect = NULL){
	double tmp_a = 0;
	if (!aspect) aspect = &tmp_a;
	*aspect = ((double)W_WIDTH) / W_HEIGHT;
	if (top) * top = Z_NEAR * TAN_30;
	if (top && left) *left = -(*top) * (*aspect);
}

using namespace std;

namespace SourceScene {
	int teapotAngle = 0;
	void DrawTeapots();
	void DrawWalls();

	void idle(void)
	{
		if (teapotAngle > 360) teapotAngle = 0;
		else teapotAngle += 1;
		glutPostRedisplay();
	}
	void reshape(int _w, int _h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		double top, left, aspect;
		SetProjectionParams(&top, &left, &aspect);
		glFrustum(left, -left, -top, top, Z_NEAR, Z_FAR);
		//gluPerspective(VIEW_ANGLE, aspect, Z_NEAR, Z_FAR);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display(void)
	{
		glViewport(0, 0, W_WIDTH, W_HEIGHT);
	
		glPushMatrix();
		glRotated(cam1.GetAngleZ(), 1, 0, 0);
		glRotated(cam1.GetAngleY(), 0, 1, 0);
		glTranslated(-cam1.X(), -cam1.Y(), -cam1.Z());
	
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
		GLfloat pos[4] = {25.0, 20.0, 25.0, 0.6};
		GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0}; 
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);

		glPushMatrix();
		glRotated(90,0,1,0);
		glTranslated(-50, 0, -50);

		DrawTeapots();
		DrawWalls();
		
		glPopMatrix();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		glPopMatrix();
	}
	
	void RenderFigure(int num){
		if (num == 1) glutSolidCone(4,4,10,10);
		else if (num == 2) glutSolidTorus(1,2,50,50);
		else if (num == 3) glutWireSphere(3,100,100);
		else if (num == 4) glutSolidTeapot(3);
		else if (num == 5) glutSolidDodecahedron();
		else if (num == 6) glutWireCube(3);
		else if (num == 7) glutWireTeapot(3);
	};
	void DrawSolidCube(double x, double y, double z){
		glPushMatrix();
		glTranslated(x, y, z);
		glutSolidCube(10);
		glPopMatrix();
	}
	void DrawFigure(double x, double y, double z, double a, int num)
	{
		glPushMatrix();
		glTranslated(x,y,z);
		glRotated(a, 1, 0, 0);
		RenderFigure(num);
		glPopMatrix();
	}
	void DrawTeapots()
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorTeapot);
		DrawFigure(95, 7, 5, -90 + teapotAngle, 1);  
		DrawFigure(95, 10, 20, 90 + teapotAngle, 2); 
		DrawFigure(95, 10, 35, teapotAngle, 3); 
		DrawFigure(95, 10, 50, -teapotAngle, 7); 
		DrawFigure(95, 9, 65, teapotAngle, 4); 
		DrawFigure(95, 9, 80, teapotAngle, 5); 
		DrawFigure(95, 9, 95, teapotAngle, 6); 
		

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorA);
		DrawSolidCube(95,2,5);
		DrawSolidCube(95,2,20);
		DrawSolidCube(95,2,35);
		DrawSolidCube(95,2,50);
		DrawSolidCube(95,2,65);
		DrawSolidCube(95,2,80);
		DrawSolidCube(95,2,95);
	}
	void DrawQuad(GLfloat vert[8][3], int* ind, GLfloat *n){
		glNormal3fv(n);
		glVertex3fv(vert[ind[0]]);
		glVertex3fv(vert[ind[1]]);
		glVertex3fv(vert[ind[2]]);
		glVertex3fv(vert[ind[3]]);
	}
	void DrawWalls()
	{
		GLfloat vertex[8][3]={
			{0,-1, 0},		//0
			{0,-1, 100},	//1
			{0, 30, 0},		//2
			{0, 30, 100},	//3
			{100,-1, 0},	//4
			{100,-1, 100},	//5
			{100, 30, 0},	//6
			{100, 30, 100},	//7
		};
		GLfloat norm[][3] = {
			{0, 1, 0},
			{0, 0, -1},
			{0, 0, 1},
			{0, 0, -1},
			{1, 0, 0},
			{-1, 0, 0}
		};
		int order[][4] = {
			{5, 4, 0, 1},
			{2, 3, 7, 6},
			{0, 4, 6, 2},
			{1, 5, 7, 3},
			{1, 0, 2, 3},
			{5, 4, 6, 7}
		};
		

		glBegin(GL_QUADS); 

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorY);
		DrawQuad(vertex, order[0], norm[0]); //floor	

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorZ);
		DrawQuad(vertex, order[1], norm[1]); //top
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorX);	
		DrawQuad(vertex, order[2], norm[2]); //near
		DrawQuad(vertex, order[3], norm[3]); //far
		DrawQuad(vertex, order[4], norm[4]); //left
		DrawQuad(vertex, order[5], norm[5]); //right
		
		glEnd();
	}
};

namespace RestoredScene
{	
	void DrawFrames();
	
	void reshape(int _w, int _h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		double top, left, aspect;
		SetProjectionParams(&top, &left, &aspect);
		//glFrustum(left, -left, -top, top, Z_NEAR, Z_FAR);
		gluPerspective(VIEW_ANGLE, aspect, 1, Z_FAR);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display(void)
	{
		glViewport(W_WIDTH, 0, W_WIDTH, W_HEIGHT);
		
		glPushMatrix();
		glRotated(cam2.GetAngleZ(), 1, 0, 0);
		glRotated(cam2.GetAngleY(), 0, 1, 0);
		glTranslated(-cam2.X(), -cam2.Y(), -cam2.Z());

		DrawFrames();

		glPopMatrix();
	}
	void DrawFrames()
	{
		double top, left;
		SetProjectionParams(&top, &left);
		double _h = 1 / (double) W_HEIGHT;
		double _w = 1 / (double) W_WIDTH;
		double _z = 1 / (double) Z_NEAR;

		glColor3d(1, 1, 1);
		glPointSize(2);

		for(int fr=0; fr < master->fCount; fr++)
		{
			Frame *f = &master->f[fr];
			glPushMatrix();
			double cx = f->camOffset[0];
			double cy = f->camOffset[1];
			double cz = f->camOffset[2];
			glTranslated(cx, cy, cz); 
			glRotated(-f->hAngle, 0,1,0);
			double y[3][3] = {
			{ cos(f->hAngle*D2R), 0, cos(f->hAngle*D2R)},
			{ 0, 1, 0 },
			{ -sin(f->hAngle*D2R), 0, cos(f->hAngle*D2R)} 
			};

			Matrix RotateMatrix = Matrix(y);
				
			glBegin(GL_POINTS);
			for(int y = 0; y < W_HEIGHT; y++)
			{
				double _y = -top + 2 * top * _h * y; //[-top; top]
				for(int x = 0; x < W_WIDTH; x++)
				{
					double z_real = f -> depth[W_WIDTH * y + x];
					if (z_real == Z_FAR) continue;
					double zz = abs(z_real * _z);
					double _x = left - 2 * left * _w * x; //[left; -left]
					double x_real = _x * zz;
					double y_real = _y * zz;

					Vector3d xyz;
					xyz.e[0] = x_real; xyz.e[1] = y_real; xyz.e[2] = z_real; 
					xyz = RotateMatrix*xyz;
					glVertex3d(xyz.e[0], xyz.e[1], xyz.e[2]);
				}
			}
			glEnd();
			glPopMatrix();
		}
		
	}
};	

void DrawFrame(int sceneNumber){
		if(sceneNumber == 0) 	glViewport(0, 0, W_WIDTH, W_HEIGHT);
		else if (sceneNumber == 1) glViewport(W_WIDTH, 0, W_WIDTH, W_HEIGHT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1,1, -1,1, 0,1);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3d(0, 1, 0);
		glLineWidth(2);

		glBegin(GL_LINE_LOOP);
		glVertex2d(-1, -1);
		glVertex2d(-1,  1);
		glVertex2d( 1,  1);
		glVertex2d( 1, -1);
		glEnd();
}
void RenderFPS(int value)
{		
	glViewport(0, 0, 2 * W_WIDTH, W_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 2*W_WIDTH, 0, W_HEIGHT, 0,1);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	char *buf;
	buf = new char[100];


	glColor3d(0,1,0);
	sprintf(buf,"FPS: %d, %dx%d", value, W_WIDTH, W_HEIGHT);
	glRasterPos3f (10, W_HEIGHT-25, 0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);

	sprintf(buf,"X:%d, Y: %d, Z: %d", (int)cam1.X(), (int)cam1.Y(), (int)cam1.Z());
	glRasterPos3f (10, 10, 0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);

	sprintf(buf,"X:%d, Y: %d, Z: %d", (int)cam2.X(), (int)cam2.Y(), (int)cam2.Z());
	glRasterPos3f (W_WIDTH+10, 10,0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);

	sprintf(buf,"Total frames: %d / %d", master->fCount, master->Capacity());
	glRasterPos3f (2 * W_WIDTH - 150, 10, 0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);


	delete[]buf;
	glColor3d(0,1,0);
}
void SetConsole()
{
    #define MY_BUFSIZE 1024 // Buffer size for console window titles.
    HWND hwndFound;         // This is what is returned to the caller.
    char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
    char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
    wsprintf(pszNewWindowTitle,"%d/%d",
                GetTickCount(),
                GetCurrentProcessId());
    SetConsoleTitle(pszNewWindowTitle);
    Sleep(40);
    hwndFound=FindWindow(NULL, pszNewWindowTitle);

    SetConsoleTitle(pszOldWindowTitle);
	SetWindowPos(hwndFound, 0, 0, CONSOLEY, 800, 300, 0);
}

void reshape(int w, int h){
	W_WIDTH = w/2;
	W_HEIGHT = h;
	glutPostRedisplay();
};
void display(void){

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	long curTime = GetTickCount();
	long dt = curTime - prevTime;
	if(dt > 1000) 
	{
		fps = frameCount;
		frameCount = 0; 
		prevTime = curTime;
	} 
	else frameCount++;
		
	SourceScene::reshape(W_WIDTH, W_HEIGHT); 
	SourceScene::display();
	
	if (createFrame)
	{
		master->CreateFrame(0, 0, W_WIDTH, W_HEIGHT, &cam1);
		createFrame = false;
	}
	
	RestoredScene::reshape(W_WIDTH, W_HEIGHT);
	RestoredScene::display();
	
	DrawFrame(activeScene);
	RenderFPS(fps);

	glFlush();
	glutSwapBuffers();
};
void idle(void)
{
	glutPostRedisplay();
}
void keybord(unsigned char key, int x, int y){
		
	Camera *c = activeScene == 0 ? &cam1 : &cam2;

	if (key == 'w' || key == 246)
	{
		c->MoveForward();
	}
	if (key == 's' || key == 251)
	{
		c->MoveBack();
	}	
	if (key == 'a' || key == 244)
	{
		c->MoveLeft();
	}
	if (key == 'd' || key == 226)
	{
		c->MoveRight();
	}
	if (key == 'q' || key == 233)
	{
		c->Rotate(-10, 0);
	//	cout << c->GetAngleY() << endl;
	}
	if (key == 'e' || key == 243)
	{
		c->Rotate(10, 0);
	//	cout << c->GetAngleY() << endl;
	}
	if (key == 'r' || key == 234)
	{
		c->MoveUp(0.7);
	}
	if (key == 'f' || key == 224)
	{
		c->MoveDown(0.7);
	}
	if (key == 'x' || key == 247)
	{
		createFrame = true;
	}
}
void click(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		activeScene = x < W_WIDTH ? 0 : 1;
	}
	mx = x;
	my = y;
}
void motion(int x, int y)
{
	int dx = mx - x;
	int dy = my - y;
	if(activeScene == 0)
	cam1.Rotate(-dx,0/* -dy*/);
	if(activeScene == 1)
	cam2.Rotate(-dx, -dy);

	mx = x;
	my = y;
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "RUS");
	SetConsole();
	
	//create data
	master = new Master(10, Z_NEAR, Z_FAR);

	//init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(2 * W_WIDTH, W_HEIGHT);

	glutInitWindowPosition(0, 00);
	glutCreateWindow("Восстановление трёхмерной сцены");
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keybord);
	glutMouseFunc(click);
	glutMotionFunc(motion);
	glutMainLoop();

	
	//release data
	delete master;
	return 0;
}