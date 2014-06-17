#pragma once

#include <gl\glut.h> // glut.h includes gl.h and glu.h
#include <ode/ode.h> // ode library header
#include "matrix.h"
#include "Ground.h"
#include "OdeGeom.h"

typedef void (*PROCRunAtakOgrX)(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak, void* dane);
extern "C" void funkcjaAtakOgrX(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak, void* dane);

typedef void (*PROCRunAtak)(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak);
extern "C" void funkcjaAtak(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak);

typedef void (*PROCRunProsta)(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX);
extern "C" void funkcjaProsta(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX);

typedef void (*PROCRunDistanceX)(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX, void* dane);
extern "C" void funkcjaDistanceX(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX, void* dane);

// some constants
#define DENSITY (0.5) // density of all planszas
#define GEOMSPERBODY 1 // maximum number of geometries per body
#define MAX_CONTACTS 4 // maximum number of contact points per body

typedef struct Myplansza //struktura reprezentujaca obiekty
{
	dBodyID Body; // the dynamics body
	dGeomID Geom[GEOMSPERBODY]; // geometries representing this body
} Myplansza;

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
	void odczyt_dll();
	void funkcja_ustawiajaca();

	MATRIX GeomMatrix;
	// -------- dynamics and collision plansza ----------
	dSpaceID Space; // collision space
	dSpaceID Space2; // collision space
	dJointGroupID jointgroup; // contact group for the new joint
	dJointID Joint; // the joint ID
	Myplansza plansza[5]; // tablica z obiektami tworzacymi plansze
	Myplansza krazek;
	Myplansza bramka[2];
	Myplansza pady[4];
	Myplansza naroznik[4];
	CGround ground; //ziemia
	dGeomID plane; //ground in ode
	COdeGeom geometry; //klasa rysujaca obiekty na scenie
	
	dJointID Joints[10];
	dJointID Joints2[10];
	dJointID Joints_bramka[5];
	dJointID Joints_naroznik[4];
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
	float PadVel1[3];
	const dReal *Pad2Vel;
	float PadVel2[3];
	const dReal *Pad1Ang; //predkosci katowe padow
	const dReal *Pad2Ang;
	const dReal *KrazekVel; //predkosc krazka
	float KraVel[3];
	const dReal *KrazekAng; //predkosc katowa krazka

	bool atakPad1;
	bool* wskAtakPad1;
	bool atakPad2;
	bool* wskAtakPad2;

	bool czyWbramke; // zmienna przechowujaca informacje czy krazek zmierza w swiatlo bramki
	bool *wsk_czyWbramke;
	float celX; // wsp. [x], w ktorej krazek potencjalnie przetnie linie koncowa (wsp. [z] bramki)
	//-----------------------------------------------------------------------------------
	char wynik[25]; // tablica znakow do wyswietlania wynikow
	float dx[3], dy[3], dz[3]; // przyrost sily
	short int taktykaPad1;
	short int taktykaPad2;
	// --------- kolizje - liczniki ----------
	int licznik1, licznik2; // liczniki odbic krazka przez pady
	int golLicznik, golLicznik2; // liczniki goli
	bool flaga1; // zeby pozycje styczna liczyl jako jedno odbicie
	bool flaga2;
	
	//----------------------------------

	int petla; // zmienna iteruj¹ca obiegi pêtli glownej

	const dReal* rotation; // rotacja krazka
	dReal rotation2[12];

	bool info; // jesli zmienna jest prawdziwa to pokazywane sa informacje, jesli nie to znikaja
};