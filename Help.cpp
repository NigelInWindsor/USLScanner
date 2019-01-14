// Help.cpp: implementation of the Help class.
//
//////////////////////////////////////////////////////////////////////

/*
	This Class is used to open the html help menu.

	This class assumes that the CHM file resides in the same directory as the executable

*/

#include "stdafx.h"
#include "USLScanner.h"
#include "Help.h"
#include "htmlhelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelp::CHelp()
{

}

CHelp::~CHelp()
{

}


bool CHelp::Open()
{
	Open(_T("Welcome.html"),NULL);
	return true;
}


bool CHelp::Open(int nResource)
{
	/*
	This function will look up the relevent page for the resource ID as located in Access page
	and call Open with the filename
	*/
	return true;
}


bool CHelp::Open(CString PageName,HWND WinHan)
{
	CString Buff;

	if(WinHan==NULL)
		WinHan = GetDesktopWindow();

	Buff.Format(_T("%s::/%s"),theApp.m_HelpPathName,PageName); 

	if(m_handle!=NULL)
		Close();
	
	m_handle = HtmlHelp(WinHan, Buff, HH_DISPLAY_TOPIC, NULL);

	if(m_handle==NULL)
		return false;

	return true;
}

/*
	This function closes all Help windows opened by this program
*/
bool CHelp::Close()
{
	HtmlHelp(NULL,NULL,HH_CLOSE_ALL,0);
	return true;
}
