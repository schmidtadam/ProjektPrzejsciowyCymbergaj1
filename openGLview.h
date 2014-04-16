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
	void drawText(int x, int y, char *string);
};