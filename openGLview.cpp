#include "StdAfx.h"
#include "openGLview.h"
#include <time.h>
#include "math.h"

openGLview::openGLview(void)
{
	dInitODE();
	dynamicWorld.InitODE(); //inicjalizacja srodowiska ode
	
	selected_id=0;
	nadawanaSila=5;
	for(int i=0; i<4; i++)
		bufor[i]=0;
	numBufor=0;

	//gluLookAt( // ustawienie pocz¹tkowe kamery
	//0.0, 350.0, 3500.0,
	//0.0, 0.0, 0.0,
	//0.0, 0.0, 1.0);
}


openGLview::~openGLview(void)
{
}

void openGLview::keyboard(unsigned char key, int x, int y)// metoda w zaleÂżnooeci od wcisniĂŞtego przycisku wywoÂłuje odpowiednie zdarzenia
{
	if (key == 27) exit(0); // escape key is ascii 27 }
	if (key == 'd') { //key d prawo 
		if(selected_id==1 && dynamicWorld.Pad1Vel[0]<=0){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
		}
		if(selected_id==2 && dynamicWorld.Pad2Vel[0]<=0){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
		}
		dynamicWorld.dx[selected_id]=nadawanaSila;
	}
	if (key == 'a') { //key a lewo
		if(selected_id==1 && dynamicWorld.Pad1Vel[0]>=0){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
		}
		if(selected_id==2 && dynamicWorld.Pad2Vel[0]>=0){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
		}
		dynamicWorld.dx[selected_id]=-nadawanaSila; 
	}
	if (key == 101) { //key e góra 
	dynamicWorld.dy[selected_id]=5; }
	if (key == 113) { //key q dół
	dynamicWorld.dy[selected_id]=-5; }
	if (key == 's') { //key s do siebie
		if(selected_id==1 && dynamicWorld.Pad1Vel[2]<0){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, dynamicWorld.Pad1Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, dynamicWorld.Pad1Vel[0], 0, 0);
		}
		if(selected_id==2 && dynamicWorld.Pad2Vel[2]<0){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, dynamicWorld.Pad2Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, dynamicWorld.Pad2Vel[0], 0, 0);
		}
	dynamicWorld.dz[selected_id]=nadawanaSila; 
	}
	if (key == 'w') { //key w w głąb
		if(selected_id==1 && dynamicWorld.Pad1Vel[2]>0) {
			dBodySetLinearVel(dynamicWorld.pady[0].Body, dynamicWorld.Pad1Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, dynamicWorld.Pad1Vel[0], 0, 0);
		}
		if(selected_id==2 &&  dynamicWorld.Pad2Vel[2]>0){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, dynamicWorld.Pad2Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, dynamicWorld.Pad2Vel[0], 0, 0);
		}
	dynamicWorld.dz[selected_id]=-nadawanaSila;
	}

	// ------ wybor taktyki z klawiatury --------
	// 1. 'p'  
	// 2. nr pada (1 lub 2) 
	// 3. 't'
	// 4. nr taktyki (od 1 do 3) lub dowolny klawisz w celu sterowania recznego
	if(key == 'p') // znak 'p' oznacza rozpoczecia wpisywania taktyki
		numBufor=0; // powrot do 0 elementu tablicy
	if(numBufor==4)
		numBufor=0;
	bufor[numBufor]=key; // przypisanie znaku do bufora
	numBufor++;
	if(bufor[0] == 'p' && bufor[2] == 't')
	{
		if(bufor[1] == '1')
		{
			if(bufor[3] == '1')
				dynamicWorld.taktykaPad1 = 1;
			if(bufor[3] == '2')
				dynamicWorld.taktykaPad1 = 2;
			if(bufor[3] == '3')
				dynamicWorld.taktykaPad1 = 3;
			if(bufor[3] == '4')
				dynamicWorld.taktykaPad1 = 4;
		}

		if(bufor[1] == '2')
		{
			if(bufor[3] == '1')
				dynamicWorld.taktykaPad2 = 1;
			if(bufor[3] == '2')
				dynamicWorld.taktykaPad2 = 2;
			if(bufor[3] == '3')
				dynamicWorld.taktykaPad2 = 3;
			if(bufor[3] == '4')
				dynamicWorld.taktykaPad2 = 4;
		}
	}
	// ---- pokaz informacje o rozgrywce ----
	if(key == 'i')
	{
		if(dynamicWorld.info)
			dynamicWorld.info = false;
		else
			dynamicWorld.info = true;
	}
}

void openGLview::animacja(void) //funkcjê odpowiedzialna za zmiane pozycji i orientacji wszystkich figur na scenie
{

}

void openGLview::display(void) {
glClearColor(0,0,0,0);//ustawiamy kolor tła na czarny
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyoecimy bufory
glEnable(GL_DEPTH_TEST); //uruchamiamy bufor głębi


rysuj_figury();
	
glutSwapBuffers();
glutPostRedisplay(); // marks the current window as needing to be redisplayed 
}
void openGLview:: resize(int width, int height) {
glViewport(0, 0, width, height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0, (float)width / (float)height, 0.1, 1000.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt( // ustawienie pocz¹tkowe kamery
0.0, 100.0, 350.0,
0.0, 0.0, 0.0,
0.0, 1.0, 0.0);
}

void openGLview::loadTextureFromFile(char *filename)
{
	RgbImage theTexMap( filename );
	// Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
	// Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,theTexMap.GetNumCols(), theTexMap.GetNumRows(),
	GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
}

void openGLview::rysuj_figury(void)
{
	if(dynamicWorld.info == false)
	{
		gluLookAt( // ustawienie pocz¹tkowe kamery
0.0, 100.0, 350.0,
0.0, 0.0, 0.0,
0.0, 1.0, 0.0);
	}
	sprintf(tekst, "info: %d", dynamicWorld.info);
	drawText(-2.0, 3.7, -4.5, tekst);
	if(dynamicWorld.info)
	{
		sprintf(tekst, "bufor: %c %c %c %c", bufor[0], bufor[1], bufor[2], bufor[3]);
		drawText(-2.0, 3.4, -4.5, tekst);
	}
	dynamicWorld.SimStep(0.0001);
}

void openGLview::drawText(float x, float y, float z, char *string) // rysuje napis "string" w pozycji x, y
{
	glRasterPos3f(x, y, z);//pozycja napisu
	for (int i = 0; i < strlen (string); i++)//literka po literce
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
};
