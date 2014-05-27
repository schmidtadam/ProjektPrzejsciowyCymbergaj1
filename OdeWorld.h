#pragma once

#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <ctime>

#include <gl\glut.h> // glut.h includes gl.h and glu.h
#include <ode/ode.h> // ode library header
#include "matrix.h"
#include "Ground.h"
#include "OdeGeom.h"

typedef void (*PROCRunThisModule)(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);
extern "C" void funkcja_komputerowa(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);

typedef void (*PROCRunAtak)(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);
extern "C" void funkcjaAtak(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);

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
	void funkcja_komputerowa(float x1, float x2);
	void odczyt_dll();
	void funkcja_komputerowa2(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);
	void funkcja_ustawiajaca();

	MATRIX GeomMatrix;
	// -------- dynamics and collision objects ----------
	dSpaceID Space; // collision space
	dSpaceID Space2; // collision space
	dJointGroupID jointgroup; // contact group for the new joint
	dJointID Joint; // the joint ID
	MyObject Object[5]; // tablica z obiektami które wystepuja na scenie
	MyObject krazek;
	MyObject bramka[2];
	MyObject pady[4];
	MyObject naroznik[4];
	CGround ground; //ziemia
	dGeomID plane; //ground in ode
	COdeGeom geometry; //klasa rysujaca obiekty na scenie

	// ------------ struktura przekazywana do funkcji nearCallback --------
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
	// ------------- pobrane wartosci (pozycja, predkosc, pr. obrotowa) -----------------
	dVector3 posStol[5]; // pozycje elementow stolu
	dVector3 posNaroznik[4];
	dVector3 posKrazek;
	dVector3 posPad1;
	dVector3 posPad2;
	dVector3 posPAD2;
	dVector3 posBramka[2];

	const dReal *Pad1Vel; //predkosci padow
	float PadVel[4];
	const dReal *Pad2Vel;
	float PadVel2[4];
	const dReal *Pad1Ang; //predkosci katowe padow
	const dReal *Pad2Ang;
	const dReal *KrazekVel; //predkosc krazka
	float KraVel[3];
	const dReal *KrazekAng; //predkosc katowa krazka
	float* wsk;
	float predkoscCz[4];
	float predkoscZiel[4];
	//-----------------------------------------------------------------------------------
	char wynik[20]; // tablica znakow do wyswietlania wynikow
	int size;
	float dx[3], dy[3], dz[3]; // przyrost sily
	float sila;			// zmienna do sterowania komputerowego
	// --------- kolizje - liczniki ----------
	int licznik1, licznik2; // liczniki odbic krazka przez pady
	int golLicznik, golLicznik2; // liczniki goli
	bool flaga1; // zeby pozycje styczna liczyl jako jedno odbicie
	bool flaga2;
	
	//----------------------------------
	dJointID Joints[10];
	dJointID Joints2[10];
	dJointID Joints_bramka[5];
	dJointID Joints_naroznik[4];
	int petla; // zmienna iteruj¹ca obiegi pêtli glownej
};