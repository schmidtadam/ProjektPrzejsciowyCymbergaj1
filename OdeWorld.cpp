#include "StdAfx.h"
#include "OdeWorld.h"
dWorldID World; // the ode simulation world
dJointGroupID contactgroup; // wykorzystywane w kolizjach
// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	int i;
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dGeomID *dane=(dGeomID *)(data); // rzutowanie do typu dGeomID
	dGeomID *krazekGeom=&dane[0];
	dGeomID *pad1Geom=&dane[1];
	dGeomID *pad2Geom=&dane[2];
	dBodyID krazekBody = dGeomGetBody(*krazekGeom);
	dVector3 posKrazek;
	dBodyCopyPosition (krazekBody, posKrazek); // Pobranie pozycji
	dContact contact[MAX_CONTACTS]; // up to MAX_CONTACTS contacts per box-box
	if((posKrazek[2]>0.1 && posKrazek[2]<0.4) || o1==*pad1Geom || o1==*pad2Geom || o2==*pad1Geom || o2==*pad2Geom || o1==*krazekGeom || o2==*krazekGeom) // jesli krazek uczestniczy w kolizji to male tarcie
	{
		for (i = 0; i < MAX_CONTACTS; i++)
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = 0.001; // change from 1  - tarcie
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.1; // changed from 0.9
			contact[i].surface.bounce_vel = 0.1; // changed from 0.5
			contact[i].surface.soft_cfm = 0.1;
			contact[i].surface.slip1 = 0; // change from 0
			contact[i].surface.slip2 = 0;
		}
	}
	else
		for (i = 0; i < MAX_CONTACTS; i++)
		{
			contact[i].surface.mode = dContactBounce | dContactSoftCFM;
			contact[i].surface.mu = 0.01; // change from 1  - tarcie
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = 0.3; // changed from 0.9
			contact[i].surface.bounce_vel = 0.3; // changed from 0.5
			contact[i].surface.soft_cfm = 0.1;
			contact[i].surface.slip1 = 0; // change from 0
			contact[i].surface.slip2 = 0;
		}
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
	// ------ przypisanie referencji geometrii obiektow do tablicy mojeDane -----
	mojeDane[0]=&krazek.Geom[0];
	mojeDane[1]=&pady[0].Geom[0];
	mojeDane[2]=&pady[2].Geom[0];
}
COdeWorld::~COdeWorld(void)
{
	dJointGroupDestroy(jointgroup);
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(Space);
	dWorldDestroy(World);
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
	dSpaceCollide(Space, (void*)(&mojeDane), &nearCallback);//ustawia sprawdzanie kolizji
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
	ground.DrawGrid(); //rysuje ziemie

	// -------- test kolizji ---------------
	dVector3 posKrazek;
	dBodyCopyPosition (krazek.Body, posKrazek); // Pobranie pozycji
	const dReal *KrazekVel;
	KrazekVel=dBodyGetLinearVel (krazek.Body); // Pobranie pr. liniowej
	const dReal *KrazekAng;
	KrazekAng=dBodyGetAngularVel (krazek.Body); // Pobranie pr. obrotowej
	const dReal *Pad1Ang;
	Pad1Ang=dBodyGetAngularVel (pady[0].Body);
	const dReal *Pad2Ang;
	Pad2Ang=dBodyGetAngularVel (pady[2].Body);
	if(*mojeDane[0]==krazek.Geom[0])
	sprintf(wynik, "Pozycja krazka:");
	drawText(-1.5, 1.6, -4.5, wynik, 1.0, 1.0, 1.0);
	sprintf(wynik, "x:%.3f y:%.3f z:%.3f", /*Pad1Ang[0], Pad1Ang[1], Pad1Ang[2]*/posKrazek[0], posKrazek[1], posKrazek[2]);
	drawText(-1.5, 1.3, -4.5, wynik, 1.0, 1.0, 1.0);
	if(posKrazek[0] < 0.076 && posKrazek[0] > -0.076)
	{
		if(posKrazek[2] < -0.368)
		{
			golLicznik++;
			dBodySetPosition(krazek.Body, 0.1, 0.052, 0);
			dBodySetLinearVel(krazek.Body, *KrazekVel, *(KrazekVel+1), 0);
		}
		if(posKrazek[2] > 0.368)
		{
			golLicznik2++;
			dBodySetPosition(krazek.Body, 0.1, 0.052, 0);
			dBodySetLinearVel(krazek.Body, *KrazekVel, *(KrazekVel+1), 0);
		}
	}
	sprintf(wynik, "Trafienia: %d : %d", golLicznik, golLicznik2);
	drawText(-1.5, 1.0, -4.5, wynik, 1.0, 1.0, 1.0);

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
	const dReal *Pad1Vel;
	Pad1Vel=dBodyGetLinearVel (pady[0].Body);
	//const dReal *Pad1Ang;
	//Pad1Ang=dBodyGetAngularVel (pady[0].Body);
	const dReal *Pad2Vel;
	Pad2Vel=dBodyGetLinearVel (pady[2].Body);
	//const dReal *Pad2Ang;
	//Pad2Ang=dBodyGetAngularVel (pady[2].Body);
	if(petla<400)
	{
		petla++;
	}
	if(petla==400)
	{
		//dBodySetAngularVel(krazek.Body, 0, 0, 0); // ograniczenie pr. obrotowej krazka
		dBodySetLinearVel(krazek.Body, *KrazekVel, 0, *(KrazekVel+2)); // ograniczenie pr. liniowej w osi y krazka
	
		dBodySetLinearVel(pady[0].Body, *Pad1Vel, 0, *(Pad1Vel+2)); // ograniczenie pr. liniowej w osi y pada1
		//dBodySetAngularVel(pady[0].Body, 0, 0, 0); // ograniczenie pr. obrotowej krazka
		dBodySetLinearVel(pady[1].Body, *Pad1Vel, 0, *(Pad1Vel+2)); // ograniczenie pr. liniowej w osi y pada1
		//dBodySetAngularVel(pady[1].Body, 0, 0, 0); // ograniczenie pr. obrotowej krazka

		dBodySetLinearVel(pady[2].Body, *Pad2Vel, 0, *(Pad2Vel+2)); // ograniczenie pr. liniowej w osi y pada2
		//dBodySetAngularVel(pady[2].Body, 0, 0, 0); // ograniczenie pr. obrotowej krazka
		dBodySetLinearVel(pady[3].Body, *Pad2Vel, 0, *(Pad2Vel+2)); // ograniczenie pr. liniowej w osi y pada2
		//dBodySetAngularVel(pady[3].Body, 0, 0, 0); // ograniczenie pr. obrotowej krazka
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
			dBodyCopyPosition (Object[i].Body, posStol[i]); // Pobranie poczatkowej predkosci stolu
		}
	}
	if(petla>20)
		for(int i=0; i<5; i++)
		{
			dBodySetPosition(Object[i].Body, posStol[i][0], posStol[i][1], posStol[i][2]); //zablokowanie POZYCJI stolu, zeby sie nie poruszal
		}
	// ---------------- obliczanie kolizji z krazekiem ------------------
	dVector3 posPad1;
	dBodyCopyPosition (pady[0].Body, posPad1);
	dVector3 posPad2;
	dBodyCopyPosition (pady[2].Body, posPad2);
	dReal distance1=sqrt(pow(posKrazek[0]-posPad1[0],2)+pow(posKrazek[2]-posPad1[2],2));
	
	dReal distance2=sqrt(pow(posKrazek[0]-posPad2[0],2)+pow(posKrazek[2]-posPad2[2],2));
	sprintf(wynik, "Odleglosc");
	drawText(1.2, 1.3, -4.5, wynik, 1.0, 1.0, 1.0);
	sprintf(wynik, "Pad1-krazek: %.3f", distance2);
	drawText(1.2, 1.0, -4.5, wynik, 1.0, 1.0, 1.0);
	if(distance1<=0.03) // suma promieni krazka i pada to 0.3
		{
			if(flaga1)
			{
				licznik1++;
				flaga1=false;
			}
		}
	else
		flaga1=true;
	if(distance2<=0.03) // suma promieni krazka i pada to 0.3
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
	//---------------- koniec band -------------------------
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
	
	// krazek
	radius=0.01;
	length=0.005;
	krazek.Body = dBodyCreate(World);
	dBodySetPosition(krazek.Body, 0.1, 0.06, 0);
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
//	glColor3f(1.0f,1.0f,1.0f); //<-to nie dziala, jak ustawic kolory ?
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
	dReal MaxForce = 0.9; // maksymalna si�a w zlaczu
	dReal TruePosition = dJointGetHingeAngle(Joints[servo_nr]); //odczytanie rzeczywistej wartosci kata w
//	przegubie, w przypadku z��cza przesuwanego nale�y zmieni� funkcj� na dJointGetSliderPosition()
	dReal DesiredPosition = value; //wartosc zadana kata w przegubie
	dReal Error = TruePosition - DesiredPosition; //b��d � r�nica pomiedzy warto�ci� zadan�, a rzeczywist�
	dReal DesiredVelocity = -Error * Gain; // wyj�cie z serwomechanizmu
	dJointSetHingeParam(Joints[servo_nr], dParamFMax, MaxForce); //dodatkowe parametry z��cza �
//	maksymalna si�a
	dJointSetHingeParam(Joints[servo_nr], dParamVel, DesiredVelocity);
}