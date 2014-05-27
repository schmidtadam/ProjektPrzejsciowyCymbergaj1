#include "StdAfx.h"
#include "openGLview.h"
#include <time.h>
#include "math.h"

openGLview::openGLview(void)
{
	dInitODE();
	dynamicWorld.InitODE(); //inicjalizacja srodowiska ode
	dynamicWorld.size=2;
	selected_id=0;
	znak_poziom=0;
	znak_pion=0;
}


openGLview::~openGLview(void)
{
}

void openGLview::keyboard(unsigned char key, int x, int y)// metoda w zaleÂżnooeci od wcisniĂŞtego przycisku wywoÂłuje odpowiednie zdarzenia
{
	if (key == 27) exit(0); // escape key is ascii 27 }
	if (key == 100) { //key d prawo 
		if(znak_poziom<0)
		{
			if(selected_id==1){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
			}
			if(selected_id==2){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
			}
			znak_poziom=1;
		}
		dynamicWorld.dx[selected_id]=5; }
	if (key == 97) { //key a lewo
		if(znak_poziom>=0)
		{
			if(selected_id==1){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, 0, 0, dynamicWorld.Pad1Vel[2]);
			}
			if(selected_id==2){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, 0, 0, dynamicWorld.Pad2Vel[2]);
			}
			znak_poziom=-1;
		}		
		dynamicWorld.dx[selected_id]=-5; }
	if (key == 101) { //key e góra 
	dynamicWorld.dy[selected_id]=5; }
	if (key == 113) { //key q dół
	dynamicWorld.dy[selected_id]=-5; }
	if (key == 115) { //key s do siebie
		if(znak_pion>=0)
		{
			if(selected_id==1){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, dynamicWorld.Pad1Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, dynamicWorld.Pad1Vel[0], 0, 0);
			}
			if(selected_id==2){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, dynamicWorld.Pad2Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, dynamicWorld.Pad2Vel[0], 0, 0);
			}
			znak_pion=-1;
		}
	dynamicWorld.dz[selected_id]=5; }
	if (key == 119) { //key w w głąb
		if(znak_pion<0)
		{
			if(selected_id==1){
			dBodySetLinearVel(dynamicWorld.pady[0].Body, dynamicWorld.Pad1Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[1].Body, dynamicWorld.Pad1Vel[0], 0, 0);
			}
			if(selected_id==2){
			dBodySetLinearVel(dynamicWorld.pady[2].Body, dynamicWorld.Pad2Vel[0], 0, 0);
			dBodySetLinearVel(dynamicWorld.pady[3].Body, dynamicWorld.Pad2Vel[0], 0, 0);
			}
			znak_pion=1;
		}
	dynamicWorld.dz[selected_id]=-5; }
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
0.0, 0.0, 350.0,
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
	dynamicWorld.SimStep(0.0001);
}

void openGLview::drawText(float x, float y, float z, char *string) // rysuje napis "string" w pozycji x, y
{
	glRasterPos3f(x, y, z);//pozycja napisu
	for (int i = 0; i < strlen (string); i++)//literka po literce
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
};
