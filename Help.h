// Help.h: interface for the Help class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELP_H__30CED6F7_8A4E_41C4_B3FE_08A3B847431C__INCLUDED_)
#define AFX_HELP_H__30CED6F7_8A4E_41C4_B3FE_08A3B847431C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHelp  
{
public:
	CHelp();
	virtual ~CHelp();
	
	bool Open();
	bool Open(int nResource);
	bool Open(CString PageName);
	bool Open(CString PageName,HWND handle=NULL);
	bool Close();



private:
	HWND m_handle;

};

#endif // !defined(AFX_HELP_H__30CED6F7_8A4E_41C4_B3FE_08A3B847431C__INCLUDED_)
