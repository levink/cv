#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <Windows.h>
#include "camera.h"
#include "mathdata.h"
#include "glut.h"    

#define TAN_30 0.57735026918962576450914878050196
#define CTAN_30 1.7320508075688772935274463415059

const double Z_NEAR = 3.0;
const double Z_FAR = 13;

/* W_WIDTH - OpenGL&CV scene(!) horizontal size (not window) */ 
/* W_HEIGHT - OpenGL&CV scene(!) vertical size */
#define SCREEN_MODE 2
#if SCREEN_MODE == 1
	int W_WIDTH = 640;
	int W_HEIGHT = 480;
#elif SCREEN_MODE == 2
	int W_WIDTH = 320;
	int W_HEIGHT = 240;
#endif

Camera cam1 = Camera(30, 10, 30, -75, 0.4);
Camera cam2 = Camera(0, 0, 0, 180, 1);

long prevTime = GetTickCount();
int fps = 0;
int frameCount = 0;

float arr[4] = {50.0, 10.0, 50.0, 1.0};
float arr2[3] = {50.0, -1.0, 50.0};
float colorBlack[3] = {0.5, 0.3, 0.2};
float colorX[3] = {1,0,1}, colorY[3] = {0,0,1}, colorZ[3] = {1,0,0}, 
	colorA[3] = {1,1,0}, colorTeapot[3] = {0,1,0}, amb[4] = {0,1,0,0};


IplImage *img = NULL;
IplImage *gray = NULL;
IplImage *dst = NULL;
float *depth;
float *mainmas;


double zTest[4] = {0,0,-3,1};
void RestoreDepthFromBuffer(float* buf, int length)
{
	//look at: http://steps3d.narod.ru/tutorials/depth-to-eyez-tutorial.html
	for(int i=0; i < length; i++)
	{
		buf[i] = (Z_FAR * Z_NEAR) / (buf[i] * (Z_FAR - Z_NEAR) - Z_FAR);
	}	
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
	void reshape(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, ((double)w)/h, 1, 100);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display(void)
	{
		glViewport(0, 0, W_WIDTH, W_HEIGHT);
	
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		glRotated(cam1.GetAngleXOZ(), 0, 1, 0);
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

		DrawTeapots();
		DrawWalls();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		
		glPopMatrix();
		
		//glReadPixels(0,0, W_WIDTH, W_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
		//RestoreDepthFromBuffer(depth,1);
	}
	void keybord(unsigned char key, int x, int y)
	{
		//if (key == 'j' || key == 238)
		//{
		//	if(!FullScreen)
		//	{
		//		glutFullScreen();
		//		FullScreen = true;
		//	}
		//	else
		//	{
		//		glutReshapeWindow(800,600);
		//		FullScreen = false;
		//	}

		//}

		//if (key == 'k' || key == 235)
		//{
		//	cout << "[Окно 1] Состояние" << endl;
		//	cout << "Текущие координаты: X: " << (int)w1camera.GetX() << ", Y: " << (int)w1camera.GetY() << ", Z: " << (int)w1camera.GetZ() << ", угол: " << (int)w1camera.GetAngleXOZ() << endl;
		//	cout << "Разрешение окна: " << W_WIDTH << "x" << W_HEIGHT << endl;
		//	cout << "Шаг камеры: " << CAMERA_STEP << endl;
		//	cout << "FPS: " << fps << endl;
		//	cout << endl;
		//}
	
		//if (key == 'w' || key == 246) // Движение вперед
		//{
		//	w1camera.MoveForward(CAMERA_STEP);
		//	MoveForward = true;
		//}

		//if (key == 's' || key == 251) // Движение назад
		//{
		//	w1camera.MoveBack(CAMERA_STEP);
		//	MoveBack = true;
		//}

		//if (key == 'a' || key == 244) // Поворот камеры направо
		//{
		//	w1camera.Rotate(-CAMERA_STEP);
		//	RotateLeft = true;
		//}

		//if (key == 'd' || key == 226) // Поворот камеры налево
		//{
		//	w1camera.Rotate(CAMERA_STEP);
		//	RotateRight = true;
		//}

		//if (key == 'r' || key == 234) // Поворот камеры налево
		//{
		//	w1camera.MoveUp(CAMERA_STEP);
		//	MoveUp = true;
		//}
		//if (key == 'f' || key == 224) // Поворот камеры налево
		//{
		//	w1camera.MoveDown(CAMERA_STEP);
		//	MoveDown = true;
		//}

		//if (key == '=' || key == 61) 
		//{
		//	CAMERA_STEP+=0.02;
		//}
		//if (key == '-' || key == 45) 
		//{
		//	CAMERA_STEP-=0.02;
		//}

	//	cout << (int)key << endl;
		//glutPostRedisplay();
	}
	void keybordUp(unsigned char key, int x, int y)
	{
		///*if (key == 'w' || key == 246) 
		//{
		//	MoveForward = false;
		//}

		//if (key == 's' || key == 251) 
		//{
		//	MoveBack = false;
		//}

		//if (key == 'a' || key == 244) 
		//{
		//	RotateLeft = false;
		//}

		//if (key == 'd' || key == 226)
		//{
		//	RotateRight = false;
		//}

		//if (key == 'r' || key == 234)
		//{
		//	MoveUp = false;
		//}
		//if (key == 'f' || key == 224) 
		//{
		//	MoveDown = false;
		//}*/
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
	void RenderFPS(int value);
	void reshape(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	
		double sw = 1.0;
		double aspect = ((double)h)/w;
		double sh = sw * aspect;

		glFrustum(-sw/2, sw/2, -sh/2, sh/2, Z_NEAR, Z_FAR);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void display(void)
	{
		glViewport(W_WIDTH, 0, W_WIDTH, W_HEIGHT);
		
		glEnable(GL_DEPTH_TEST);
		glPushMatrix();

		glColor3d(1, 1, 1);
		glBegin(GL_QUADS);
		glVertex3d(-0.5, -0.5, zTest[2]);
		glVertex3d(-0.5, 0.5, zTest[2]);
		glVertex3d(0.5, 0.5, zTest[2]);
		glVertex3d(0.5, -0.5, zTest[2]);
		glEnd();

		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
		RenderFPS(fps);
		
		glReadPixels(160, 120, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth); //GL_DEPTH_BITS = 24 bit per pixel
		RestoreDepthFromBuffer(depth,1);
	
		/*
		cout << "realVal=" << zTest[2]  << " "
			 << "restoredVal=" << depth[0] << " "
			 << endl;
		zTest[2] = zTest[2] - 1;
		*/
	}
	void keybord(unsigned char key, int x, int y)
	{
		/*if (key == '=')
		{
			zTest[2] -= 1;
		}
		if (key == '-')
		{
			zTest[2]+= 1;
		}*/
	
		if (key == 'w' || key == 246) // Движение вперед
		{
			zTest[2] += 1;
			//cout << zTest[2] << endl;
			//w2camera.MoveForward(CAMERA_STEP);
		}

		if (key == 's' || key == 251) // Движение назад
		{
			zTest[2] -= 1;
			//cout << zTest[2] << endl;
			//w2camera.MoveBack(CAMERA_STEP);
		}

		//if (key == 'a' || key == 244) // Поворот камеры направо
		//{
		//	w2camera.Rotate(-CAMERA_STEP);
		//}

		//if (key == 'd' || key == 226) // Поворот камеры налево
		//{
		//	w2camera.Rotate(CAMERA_STEP);
		//}

		glutPostRedisplay();
	}
	void RenderFPS(int value){
		glPushMatrix();
		glLoadIdentity();

		char buf[20];
		double d1 = -0.5 + 0.04; //values d1 and d2 depends on RestoredScene::reshape()
		double d2 = d1 * 0.75;

		sprintf(buf,"FPS: %d", value);
		
		glColor3d(0,1,0);
		glRasterPos3f (d1, d2, -3);
		for(int i=0; buf[i]; i++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
		}
		glPopMatrix();
	};
};	


void DrawFPS(IplImage * pic, int _fps, int clusters)
{
	CvPoint pt = cvPoint( W_WIDTH-100, W_HEIGHT-30 );
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 0.7, 0.7, 0, 0, CV_AA);
	char buf[20];
	sprintf(buf,"FPS: %d",_fps);
	cvPutText(pic, buf, pt, &font, CV_RGB(100, 100, 255));
	pt = cvPoint( 10, W_HEIGHT-30 );
	sprintf(buf,"CLUSTERS: %d", clusters);
	cvPutText(pic, buf, pt, &font, CV_RGB(100, 100, 255));
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
	SetWindowPos(hwndFound, 0, 0, 350, 800, 300, 0);
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

	RestoredScene::reshape(W_WIDTH, W_HEIGHT);
	RestoredScene::display();

	glFlush();
	glutSwapBuffers();
};
void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "RUS");
	SetConsole(); //this item does not work!
	
	//create buffers
	CvSize s = cvSize(W_WIDTH, W_HEIGHT);
	img = cvCreateImage(s, IPL_DEPTH_8U, 3);
	gray = cvCreateImage(s, IPL_DEPTH_8U, 1);
	dst = cvCreateImage(s, IPL_DEPTH_8U, 1);
	img->origin = IPL_ORIGIN_BL;
	gray->origin = IPL_ORIGIN_BL;
	dst->origin = IPL_ORIGIN_BL;
	depth = new float[W_WIDTH * W_HEIGHT];
	mainmas = new float[W_WIDTH * W_HEIGHT * 3];

	//init OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(2 * W_WIDTH, W_HEIGHT);

	glutInitWindowPosition(50, 500);
	glutCreateWindow("Восстановление трёхмерной сцены");
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(RestoredScene::keybord);
	
	glutMainLoop();
	
	//release data
	cvReleaseImage(&img);
	cvReleaseImage(&gray);
	cvReleaseImage(&dst);
	delete[] depth;
	delete[] mainmas;
	
	return 0;
}
