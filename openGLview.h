#pragma once
#include <gl\glut.h>
#include "OdeWorld.h"
#include "RgbImage.h"
class openGLview
{
public:
	openGLview(void);
public:
	~openGLview(void);
	void display(void);
	void resize(int width, int height);
	void rysuj_figury(void);
	void loadTextureFromFile(char *filename);

	int selected_id;//id wybranego obiektu na scenie
	char* filename;
	GLuint textures[3];
	
	COdeWorld dynamicWorld; //swiat symulujacy dynamike i kolizje pomiedzy obiektami

	void keyboard(unsigned char key, int x, int y);
	void animacja(void);
	void drawText(float x, float y, float z, char *string);

	int znak_pion;				//zmienna okreslajaca w ktorym kierunku jedzie pad (gora czy dol)
	int znak_poziom;			//zmienna okreslajaca w ktorym kierunku jedzie pad (lewo czy prawo)
};