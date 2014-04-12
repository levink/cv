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
#define TAN_30 0.57735026918962576450914878050196
#define CTAN_30 1.7320508075688772935274463415059

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
HWND GetConsoleHwnd();

const int SCREEN_WIDTH = 800;  // Разрешение окона OpenGL&CV по горизонтали
const int SCREEN_HEIGHT = 600;  // Разрешение окона OpenGL&CV по вертикали

using namespace std; //Определение пространства имён

float *depth, *depth2, *mainmas;
double angle = 0 /*Угол поворота камеры XOZ*/, step = 0 /*Шаг камеры*/, camLookAt[3] = {0,0,0}/*Направление взгляда камеры*/;
int w1 = -1, w2 = -1, last = 0, fpstmp = 0/*Техническая переменная для определения FPS*/, fps = 0/*Количество кадров в секунду*/;
Camera w1camera, w2camera; // Главный класс, отвечающий за управление камерой
char *output, *color;
long int t1 = 0;
int *out;

GLfloat arr[4] = {50.0,10.0,50.0,1.0};
GLfloat arr2[3] = {50.0,-1.0,50.0};
GLfloat colorX[3] = {1,0,1};
GLfloat colorY[3] = {0,0,1};
GLfloat colorZ[3] = {1,0,0};
GLfloat colorA[3] = {1,1,0};
GLfloat colorBlack[3] = {0.5,0.3,0.2};
GLfloat amb[4] = {0,1,0,0};
GLfloat colorTeapot[3] = {0,1,0};

HWND hwnd = NULL;

CvSize Size;
IplImage *img = 0/*Главное изображение, использующиеся OpenCV*/, *gray, *dst;


class Matrix;
class Vector4d
{
public:
	double e[4];
	Vector4d(double v[4])
	{
		e[0] = v[0];
		e[1] = v[1];
		e[2] = v[2];
		e[3] = v[3];
	}
};
class Matrix
{
public:
	double e[4][4];
	Matrix(double values[4][4])
	{
		e[0][0] = values[0][0];
		e[0][1] = values[0][1];
		e[0][2] = values[0][2];
		e[0][3] = values[0][3];	

		e[1][0] = values[1][0];
		e[1][1] = values[1][1];
		e[1][2] = values[1][2];
		e[1][3] = values[1][3];
		
		e[2][0] = values[2][0];
		e[2][1] = values[2][1];
		e[2][2] = values[2][2];
		e[2][3] = values[2][3];
		
		e[3][0] = values[3][0];
		e[3][1] = values[3][1];
		e[3][2] = values[3][2];
		e[3][3] = values[3][3];
	}
	Vector4d operator*(const Vector4d &el)
	{
		double res[4];
		res[0] = e[0][0]*el.e[0] + e[0][1]*el.e[1] + e[0][2]*el.e[2] + e[0][3]*el.e[3];
		res[1] = e[1][0]*el.e[0] + e[1][1]*el.e[1] + e[1][2]*el.e[2] + e[1][3]*el.e[3];
		res[2] = e[2][0]*el.e[0] + e[2][1]*el.e[1] + e[2][2]*el.e[2] + e[2][3]*el.e[3];
		res[3] = e[3][0]*el.e[0] + e[3][1]*el.e[1] + e[3][2]*el.e[2] + e[3][3]*el.e[3];
		return Vector4d(res);
	};
};

double v[4][4] = {
	{(SCREEN_WIDTH/SCREEN_HEIGHT) / CTAN_30, 0, 0, 0},
	{0, CTAN_30, 0, 0},
	{0, 0, (100+1)/(1-100), (2*100*1)/(1-100)},
    {0, 0, -1, 0}                //	{0, 0, -0.495, 0.505}
};

double v1[4][4] = {
	{1/(SCREEN_WIDTH/SCREEN_HEIGHT)*TAN_30, 0, 0, 0},
	{0, 1/TAN_30, 0, 0},
	{0, 0, 101/99, 200/-99},
	{0, 0, 1, 0}
};


void keybord(unsigned char key, int x, int y)
{
	if (key == 'j' || key == 238)
	{
	
	}
	
	if (key == 'w' || key == 246) // Движение вперед
	{
		w1camera.MoveForward(CAMERA_STEP);
	}

	if (key == 's' || key == 251) // Движение назад
	{
		w1camera.MoveBack(CAMERA_STEP);
	}

	if (key == 'a' || key == 244) // Поворот камеры направо
	{
		w1camera.Rotate(-CAMERA_STEP);
	}

	if (key == 'd' || key == 226) // Поворот камеры налево
	{
		w1camera.Rotate(CAMERA_STEP);
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
		w2camera.MoveForward(CAMERA_STEP);
	}

	if (key == 's' || key == 251) // Движение назад
	{
		w2camera.MoveBack(CAMERA_STEP);
	}

	if (key == 'a' || key == 244) // Поворот камеры направо
	{
		w2camera.Rotate(-CAMERA_STEP);
	}

	if (key == 'd' || key == 226) // Поворот камеры налево
	{
		w2camera.Rotate(CAMERA_STEP);
	}

	glutPostRedisplay();
}

double m[4] = {0,0,-3,1};

/*
z=1:       0.000
z=1.5:     0.336
z=2:       0.505
z=50:      0.989
z=75:      0.996
z=99.999:  1
*/


void mouse2(int button, int state, int x, int y)
{
	//y = SCREEN_HEIGHT - y;
	glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth2);
	int k = 0;
	for(int i=0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		if(depth2[i] < 1) k++;
	}
	
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)  
	{
		Vector4d b = Vector4d(m);
		Matrix m1 = Matrix(v);

		Vector4d a = m1 * b;

		std::cout << "[Window 2] Mouse X: " << x << ", Y: " << y << ", depth: " << (float)depth2[SCREEN_WIDTH*y+x] << std::endl;
		std::cout << "           Depth (Formula): " << ((100+1)/(100-1)) + ((-2*100*1)/(m[2]*(100-1))) << std::endl;
		std::cout << "           Realc X: " << m[0] << ", Y: " << m[1] << ", Z: " << m[2] << std::endl;
		std::cout << "           Vectr X: " << a.e[0] << ", Y: " << a.e[1]  << ", depth: " << a.e[2] << std::endl << std::endl;
	}
}

void idle(void)
{
	// Команды поворота чайника 
	angle+=1;
	if(angle>360) angle = 0;
	glutPostRedisplay();
}

void idle2(void)
{
	glutPostRedisplay();
}

void display2(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glEnable(GL_LIGHT0);
	//glShadeModel(GL_SMOOTH);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, amb);
	glEnable(GL_DEPTH_TEST);
	//glLightfv(GL_LIGHT0, GL_POSITION, arr);
	gluPerspective(60,SCREEN_WIDTH/SCREEN_HEIGHT,1,100);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glColor3d(1,1,1);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3d(m[0],m[1],m[2]);
	glEnd();




	//glBegin(GL_LINES);
	//glColor3d(1,0,0);
	//glVertex3i(0,0,0);
	//glVertex3i(500,0,0);
	//glColor3d(0,1,0);
	//glVertex3i(0,0,0);
	//glVertex3i(0,500,0);
	//glColor3d(0,0,1);
	//glVertex3i(0,0,0);
	//glVertex3i(0,0,500);
	//glEnd();

	


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
	glRotated(180,0,1,0);
	glRotated(w1camera.GetAngleXOZ(),0,1,0);
	glTranslated(-w1camera.GetX(), -w1camera.GetY(), -w1camera.GetZ());


	DrawTeapots();
	DrawWalls();

	//img->origin = IPL_ORIGIN_BL;
	//gray->origin = IPL_ORIGIN_BL;

	dst->origin = IPL_ORIGIN_BL;

 //   glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
//	glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, img->imageData); 

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	hwnd = GetConsoleHwnd();
	SetWindowPos(hwnd,0,0,650,800,300,0);
	Size.height = SCREEN_HEIGHT; Size.width = SCREEN_WIDTH;
	img = cvCreateImage(Size, IPL_DEPTH_8U, 3);
	gray = cvCreateImage(Size, IPL_DEPTH_8U, 1);
	dst = cvCreateImage(Size, IPL_DEPTH_8U, 1);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	w1 = glutCreateWindow("OpenGL - Имитация реального мира");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keybord);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutInitWindowPosition(850,0);
	w2 = glutCreateWindow("OpenGL - Восстановление трёхмерной сцены");
	glutSetWindow(w2);
	glutDisplayFunc(display2);
	glutKeyboardFunc(keybord2);
	glutMouseFunc(mouse2);
	glutIdleFunc(idle2);
	depth = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
	depth2 = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
	mainmas = new float[SCREEN_WIDTH*SCREEN_HEIGHT*3];
	glutMainLoop();
	delete[]depth;
	delete[]depth2;
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

	glPushMatrix();
	glTranslated(95, 2, 40);
	glutSolidOctahedron();
	glScaled(2.0,2.0,2.0);
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

HWND GetConsoleHwnd()
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
       return(hwndFound);
   }