#include "stdafx.h"
#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */


extern "C"
{
    void DLLIMPORT funkcjaAtakOgrX(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak, void* dane);
}
extern "C"
{
    void DLLIMPORT funkcjaAtak(float* posKra, float* posPad, float* velKra, float* velPad, bool* atak);
}
extern "C"
{
	// -- funkcjaProsta -- analizuje prosta po ktorej porusza sie krazek
    void DLLIMPORT funkcjaProsta(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX);
}
extern "C"
{
	// -- funkcjaDistanceX -- pad podaza za krazkiem w osi [x] z predkoscia zalezna od dystansu pomiedzy nimi w tej osi
	void DLLIMPORT funkcjaDistanceX(float* posKra, float* posPad, float* velKra, float* velPad, bool* czyWbramke, float* celX, void* dane);
}
#endif /*DLL_H_*/