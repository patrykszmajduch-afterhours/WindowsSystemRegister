
// WindowsSystemRegister.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "CRejestr.h"
#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWindowsSystemRegisterApp:
// See WindowsSystemRegister.cpp for the implementation of this class
//

class CWindowsSystemRegisterApp : public CWinApp
{
public:
	CWindowsSystemRegisterApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWindowsSystemRegisterApp theApp;
