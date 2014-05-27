// VSDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "stdafx.h"
#include "VSDLL.h"
#include <stdexcept>
#include <Windows.h>
#include <time.h>


using namespace std;


void funkcja_komputerowa(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc)
{
	float xKra=posKra[0];
	float xPad=posPad[0];
	//float Vster=predkosc[0];
	predkosc[0]=velPad[0];
	predkosc[1]=velPad[1];
	predkosc[2]=velPad[2];
	predkosc[3]=velPad[3];
	bool atak;
	/*bool czasomierz;

	if(czasomierz==false)
	{

	}*/
	
	if(predkosc[3]==1)
		atak=true;
	else
		atak=false;


	if(abs(xKra-xPad)>0.015 )
	{
		if(xKra>xPad)
		{
			if(predkosc[0]<10){
				predkosc[0]+=0.5;
			}
			else
				predkosc[0]=10;
		}
		if(xKra<xPad)
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
	if((xKra>0.08 && xPad>0.08) || (xKra<-0.08 && xPad<-0.08))
		predkosc[0]=0;
	//------------- atak ----------------
	float prAtaku=22;
	if(posPad[2]>=-0.25)
		atak=false;
	if((xKra>0.01 && xPad>0.015) || (xKra<-0.01 && xPad<-0.015))   //warunki sa male bo glownie defensywa
		atak=false;												//ogolnie mniejszy ma obszar ataku niz obrony

	if(atak && posKra[2]<-0.1) 
	{
		predkosc[2]=prAtaku; // do przodu
	}
	
	if(atak==false && posPad[2]>-0.35)
	{
		predkosc[2]=-prAtaku; // do tylu
	}
	if(atak==false && posPad[2]<-0.35)
	{
		predkosc[2]=0; // zatrzymanie
		atak=true;
	}
	if(atak)
		predkosc[3]=1;
	else
		predkosc[3]=0;

}

void funkcjaAtak(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc)
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
	float prAtaku=24;
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
}