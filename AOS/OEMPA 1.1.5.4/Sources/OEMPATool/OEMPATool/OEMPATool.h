// ExampleAscanDisplay.h : main header file for the ExampleAscanDisplay application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COEMPATool:
// See ExampleAscanDisplay.cpp for the implementation of this class
//

class COEMPATool : public CWinApp
{
public:
	COEMPATool();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COEMPATool theApp;
