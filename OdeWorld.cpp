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
	dContact contact[MAX_CONTACTS]; // up to MAX_CONTACTS contacts per box-box
	for (i = 0; i < MAX_CONTACTS; i++)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = 0.05; // change from 1  - tarcie
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.1; // changed from 0.9
		contact[i].surface.bounce_vel = 0.1; // changed from 0.5
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
	pom=0;
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
		geometry.DrawBox(sides, (const float*)pos, (const float*)R); //rysuje szescian
	}

	
}
// Simulation step
void COdeWorld::SimStep(double dt)
{
	dSpaceCollide(Space, 0, &nearCallback);//ustawia sprawdzanie kolizji
	dWorldQuickStep(World, dt); //wykonuje krok symulacji
	dJointGroupEmpty(contactgroup);
	for (int bodies = 0;bodies<5;bodies++)//rysowanie obiektow
		DrawGeom(Object[bodies].Geom[0], 0, 0, 1, 1, 1); //rysuje obiekt
	glPushName(1); //id obiektu - oblsuga myszki
	DrawGeom(pady[0].Geom[0], 0, 0, 1, 0, 0); //rysuje pady
	glPushName(1); //id obiektu - oblsuga myszki
	DrawGeom(pady[1].Geom[0], 0, 0, 1, 0, 0);
	glPushName(2); //id obiektu - oblsuga myszki
	DrawGeom(pady[2].Geom[0], 0, 0, 0, 1, 0);
	glPushName(2); //id obiektu - oblsuga myszki
	DrawGeom(pady[3].Geom[0], 0, 0, 0, 1, 0);
	glPushName(0); //id obiektu - oblsuga myszki
	DrawGeom(Object[5].Geom[0], 0, 0, 1, 1, 1); //rysuje sfere
	DrawGeom(krazek.Geom[0], 0, 0, 0, 0, 1); //rysuje krazek
	ground.DrawGrid(); //rysuje ziemie

	//----------- sterowanie obiektami ---------------------
	if(dx[0] || dy[0] || dz[0] !=0)
	{
		dBodySetLinearVel(Object[5].Body, dx[0], dy[0], dz[0]);
		dx[0]=0;dy[0]=0;dz[0]=0;
	}
	if(dx[1] || dz[1] !=0)
	{
		dBodySetLinearVel(pady[0].Body, dx[1], 0, dz[1]);
		dx[1]=0;dz[1]=0;
	}
	if(dx[2] || dz[2] !=0)
	{
		dBodySetLinearVel(pady[2].Body, dx[2], 0, dz[2]);
		dx[2]=0;dz[2]=0;
	}

	// -------------- zeby sie odwrocily pady:
	// nadanie padom odpowiedniej rotacji w pierwszych obiegach petli programu
	if(pom==0)
	{
		dMatrix3 R;//macierz rotacji
		dRFromAxisAndAngle(R, 1, 0, 0, 30);
		dBodySetRotation(pady[0].Body, R);
		dBodySetRotation(pady[1].Body, R);
		dBodySetRotation(pady[2].Body, R);
		dBodySetRotation(pady[3].Body, R);
		pom=1;
	}

	if(pom<12)
	{
		pom++;
		if(pom==11)
		{
			dMatrix3 R;//macierz rotacji
			dRFromAxisAndAngle(R, 1, 0, 0, 0);
			dBodySetRotation(pady[0].Body, R);
			dBodySetRotation(pady[1].Body, R);
			dBodySetRotation(pady[2].Body, R);
			dBodySetRotation(pady[3].Body, R);
		}
	}
	
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
	dRFromAxisAndAngle(R, 0, 0, 0, 0);//ustawienie pocatkowej orientacji obiektu
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
	
	//sfera ---- kulka zamiast kr¹¿ka
	Object[5].Body = dBodyCreate(World);
	dBodySetPosition(Object[5].Body, -0.1, 0.08, 0.1);
	dBodySetLinearVel(Object[5].Body, 0, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 1, 1);
	dBodySetRotation(Object[5].Body, R);
	dMassSetSphere(&m, mass/10, 0.05);
	Object[5].Geom[0] = dCreateSphere(Space, 0.01);
	dGeomSetBody(Object[5].Geom[0], Object[5].Body);
	dBodySetMass(Object[5].Body, &m);

	// krazek
	radius=0.01;
	length=0.005;
	krazek.Body = dBodyCreate(World);
	dBodySetPosition(krazek.Body, 0.1, 0.07, 0);
	dBodySetLinearVel(krazek.Body, 0, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 90);
	dBodySetRotation(krazek.Body, R);
	dMassSetCylinder(&m, mass*3, 1, radius, length);
	krazek.Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(krazek.Geom[0], krazek.Body);
	dBodySetMass(krazek.Body, &m);
	//*********************************pady****************************
	// pad1 - podstawa
	radius=0.02;
	length=0.01;
	pady[0].Body = dBodyCreate(World);
	dBodySetPosition(pady[0].Body, -0.1, 0.07, 0);
	dBodySetLinearVel(pady[0].Body, 0, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[0].Body, R);
	dMassSetCylinder(&m, mass*3, 1, radius, length);
	pady[0].Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(pady[0].Geom[0], pady[0].Body);
	dBodySetMass(pady[0].Body, &m);

	// pad1 - uchwyt
	radius=0.01;
	length=0.02;
	pady[1].Body = dBodyCreate(World);
	dBodySetPosition(pady[1].Body, -0.1, 0.07, 0.015);
	dBodySetLinearVel(pady[1].Body, 0, 0, 0);
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
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[2].Body, R);
	dMassSetCylinder(&m, mass*3, 1, radius, length);
	pady[2].Geom[0] = dCreateCylinder(Space, radius, length);
	dGeomSetBody(pady[2].Geom[0], pady[2].Body);
	dBodySetMass(pady[2].Body, &m);

	// pad2 - uchwyt
	radius=0.01;
	length=0.02;
	pady[3].Body = dBodyCreate(World);
	dBodySetPosition(pady[3].Body, 0, 0.07, 0.015);
	dBodySetLinearVel(pady[3].Body, 0, 0, 0);
	dRFromAxisAndAngle(R, 1, 0, 0, 0);
	dBodySetRotation(pady[3].Body, R);
	dMassSetCylinder(&m, mass*3, 1, radius, length);
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

}
void COdeWorld::CloseODE()
{
	dJointGroupDestroy(jointgroup);
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(Space);
	dWorldDestroy(World);
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