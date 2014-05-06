#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <Windows.h>
#include "scene.h"
#include "glut.h"          

#define TAN_30 0.57735026918962576450914878050196
#define CTAN_30 1.7320508075688772935274463415059

float LinearDepth(float depth, float Near, float Far);
void DrawFPS(IplImage * pic, int _fps, int clusters);
void DrawTeapots();
void DrawWalls();
float GetZ(float z);

HWND GetConsoleHwnd();

int SCREEN_WIDTH = 800,  /*Разрешение окона OpenGL&CV по горизонтали*/ 
	SCREEN_HEIGHT = 600; /*Разрешение окона OpenGL&CV по вертикали*/
float CAMERA_STEP = 0.4;

using namespace std; //Определение пространства имён

bool FullScreen = false, MoveForward = false, MoveBack = false, 	
	 RotateLeft = false, RotateRight = false, MoveUp = false, MoveDown = false;

float *depth, *depth2, *mainmas, angle = 0/*Угол поворота камеры XOZ*/;
int last = 0, fpstmp = 0/*Техническая переменная для определения FPS*/, fps = 0/*Количество кадров в секунду*/, w1 = 0, w2 = 0;
Camera w1camera = Camera(30,10,30,-45), w2camera = Camera(0,0,0,180); // Главный класс, отвечающий за управление камерой
char *output, *color;
long int t1 = 0;

float arr[4] = {50.0,10.0,50.0,1.0}, arr2[3] = {50.0,-1.0,50.0}, colorBlack[3] = {0.5,0.3,0.2},
	  colorX[3] = {1,0,1}, colorY[3] = {0,0,1}, colorZ[3] = {1,0,0}, colorA[3] = {1,1,0},
	  colorTeapot[3] = {0,1,0}, amb[4] = {0,1,0,0};

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
    {0, 0, -1, 0}    
};


double n = 1;
double r = 0.5;
double t = 0.5*3/4;
double f = 10;

double v1[4][4] = {
	{n/r, 0, 0, 0},
	{0, n/t, 0, 0},
	{0, 0, (f+n)/(n-f), (2*f*n)/(n-f)},
    {0, 0, -1, 0}    
};

//
//double v1[4][4] = {
//	{1/(SCREEN_WIDTH/SCREEN_HEIGHT)*TAN_30, 0, 0, 0},
//	{0, 1/TAN_30, 0, 0},
//	{0, 0, 101/99, 200/-99},
//	{0, 0, 1, 0}
//};


void keybord(unsigned char key, int x, int y)
{
	if (key == 'j' || key == 238)
	{
		if(!FullScreen)
		{
			glutFullScreen();
			FullScreen = true;
		}
		else
		{
			glutReshapeWindow(800,600);
			FullScreen = false;
		}

	}

	if (key == 'k' || key == 235)
	{
		cout << "[Окно 1] Состояние" << endl;
		cout << "Текущие координаты: X: " << (int)w1camera.GetX() << ", Y: " << (int)w1camera.GetY() << ", Z: " << (int)w1camera.GetZ() << ", угол: " << (int)w1camera.GetAngleXOZ() << endl;
		cout << "Разрешение окна: " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << endl;
		cout << "Шаг камеры: " << CAMERA_STEP << endl;
		cout << "FPS: " << fps << endl;
		cout << endl;
	}
	
	if (key == 'w' || key == 246) // Движение вперед
	{
		w1camera.MoveForward(CAMERA_STEP);
		MoveForward = true;
	}

	if (key == 's' || key == 251) // Движение назад
	{
		w1camera.MoveBack(CAMERA_STEP);
		MoveBack = true;
	}

	if (key == 'a' || key == 244) // Поворот камеры направо
	{
		w1camera.Rotate(-CAMERA_STEP);
		RotateLeft = true;
	}

	if (key == 'd' || key == 226) // Поворот камеры налево
	{
		w1camera.Rotate(CAMERA_STEP);
		RotateRight = true;
	}

	if (key == 'r' || key == 234) // Поворот камеры налево
	{
		w1camera.MoveUp(CAMERA_STEP);
		MoveUp = true;
	}
	if (key == 'f' || key == 224) // Поворот камеры налево
	{
		w1camera.MoveDown(CAMERA_STEP);
		MoveDown = true;
	}

	if (key == '=' || key == 61) 
	{
		CAMERA_STEP+=0.02;
	}
	if (key == '-' || key == 45) 
	{
		CAMERA_STEP-=0.02;
	}

//	cout << (int)key << endl;
	glutPostRedisplay();
}

void keybordUp(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 246) 
	{
		MoveForward = false;
	}

	if (key == 's' || key == 251) 
	{
		MoveBack = false;
	}

	if (key == 'a' || key == 244) 
	{
		RotateLeft = false;
	}

	if (key == 'd' || key == 226)
	{
		RotateRight = false;
	}

	if (key == 'r' || key == 234)
	{
		MoveUp = false;
	}
	if (key == 'f' || key == 224) 
	{
		MoveDown = false;
	}

}


double m[4] = {0,0,-2,1};

void keybord2(unsigned char key, int x, int y)
{

	if (key == '=')
	{
		m[2]--;
	}
	if (key == '-')
	{
		m[2]++;
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

void mouse2(int button, int state, int x, int y)
{
	//y = SCREEN_HEIGHT - y;
	depth2 = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
	glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth2);
	
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)  
	{
		Vector4d a = Matrix(v1) * Vector4d(m);

		std::cout << "[Окно 2] Мышь X: " << x << ", Y: " << y << ", глубина: " << (float)depth2[SCREEN_WIDTH*y+x] << std::endl;
		std::cout << "     Вектор   X: " << a.e[0] << ", Y: " << a.e[1]  << ", depth: " << a.e[2]/10 << std::endl << std::endl;
	}
	delete[]depth2;
}

void idle(void)
{
	// Команды поворота чайника 
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
	glEnable(GL_DEPTH_TEST);

	//gluPerspective(60,((double)SCREEN_WIDTH)/SCREEN_HEIGHT,1,100);
	glFrustum(-0.5, 0.5, -0.5 * 3/4, 0.5 * 3/4, 1,10);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	/*glRotated(w2camera.GetAngleXOZ(),0,1,0);
	glRotated(180,0,1,0);
	glTranslated(-w2camera.GetX(),-w2camera.GetY(), -w2camera.GetZ());
*/
	
	glPushMatrix();
	glColor3d(1,1,1);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3d(m[0],m[1],m[2]);
	glEnd();
	glPopMatrix();


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

	if(t2-t1>10)
	{
		if(MoveForward)
		{
			w1camera.MoveForward(CAMERA_STEP);
		}
		if(MoveBack)
		{
			w1camera.MoveBack(CAMERA_STEP);
		}
		if(RotateLeft)
		{
			w1camera.Rotate(-CAMERA_STEP);
		}
		if(RotateRight)
		{
			w1camera.Rotate(CAMERA_STEP);
		}
		if(MoveUp)
		{
			w1camera.MoveUp(CAMERA_STEP);
		}
		if(MoveDown)
		{
			w1camera.MoveDown(CAMERA_STEP);
		}
	}


	if(t2-t1>1000) 
	{
		fps = fpstmp;
		fpstmp = 0;
		t1=t2; 
	} 
	else fpstmp++;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_DEPTH_TEST);
	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0}; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	
	gluPerspective(60,((double)SCREEN_WIDTH)/SCREEN_HEIGHT,1,100);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glRotated(180,0,1,0);
	glRotated(w1camera.GetAngleXOZ(),0,1,0);
	glTranslated(-w1camera.GetX(),-w1camera.GetY(), -w1camera.GetZ());

	DrawTeapots();
	DrawWalls();

//  img->origin = IPL_ORIGIN_BL;
//  gray->origin = IPL_ORIGIN_BL;
//	dst->origin = IPL_ORIGIN_BL;

//  glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
//	glReadPixels(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, img->imageData); 

	

	glFlush();
	glutSwapBuffers();
}

void reshape(int weight, int height)
{
	SCREEN_HEIGHT = height;
	SCREEN_WIDTH = weight;
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "RUS");
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
	glutReshapeFunc(reshape);
	glutKeyboardUpFunc(keybordUp);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutInitWindowPosition(850,0);
	w2 = glutCreateWindow("OpenGL - Восстановление трёхмерной сцены");
	glutSetWindow(w2);
	glutDisplayFunc(display2);
	glutKeyboardFunc(keybord2);
	glutMouseFunc(mouse2);
	glutIdleFunc(idle2);
	depth = new float[SCREEN_WIDTH*SCREEN_HEIGHT];
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
	float pos[4] = {25.0, 20.0, 25.0, 0.6};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glPopMatrix();

	glNormal3d(0,1,0);

	glPushMatrix();
	glTranslated(95,7,5);
	glRotated(angle,0,1,0);
	glRotated(-90,1,0,0);
	glutSolidCone(4,4,10,10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95,10,20);
	glRotated(angle,0,1,0);
	glRotated(90,0,1,0);
	glutSolidTorus(1,2,50,50);
	glPopMatrix();


	glPushMatrix();
	glTranslated(95,10,35);
	glRotated(angle,0,1,0);
	glutWireSphere(3,100,100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95,9,65);
	glRotated(angle,0,1,0);
	glutSolidTeapot(3);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95,9,80);
	glRotated(angle,0,1,0);
	glutSolidDodecahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslated(95,9,95);
	glRotated(angle,0,1,0);
	glutWireCube(3);
	glPopMatrix();



	glPushMatrix();
	glTranslated(95, 9, 50);
	glRotated(-angle,0,1,0);
	glutWireTeapot(3);
	glPopMatrix();


	glNormal3d(1,1,0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorA);


	glPushMatrix();
	glTranslated(95, 2, 5);
	glutSolidCube(10);
	glPopMatrix();


	glPushMatrix();
	glTranslated(95, 2, 20);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(95, 2, 35);
	glutSolidCube(10);
	glPopMatrix();

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
	glTranslated(95, 2, 95);
	glutSolidCube(10);
	glPopMatrix();


}

void DrawWalls()
{
	glNormal3d(0,1,0);
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

float LinearDepth(float depth, float Near, float Far)
{
	return (2.0 * Near) / (Far + Near - depth * (Far - Near));
}