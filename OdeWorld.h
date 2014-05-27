#pragma once

#include <gl\glut.h> // glut.h includes gl.h and glu.h
#include <ode/ode.h> // ode library header
#include "matrix.h"
#include "Ground.h"
#include "OdeGeom.h"

typedef float (*PROCRunThisModule)(float x1, float x2, float sila);
extern "C" float funkcja_komputerowa(float x1, float x2, float sila);
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
	void funkcja_komputerowa(float x1, float x1_stare, float x2);
	void odczyt_dll(float x1, float x2, float force);
	void funkcja_ustawiajaca();

	MATRIX GeomMatrix;
	// dynamics and collision objects
	dSpaceID Space; // collision space
	dSpaceID Space2; // collision space
	dJointGroupID jointgroup; // contact group for the new joint
	dJointID Joint; // the joint ID
	MyObject Object[5]; // tablica z obiektami które wystepuja na scenie
	MyObject krazek;
	MyObject bramka[2];
	MyObject pady[4];
	CGround ground; //ziemia
	dGeomID plane; //ground in ode
	COdeGeom geometry; //klasa rysujaca obiekty na scenie

	/*dGeomID mojeDane[5];*/
	struct mD
	{
		dGeomID krazek;
		dGeomID boisko;
		dGeomID Pad1;
		dGeomID Pad2;
		dGeomID bramka1;
		dGeomID bramka2;
		bool trafienie1;
		bool trafienie2;
	}mD;

	dVector3 posStol[5]; // pozycje elementow stolu
	dVector3 posKrazek;
	dVector3 posPAD2;

	char wynik[20]; // tablica znakow do wyswietlania wynikow
	int size;
	float dx[3], dy[3], dz[3]; // przyrost sily
	float sila;			// zmienna do sterowania komputerowego
	float stare_x;		// zmienna do sterowania komputerowego
	// --------- test kolizji ----------
	
	int licznik1, licznik2;
	int golLicznik, golLicznik2;
	bool flaga1; // zeby pozycje styczna liczyl jako jedno odbicie
	bool flaga2;
	
	//----------------------------------
	dJointID Joints[10];
	dJointID Joints2[10];
	dJointID Joints_bramka[5];
	float ster[5];
	int petla; // zmienna iteruj¹ca obiegi pêtli glownej
	
	const dReal *Pad1Vel;
	const dReal *Pad2Vel;
};