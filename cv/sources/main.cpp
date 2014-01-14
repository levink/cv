//Подключение необходимых библиотек

#include <iostream>
#include <string>
#include <Windows.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <glut.h>
#include <kmeans.h> 
#include <scene.h>
//some useful changes



void kmeans(
            int  dim,		                  // dimension of data 
            double *X,                        // pointer to data
            int   n,                          // number of elements
            int   k,                          // number of clusters
            double *cluster_centroid,         // initial cluster centroids
            int   *cluster_assignment_final   // output
           );

const int SCR_WIDTH = 1440;  // Разрешение окона OpenGL/CV по горизонтали
const int SCR_HEIGHT = 900;  // Разрешение окона OpenGL/CV по вертикали

//Определение пространства имён (cv - для OpenCV)
using namespace std;

double angle = 0 /*Угол поворота камеры XOZ*/, step = 0 /*Шаг камеры*/, camLookAt[3] = {0,0,0}/*Направление взгляда камеры*/;
GLfloat colorTeapot[3] = {0,1,0}; // Цвет великолепных чайников
int last = 0;
Camera MyCam; // Главный класс, отвечающий за управление камерой
char * output;
long int t1 = 0; int fpstmp = 0/*Техническая переменная для определения FPS*/, fps = 0/*Количество кадров в секунду*/;

IplImage* img = cvLoadImage("D:\\2.jpg"); 
IplImage* imgK = cvLoadImage("D:\\2.jpg"); 

void keybord(unsigned char key, int x, int y)
{
	//std::cout << "Key: " << key << "\n";
	

	if (key == 'j' || key == 238)
	{
	

	//cvReleaseImage(&img); // Освобождаем память из под картинки
	}

	
	if (key == 'w' || key == 246) // Движение вперед
	{
		MyCam.MoveForward(1);
	}
	if (key == 's' || key == 251) // Движение назад
	{
		MyCam.MoveBack(1);
	}

	if (key == 'a' || key == 244) // Поворот камеры направо
	{
		MyCam.Rotate(-1);
	}
	if (key == 'd' || key == 226) // Поворот камеры налево
	{
		MyCam.Rotate(1);
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
	GLfloat arr[4] = {50.0,30.0,50.0,1.0};
	GLfloat arr2[3] = {50.0,-1.0,50.0};
	GLfloat colorX[3] = {1,0,1};
	GLfloat colorY[3] = {0,0,1};
	GLfloat colorZ[3] = {1,0,0};
	GLfloat colorA[3] = {1,1,0};
	GLfloat colorBlack[3] = {0.5,0.3,0.2};
	GLfloat amb[4] = {0,1,0,0};
	
	long int t2 = GetTickCount(); 

	/* Определение количества кадров в секунду (FPS)*/
	if(t2-t1>1000)
	{
		fps = fpstmp;
		fpstmp = 0;
		t1=t2;
	
	}
	else
	{
		fpstmp++;
	}
	 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_SPECULAR, amb);
	glEnable(GL_DEPTH_TEST);//ВКЛЮЧЕНИЕ БУФФЕРА ГЛУБИНЫ


	glLightfv(GL_LIGHT0, GL_POSITION, arr);
//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, arr2);
		

	gluPerspective(60,SCR_WIDTH/SCR_HEIGHT,10,200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotated(180,0,1,0);
	glRotated(MyCam.GetAngleXOZ(),0,1,0);
	glTranslated(-MyCam.GetX(), -MyCam.GetY(), -MyCam.GetZ());


	//std::cout << a << std::endl;

	glNormal3d(1,1,1);


	glPushMatrix();
	glTranslated(-50,0,-50);
	glutSolidSphere(5,200,200);
	glPopMatrix();



	// Стены

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
   

  // Великолепные чайники

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
	glFlush();
	glutSwapBuffers();
 
	float * depth;
	int c = 0;
	//depth = new float[SCR_WIDTH*SCR_HEIGHT];
    /*glReadPixels(0,0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depth);

    for(int i = 0; i<(SCR_WIDTH*SCR_HEIGHT*3); i+=3, c++)
	{
		img->imageData[i] = 255-(depth[c]/0.00393);
		img->imageData[i+1] = 255-(depth[c]/0.00393);
		img->imageData[i+2] = 255-(depth[c]/0.00393);
	}
	img->origin = IPL_ORIGIN_BL;
		
		
	double clusters[2] = {2,9};
	double test[10] = {1,2,3,4,5,6,7,8,9,10};
	int out[10];
		
	kmeans(1, test, 9, 2, clusters, out); 

	delete[]depth;*/

	//for(int i = 0; i<(SCR_WIDTH*SCR_HEIGHT*3); i+=3)
	//{
	//	char tmp;
	//	tmp = img->imageData[i];
	//	img->imageData[i] = img->imageData[i+2];
	//	img->imageData[i+2] = tmp;
	//}
		

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorBlack);
	
	CvPoint pt = cvPoint( 1300, 850 );
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX,1.0, 1.0, 0, 0, CV_AA);
	char buf[20];
	sprintf_s(buf, 20, "FPS: %d",fps);
	cvPutText(img, buf, pt, &font, CV_RGB(0, 0, 255));

    cvShowImage("Определение контура объектов", img);

	//cvShowImage("OpenCV - Translation", img); // Выводим картинку в окно
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("OpenGL тестовый мир");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keybord);
	glutMotionFunc(mouseMotion);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutMainLoop();
	
	system("Pause");
	return 0;
}