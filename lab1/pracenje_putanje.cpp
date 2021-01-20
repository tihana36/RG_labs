#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL\freeglut.h>
#include <glm\glm.hpp>
#include <math.h>

using namespace std;

GLuint window;
GLuint width = 550, height = 550;

void idle();
void myDisplay();
void myReshape(int width, int height);
void ravnina();
vector<string> split(string s, string delimiter);

void splajn();
void draw();


double xTocka, yTocka, zTocka;
int nacrtaj = 0;
int** poligoni;
double** vrhovi;
int pNum = 0;
int vNum = 0;

double xSrednji, ySrednji, zSrednji;

//ime .obj datoteke
string file;
string file2;

int krivulja = 1;

int n;

//broj točaka b-splajna
int m;
int brSegm;

double** kont;

//za putanju krivulje
double Px[100] = { 0 };
double Py[100] = { 0 };
double Pz[100] = { 0 };

//za tangente
double Tx[100] = { 0 };
double Ty[100] = { 0 };
double Tz[100] = { 0 };


double** bspline;
//tangente
double** tangente;


//vektori orijentacije
double ex, ey, ez;
double sx, sy, sz;
double osx, osy, osz;




int main(int argc, char** argv) {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(900, 200);
	glutInit(&argc, argv);

	window = glutCreateWindow("praćenje putanje");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	file = argv[1];

	file2 = argv[2];
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

int objekt = 1;

int broj = 0;


//ANIMACIJA
void idle() {
	//animacija
	//Sleep(100);
	myDisplay();

}

void myDisplay()
{
	myReshape(width, height);
	


	//ucitaj sve podatke objekta
	if (objekt == 1) {
		ravnina();
		
	}

	//ucitaj podatke o krivulji
	if (krivulja == 1) {
		cout << "crtaj";
		splajn();

		glFlush();
		Sleep(3000);		
	}

	//RESET ANIMACIJE - kada dode na kraj da se vrati na pocetak
	if (broj==100 * brSegm) {
		broj = 0;
	}

	
	

	//cout << "broj:" << broj << "\n";
	int i = broj;
	for (int j = 0; j < 100 * brSegm; j++) {
		//nacrtaj krivulju
		int boja = 0;
		//int j = broj;
		double xs = bspline[j][0];
		double ys = bspline[j][1];
		double zs = bspline[j][2];

		//nacrtaj krivulju u boji tako da se vide razliciti segmenti
		//krivulju crtaj kao niz tocaka
		if (j >= 100) {
			boja = j / 100;
		}
		if (boja % 2 == 0) {
			glColor3f(0, 1, 0);
		}
		else {
			glColor3f(0, 0, 1);
		}
		glBegin(GL_POINTS);
		glVertex3f(xs, ys, 0);
		glEnd();
		glColor3f(0, 0, 0);

		//glFlush();
	}
	

	//draw();

	//Sleep(500);

	
	//glFlush();



	//ORIJENTACIJA OBJEKTA

	//zeljena orijentacija je u smjeru tangente
	ex = tangente[i][0];
	ey = tangente[i][1];
	ez = tangente[i][2];

	//zadamo neku pocetnu orijentaciju
	sx = 1;
	sy = 0;
	sz = 0;
	
	
	//os rotacije
	osx = sy * ez - ey * sz;
	osy = -(sx * ez - ex * sz);
	osz = sx * ey - sy * ex;

	//cout << " --" << osz << "\n";

	double absE = sqrt(ex * ex + ey * ey + ez * ez);
	double absS = sqrt(sx * sx + sy * sy + sz * sz);

	double fi = acos((sx * ex + sy * ey + sz * ez) / (absS * absE));

	



	//TANGENTE
	cout << "crtam tangente";

	cout << i << "\n";
	double xs = bspline[i][0];
	double ys = bspline[i][1];
	double zs = bspline[i][2];

	//tangenta je duzina kojoj je prva tocka tocka krivulje a druga tocka je u smjeru tangente
	double xe = bspline[i][0] + tangente[i][0] * 1.2;
	double ye = bspline[i][1] + tangente[i][1] * 1.2;
	double ze = bspline[i][2] + tangente[i][2] * 1.2;

	cout << xs << " " << xe << "\n";
	//crtaj tangente crvenom bojom
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	{
		glVertex3d(xs, ys, 0);			//	crtanje gotove linije
		glVertex3d(xe, ye, 0);
	}
	glEnd();


	
	//premjesti objekt na poziciju u B-splajnu
	glTranslatef(bspline[i][0], bspline[i][1], 0);	



	//rotiraj oko osi rotacije za kut fi da mu orijentacija bude u smjeru tangente na krivulju
	fi = fi * (180 / 3.14159);	//fi treba biti u stupnjevima
	cout << "kut je:" << fi << "\n";
	glRotatef(fi, osx, osy, osz);

	//NACRTAJ OBJEKT
	glColor3f(0, 0, 0);
	cout << "crtaj objekt";
	draw();


	
	glFlush();

	Sleep(50);
	broj++;
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


//FUNKCIJA ZA CRTANJE OBJEKTA
void draw() {

	int povecaj = 60;
	for (int i = 0; i < pNum; i++)
	{
		//cout << "crtam objekt";
		int prvi = poligoni[i][0];
		int drugi = poligoni[i][1];
		int treci = poligoni[i][2];

		double y2 = vrhovi[drugi - 1][1] * povecaj;
		double y1 = vrhovi[prvi - 1][1] * povecaj;
		double y3 = vrhovi[treci - 1][1] * povecaj;

		double z1 = 0; double z2 = 0; double z3 = 0;
		if (file2 == "spirala.txt") {
			z3 = vrhovi[treci - 1][2] * povecaj;
			z1 = vrhovi[prvi - 1][2] * povecaj;
			z2 = vrhovi[drugi - 1][2] * povecaj;
		}


		double x2 = vrhovi[drugi - 1][0] * povecaj;
		double x1 = vrhovi[prvi - 1][0] * povecaj;
		double x3 = vrhovi[treci - 1][0] * povecaj;



		glBegin(GL_LINE_STRIP);
		{
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
			glVertex3d(x3, y3, z3);
			glVertex3d(x1, y1, z1);
		}
		glEnd();
	}
}


vector<string> split(string s, string delimiter) {
	vector<string> res;
	size_t start, end;

	string token;
	string r;

	start = 0;

	while ((end = s.find(delimiter, start)) != string::npos) {
		token = s.substr(start, end - start);
		res.push_back(token);

		start = end + delimiter.length();

	}

	r = s.substr(start);

	res.push_back(r);
	return res;
}


void splajn() {
	string line;
	ifstream myFile1(file2);

	m = 0;
	if (myFile1.is_open()) {
		while (getline(myFile1, line)) {
			++m;
		}
		myFile1.close();
		myFile1.clear();
	}	

	myFile1.open(file2);
	
	//svaki segment se sastoji od 4 vrha pa ih ima brojčano manje za 3 od vrhova
	brSegm = m - 3;
	
	cout << "\n"<< m << "\n";
	kont = new double* [m];
	for (int i = 0; i < m; i++) {
		kont[i] = new double[3];
	}

	

	std::string delimiter = " ";

	int u = 0;
	int a = m;
	if (myFile1.is_open()) {
		while (getline(myFile1, line) && a > 0) {

			double n[3];
			vector<string> vec = split(line, delimiter);

			double num;
			for (int i = 0; i < vec.size(); i++) {
				num = atof(vec[i].c_str());
				if (file2 == "spirala.txt") {
					num += 10;
					num *= 10;
				}
				kont[u][i] = num;
			}
			++u;
			a--;
		}
	}

	myFile1.close();
	myFile1.clear();

	//vrhovi kontrolnog poligona
	printf("vrhovi:\n");
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%f	", kont[i][j]);
		}
		printf("\n");
	}

	for (int i = 0; i < m - 1; i++) {
		double xs = kont[i][0];
		double ys = kont[i][1];
		double zs = kont[i][2];

		double xe = kont[i + 1][0];
		double ye = kont[i + 1][1];
		double ze = kont[i + 1][2];

		//nacrtaj poligon
		glBegin(GL_LINE_STRIP);
		{
			glVertex3d(xs, ys, 0);			//	crtanje gotove linije
			glVertex3d(xe, ye, 0);
		}
		glEnd();
	}

	


	//tangenti ce biti po sto na svakom segmentu
	int c = 100 * brSegm;
	
	tangente = new double* [c];
	for (int i = 0; i < c; i++) {
		tangente[i] = new double[3];
	}

	//za točke krivulje
	bspline = new double* [c];
	for (int i = 0; i < c; i++) {
		bspline[i] = new double[3];
	}
	

	int k=0;

	//aprox b-splajn krivulja
	for (int i = 0; i < brSegm; i++) {

		//glColor3f(0, 0, 1);

		int rx1, ry1, rz1, rx2, ry2, rz2, rx3, ry3, rz3, rx4, ry4, rz4;

		rx1 = kont[i][0]; ry1 = kont[i][1]; rz1 = kont[i][2];

		rx2 = kont[i+1][0]; ry2 = kont[i+1][1]; rz2 = kont[i+1][2];
		
		rx3 = kont[i+2][0]; ry3 = kont[i+2][1]; rz3 = kont[i+2][2];

		rx4 = kont[i+3][0]; ry4 = kont[i+3][1]; rz4 = kont[i+3][2];

		for (int a = 0; a < 100; a++) {
			double t = a * 0.01;

			//smjer tangente krivulje
			double b1, b2, b3, b4;

			
			double t2 = t * t;
			double t3 = t * t * t;

			b1 = -1 * t2 + 2 * t - 1;
			b2 = 3 * t2 - 4 * t + 0;
			b3 = -3 * t2 + 2 * t + 1;
			b4 = 1 * t2 + 0 + 0;

			//cout << b1 << "\n";	

			//B-splajn krivulja
			double n1, n2, n3, n4;
			
			n1 = -1 * t3 + 3 * t2 - 3 * t + 1;
			n2 = 3 * t3 - 6 * t2 + 0 * t + 4;
			n3 = -3 * t3 + 3 * t2 + 3 * t + 1;
			n4 = 1 * t3 + 0 + 0 + 0;


			//cout << "nacrtaj"

			/*glColor3f(0, 1, 0);

			if (i %2== 0) {
				glColor3f(0, 0, 1);
			}*/

			//Sleep(10000)



			Tx[a] = (b1 * rx1 + b2 * rx2 + b3 * rx3 + b4 * rx4)/2;

			Ty[a] = (b1 * ry1 + b2 * ry2 + b3 * ry3 + b4 * ry4)/2;
			
			Tz[a] = (b1 * rz1 + b2 * rz2 + b3 * rz3 + b4 * rz4)/2;


			Px[a] = (n1 * rx1 + n2 * rx2 + n3 * rx3 + n4 * rx4)/6;
			
			Py[a] = (n1 * ry1 + n2 * ry2 + n3 * ry3 + n4 * ry4)/6;
			
			Pz[a] = (n1 * rz1 + n2 * rz2 + n3 * rz3 + n4 * rz4)/6;


		}

		cout << "gotov segment" << "\n";

		//pospremi sve u isto polje
		for (int j = 0; j < 100; j++) {
			

			/*glColor3f(0, 1, 0);
			if (i %2== 0) {
				glColor3f(0, 0, 1);
			}*/

			/*glBegin(GL_POINTS);
			glVertex2f(xs, ys);
			glEnd();
			glColor3f(0, 0, 0);*/

			
			bspline[k][0] = Px[j];
			bspline[k][1] = Py[j];
			bspline[k][2] = Pz[j];

			tangente[k][0] = Tx[j];
			tangente[k][1] = Ty[j];
			tangente[k][2] = Tz[j];

			k++;
		}


	}
	krivulja = 0;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ravnina() {
	string line;
	ifstream myFile(file);

	std::string delimiter = " ";
	//u prvom prolazu izbrojiti broj vrhova i poligona(prvo slovo v, f ili #)
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			std::string token = line.substr(0, line.find(delimiter));
			if (token == "v") {
				++vNum;
			}
			else if (token == "f") {
				++pNum;
			}
		}
		myFile.close();
		myFile.clear();
	}
	//else cout << "could not open file";

	std::cout << "ukupno vrhova: " << vNum << "\n";
	std::cout << "ukupno poligona: " << pNum << "\n";


	//naciniti alokaciju potrebnog memorijskog prostora
	vrhovi = new double* [vNum];
	for (int i = 0; i < vNum; i++) {
		vrhovi[i] = new double[3];
	}

	poligoni = new int* [pNum];
	for (int i = 0; i < pNum; i++) {
		poligoni[i] = new int[3];
	}

	int v = 0, p = 0;

	//ucitati vrhove i poligone(geometrijske i topološke podatke)
	printf("Ucitavanje vrhova i poligona\n");
	myFile.open(file);

	while (getline(myFile, line)) {
		string red = line;
		if (red.find("#") != string::npos) {

		}
		else if (red.empty()) {

		}
		else {
			std::string token = line.substr(0, line.find(delimiter));
			if (token == "v") {
				double n[3];
				vector<string> vec = split(line, delimiter);

				//for (auto i : v) cout << i << endl;
				double num;
				for (int i = 1; i < vec.size(); i++) {
					num = atof(vec[i].c_str());
					int j = i - 1;
					vrhovi[v][j] = num;
				}
				++v;

			}
			else if (token == "f") {
				double n[3];
				vector<string> vec = split(line, delimiter);

				//for (auto i : v) cout << i << endl;
				int num;
				for (int i = 1; i < vec.size(); i++) {
					num = atof(vec[i].c_str());
					int j = i - 1;
					n[j] = num;
				}
				for (int i = 0; i < 3; i++) {
					poligoni[p][i] = n[i];
				}
				p++;
			}
		}
	}
	myFile.close();
	myFile.close();

	//odrediti minimalne i maksimalne xmin, xmax, ymin, ymax, zmin, zmax koordinate
	double xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = vrhovi[0][0];
	xmax = vrhovi[0][0];
	ymin = vrhovi[0][1];
	ymax = vrhovi[0][1];
	zmin = vrhovi[0][2];
	zmax = vrhovi[0][2];

	for (int i = 0; i < vNum; i++) {
		if (vrhovi[i][0] < xmin) xmin = vrhovi[i][0];
		if (vrhovi[i][0] > xmax) xmax = vrhovi[i][0];

		if (vrhovi[i][1] < ymin) ymin = vrhovi[i][1];
		if (vrhovi[i][1] > ymax) ymax = vrhovi[i][1];

		if (vrhovi[i][2] < zmin) zmin = vrhovi[i][2];
		if (vrhovi[i][2] > zmax) zmax = vrhovi[i][2];
	}

	//prvjera min i max
	/*printf("Min: %f, %f, %f\n", xmin, ymin, zmin);
	printf("Max: %f, %f, %f\n", xmax, ymax, zmax);*/


	//izracunaj srediste tijela
	

	xSrednji = (xmin + xmax) / 2;
	ySrednji = (ymin + ymax) / 2;
	zSrednji = (zmin + zmax) / 2;

	//postaviti u ishodiste
	//for (int i = 0; i < vNum; i++) {
	//	vrhovi[i][0] -= xSrednji;
	//	vrhovi[i][1] -= ySrednji;
	//	vrhovi[i][2] -= zSrednji;
	//}

	///*printf("vrhovi novi:\n");
	//for (int i = 0; i < vNum; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		printf("%f	", vrhovi[i][j]);
	//	}
	//	printf("\n");
	//}*/

	//skalirati tijelo na [-1, 1]
	double velX = xmax - xmin;
	double velY = ymax - ymin;
	double velZ = zmax - zmin;

	double max = velX;
	if (max < velY) max = velY;
	if (max < velZ) max = velZ;

	double skaliraj = 2 / max;
	for (int i = 0; i < vNum; i++) {
		for (int j = 0; j < 3; j++) {
			vrhovi[i][j] *= skaliraj;
		}
	}

	/*printf("vrhovi skalirani:\n");
	for (int i = 0; i < vNum; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%f	", vrhovi[i][j]);
		}
		printf("\n");
	}*/

	//double xTocka, yTocka, zTocka;
	

	cout << "unio podatke o objektu\n";
	
	objekt = 0;
}