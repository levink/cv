#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <Windows.h>
#include "glut.h"    
#include "camera.h"
#include "master.h"

const double Z_NEAR = 0.1;
const double Z_FAR = 150;
const double VIEW_ANGLE = 60; 

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

Camera cam1 = Camera(0, 10, 0, 2);
Camera cam2 = Camera(0, 10, 0, 1);
int activeScene = 0;

long prevTime = GetTickCount();

float arr[4] = {50.0, 10.0, 50.0, 1.0};
float arr2[3] = {50.0, -1.0, 50.0};
float colorBlack[3] = {0.5, 0.3, 0.2};
float colorX[3] = {1,0,1}, colorY[3] = {0,0,1}, colorZ[3] = {1,0,0}, 
	colorA[3] = {1,1,0}, colorTeapot[3] = {0,1,0}, amb[4] = {0,1,0,0};

int MODE_FREE = 0;
int MODE_CENTER_ROTATE = 1;
int MODE_STEREO = 2;

int fps = 0, frameCount = 0, window1, window2, scrCount = 1;
int renderMode = MODE_FREE;
int mx = 0, my = 0;

Master* master = NULL;
bool createFrame = false;
int ShowText = 0;
IplImage* img1, *img2;

void SetProjectionParams(double *top, double *left, double viewAngle, double *aspect = NULL){
	double tmp_a = 0;
	if (!aspect) aspect = &tmp_a;
	*aspect = ((double)W_WIDTH) / W_HEIGHT;
	if (top) * top = Z_NEAR * tan((viewAngle/2)*D2R);
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
		SetProjectionParams(&top, &left, VIEW_ANGLE, &aspect);
		glFrustum(left, -left, -top, top, Z_NEAR, Z_FAR);
		//gluPerspective(VIEW_ANGLE, aspect, Z_NEAR, Z_FAR);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display(void)
	{
		glViewport(0, W_HEIGHT, W_WIDTH, W_HEIGHT);
	
		glPushMatrix();

		if(renderMode == MODE_FREE)
		{
			glRotated(cam1.GetAngleZ(), 1, 0, 0);
			glRotated(cam1.GetAngleY(), 0, 1, 0);
			glTranslated(-cam1.X(), -cam1.Y(), -cam1.Z());
		}
		if(renderMode == MODE_CENTER_ROTATE)
		{
			glTranslated(0, -10, -30);
			glRotated(cam1.GetAngleY(), 0, 1, 0);
		}
	
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
		GLfloat pos[4] = {10, 10,10, 0.5};
		GLfloat ambient[] = {0.2, 0.2, 0.2, 1}; 
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

		glPushMatrix();
		glRotated(cam2.GetAngleY(), 0,1,0);
		glTranslated(pos[0], pos[1], pos[2]);
		//glutSolidSphere(1,10,10);
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		glPopMatrix();

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
		if (num == 1)
		{
			glRotated(-90, 1, 0, 0);
			glutSolidCone(4, 4, 10, 10);
		}
		else if (num == 2) glutSolidTorus(1,2,50,50);
		else if (num == 3) glutWireSphere(3,30,30);
		else if (num == 4) glutSolidTeapot(3);
		else if (num == 5) glutSolidDodecahedron();
		else if (num == 6) glutWireCube(3);
		else if (num == 7) glutWireTeapot(4);
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
		glRotated(90, 0, 1, 0);
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
		glPushMatrix();
		glTranslated(15, 13, 25);
		glRotated(-90, 0, 1, 0);
		glutSolidTorus(3, 6, 100, 100);
		glPopMatrix();

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorZ);
		glPushMatrix();
		glTranslated(20, 13, 75);
		glRotated(-90, 0, 1, 0);
		glutSolidTorus(3, 6, 100, 100);
		glPopMatrix();

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorA);
		glPushMatrix();
		glTranslated(1, 13, 50);
		glRotated(-90, 0, 0, 1);
		glRotated(-90, 1, 0, 0);
		glutSolidCone(10, 10, 50, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslated(2, 13, 10);
		glScaled(1, 15, 1);
		glutSolidCube(3);
		glPopMatrix();

		glPushMatrix();
		glTranslated(2, 13, 90);
		glScaled(1, 15, 1);
		glutSolidCube(3);
		glPopMatrix();



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
		
		if(renderMode == MODE_CENTER_ROTATE)
		{
			glPushMatrix();
			glTranslated(50,10,50);
			glutSolidSphere(4,50,50);
			glPopMatrix();
		}

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
	void reshape(int _w, int _h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		double top, left, aspect;
		SetProjectionParams(&top, &left, VIEW_ANGLE, &aspect);
		//glFrustum(left, -left, -top, top, Z_NEAR, Z_FAR);
		gluPerspective(VIEW_ANGLE, aspect, Z_NEAR, Z_FAR);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display(void)
	{
		glViewport(W_WIDTH, 0, W_WIDTH, W_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();
		glRotated(cam2.GetAngleZ(), 1, 0, 0);
		glRotated(cam2.GetAngleY(), 0, 1, 0);
		glTranslated(-cam2.X(), -cam2.Y(), -cam2.Z());

		master->DrawFrames();

		glPopMatrix();
	}
};	

void DrawFrame(int sceneNumber){
		if(sceneNumber == 0) 	glViewport(0, W_HEIGHT, 2*W_WIDTH, 2*W_HEIGHT);
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

		glColor3d(1,1,1);
		glPointSize(3);
		glBegin(GL_POINTS);
		glVertex2d(1/2, 1/2);
		glEnd();
}
void RenderFPS(int value)
{		
	glViewport(0, 0, 2 * W_WIDTH, 2 * W_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 2*W_WIDTH, 0, 2*W_HEIGHT, 0,1);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	char *buf;
	buf = new char[100];


	glColor3d(0,1,0);
	if(renderMode == MODE_FREE)
	sprintf(buf,"FPS: %d, %dx%d, mode: Free", value, W_WIDTH, W_HEIGHT);

	glRasterPos3f (10, 2*W_HEIGHT-25, 0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);

	sprintf(buf,"X:%d, Y: %d, Z: %d", (int)cam1.X(), (int)cam1.Y(), (int)cam1.Z());
	glRasterPos3f (10, W_HEIGHT+10, 0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);

	sprintf(buf,"X:%d, Y: %d, Z: %d", (int)cam2.X(), (int)cam2.Y(), (int)cam2.Z());
	glRasterPos3f (W_WIDTH+10, 10,0);
	for(int i=0; buf[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);

	sprintf(buf,"Created frames: %d", master->GetFramesCount());
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
	W_HEIGHT = h/2;
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
		master->AddFrame(0, W_HEIGHT, W_WIDTH, 2*W_HEIGHT, Z_FAR, Z_NEAR, &cam1);
		createFrame = false;
	}
	
	RestoredScene::reshape(W_WIDTH, W_HEIGHT);
	RestoredScene::display();
	
	if(ShowText)
	{
		char *buf, *buff;
		buf = new char[20];
		buff = new char[100];
		CvSize size; size.height=W_HEIGHT; size.width=W_WIDTH;
		img1 = cvCreateImage(size, IPL_DEPTH_8U, 3);
		img1->origin = IPL_ORIGIN_BL;
		glReadPixels(0, W_HEIGHT, W_WIDTH, 2*W_HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, img1->imageData);
		time_t now = time(NULL);
		strftime(buf, 100, "%H-%M-%S.jpg", localtime(&now));
		sprintf(buff, "(%d) %s", scrCount, buf);
		cvSaveImage(buff, img1);
		cout << "Скриншот сохранён (" << buff << ")" << endl;
		ShowText = 0; scrCount++;
	}
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
	if(renderMode == MODE_FREE)
	{
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
			c->MoveLeft(1);
		}
		if (key == 'd' || key == 226)
		{
			c->MoveRight(1);
		}
		if (key == 'q' || key == 233)
		{
			c->Rotate(-1, 0);
		//	cout << c->GetAngleY() << endl;
		}
		if (key == 'e' || key == 243)
		{
			c->Rotate(1, 0);
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
	}
	if (key == 'm' || key == 252)
	{
		renderMode++;
		if(renderMode > 1) renderMode-=2;
	}
	if (key == 'x' || key == 247)
	{
		createFrame = true;
	}
	if (key == '1')
	{
		ShowText = 1;
	}
	if (key == 61)
	{
		master->AddPointSize(1);
	}
	if (key == 45)
	{
		master->AddPointSize(-1);
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
	cam1.Rotate(-dx, -dy);
	if(activeScene == 1)
	cam2.Rotate(-dx, -dy);

	mx = x;
	my = y;
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "RUS");
	SetConsole();
	cout << "Мышь + ЛКМ - поворот камеры, WASD(ЦФЫВ) - передвижение камеры" << endl 
		 << "X(Ч) - построение кадра" << endl
		 << "M(Ь) - смена режима управления" << endl
		 << "+- - изменение размера точек" << endl
		 << "1 - сделать скриншот (сохраняется в папку с программой)" << endl;
	//create data
	master = new Master(W_WIDTH, W_HEIGHT, Z_NEAR, Z_FAR, VIEW_ANGLE);
	//init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(2*W_WIDTH, 2*W_HEIGHT);

	glutInitWindowPosition(0, 0);
	window1 = glutCreateWindow("Восстановление трёхмерной сцены");
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