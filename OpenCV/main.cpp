//Подключение необходимых библиотек
#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include "scene.h"
#include "glut.h"          

#define KMEANS_CLUSTERS 2
#define CAMERA_STEP 10

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
void BGRToRGB(IplImage * pic);

const int SCREEN_WIDTH = 1440;  // Разрешение окона OpenGL&CV по горизонтали
const int SCREEN_HEIGHT = 900;  // Разрешение окона OpenGL&CV по вертикали

using namespace std; //Определение пространства имён

float *depth, *forkmeans;
double angle = 0 /*Угол поворота камеры XOZ*/, step = 0 /*Шаг камеры*/, camLookAt[3] = {0,0,0}/*Направление взгляда камеры*/;
int last = 0, fpstmp = 0/*Техническая переменная для определения FPS*/, fps = 0/*Количество кадров в секунду*/;
Camera MyCam; // Главный класс, отвечающий за управление камерой
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
IplImage *img = 0/*Главное изображение, использующиеся OpenCV*/, *gray = 0, *dst = 0;


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

void mouseMotion(int x, int y)
{
	//std::cout << "Mouse X: " << x << ", Y: " << y << std::endl;
}

void idle(void)
{
	// Команды поворота чайника 
	angle+=0.3;
	if(angle>360) angle = 0;

	glutPostRedisplay();
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
	gluPerspective(60,SCREEN_WIDTH/SCREEN_HEIGHT,10,200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(180,0,1,0);
	glRotated(MyCam.GetAngleXOZ(),0,1,0);
	//glRotated(-60,0,1,0);
	glTranslated(-MyCam.GetX(), -MyCam.GetY(), -MyCam.GetZ());
	img->origin = IPL_ORIGIN_BL;

	DrawTeapots();
	DrawWalls();

	float clusters[] = {0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};

    glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
	glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, img->imageData); BGRToRGB(img);
	dst->origin = CV_ORIGIN_BL;
	cvCvtColor(img, gray, CV_RGB2GRAY);
    cvCanny(gray, dst, 4, 100, 3);
	int n = 0; 
	for(int i = 0, c = 0, k = 0; i<SCREEN_WIDTH*SCREEN_HEIGHT; i++, c++)
	{
		if(dst->imageData[i])
		{
			forkmeans[k] = depth[c];
			int x = i, y = 0;
			while(x>SCREEN_WIDTH)
			{
				x-=SCREEN_WIDTH;
				y++;
			}
			forkmeans[k+1] = (float)((float)(x)/(float)(SCREEN_WIDTH));
			forkmeans[k+2] = (float)((float)(y)/(float)(SCREEN_HEIGHT));
			k+=3;
			n++;
		} 
		else
		{
		}
	}

	kmeans(3, forkmeans, n, KMEANS_CLUSTERS, clusters, out);
	
	//for(int i = 0; i<SCREEN_WIDTH*SCREEN_HEIGHT*3; i+=3)
	//{
	//	img->imageData[i] = 0;
	//	img->imageData[i+1] = 0;
	//	img->imageData[i+2] = 0;
	//}

	float x = 0, y = 0; int z = 0;

	z = 0;
	for(int i = 0; i<n; i+=3, z++)
	{
		x = forkmeans[i+1]; 
		y = forkmeans[i+2];
		if(!x) x=0.0000001;
		if(!y) y=0.0000001;
		cout << (int)(x*SCREEN_WIDTH*SCREEN_WIDTH*y*SCREEN_HEIGHT)*3 << endl;
		if(out[z])
		{
			img->imageData[(int)(SCREEN_WIDTH*y*SCREEN_HEIGHT+x*SCREEN_WIDTH)*3] = 255;   
			img->imageData[(int)(SCREEN_WIDTH*y*SCREEN_HEIGHT+x*SCREEN_WIDTH)*3+1] = 255;   
			img->imageData[(int)(SCREEN_WIDTH*y*SCREEN_HEIGHT+x*SCREEN_WIDTH)*3+2] = 255;   
		}
		else
		{
			img->imageData[(int)(SCREEN_WIDTH*y*SCREEN_HEIGHT+x*SCREEN_WIDTH)*3] = 0;   
			img->imageData[(int)(SCREEN_WIDTH*y*SCREEN_HEIGHT+x*SCREEN_WIDTH)*3+1] = 255;   
			img->imageData[(int)(SCREEN_WIDTH*y*SCREEN_HEIGHT+x*SCREEN_WIDTH)*3+2] = 0;   
		}

	}
	
	z = 0; n = 0;

	DrawFPS(img, fps, KMEANS_CLUSTERS);
	glFlush();
	glutSwapBuffers();
	cvShowImage("Определение объектов", img);	
}

int main(int argc, char **argv)
{
	Size.height = SCREEN_HEIGHT; Size.width = SCREEN_WIDTH;
	img = cvCreateImage(Size, IPL_DEPTH_8U, 3);
	gray = cvCreateImage(Size, IPL_DEPTH_8U, 1);
    dst = cvCreateImage(Size, IPL_DEPTH_8U, 1);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("OpenGL тестовый мир");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keybord);
	glutMotionFunc(mouseMotion);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	out = new int[SCREEN_WIDTH*SCREEN_HEIGHT];
	depth = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
	color = new char[SCREEN_WIDTH*SCREEN_HEIGHT*3];
	forkmeans = new float[SCREEN_WIDTH*SCREEN_HEIGHT*4];
	glutMainLoop();
	delete[]out;
	delete[]depth;
	delete[]color;
	delete[]forkmeans;
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

void BGRToRGB(IplImage * pic)
{
	for(int i = 0; i<(SCREEN_WIDTH*SCREEN_HEIGHT*3); i+=3)
	{
		char tmp;
		tmp = pic->imageData[i];
		pic->imageData[i] = pic->imageData[i+2];
		pic->imageData[i+2] = tmp;
	}
}

void DrawTeapots()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorTeapot);
  
	glPushMatrix();
	glTranslated(95,9.5,65);
	glRotated(angle,0,1,0);
	glutSolidTeapot(3);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95,9.5,80);
	glRotated(angle,0,1,0);
	glutSolidDodecahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslated(95, 9.5, 50);
	glRotated(-angle,0,1,0);
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