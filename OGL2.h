
// OGL2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COGL2App:
// See OGL2.cpp for the implementation of this class
//

class COGL2App : public CWinApp
{
public:
	COGL2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern COGL2App theApp;