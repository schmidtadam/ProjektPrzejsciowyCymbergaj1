#pragma once

#include <gl\glut.h> // glut.h includes gl.h and glu.h
#include <ode/ode.h> // ode library header
//#include <src/ccd/ccd.h>
#include "matrix.h"
#include "Ground.h"
#include "OdeGeom.h"

// some constants
#define DENSITY (0.5) // density of all objects
#define GEOMSPERBODY 1 // maximum number of geometries per body
#define MAX_CONTACTS 4 // maximum number of contact points per body

typedef struct MyObject //struktura reprezentujaca obiekty
{
	dBodyID Body; // the dynamics body
	dGeomID Geom[GEOMSPERBODY]; // geometries representing this body
} MyObject;

class COdeWorld
{
public:
	COdeWorld(void);
public:
	~COdeWorld(void);
	void SimStep(double dt); /// krok symulacji z ODE
	void InitODE(); /// inicjalizacja ODE
	void CloseODE();/// zamkniecie ODE
	void DrawGeom(dGeomID g, const dReal *pos, const dReal *R, float red, float green, float blue);//rysuje figury na scenie
	void setServo(int servo_nr, double value);
	void drawText(float x, float y, float z, char *string, float red, float green, float blue);//tekst

	MATRIX GeomMatrix;
	// dynamics and collision objects
	dSpaceID Space; // collision space
	dSpaceID Space2; // collision space
	dJointGroupID jointgroup; // contact group for the new joint
	dJointID Joint; // the joint ID
	MyObject Object[15]; // tablica z obiektami które wystepuja na scenie
	MyObject krazek;
	MyObject bramka[2];
	MyObject pady[4];
	CGround ground; //ziemia
	dGeomID plane; //ground in ode
	COdeGeom geometry; //klasa rysujaca obiekty na scenie

	int licznik1, licznik2;
	char wynik[20];
	int size;
	int dx[3], dy[3], dz[3]; // sfera


	dJointID Joints[10];
	dJointID Joints2[10];
	dJointID Joints_bramka[5];
	float ster[5];
	int pom; // zmienna iteruj¹ca obiegi pêtli glownej
};