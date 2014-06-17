// VSDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "stdafx.h"
#include "VSDLL.h"
#include <stdexcept>

using namespace std;

// -- ograniczone poruszanie sie w osi [x] z atakiem
void funkcjaAtakOgrX(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak, void* dane)
{
	float xKra=posKra[0];
	float xPad=posPad[0];
	float distanceX=abs(xKra-xPad); // odleglosc krazek-pad w osi [x]
	float distanceZ=abs(posKra[2]-posPad[2]); // odleglosc krazek-pad w osi [z]
	
	float* tab=(float*)(dane); // odczyt dodatkowych danych
	float rangeX=tab[0]; // odczyt zakresu poruszania sie w osi [x]
	float vLimitX=tab[1]; // odczyt predkosci pada (przy dystansie = 1)

	if(distanceX>0.015 )
	{
		if(xKra>xPad)
		{
			if(velPad[0] < vLimitX){
				velPad[0]+=0.8;
			}
			else
				velPad[0] = vLimitX;
		}
		if(xKra<xPad)
		{
			if(velPad[0] > -vLimitX){
				velPad[0]-=0.8;
			}
			else
				velPad[0]=-vLimitX;
		}
	}
	else
		velPad[0]=0;
	if((xKra>rangeX && xPad>rangeX) || (xKra<-rangeX && xPad<-rangeX))
		velPad[0]=0;
	
	//------------- atak ----------------
	float prAtaku=tab[4];
	float Zstart=0.35;
	float Zpowrot=0.2;
	float atakDistanceX=tab[2];
	float atakDistanceZ=tab[3];

	if(posPad[2] > 0) // atakuje pad czerwony (nr 1)
	{
		// atak jesli: krazek ma (prawie) ta sama wsp. [x] co pad  i  krazek nie oddali sie za bardzo w osi [z]  
		//			   i  pad blizej bramki niz krazek
		if(distanceX <= atakDistanceX && distanceZ < atakDistanceZ && posPad[2] > posKra[2]) 
			*atak=true;
		// brak ataku jesli: krazek za daleko od pada w osi [z]  lub  pad za daleko od swojej bramki
		//					 lub  krazek nie jest w osi z padem (ten sam [x])  lub  krazek nie przekroczyl polowy
		if((distanceZ > atakDistanceZ || posPad[2] < Zpowrot) || (distanceX > atakDistanceX || posKra[2]<0))
			*atak=false;
	
		if(*atak==true)
		{
			velPad[2]=-prAtaku;
			velPad[0]=velKra[0]; // przy ataku przyjmuje predkosc [x] krazka
		}
		if(*atak==false && posPad[2] < Zstart) // jesli nie ma warunkow do ataku i pad nie osiagnal jeszcze zadanej pozycji w osi [z]
			velPad[2]=prAtaku;				  // -> cofnij pada
		if(*atak==false && posPad[2] > Zstart) // po skonczinym ataku i dotarciu na zadana pozycje w osi [z]
			velPad[2]=0;					  // -> zatrzymaj pada
	}
	if(posPad[2] < 0) // atakuje pad zielony (nr 2)
	{
		// atak jesli: krazek ma (prawie) ta sama wsp. [x] co pad  i  krazek nie oddali sie za bardzo w osi [z]  
		//			   i  pad blizej bramki niz krazek
		if(distanceX <= atakDistanceX && distanceZ < atakDistanceZ && posPad[2] < posKra[2]) 
			*atak=true;
		// brak ataku jesli: krazek za daleko od pada w osi [z]  lub  pad za daleko od swojej bramki
		//					 lub  krazek nie jest w osi z padem (ten sam [x])  lub  krazek nie przekroczyl polowy
		if((distanceZ > atakDistanceZ || posPad[2] > -Zpowrot) || (distanceX > atakDistanceX || posKra[2]>0))
			*atak=false;
	
		if(*atak==true)
		{
			velPad[2]=prAtaku;
			velPad[0]=velKra[0]; // przy ataku przyjmuje predkosc [x] krazka
		}
		if(*atak==false && posPad[2] > -Zstart) // jesli nie ma warunkow do ataku i pad nie osiagnal jeszcze zadanej pozycji w osi [z]
			velPad[2]=-prAtaku;				  // -> cofnij pada
		if(*atak==false && posPad[2] < -Zstart) // po skonczinym ataku i dotarciu na zadana pozycje w osi [z]
			velPad[2]=0;					  // -> zatrzymaj pada
	}

	/*if(distanceX<0.005)
		*atak=true;
	if(distanceX>0.005)
		*atak=false;*/

	//if(posPad[2]>=-0.25)
	//	*atak=false;
	//if((xKra>0.01 && xPad>0.015) || (xKra<-0.01 && xPad<-0.015))   //warunki sa male bo glownie defensywa
	//	*atak=false;												//ogolnie mniejszy ma obszar ataku niz obrony

	//if(*atak==true && posKra[2]<-0.1) 
	//{
	//	velPad[2]=prAtaku; // do przodu
	//}
	//
	//if(*atak==false && posPad[2]>-0.35)
	//{
	//	velPad[2]=-prAtaku; // do tylu
	//}
	//if(*atak==false && posPad[2]<-0.35)
	//{
	//	velPad[2]=0; // zatrzymanie
	//	*atak=true;
	//}
}

void funkcjaAtak(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak)
{
	float distanceX=abs(posKra[0]-posPad[0]); // odleglosc krazek-pad w osi [x]
	if(distanceX > 0.015) // strefa martwa w poblizu wsp. [x] pada
	{
		if(posKra[0]>posPad[0])
		{
			if(velPad[0]<10){
				velPad[0]+=0.5;
			}
			else
				velPad[0]=10;
		}
		if(posKra[0]<posPad[0])
		{
			if(velPad[0]>-10){
				velPad[0]-=0.5;
			}
			else
				velPad[0]=-10;
		}
	}
	else
		velPad[0]=0;
	//------------- atak ----------------
	float prAtaku=15;
	float Zstart=0.35;
	float Zpowrot=0.1;
	if(posPad[2] > 0) // atakuje pad czerwony (nr 1)
	{
		if(posPad[2]<=Zpowrot) // pad atakujac nie przekracza pewnej wysokosci w [z]
			*atak=false;
	
		if(*atak && posKra[2]>Zpowrot) // jesli atak  i  pad nie jest za bardzo wysuniety z bramki
		{
			velPad[2]=-prAtaku; // do przodu
		}
	
		if(*atak==false && posPad[2]<Zstart) // jesli  nie atak  i  pad nie jest za blisko bramki
		{
			velPad[2]=prAtaku; // do tylu
		}
		if(*atak==false && posPad[2]>Zstart && posKra[2] < posPad[2]) // jesli: nie atak  i  pad przekroczyl linie startowa [z]  
		{															//		  i  pad blizej swojej bramki niz krazek
			velPad[2]=0; // zatrzymanie
			*atak=true; // zezwolenie na atak
		}
	}
	else // atakuje pad zielony (nr 2)
	{
		prAtaku=-prAtaku;
		Zstart=-Zstart;
		Zpowrot=-Zpowrot;
		if(posPad[2]>=Zpowrot) // jesli pad atakujac przekroczy pewna wysokosc w [z]
			*atak=false; // zezwolenie na atak zostaje przerwane
	
		if(*atak && posKra[2]<Zpowrot) // jesli atak  i  pad nie jest za bardzo wysuniety z bramki
		{
			velPad[2]=-prAtaku; // do przodu
		}
	
		if(*atak==false && posPad[2]>Zstart) // jesli:  nie atak  i  pad nie jest za blisko bramki
		{
			velPad[2]=prAtaku; // do tylu
		}
		if(*atak==false && posPad[2]<Zstart && posKra[2] > posPad[2]) // jesli: nie atak  i  pad przekroczyl linie startowa [z]  
		{															//		  i  pad blizej swojej bramki niz krazek
			velPad[2]=0; // zatrzymanie
			*atak=true; // zezwolenie na atak
		}
	}
}

// -- funkcjaProsta -- analizuje prosta po ktorej porusza sie krazek
void funkcjaProsta(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX)
{
	float pozycjaBramki;
	if(posPad[2] < 0) // pad zielony
		pozycjaBramki=-0.379;
	else // pad czerwony
		pozycjaBramki=0.379;
	float wspA, wspB;

	// Uklad wspolrzednych -> [x] w osi wartosci
	if(abs(velKra[2]) > abs(velKra[0]) && abs(velKra[2]) > 0.1) // pr. krazka w osi [z] wieksza od pr. w osi [x]
	{															// i  nizerowa pr. krazka w osi [z]  
		// funkcja liniowa x = a*z + b
		wspA = velKra[0] / velKra[2]; // wspolczynnik a
		wspB = posKra[0] - posPad[2] * wspA; // wspolczynnik b
	
		*celX = (pozycjaBramki * wspA) + wspB; // wspolrzedna [x], w ktorej prosta po ktorej porusza sie krazek przetnie linie koncowa (wsp [z] bramki)
		
		if(abs(*celX) < 0.08) // jesli krazek zmierza w swiatlo bramki
			*czyWbramke=true;
		else
			*czyWbramke=false;
	}
	
	// Uklad wspolrzednych -> [z] w osi wartosci
	if(abs(velKra[0]) > abs(velKra[2]) && abs(velKra[0]) > 0.1) // pr. krazka w osi [x] wieksza od pr. w osi [z]
	{															// i  nizerowa pr. krazka w osi [x]  
		// funkcja liniowa z = a*x + b
		wspA = velKra[2] / velKra[0]; // wspolczynnik a
		wspB = posKra[2] - posPad[0] * wspA; // wspolczynnik b
	
		*celX = (pozycjaBramki - wspB)/wspA; // wspolrzedna [x], w ktorej prosta po ktorej porusza sie krazek przetnie linie koncowa (wsp [z] bramki)
		
		if(abs(*celX) < 0.08) // jesli krazek zmierza w swiatlo bramki
			*czyWbramke=true;
		else
			*czyWbramke=false;
	}

	if(posPad[2] < 0) // pad zielony
		if((abs(velKra[0]) < 0.2 && abs(velKra[2]) < 0.2) || velKra[2] > 0) // za mala pr. krazka lub ruch krazka w kierunku dodatnich [z]
			{*czyWbramke=false; wspA=0; wspB=0; *celX=0;}
	if(posPad[2] > 0) // pad czerwony
		if((abs(velKra[0]) < 0.2 && abs(velKra[2]) < 0.2) || velKra[2] < 0) // za mala pr. krazka lub ruch krazka w kierunku ujemnych [z]
			{*czyWbramke=false; wspA=0; wspB=0; *celX=0;}
}

// -- funkcjaDistanceX -- pad podaza za krazkiem w osi [x] z predkoscia zalezna od dystansu pomiedzy nimi w tej osi
void funkcjaDistanceX(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX, void* dane)
{
	float distanceX=abs(posKra[0]-posPad[0]); // odleglosc krazek-pad w osi [x]
	float distanceZ=abs(posKra[2]-posPad[2]); // odleglosc krazek-pad w osi [z]
	float xKra; // wsp. [x] krazka
	float* tab=(float*)(dane); // odczyt dodatkowych danych
	float rangeX=tab[0]; // odczyt zakresu poruszania sie w osi [x]
	float speed=tab[1]; // odczyt predkosci pada (przy dystansie = 1)

	// jesli krazek zmierza w bramke i krazek jest oddalony od pada o przynajmniej 0.05
	// to pad ma dazyc do miejsca potencjalnego zderzenia krazka z bramka
	if(*czyWbramke && (distanceZ > 0.05 || distanceX > 0.05))
	{
		xKra=*celX;
	}
	else
		xKra=posKra[0]; // jesli nie, to pad ma podazac, w osi [x], za krazkiem 
	
	float xPad=posPad[0]; // wsp. [x] pada
	
	float velocityX = speed * sqrt(distanceX); // szybkosc pada proporcjonalna do pierwiastka odleglosci miedzy padem a krazkiem
	
	if(distanceX > 0.005)
	{
		if(xKra > xPad)
		{
			velPad[0] = velocityX;
		}
		if(xKra < xPad)
		{
			velPad[0] = -velocityX;
		}
	}
	else
		velPad[0]=0; // zatrzymanie pada gdy wsp. [x] jest prawie taka sama
	if((xPad > rangeX && xKra > rangeX) || (xPad < -rangeX && xKra < -rangeX)) // jesli wsp. [x] pada i krazka sa poza swiatlem bramki...
		velPad[0]=0; // nastepuje zatrzymanie pada

	// zerowanie predkosci w osi [y] oraz [z]
	velPad[1]=0;
	velPad[2]=0;
}