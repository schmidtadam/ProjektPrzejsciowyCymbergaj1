#pragma once

#include <gl\glut.h> // glut.h includes gl.h and glu.h
#include <ode/ode.h>
#include "matrix.h"//biblioteka obslugujaca macierze 4x4;
#include "RgbImage.h"

class COdeGeom
{
public:
	COdeGeom(void);
public:
	~COdeGeom(void);
	void DrawSphere(float radius, const float *pos, const float *R);//rysuje sfere
	void DrawCappedCylinder(const float * pos, const float *R, float radius, float length);//rysuje pigule
	void DrawBox(dReal * sides, const float * pos, const float * R);//rysuje szescian
	void loadTextureFromFile(char *filename);
	void DrawCylinder(const float * pos, const float *R, float radius, float length, float red, float green, float blue); // rusuje walec
	MATRIX GeomMatrix; //macierz do przeksztalcen
	//char* filename;
	//GLuint textures[3];
	
};