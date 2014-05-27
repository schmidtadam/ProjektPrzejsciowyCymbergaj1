#include "StdAfx.h"
#include "OdeWorld.h"
dWorldID World; // the ode simulation world
dJointGroupID contactgroup; // wykorzystywane w kolizjach
// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	int i;
	
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
	};
	mD *dane=(mD *)(data);
	dGeomID krazekGeom=dane->krazek;
	dGeomID boiskoGeom=dane->boisko;
	dGeomID pad1Geom=dane->Pad1;
	dGeomID pad2Geom=dane->Pad2;
	dGeomID bramka1Geom=dane->bramka1;
	dGeomID bramka2Geom=dane->bramka2;
	
	if(o2==krazekGeom)
	{
		dGeomID temp=o1;
		o1=o2;
		o2=temp;
	}
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if(dane->trafienie1==false && o1==krazekGeom && o2==bramka1Geom)
		dane->trafienie1=true;
	if(dane->trafienie2==false && o1==krazekGeom && o2==bramka2Geom)
		dane->trafienie2=true;

	dContact contact[MAX_CONTACTS]; // up to MAX_CONTACTS contacts per box-box
	if((o1==boiskoGeom && o2==pad2Geom) || (o1==boiskoGeom && o2==pad1Geom) || (o2==boiskoGeom && o1==pad2Geom) || (o2==boiskoGeom && o1==pad1Geom) ||
		(o1==krazekGeom && o2==boiskoGeom)) // kontakt padow i krazka z boiskiem
	{
		for (i = 0; i < MAX_CONTACTS; i++)
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = 0.001; // change from 1  - tarcie
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0; // changed from 0.9 - bez odbijania
			contact[i].surface.bounce_vel = 0; // changed from 0.5
			contact[i].surface.soft_cfm = 0.1;
			contact[i].surface.slip1 = 0; // change from 0
			contact[i].surface.slip2 = 0;
		}
	}
	//if(o1==pad1Geom || o1==pad2Geom || o2==pad1Geom || o2==pad2Geom || o1==krazekGeom) // jesli krazek uczestniczy w kolizji to male tarcie
	else
	{
		for (i = 0; i < MAX_CONTACTS; i++)
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = 0.01; // change from 1  - tarcie
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.9; // changed from 0.9
			contact[i].surface.bounce_vel = 0.1; // changed from 0.5 minimalna predkosci przy ktorej nastepuje odbicie
			contact[i].surface.soft_cfm = 0.01;
			contact[i].surface.slip1 = 0; // change from 0
			contact[i].surface.slip2 = 0;
		}
	}
	//else
	//	for (i = 0; i < MAX_CONTACTS; i++)
	//	{
	//		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
	//		contact[i].surface.mu = 0.1; // change from 1  - tarcie
	//		contact[i].surface.mu2 = 0;
	//		contact[i].surface.bounce = 0.3; // changed from 0.9
	//		contact[i].surface.bounce_vel = 0.3; // changed from 0.5
	//		contact[i].surface.soft_cfm = 0.1;
	//		contact[i].surface.slip1 = 0; // change from 0
	//		contact[i].surface.slip2 = 0;
	//	}
	if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
	{
		for (i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(World, contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

COdeWorld::COdeWorld(void)
{
	petla=0;
	flaga1=true;
	flaga2=true;
	licznik1=0;
	licznik2=0;
	golLicznik=0;
	golLicznik2=0;
	sila=0;
	predkoscCz[3]=1;
	predkoscZiel[3]=1;
	srand(time(0));				//punkt poczatkowy losowania
	
}
COdeWorld::~COdeWorld(void)
{
	dJointGroupDestroy(jointgroup);
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(Space);
	dWorldDestroy(World);
}
void COdeWorld::funkcja_komputerowa(float x1, float x2)
{
	if(abs(x1-x2)>0.015 )
	{
		if(x1>x2)
		{
			if(sila<0.5)
			{
				sila+=0.1;
				dx[2]=sila;
			}
			else
				sila=0;
		}
		if(x1<x2)
		{
			if(sila>-0.5)
			{
				sila-=0.1;
				dx[2]=sila;
			}
			else
				sila=0;
		}
	}
	
	//dBodySetPosition(pady[0].Body, 0, 0.055, 0.25);	//ustawienie pozycji pada1
	//dBodySetPosition(pady[1].Body, 0, 0.07, 0.25);  //ustawienie pozycji pada1
	//dBodySetPosition(pady[2].Body, 0, 0.055, -0.25);//ustawienie pozycji pada2
	//dBodySetPosition(pady[3].Body, 0, 0.07, -0.25);	//ustawienie pozycji pada2
}
//*********************************DLL***************************************
void COdeWorld::odczyt_dll()
{
	HINSTANCE hDll;
	hDll = LoadLibrary("VSDLL.dll" );

	if( hDll != NULL )
	{
		// jeœli wszystko posz³o dobrze, tutaj mo¿emy wywo³aæ jak¹œ funkcjê biblioteczn¹
		//FunkcjaOdczytu =( MYPROC ) GetProcAddress( hDll, "funkcja_komputerowa" );
		PROCRunThisModule FunkcjaOdczytu = (PROCRunThisModule)GetProcAddress(hDll, "funkcja_komputerowa"); 
		PROCRunAtak FunkcjaAtak = (PROCRunAtak)GetProcAddress(hDll, "funkcjaAtak");
		if( FunkcjaOdczytu != NULL )
		{
			FunkcjaOdczytu(posKrazek, posPad2, KraVel, PadVel2, predkoscZiel);
			dBodySetLinearVel(pady[2].Body, predkoscZiel[0], predkoscZiel[1], predkoscZiel[2]);
			dBodySetLinearVel(pady[3].Body, predkoscZiel[0], predkoscZiel[1], predkoscZiel[2]);
			
		}
		else
			drawText(-3.5, 1.6, -4.5, "BLAD", 1.0, 1.0, 1.0);
    
		if( FunkcjaAtak != NULL )
		{
			FunkcjaAtak(posKrazek, posPad1, KraVel, PadVel, predkoscCz);
			if(petla==400)
			{
				dBodySetLinearVel(pady[0].Body, predkoscCz[0], predkoscCz[1], predkoscCz[2]);
				dBodySetLinearVel(pady[1].Body, predkoscCz[0], predkoscCz[1], predkoscCz[2]);
			}
		}
		else
			drawText(-3.5, 1.6, -4.5, "BLAD funkcji z atakiem", 1.0, 1.0, 1.0);
		FreeLibrary( hDll );
	
	}
	else
		drawText(-3.5, 1.6, -4.5, "BLAD2", 1.0, 1.0, 1.0);
}

void COdeWorld::funkcja_komputerowa2(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc)
{
	predkosc[0]=velPad[0];
	predkosc[1]=velPad[1];
	predkosc[2]=velPad[2];
	predkosc[3]=velPad[3];
	bool atak;
	if(predkosc[3]==1)
		atak=true;
	else
		atak=false;

	if(abs(posKra[0] - posPad[0]) > 0.015)
	{
		if(posKra[0]>posPad[0])
		{
			if(predkosc[0]<10){
				predkosc[0]+=0.5;
			}
			else
				predkosc[0]=10;
		}
		if(posKra[0]<posPad[0])
		{
			if(predkosc[0]>-10){
				predkosc[0]-=0.5;
			}
			else
				predkosc[0]=-10;
		}
	}
	else
		predkosc[0]=0;
	//------------- atak ----------------
	float prAtaku=15;
	if(posPad[2]<=0.1)
		atak=false;
	
	if(atak && posKra[2]>0.1) 
	{
		predkosc[2]=-prAtaku; // do przodu
	}
	
	if(atak==false && posPad[2]<0.25)
	{
		predkosc[2]=prAtaku; // do tylu
	}
	if(atak==false && posPad[2]>0.25)
	{
		predkosc[2]=0; // zatrzymanie
		atak=true;
	}
	if(atak)
		predkosc[3]=1;
	else
		predkosc[3]=0;

	
	//bool atak;
	//float predkosc[4];
	//predkosc[0]=velPad[0];
	//predkosc[2]=velPad[2];
	//predkosc[3]=velPad[3];
	//if(predkosc[3]==1)
	//	atak=true;
	//if(predkosc[3]==0)
	//	atak=false;
	//if(abs(posKra[0]-posPad[0])>0.015 )
	//{
	//	if(posKra[0]>posPad[0])
	//	{
	//		if(predkosc[0]<10){
	//			predkosc[0]+=0.5;
	//		}
	//		else
	//			predkosc[0]=10;
	//	}
	//	if(posKra[0]<posPad[0])
	//	{
	//		if(predkosc[0]>-10){
	//			predkosc[0]-=0.5;
	//		}
	//		else
	//			predkosc[0]=-10;
	//	}
	//}
	//else
	//	predkosc[0]=0;
	////-------atak-------------
	//if(posPad[2]<=0.1)
	//	atak=false;
	//
	//if(atak && posKra[2]>0.1)
	//{
	//	predkosc[2]=-15;
	//}
	//
	//if(atak==false && posPad[2]<0.25)
	//{
	//	predkosc[2]=15;
	//}
	//if(atak==false && posPad[2]>0.25)
	//{
	//	predkosc[2]=0;
	//	atak=true;
	//}
	//if(atak)
	//	predkosc[3]=1;
	//else
	//	predkosc[3]=0;
	//sprintf(wynik, "atak %d", atak);
	//drawText(2.0, 2.2, -4.5, wynik, 1.0, 1.0, 1.0);
	//return predkosc;
}



//********************************KONIEC DLL*********************************
void COdeWorld::funkcja_ustawiajaca()
{
	dBodySetPosition(pady[0].Body, 0, 0.055, 0.25);	//ustawienie pozycji pada1
	dBodySetPosition(pady[1].Body, 0, 0.07, 0.25);  //ustawienie pozycji pada1
	dBodySetPosition(pady[2].Body, 0, 0.055, -0.35);//ustawienie pozycji pada2
	dBodySetPosition(pady[3].Body, 0, 0.07, -0.35);	//ustawienie pozycji pada2
	dBodySetLinearVel(pady[0].Body, 0, 0, 0);		//predkosci=0
	dBodySetLinearVel(pady[1].Body, 0, 0, 0);
	dBodySetLinearVel(pady[2].Body, 0, 0, 0);
	dBodySetLinearVel(pady[3].Body, 0, 0, 0);

}
// Draw a geom
void COdeWorld::DrawGeom(dGeomID g, const dReal *pos, const dReal *R, float red, float green, float blue)
{
	if (!g)
		return;
	if (!pos)
		pos = dGeomGetPosition(g);//pobiera pozycje obiektu
	if (!R) R = dGeomGetRotation(g); //pobiera orientacje obiektu
		int type = dGeomGetClass(g); //pobiera klase obiektu
	if (type == dSphereClass)//rysuje sfere
	{
		dReal radius;
		radius = dGeomSphereGetRadius(g);
		geometry.DrawSphere(radius, (const float*)pos, (const float*)R); //rysuje sfere
	}
	
	if (type == dCylinderClass)//rysuje pigule
	{
		dReal radius;
		dReal length;
		dGeomCylinderGetParams (g, &radius, &length);
		geometry.DrawCylinder((const float*)pos,(const float*) R, radius, length, red, green, blue);//rysuje walec
	}
	if (type == dBoxClass) //rysuje szescian
	{
		dReal sides[3];
		dGeomBoxGetLengths(g, sides);
		geometry.DrawBox(sides, (const float*)pos, (const float*)R, red, green, blue); //rysuje szescian
	}
}
// Simulation step
void COdeWorld::SimStep(double dt)
{
	
	dSpaceCollide(Space, (void*)(&mD), &nearCallback);//ustawia sprawdzanie kolizji
	dWorldQuickStep(World, dt); //wykonuje krok symulacji
	dJointGroupEmpty(contactgroup);
	for (int bodies = 0;bodies<5;bodies++)//rysowanie obiektow
		DrawGeom(Object[bodies].Geom[0], 0, 0, 1, 1, 1); //rysuje stol (bialy)
	glPushName(1); //id obiektu - oblsuga myszki
	DrawGeom(pady[0].Geom[0], 0, 0, 1, 0, 0); //rysuje pady
	glPushName(1); //id obiektu - oblsuga myszki
	DrawGeom(pady[1].Geom[0], 0, 0, 1, 0, 0);
	glPushName(2); //id obiektu - oblsuga myszki
	DrawGeom(pady[2].Geom[0], 0, 0, 0, 1, 0);
	glPushName(2); //id obiektu - oblsuga myszki
	DrawGeom(pady[3].Geom[0], 0, 0, 0, 1, 0);
	glPushName(0); //id obiektu - oblsuga myszki
	
	DrawGeom(krazek.Geom[0], 0, 0, 0, 0, 1); //rysuje krazek
	DrawGeom(bramka[0].Geom[0], 0, 0, 0, 1, 1); //rysuje bramke 1 (blekitne)
	DrawGeom(bramka[1].Geom[0], 0, 0, 0, 1, 1); //rysuje bramke 2
	// --------- do usuniecia po przetestowaniu naroznikow --------------
	DrawGeom(naroznik[0].Geom[0], 0, 0, 1, 0, 0);
	DrawGeom(naroznik[1].Geom[0], 0, 0, 1, 0, 0);
	DrawGeom(naroznik[2].Geom[0], 0, 0, 1, 0, 0);
	DrawGeom(naroznik[3].Geom[0], 0, 0, 1, 0, 0);
	//-------------------------------------------------------------------
	ground.DrawGrid(); //rysuje ziemie

	// -------- Pobieranie danych obiektow ------------
	dBodyCopyPosition (krazek.Body, posKrazek); // Pobranie nowej pozycji krazka
	KrazekVel=dBodyGetLinearVel (krazek.Body); // Pobranie pr. liniowej krazka
	KrazekAng=dBodyGetAngularVel (krazek.Body); // Pobranie pr. obrotowej krazka
	dBodyCopyPosition (pady[0].Body, posPad1); // Pobranie pozycji pada 1
	dBodyCopyPosition (pady[2].Body, posPad2); // Pobranie pozycji pada 2
	Pad1Vel=dBodyGetLinearVel (pady[0].Body); // Pobranie pr. pada 1
	Pad2Vel=dBodyGetLinearVel (pady[2].Body); // Pobranie pr. pada 2
	Pad1Ang=dBodyGetAngularVel (pady[0].Body); // Pobranie pr. obrotowej pada 1
	Pad2Ang=dBodyGetAngularVel (pady[2].Body); // Pobranie pr. obrotowej pada 2

	sprintf(wynik, "Predkosc pada: %.3f", predkoscCz[3]/*Pad2Vel[0]*/);
	drawText(-2.0, 2.5, -4.5, wynik, 1.0, 1.0, 1.0);

	sprintf(wynik, "Predkosc pada: %.3f", Pad1Vel[2]);
	drawText(2.0, 2.5, -4.5, wynik, 1.0, 1.0, 1.0);

	sprintf(wynik, "Petla: %d sila %f", petla, sila);
	drawText(-1.5, 1.6, -4.5, wynik, 1.0, 1.0, 1.0);
	sprintf(wynik, "x:%.3f y:%.3f z:%.3f", /*Pad1Ang[0], Pad1Ang[1], Pad1Ang[2]*/posKrazek[0], posKrazek[1], posKrazek[2]);
	drawText(-1.5, 1.3, -4.5, wynik, 1.0, 1.0, 1.0);
	// --------------- sprawdzanie i liczenie goli -------------------
	if(mD.trafienie1==true)
	{
		float liczba = float (rand()%21-10);				//losowanie pozycji krazka
		liczba=liczba/1000;
		golLicznik++;
		dBodySetPosition(krazek.Body, liczba, 0.052, 0.2);
		dBodySetLinearVel(krazek.Body, 0, 0, 0);
		funkcja_ustawiajaca();
		mD.trafienie1=false;
	}
	if(mD.trafienie2==true)
	{
		float liczba = float (rand()%21-10);				//losowanie pozycji krazka
		liczba=liczba/1000;
		golLicznik2++;
		dBodySetPosition(krazek.Body, liczba, 0.052, -0.27);
		dBodySetLinearVel(krazek.Body, 0, 0, 0);
		funkcja_ustawiajaca();
		mD.trafienie2=false;
	}
	sprintf(wynik, "Trafienia: %d : %d", golLicznik, golLicznik2);
	drawText(-1.5, 1.0, -4.5, wynik, 1.0, 1.0, 1.0);
	//-----------wywolanie funkcji komputerowej------------
	dVector3 posPAD2;
	dBodyCopyPosition (pady[2].Body, posPAD2); // Pobranie pozycji
	//funkcja_komputerowa(posKrazek[0], posPAD2[0]);
	
	// konwersja const dReal --> float
	KraVel[0]=KrazekVel[0]; 
	KraVel[1]=KrazekVel[1];
	KraVel[2]=KrazekVel[2];
	PadVel[0]=Pad1Vel[0];
	PadVel[1]=Pad1Vel[1];
	PadVel[2]=Pad1Vel[2];
	PadVel2[0]=Pad2Vel[0];
	PadVel2[1]=Pad2Vel[1];
	PadVel2[2]=Pad2Vel[2];
	// - - - - - - - - - - -
	PadVel[3]=predkoscCz[3]; // indeks "3" pelni role wartosci logicznej czy atakowac
	PadVel2[3]=predkoscZiel[3]; // indeks "3" pelni role wartosci logicznej czy atakowac
			//predkoscZiel[0]=Pad2Vel[0];
	
	//funkcja_komputerowa2(posKrazek, posPad1, KraVel, PadVel, wsk);
	
	sprintf(wynik, "Pr. pada: %.2f %.2f atak: %0.f", predkoscCz[0], predkoscCz[2], predkoscCz[3]);
	drawText(0, 3, -4.5, wynik, 1.0, 1.0, 1.0);

	if(petla==400)
		odczyt_dll(); // wywolanie odczytu DLL i wraz z nim sterowania komputerowego
	
	//----------- sterowanie obiektami ---------------------
	if(dx[1] || dz[1] !=0)
	{
		dBodySetForce(pady[0].Body, dx[1], 0, dz[1]);
		dx[1]=0;dz[1]=0;
	}
	if(dx[2] || dz[2] !=0)
	{
		dBodySetForce(pady[2].Body, dx[2], 0, dz[2]);
		dx[2]=0;dz[2]=0;
	}
	// ----------- Ograniczenia obrotow i pozycji po ustaleniu sie pozycji obiektow -----------
	if(petla<400)
	{
		petla++;
	}
	if(petla==1)
	{
		dBodySetPosition(pady[0].Body, 0, 0.07, 0.25);	//ustawienie pocz¹tkowe pada1
		dBodySetPosition(pady[1].Body, 0, 0.07, 0.265);	//ustawienie pocz¹tkowe pada1
		dBodySetPosition(pady[2].Body, 0, 0.07, -0.35);	//ustawienie pocz¹tkowe pada2
		dBodySetPosition(pady[3].Body, 0, 0.07, -0.35); //ustawienie pocz¹tkowe pada2
		float liczba = float (rand()%21-10);							//losowanie pozycji krazka
		liczba=liczba/1000;
		dBodySetPosition(krazek.Body, liczba, 0.063, 0.2); // krazek
	}
	if(petla==400)
	{
		dBodySetAngularVel(krazek.Body, 0, 0, 0); // ograniczenie pr. obrotowej krazka
		dBodySetLinearVel(krazek.Body, *KrazekVel, 0, *(KrazekVel+2)); // ograniczenie pr. liniowej w osi y krazka
	
		dBodySetLinearVel(pady[0].Body, *Pad1Vel, 0, *(Pad1Vel+2)); // ograniczenie pr. liniowej w osi y pada1
		//dBodySetAngularVel(pady[0].Body, 0, 0, 0); // ograniczenie pr. obrotowej pada1
		dBodySetLinearVel(pady[1].Body, *Pad1Vel, 0, *(Pad1Vel+2)); // ograniczenie pr. liniowej w osi y pada1
		//dBodySetAngularVel(pady[1].Body, 0, 0, 0); // ograniczenie pr. obrotowej pada1

		dBodySetLinearVel(pady[2].Body, *Pad2Vel, 0, *(Pad2Vel+2)); // ograniczenie pr. liniowej w osi y pada2
		//dBodySetAngularVel(pady[2].Body, 0, 0, 0); // ograniczenie pr. obrotowej pada2
		dBodySetLinearVel(pady[3].Body, *Pad2Vel, 0, *(Pad2Vel+2)); // ograniczenie pr. liniowej w osi y pada2
		//dBodySetAngularVel(pady[3].Body, 0, 0, 0); // ograniczenie pr. obrotowej pada2
	}

	// ------------ pozycja stolu - zablokowanie POZYCJI (z mozliwoscia blokady PREDKOSCI) ---------------
	//for(int i=0; i<5; i++)
	//{
	//	dBodySetLinearVel(Object[i].Body, 0, 0, 0); //zablokowanie PREDKOSCI stolu, zeby sie nie poruszal
	//}
	if(petla==20)
	{
		for(int i=0; i<5; i++)
		{
			dBodyCopyPosition (Object[i].Body, posStol[i]); // Pobranie poczatkowej pozycji stolu
			if(i<4)
				dBodyCopyPosition (naroznik[i].Body, posNaroznik[i]); // Pobranie poczatkowej pozycji naroznikow
			if(i<2)
				dBodyCopyPosition (bramka[i].Body, posBramka[i]); // Pobranie poczatkowej pozycji bramek
		}
	}
	if(petla>20)
		for(int i=0; i<5; i++)
		{
			dBodySetPosition(Object[i].Body, posStol[i][0], posStol[i][1], posStol[i][2]); //zablokowanie POZYCJI stolu, zeby sie nie poruszal
			if(i<4)
				dBodySetPosition(naroznik[i].Body, posNaroznik[i][0], posNaroznik[i][1], posNaroznik[i][2]); // zablokowanie pozycji naro¿ników
			if(i<2)
				dBodySetPosition(bramka[i].Body, posBramka[i][0], posBramka[i][1], posBramka[i][2]); // zablokowanie pozycji bramek
		}
	// ---------------- obliczanie kolizji z krazekiem ------------------
	
	dReal distance1=sqrt(pow(posKrazek[0]-posPad1[0],2)+pow(posKrazek[2]-posPad1[2],2));
	dReal distance2=sqrt(pow(posKrazek[0]-posPad2[0],2)+pow(posKrazek[2]-posPad2[2],2));
	sprintf(wynik, "Odleglosc");
	drawText(1.2, 1.3, -4.5, wynik, 1.0, 1.0, 1.0);
	sprintf(wynik, "Pad1-krazek: %.3f", distance2);
	drawText(1.2, 1.0, -4.5, wynik, 1.0, 1.0, 1.0);
	if(distance1<=0.03) // suma promieni krazka i pada1 to 0.3
		{
			if(flaga1)
			{
				licznik1++;
				flaga1=false;
			}
		}
	else
		flaga1=true;
	if(distance2<=0.03) // suma promieni krazka i pada2 to 0.3
		{
			if(flaga2)
			{
				licznik2++;
				flaga2=false;
			}
		}
	else
		flaga2=true;
	sprintf(wynik, "Odbicia krazka");
	drawText(-1.5, 2.2, -4.5, wynik, 1.0, 1.0, 1.0);
	sprintf(wynik, "%d : %d", licznik1, licznik2);
	drawText(-1.5, 1.9, -4.5, wynik, 1.0, 1.0, 1.0);
	//-------------------------------------------------------------------
}
void COdeWorld::InitODE()
{
	World = dWorldCreate();//tworzenie
	Space = dHashSpaceCreate(0);//swiata
	contactgroup = dJointGroupCreate(0);//tworzenie punktow styku
	jointgroup = dJointGroupCreate(0);// i zlacz
	dCreatePlane(Space, 0, 1, 0, 0);//ziemia
	dWorldSetGravity(World, 0, -9.81*15,0);//ustawienie grawitacji
	dWorldSetCFM(World, 1e-5);//ustawienie parametrow
	dWorldSetERP(World, 0.2);// silnika
	dWorldSetContactMaxCorrectingVel(World, 0.9);//open
	dWorldSetContactSurfaceLayer(World, 0);//dynamics
	dWorldSetAutoDisableFlag(World, 0);//engine
	//kilka zmiennych pomocniczych
	dReal sides[3];//wymiary szescianu
	dReal radius; // promien krazka
	dReal length; // wysokosc krazka
	dMass m;//masa
	dMatrix3 R;//macierz rotacji
	double mass=DENSITY * 10;
	//inicjalizacja obiektow znajdujacych sie w swiecie
	
	//----- podstawa
	Object[0].Body = dBodyCreate(World);//tworzony jest obiekt
	sides[0] = 0.40;//ustalane sa wymiary szescianu
	sides[1] = 0.05;
	sides[2] = 0.80;
	dBodySetPosition(Object[0].Body, 0.0, sides[1]/2, 0); // ustawienie pozycji
	dBodySetLinearVel(Object[0].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie pocatkowej orientacji obiektu
	dBodySetRotation(Object[0].Body, R);
	dMassSetBox(&m, mass, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	Object[0].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(Object[0].Geom[0], Object[0].Body);//powiazanie wymiarow i masy
	dBodySetMass(Object[0].Body, &m);
	// ------------------ bandy ------------------
	float bandy_h=0.02; // wysokosc band
	float bandy_w=0.02; // szerokosc band

	Object[1].Body = dBodyCreate(World);//tworzona jest prawa banda
	sides[0] = bandy_w;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = 0.80;
	dBodySetPosition(Object[1].Body, 0.19, 0.06, 0.0); // ustawienie pozycji
	dBodySetLinearVel(Object[1].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie pocatkowej orientacji obiektu
	dBodySetRotation(Object[1].Body, R);
	dMassSetBox(&m, mass, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	Object[1].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(Object[1].Geom[0], Object[1].Body);//powiazanie wymiarow i masy
	dBodySetMass(Object[1].Body, &m);
	
	Object[2].Body = dBodyCreate(World);//tworzona jest lewa banda
	sides[0] = bandy_w;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = 0.80;
	dBodySetPosition(Object[2].Body, -0.19, 0.06, 0); // ustawienie pozycji
	dBodySetLinearVel(Object[2].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie pocatkowej orientacji obiektu
	dBodySetRotation(Object[2].Body, R);
	dMassSetBox(&m, mass, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	Object[2].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(Object[2].Geom[0], Object[2].Body);//powiazanie wymiarow i masy
	dBodySetMass(Object[2].Body, &m);

	Object[3].Body = dBodyCreate(World);//tworzona jest banda tylnia
	sides[0] = 0.36;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = bandy_h;
	dBodySetPosition(Object[3].Body, 0.0, 0.06, -0.39); // ustawienie pozycji
	dBodySetLinearVel(Object[3].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie pocatkowej orientacji obiektu
	dBodySetRotation(Object[3].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	Object[3].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(Object[3].Geom[0], Object[3].Body);//powiazanie wymiarow i masy
	dBodySetMass(Object[3].Body, &m);

	Object[4].Body = dBodyCreate(World);//tworzona jest banda przednia
	sides[0] = 0.36;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = bandy_h;
	dBodySetPosition(Object[4].Body, 0.0, 0.06, 0.39); // ustawienie pozycji
	dBodySetLinearVel(Object[4].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(Object[4].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	Object[4].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(Object[4].Geom[0], Object[4].Body);//powiazanie wymiarow i masy
	dBodySetMass(Object[4].Body, &m);

	//---------------- Jointy band -------------------------
	Joints[0] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints[0], Object[0].Body, Object[1].Body);
    dJointSetHingeAnchor(Joints[0], 0.19, 0, 0);//pozycja
    dJointSetHingeAxis(Joints[0], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints[0], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints[0], dParamHiStop, 0);//ruchu

	Joints[1] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints[1], Object[0].Body, Object[2].Body);
    dJointSetHingeAnchor(Joints[1], -0.19, 0, 0);//pozycja
    dJointSetHingeAxis(Joints[1], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints[1], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints[1], dParamHiStop, 0);//ruchu

	Joints[2] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints[2], Object[0].Body, Object[3].Body);
    dJointSetHingeAnchor(Joints[2], 0, 0, 0.39);//pozycja
    dJointSetHingeAxis(Joints[2], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints[2], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints[2], dParamHiStop, 0);//ruchu

	Joints[3] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints[3], Object[0].Body, Object[4].Body);
    dJointSetHingeAnchor(Joints[3], 0, 0, -0.39);//pozycja
    dJointSetHingeAxis(Joints[3], 0, 1, 0);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints[3], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints[3], dParamHiStop, 0);//ruchu
	
	//------------------ narozniki zeby krazek sie nie blokowal w rogach -------------------------
	naroznik[0].Body = dBodyCreate(World);//tworzona jest banda przednia
	sides[0] = 0.005;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = 0.005;
	dBodySetPosition(naroznik[0].Body, 0.1775, 0.06, 0.3775); // ustawienie pozycji
	dBodySetLinearVel(naroznik[0].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(naroznik[0].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	naroznik[0].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(naroznik[0].Geom[0], naroznik[0].Body);//powiazanie wymiarow i masy
	dBodySetMass(naroznik[0].Body, &m);

	naroznik[1].Body = dBodyCreate(World);//tworzona jest banda przednia
	sides[0] = 0.005;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = 0.005;
	dBodySetPosition(naroznik[1].Body, -0.1775, 0.06, 0.3775); // ustawienie pozycji
	dBodySetLinearVel(naroznik[1].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(naroznik[1].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	naroznik[1].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(naroznik[1].Geom[0], naroznik[1].Body);//powiazanie wymiarow i masy
	dBodySetMass(naroznik[1].Body, &m);

	naroznik[2].Body = dBodyCreate(World);//tworzona jest banda przednia
	sides[0] = 0.005;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = 0.005;
	dBodySetPosition(naroznik[2].Body, -0.1775, 0.06, -0.3775); // ustawienie pozycji
	dBodySetLinearVel(naroznik[2].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(naroznik[2].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	naroznik[2].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(naroznik[2].Geom[0], naroznik[2].Body);//powiazanie wymiarow i masy
	dBodySetMass(naroznik[2].Body, &m);

	naroznik[3].Body = dBodyCreate(World);//tworzona jest banda przednia
	sides[0] = 0.005;//ustalane sa wymiary bandy
	sides[1] = bandy_h;
	sides[2] = 0.005;
	dBodySetPosition(naroznik[3].Body, 0.1775, 0.06, -0.3775); // ustawienie pozycji
	dBodySetLinearVel(naroznik[3].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(naroznik[3].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	naroznik[3].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(naroznik[3].Geom[0], naroznik[3].Body);//powiazanie wymiarow i masy
	dBodySetMass(naroznik[3].Body, &m);

	Joints_naroznik[0] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints_naroznik[0], Object[0].Body, naroznik[0].Body);
    dJointSetHingeAnchor(Joints_naroznik[0], 0.1775, 0.3775, 0);//pozycja
    dJointSetHingeAxis(Joints_naroznik[0], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints_naroznik[0], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints_naroznik[0], dParamHiStop, 0);//ruchu

	Joints_naroznik[1] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints_naroznik[1], Object[0].Body, naroznik[1].Body);
    dJointSetHingeAnchor(Joints_naroznik[1], -0.1775, 0.3775, 0);//pozycja
    dJointSetHingeAxis(Joints_naroznik[1], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints_naroznik[1], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints_naroznik[1], dParamHiStop, 0);//ruchu

	Joints_naroznik[2] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints_naroznik[2], Object[0].Body, naroznik[2].Body);
    dJointSetHingeAnchor(Joints_naroznik[2], -0.1775, -0.3775, 0);//pozycja
    dJointSetHingeAxis(Joints_naroznik[2], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints_naroznik[2], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints_naroznik[2], dParamHiStop, 0);//ruchu

	Joints_naroznik[3] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints_naroznik[3], Object[0].Body, naroznik[3].Body);
    dJointSetHingeAnchor(Joints_naroznik[3], 0.1775, -0.3775, 0);//pozycja
    dJointSetHingeAxis(Joints_naroznik[3], 0, 1, 0);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints_naroznik[3], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints_naroznik[3], dParamHiStop, 0);//ruchu
	
	// krazek
	radius=0.01;
	length=0.005;
	krazek.Body = dBodyCreate(World);
	dBodySetPosition(krazek.Body, 0, 0.063, 0);
	dBodySetLinearVel(krazek.Body, 0, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 90);
	dBodySetRotation(krazek.Body, R);
	dMassSetCylinder(&m, mass*5, 1, radius, length);
	krazek.Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(krazek.Geom[0], krazek.Body);
	dBodySetMass(krazek.Body, &m);
	//*********************************pady****************************
	int massScaler=5; // dla podstaw
	int XinitAngVel=-100; // ustawienie poczatkowej pr. obrotowej - zeby sie odwrocily
	// pad1 - podstawa
	radius=0.02;
	length=0.01;
	pady[0].Body = dBodyCreate(World);
	dBodySetPosition(pady[0].Body, -0.1, 0.07, 0);
	dBodySetLinearVel(pady[0].Body, 0, 0, 0);
	dBodySetAngularVel(pady[0].Body, XinitAngVel, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[0].Body, R);
	dMassSetCylinder(&m, mass*massScaler, 1, radius, length);
	pady[0].Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(pady[0].Geom[0], pady[0].Body);
	dBodySetMass(pady[0].Body, &m);

	// pad1 - uchwyt
	radius=0.01;
	length=0.02;
	pady[1].Body = dBodyCreate(World);
	dBodySetPosition(pady[1].Body, -0.1, 0.07, 0.015);
	dBodySetLinearVel(pady[1].Body, 0, 0, 0);
	dBodySetAngularVel(pady[1].Body, XinitAngVel, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[1].Body, R);
	dMassSetCylinder(&m, mass, 1, radius, length);
	pady[1].Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(pady[1].Geom[0], pady[1].Body);
	dBodySetMass(pady[1].Body, &m);
	
	//polaczenie podstawy z uchwytem pada1
	Joints2[0] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints2[0], pady[0].Body, pady[1].Body);
    dJointSetHingeAnchor(Joints2[0], -0.1, 0, 0);//pozycja
    dJointSetHingeAxis(Joints2[0], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints2[0], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints2[0], dParamHiStop, 0);//ruchu

	// pad2 - podstawa
	radius=0.02;
	length=0.01;
	pady[2].Body = dBodyCreate(World);
	dBodySetPosition(pady[2].Body, 0, 0.07, 0);
	dBodySetLinearVel(pady[2].Body, 0, 0, 0);
	dBodySetAngularVel(pady[2].Body, XinitAngVel, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[2].Body, R);
	dMassSetCylinder(&m, mass*massScaler, 1, radius, length);
	pady[2].Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(pady[2].Geom[0], pady[2].Body);
	dBodySetMass(pady[2].Body, &m);

	// pad2 - uchwyt
	radius=0.01;
	length=0.02;
	pady[3].Body = dBodyCreate(World);
	dBodySetPosition(pady[3].Body, 0, 0.07, 0.015);
	dBodySetLinearVel(pady[3].Body, 0, 0, 0);
	dBodySetAngularVel(pady[3].Body, XinitAngVel, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[3].Body, R);
	dMassSetCylinder(&m, mass, 1, radius, length);
	pady[3].Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(pady[3].Geom[0], pady[3].Body);
	dBodySetMass(pady[3].Body, &m);
	//polaczenie podstawy z uchwytem pada1
	Joints2[1] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints2[1], pady[2].Body, pady[3].Body);
    dJointSetHingeAnchor(Joints2[1], 0, 0, 0);//pozycja
    dJointSetHingeAxis(Joints2[1], 0, 0, 1);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints2[1], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints2[1], dParamHiStop, 0);//ruchu

	//**********bramki**************
	bramka[0].Body = dBodyCreate(World);//tworzona jest bramka tylna
	sides[0] = 0.13;//ustalane sa wymiary bramki
	sides[1] = bandy_h;
	sides[2] = 0.001;
	dBodySetPosition(bramka[0].Body, 0.0, 0.06, 0.379); // ustawienie pozycji
	dBodySetLinearVel(bramka[0].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(bramka[0].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	bramka[0].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(bramka[0].Geom[0], bramka[0].Body);//powiazanie wymiarow i masy
	dBodySetMass(bramka[0].Body, &m);

	bramka[1].Body = dBodyCreate(World);//tworzona jest bramka przednia
	sides[0] = 0.13;//ustalane sa wymiary bramki
	sides[1] = bandy_h;
	sides[2] = 0.001;
	dBodySetPosition(bramka[1].Body, 0.0, 0.06, -0.379); // ustawienie pozycji
	dBodySetLinearVel(bramka[1].Body, 0, 0, 0);//ustawienie poczatkowej predkosci obiektow
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie poczatkowej orientacji obiektu
	dBodySetRotation(bramka[1].Body, R);
	dMassSetBox(&m, mass*10, sides[0], sides[1], sides[2]);//utworzenie szescianu - masa
	bramka[1].Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);//utworzenie szescianu - wymiary
	dGeomSetBody(bramka[1].Geom[0], bramka[1].Body);//powiazanie wymiarow i masy
	dBodySetMass(bramka[1].Body, &m);

	Joints_bramka[0] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints_bramka[0], Object[3].Body, bramka[0].Body);
    dJointSetHingeAnchor(Joints_bramka[0], 0.19, 0, 0);//pozycja
    dJointSetHingeAxis(Joints_bramka[0], 0, 1, 0);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints_bramka[0], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints_bramka[0], dParamHiStop, 0);//ruchu

	Joints_bramka[1] = dJointCreateHinge(World, jointgroup);
    dJointAttach(Joints_bramka[1], Object[4].Body, bramka[1].Body);
    dJointSetHingeAnchor(Joints_bramka[1], 0.19, 0, 0);//pozycja
    dJointSetHingeAxis(Joints_bramka[1], 0, 1, 0);//os wzdluz ktorej prowadzi zlacze
    dJointSetHingeParam(Joints_bramka[1], dParamLoStop, 0);//mozliwy zakres 
    dJointSetHingeParam(Joints_bramka[1], dParamHiStop, 0);//ruchu

	mD.krazek=krazek.Geom[0];
	mD.boisko=Object[0].Geom[0];
	mD.Pad1=pady[0].Geom[0];
	mD.Pad2=pady[2].Geom[0];
	mD.bramka1=bramka[0].Geom[0];
	mD.bramka2=bramka[1].Geom[0];
	mD.trafienie1=false;
	mD.trafienie2=false;

}
void COdeWorld::CloseODE()
{
	dJointGroupDestroy(jointgroup);
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(Space);
	dWorldDestroy(World);
}

void COdeWorld::drawText(float x, float y, float z, char *string, float red, float green, float blue) // rysuje napis "string" w pozycji x, y, z
{
	float kolor[3];
	kolor[0]=red;
	kolor[1]=green;
	kolor[2]=blue;
	glColor3fv( kolor );
	glRasterPos3f(x, y, z);//pozycja napisu
	for (int i = 0; i < strlen (string); i++)//literka po literce
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void COdeWorld::setServo(int servo_nr, double value) {
	dReal Gain = 5; //wzmocnienie regulatora
	dReal MaxForce = 0.9; // maksymalna si³a w zlaczu
	dReal TruePosition = dJointGetHingeAngle(Joints[servo_nr]); //odczytanie rzeczywistej wartosci kata w
//	przegubie, w przypadku z³¹cza przesuwanego nale¿y zmieniæ funkcjê na dJointGetSliderPosition()
	dReal DesiredPosition = value; //wartosc zadana kata w przegubie
	dReal Error = TruePosition - DesiredPosition; //b³¹d – ró¿nica pomiedzy wartoœci¹ zadan¹, a rzeczywist¹
	dReal DesiredVelocity = -Error * Gain; // wyjœcie z serwomechanizmu
	dJointSetHingeParam(Joints[servo_nr], dParamFMax, MaxForce); //dodatkowe parametry z³¹cza –
//	maksymalna si³a
	dJointSetHingeParam(Joints[servo_nr], dParamVel, DesiredVelocity);
}