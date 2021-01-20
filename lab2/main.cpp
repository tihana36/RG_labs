#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL\freeglut.h>
#include <glm\glm.hpp>
#include <math.h>

#include <stdlib.h>
#include <time.h>


using namespace std;

GLuint window;
GLuint width = 550, height = 550;

struct Cestica {
	glm::vec3 polozaj;
	float pomak;

	glm::vec3 boja;
	float zivot; //ako nula onda ugasena cestica
	float kut;
	float velicina;

	glm::vec3 rotacija; //za pohraniti os vektor

};


void stvaranje();
void zivot(int n);
void umiranje(Cestica c, int n);
void idle();


void myDisplay();
void myReshape(int width, int height);
//void updatePerspective();
void myObject();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
//void Mouse(int button, int state, int x, int y);



const int maximum = 500;
int now = 0, before = 0;


int main(int argc, char** argv)
{
	srand(time(0));

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(900, 200);
	glutInit(&argc, argv);

	window = glutCreateWindow("Particle system");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	//glutMouseFunc(Mouse);
	glutKeyboardFunc(myKeyboard);
	
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}




int previous = 0;


std::vector<Cestica> particles;


double izvor[3] = {300, 300, 0 };

double ociste[3] = { 1,0,0 };
double ex, ey, ez;
double sx, sy, sz;
double osx, osy, osz;


void zivot(int n) {

	for (int i = n-1; i >= 0; i--) {
		Cestica p = particles.at(i);

		glm::vec3 polozaj = p.polozaj;


		//vektor e se dobije pomocu pocetne orijentacije i vektora prema ocistu
		ex = polozaj.x-ociste[0];
		ey = polozaj.y-ociste[1];
		ez = polozaj.z-ociste[2];

		sx = 0;
		sy = 1;
		sz = 0;

		//pomakni
		float pom = p.pomak;

		//pomak veci po y nego po x
		glm::vec3 pomakni = glm::vec3();


		pomakni.x = pom * polozaj.x + rand() % 100 - 50;
		pomakni.y = pom * polozaj.y + rand() % 70 - 35;
		pomakni.z = polozaj.z;

		p.polozaj = pomakni;



		osx = sy * ez - ey * sz;
		osy = -(sx * ez - ex * sz);
		osz = sx * ey - sy * ex;

		//cout << " --" << osz << "\n";

		double absE = sqrt(ex * ex + ey * ey + ez * ez);
		double absS = sqrt(sx * sx + sy * sy + sz * sz);

		double fi = acos((sx * ex + sy * ey + sz * ez) / (absS * absE));

		//kut u stupnjevima
		fi = fi * (180 / 3.14159);
		cout << "kut je:" << fi << "\n";

		p.kut = fi;

		p.rotacija = glm::vec3(osx, osy, osz);

		//pohrani promjene
		particles.at(i) = p;

		particles.at(i).velicina++;

		cout <<"novi"<< particles.at(i).polozaj.x << " " << particles.at(i).polozaj.y;

		umiranje(particles.at(i), i);


	}
	myObject();
}

void umiranje(Cestica p, int i) {
	//smanji zivot
	particles.at(i).zivot = particles.at(i).zivot - 1;

	//promjena boje - smnaji intenzitet
	float r = particles.at(i).boja.x + 0.1;
	float g = particles.at(i).boja.y+ 0.1;
	//float b = particles.at(i).boja.z + 0.1;
	float b = 0;

	particles.at(i).boja = glm::vec3(r, g, b);

	//provjeri je li gotova
	if (particles.at(i).zivot > 0) return;
	else {
		//ugasi cesticu
		particles.erase(particles.begin() + i);
		cout << "obisano\n";
	}
}



void idle() {
	//animacija
	//Sleep(50);
	myDisplay();

}


void myDisplay(void)
{
	//glTranslatef(ociste[0], ociste[1], ociste[2]);


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		         // boja pozadine - bijela

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	//svake 0.2 sekunde napravi nove cestice
	
	stvaranje();
		
	
	glFlush();

	
	

	

	//Sleep(1000);
	
	
	
	
	
	//glutSwapBuffers();      // iscrtavanje iz dvostrukog spemnika (umjesto glFlush)

	//

	

	
}



//napravi 5 cestica random boje svake sekunde
//pomakni ih
//ugasi ih ako potrebno







void stvaranje() {		

	//napravi svaki put pet novih cestica
	int nove = 25;
	for (int i = 0; i < nove; i++) {

		//float random = ((rand() % 100) - 50) / 10.0f;
		//float rColor = 0.5f + ((rand() % 100) / 100.0f);



		//random pozicija koja malo udaljena od izvora
		double x = 1 + rand() % 20 - 10 + izvor[0];
		double y = 1 + rand() % 20 - 10 + izvor[1];
		double z = 0;

		Cestica p;
		glm::vec3 polozaj = {x,y,z};

		p.polozaj = polozaj;

		//crvena boja
		

		float r = ((rand() % 10) + 1) / 10;
		//float b = ((rand() % 10) + 1) / 10;

		
		
		float g = ((rand() % 10) + 1) / 10;

		cout << r;

		p.boja = glm::vec3(r, g, 0);

		p.zivot = 16;
		p.pomak = 0.99;
		p.velicina = 3;

		particles.push_back(p);


	}

	int n = particles.size();

	zivot(n);

	

	
}





void myReshape(int w, int h)
{
	//printf("Pozvan myReshape()\n");
	width = w; height = h;               //promjena sirine i visine prozora
	glViewport(0, 0, width, height);	//  otvor u prozoru

	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	gluOrtho2D(0, width, 0, height); 	//	okomita projekcija
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);					//	postavi velicinu tocke za liniju
	glColor3f(1.0f, 0.0f, 0.0f);		//	postavi boju linije
}


void myObject()
{
	cout << "crtam\n";
	
	int n = particles.size();
	cout << "n:" << n;

	//int velicina = 2;
	for (int i = n-1 ; i >=0; i--) {
		Cestica p = particles.at(i);

		double r = particles.at(i).boja.x;
		double g = particles.at(i).boja.y;
		double b = particles.at(i).boja.z;
		glColor3f(r, g, b);

		cout << r << "\n";

		double x = p.polozaj.x;
		double y = p.polozaj.y;
		double z = p.polozaj.z;
		glTranslatef(x, y, z);

		double fi = p.kut;
		double osx = p.rotacija.x;
		double osy = p.rotacija.y;
		double osz = p.rotacija.z;

		glRotatef(fi, osx, osy, osz);

		cout << "\npolozaj:" << p.polozaj.x<<" "<< p.polozaj.y<<"\n";

		//glColor3f(1, 0, 0);
		int velicina = particles.at(i).velicina;
		if (velicina >= 8) velicina = 8;

		glPointSize(velicina);
		glBegin(GL_POINTS);
		glVertex2f(particles.at(i).polozaj.x, particles.at(i).polozaj.y);
		glEnd();
		glFlush();
		


	}

	//glColor3f(1, 0, 0);
	//glPointSize(5);
	//glBegin(GL_POINTS);
	//glVertex2f(100, 100);
	//glEnd();
	////glFlush();

	/*n = particles.size();
	for (int i = 0; i < particles.size(); i++) {
		cout << "cestica:" << particles.at(i).polozaj.x << " " << particles.at(i).polozaj.y << "\n";
	}*/

	



}

//



void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch (theKey)
	{
	case 'l': izvor[0] = izvor[0] + 10;
		break;

	case 'k': izvor[0] = izvor[0] - 10;
		break;


	case 'a': izvor[1] = izvor[1] + 10;
		break;

	case 's': izvor[1] = izvor[1] - 10;
		break;



	case 'r': 
		particles.clear();
		
		myReshape(width, height);

		Sleep(1000);
		break;

	case 27:  exit(0);
		break;
	}
	//updatePerspective();
	glutPostRedisplay();
}


