// USLPrint.h: interface for the CUSLPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USLPRINT_H__17E6C600_A353_11D4_A109_50525BC10000__INCLUDED_)
#define AFX_USLPRINT_H__17E6C600_A353_11D4_A109_50525BC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUSLPrint  
{
public:
	CUSLPrint();
	virtual ~CUSLPrint();
	CDC* StartDoc(CString PrintingName);
	bool EndDoc();
	bool StartPage();
	bool StopPage();
	CDC m_dc;
	//DEVMODE m_DevMode;
	CPrintDialog* m_dlgPtr/*(TRUE);*/;
	//HDC m_hdc;
	int m_XSizePage;
	int m_YSizePage;
	void DeSelectCurrentFont();
	void CreateFont(WCHAR *Name, int Size,int lfEscapment=0);
	void DisplaySetupDialog();

	//We use this object to remenber the printing setup (orientation, papersize...) choosen by the user
	//It is initialized when the CUSLPrint object is instancied and free in the destructor.
	DEVMODE FAR m_DevMode ;

	//Printing Parameters from the Setup Printing Dialog
	void GetPrintParameters();
	int	m_nYResolutionInches;
	int m_PrintingAreaWidth;
    int m_PrintingAreaHeight ;
	int m_PhysicalPageWidth ;
	int m_PhysicalPageHeight;
	int m_PrintingAreaOffsetLeft	;
	int m_PrintingAreaOffsetTop;
	bool m_PortraitLandscapeFlag;
    //////////////////////////////////////////////////

  
	CFont *m_pOldFont; //??
	CFont *m_FontPtr; //??


};

#endif // !defined(AFX_USLPRINT_H__17E6C600_A353_11D4_A109_50525BC10000__INCLUDED_)
