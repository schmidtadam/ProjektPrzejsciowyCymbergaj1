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
    void DLLIMPORT funkcja_komputerowa(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);
}
extern "C"
{
    void DLLIMPORT funkcjaAtak(float* posKra, float* posPad, float* velKra, float* velPad, float* predkosc);
}

#endif /*DLL_H_*/