//Подключение необходимых библиотек
#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "scene.h"
#include "glut.h"          

#define KMEANS_CLUSTERS 2
#define CAMERA_STEP 1

void kmeans(
            int  dim,		                  // dimension of data 
            float *X,                        // pointer to data
            int   n,                          // number of elements
            int   k,                          // number of clusters
            float *cluster_centroid,         // initial cluster centroids
            int   *cluster_assignment_final   // output
           );
void DrawFPS(IplImage * pic, int _fps, int clusters);
void DrawTeapots();
void DrawWalls();

const int SCREEN_WIDTH = 800;  // Разрешение окона OpenGL&CV по горизонтали
const int SCREEN_HEIGHT = 600;  // Разрешение окона OpenGL&CV по вертикали

using namespace std; //Определение пространства имён

float *depth, *mainmas;
double angle = 0 /*Угол поворота камеры XOZ*/, step = 0 /*Шаг камеры*/, camLookAt[3] = {0,0,0}/*Направление взгляда камеры*/;
int w1 = -1, w2 = -1, last = 0, fpstmp = 0/*Техническая переменная для определения FPS*/, fps = 0/*Количество кадров в секунду*/;
Camera MyCam; // Главный класс, отвечающий за управление камерой
Camera2 MyCam2;
char *output, *color;
long int t1 = 0;
int *out;

GLfloat arr[4] = {50.0,30.0,50.0,1.0};
GLfloat arr2[3] = {50.0,-1.0,50.0};
GLfloat colorX[3] = {1,0,1};
GLfloat colorY[3] = {0,0,1};
GLfloat colorZ[3] = {1,0,0};
GLfloat colorA[3] = {1,1,0};
GLfloat colorBlack[3] = {0.5,0.3,0.2};
GLfloat amb[4] = {0,1,0,0};
GLfloat colorTeapot[3] = {0,1,0};


CvSize Size;
IplImage *img = 0/*Главное изображение, использующиеся OpenCV*/;


void keybord(unsigned char key, int x, int y)
{
	if (key == 'j' || key == 238)
	{
	
	}
	
	if (key == 'w' || key == 246) // Движение вперед
	{
		MyCam.MoveForward(CAMERA_STEP);
	}

	if (key == 's' || key == 251) // Движение назад
	{
		MyCam.MoveBack(CAMERA_STEP);
	}

	if (key == 'a' || key == 244) // Поворот камеры направо
	{
		MyCam.Rotate(-CAMERA_STEP);
	}

	if (key == 'd' || key == 226) // Поворот камеры налево
	{
		MyCam.Rotate(CAMERA_STEP);
	}

	glutPostRedisplay();
}


void keybord2(unsigned char key, int x, int y)
{
	if (key == 'j' || key == 238)
	{
	
	}
	
	if (key == 'w' || key == 246) // Движение вперед
	{
		MyCam2.MoveForward(CAMERA_STEP);
	}

	if (key == 's' || key == 251) // Движение назад
	{
		MyCam2.MoveBack(CAMERA_STEP);
	}

	if (key == 'a' || key == 244) // Поворот камеры направо
	{
		MyCam2.Rotate(-CAMERA_STEP);
	}

	if (key == 'd' || key == 226) // Поворот камеры налево
	{
		MyCam2.Rotate(CAMERA_STEP);
	}

	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
	//std::cout << "Mouse X: " << x << ", Y: " << y << std::endl;
}

void idle(void)
{
	// Команды поворота чайника 
	angle+=1;
	if(angle>360) angle = 0;

	glutPostRedisplay();
}

void display2(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_SPECULAR, amb);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_POSITION, arr);
	gluPerspective(60,SCREEN_WIDTH/SCREEN_HEIGHT,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glTranslated(0,-2.5,0);
//	glRotated(150,0,1,0);
//	glRotated(angle,0,1,0);
	glRotated(180,0,1,0);
	glRotated(MyCam2.GetAngleXOZ(),0,1,0);
	glTranslated(-MyCam2.GetX(), -2.5, -MyCam2.GetZ());
	glColor3d(1,1,1);
	glBegin(GL_POINTS);
			for(int c = 0; c<SCREEN_WIDTH*SCREEN_HEIGHT; c++)
			{
				glVertex3d(mainmas[c*3]*0.01, mainmas[c*3+1]*0.01, mainmas[c*3+2]*10);
			} 

	glEnd();

	glFlush();
	glutSwapBuffers();

}

void display(void)
{

	/* Определение количества кадров в секунду (FPS)*/
	long int t2 = GetTickCount(); 
	if(t2-t1>1000)
	{
		fps = fpstmp;
		fpstmp = 0;
		t1=t2;
	} else fpstmp++;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_SPECULAR, amb);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_POSITION, arr);
	gluPerspective(60,SCREEN_WIDTH/SCREEN_HEIGHT,1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(180,0,1,0);
	glRotated(MyCam.GetAngleXOZ(),0,1,0);
	glTranslated(-MyCam.GetX(), -MyCam.GetY(), -MyCam.GetZ());

	DrawTeapots();
	DrawWalls();

    glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
	//glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, img->imageData); 
	cvCvtColor(img, img, CV_BGR2RGB);
	img->origin = IPL_ORIGIN_BL;

	int y = 0, x = 0;
	for(int i = 0; i<SCREEN_WIDTH*SCREEN_HEIGHT; i++)
	{
		if(x>=SCREEN_WIDTH)
		{
			x = 0;
			y++;
		}
		mainmas[i*3] = x;
		mainmas[i*3+1] = y;
		mainmas[i*3+2] = depth[i];
		x++;
	}

//	DrawFPS(img, fps, KMEANS_CLUSTERS);
	glFlush();
	glutSwapBuffers();
	glutSetWindow(w2);
	glutPostRedisplay();
	glutSetWindow(w1);
//	cvShowImage("Определение объектов", img);	
}

int main(int argc, char **argv)
{
	Size.height = SCREEN_HEIGHT; Size.width = SCREEN_WIDTH;
	img = cvCreateImage(Size, IPL_DEPTH_8U, 3);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	w1 = glutCreateWindow("OpenGL - Имитация реального мира");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keybord);
	glutMotionFunc(mouseMotion);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	w2 = glutCreateWindow("OpenGL - Восстановление трёхмерной сцены");
	glutSetWindow(w2);
	glutDisplayFunc(display2);
	glutKeyboardFunc(keybord2);
	depth = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
	mainmas = new float[SCREEN_WIDTH*SCREEN_HEIGHT*3];
	glutMainLoop();
	delete[]depth;
	return 0;
}

void DrawFPS(IplImage * pic, int _fps, int clusters)
{
	CvPoint pt = cvPoint( SCREEN_WIDTH-100, SCREEN_HEIGHT-30 );
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,0.7, 0.7, 0, 0, CV_AA);
	char buf[20];
	sprintf(buf,"FPS: %d",_fps);
	cvPutText(pic, buf, pt, &font, CV_RGB(100, 100, 255));
	pt = cvPoint( 10, SCREEN_HEIGHT-30 );
	sprintf(buf,"CLUSTERS: %d", clusters);
	cvPutText(pic, buf, pt, &font, CV_RGB(100, 100, 255));
}

void DrawTeapots()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorTeapot);
  
	glPushMatrix();
	glTranslated(95,9.5,65);
//	glRotated(angle,0,1,0);
	glutSolidTeapot(3);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95,9.5,80);
//	glRotated(angle,0,1,0);
	glutSolidDodecahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslated(95, 9.5, 50);
//	glRotated(-angle,0,1,0);
	glutWireTeapot(3);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorA);
	glPushMatrix();
	glTranslated(95, 2, 50);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95, 2, 65);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95, 2, 80);
	glutSolidCube(10);
	glPopMatrix();
}

void DrawWalls()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorY);
  glBegin(GL_QUADS);
		glVertex3d(100,-1,100);
		glVertex3d(100,-1,0);
		glVertex3d(0,-1,0);
		glVertex3d(0,-1,100);
  glEnd();

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorX);

    glBegin(GL_QUADS);
		glVertex3d(0,-1,0);
		glVertex3d(100,-1,0);
		glVertex3d(100,30,0);
		glVertex3d(0,30,0);
  glEnd();

      glBegin(GL_QUADS);
		glVertex3d(0,-1,100);
		glVertex3d(0,-1,0);
		glVertex3d(0,30,0);
		glVertex3d(0,30,100);
  glEnd();

        glBegin(GL_QUADS);
		glVertex3d(0,-1,100);
		glVertex3d(100,-1,100);
		glVertex3d(100,30,100);
		glVertex3d(0,30,100);
  glEnd();

          glBegin(GL_QUADS);
		glVertex3d(100,-1,100);
		glVertex3d(100,-1,0);
		glVertex3d(100,30,0);
		glVertex3d(100,30,100);
  glEnd();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorZ);
            glBegin(GL_QUADS);
		glVertex3d(0,30,0);
		glVertex3d(0,30,100);
		glVertex3d(100,30,100);
		glVertex3d(100,30,0);
  glEnd();
}