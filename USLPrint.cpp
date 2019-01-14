// USLPrint.cpp: implementation of the CUSLPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "WindowsICAM.h"
#include "USLPrint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSLPrint::CUSLPrint()
{
	m_pOldFont = NULL;
	m_FontPtr = NULL;
	m_dlgPtr   = NULL;

	
	//The call to GetDefault allocate the 
	CPrintDialog dlg (TRUE);
	if (dlg.GetDefaults ()!=0)
		{
		DEVMODE FAR *lpDevMode = (DEVMODE FAR *)::GlobalLock(dlg.m_pd.hDevMode);
		m_DevMode = *lpDevMode;
//		m_DevMode.dmOrientation = DMORIENT_PORTRAIT;
//		m_DevMode.dmPaperSize   = DMPAPER_A3;
//		m_DevMode.dmPrintQuality = DMRES_MEDIUM;
//		m_DevMode.dmColor = DMCOLOR_COLOR;

		m_nYResolutionInches = m_DevMode.dmYResolution;

		if (dlg.m_pd.hDevMode!=NULL)	GlobalUnlock(dlg.m_pd.hDevMode);
		if (dlg.m_pd.hDevNames!=NULL) 	GlobalUnlock(dlg.m_pd.hDevNames);
		if (dlg.m_pd.hDC!=NULL) DeleteDC(dlg.m_pd.hDC);
		GetPrintParameters();
		}
}

CUSLPrint::~CUSLPrint()
{
	//delete m_dlgPtr;
	//GlobalUnlock(&m_DevMode);
	if (m_FontPtr!= NULL) delete m_FontPtr;
	if (m_dlgPtr!=NULL) 
	{
		DeleteDC(m_dlgPtr->m_pd.hDC);
		delete m_dlgPtr;
		m_dlgPtr = NULL;
	}


}

bool CUSLPrint::StartPage()
{
	m_dc.StartPage();
	return TRUE;


}

void CUSLPrint::DisplaySetupDialog()
{

  //This is a trick to initialize the Setp Printing Dialog and retreive and save 
  
 


  CPrintDialog dlg1 (TRUE), dlg2(TRUE);
  dlg1.GetDefaults ();
  DEVMODE FAR *lpDevMode = (DEVMODE FAR *)::GlobalLock(dlg1.m_pd.hDevMode);

  //Initialize Parameters
  *lpDevMode = m_DevMode;


  dlg2.m_pd.hDevMode = dlg1.m_pd.hDevMode ;
  dlg2.DoModal(); 
 
  DEVMODE FAR *lpDevMode2 = (DEVMODE FAR *)::GlobalLock(dlg2.m_pd.hDevMode);

  //Retreive Parameters
  m_DevMode = *lpDevMode2;

  //Free the ressources allocated by GetDefault.
  GlobalFree(dlg2.m_pd.hDevMode);
  GlobalFree(dlg1.m_pd.hDevMode);
  DeleteDC(dlg1.m_pd.hDC);
  DeleteDC(dlg2.m_pd.hDC);

 

  GetPrintParameters();

   /*Logical way of doing it does not work....
	//Does not work! Crash! Why!
	CPrintDialog dlg (TRUE);
	//Allocate and Lock a DEVMODE object
	HGLOBAL hdevMode = GlobalAlloc(GHND, sizeof(DEVMODE));
	DEVMODE* pdevmode = (DEVMODE*) GlobalLock(hdevMode);
	memcpy(pdevmode ,  &m_DevMode, sizeof(DEVMODE));
	dlg.m_pd.hDevMode = hdevMode;
	dlg.DoModal();
	GlobalUnlock(hdevMode);
	*/

}

void CUSLPrint::GetPrintParameters()
{

 
   CDC dc;
   CPrintDialog dlg (TRUE);
   dlg.GetDefaults ();
   DEVMODE FAR *lpDevMode =
  (DEVMODE FAR *)::GlobalLock(dlg.m_pd.hDevMode);

   *lpDevMode = m_DevMode;
   HDC hdc;
   hdc = dlg.CreatePrinterDC( );
   dc.Attach (hdc);
  
//////////////////////////////////////////////////////////
  m_PrintingAreaWidth	= dc.GetDeviceCaps(HORZRES);
  m_PrintingAreaHeight  = dc.GetDeviceCaps(VERTRES);

  m_PhysicalPageWidth   = dc.GetDeviceCaps(PHYSICALWIDTH);
  m_PhysicalPageHeight  = dc.GetDeviceCaps(PHYSICALHEIGHT);

  m_PrintingAreaOffsetLeft			= dc.GetDeviceCaps(PHYSICALOFFSETX);
  m_PrintingAreaOffsetTop			= dc.GetDeviceCaps(PHYSICALOFFSETY);
  /////////////////////////////////////////////////

  if (m_DevMode.dmOrientation == DMORIENT_PORTRAIT) 
  	  m_PortraitLandscapeFlag = TRUE;
  else m_PortraitLandscapeFlag = FALSE;


  GlobalUnlock(dlg.m_pd.hDevMode);
  DeleteDC(hdc);

  







}


/*bool CUSLPrint::GetPrintParameters()
{
   CDC dc;
   CPrintDialog dlg (TRUE);
   dlg.GetDefaults ();
   DEVMODE FAR *lpDevMode =
   (DEVMODE FAR *)::GlobalLock(dlg.m_pd.hDevMode);

   *lpDevMode = m_DevMode;
   HDC hdc;
   hdc = dlg.CreatePrinterDC( );
   dc.Attach (hdc);
  
//////////////////////////////////////////////////////////
   m_PrintingAreaWidth		= dc.GetDeviceCaps(HORZRES);
   m_PrintingAreaHeight		= dc.GetDeviceCaps(VERTRES);

   m_PhysicalPageWidth		= dc.GetDeviceCaps(PHYSICALWIDTH);
   m_PhysicalPageHeight		= dc.GetDeviceCaps(PHYSICALHEIGHT);

   m_PrintingAreaOffsetLeft  = dc.GetDeviceCaps(PHYSICALOFFSETX);
   m_PrintingAreaOffsetTop	= dc.GetDeviceCaps(PHYSICALOFFSETY);
  /////////////////////////////////////////////////

   if (m_DevMode.dmOrientation == DMORIENT_PORTRAIT) 
  	  m_PortraitLandscapeFlag = TRUE;
   else m_PortraitLandscapeFlag = FALSE;

   GlobalUnlock(dlg.m_pd.hDevMode);
   DeleteDC(hdc);

}
*/

bool CUSLPrint::StopPage() //>=0 if Succefful
{
	switch(m_dc.EndPage())
	{
	case SP_ERROR:
	MessageBox(NULL,_T("Printing Process: SP_ERROR before end of page"), _T("Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	break;
	case SP_APPABORT:
	MessageBox(NULL,_T("Printing Process: SP_APPABORT before end of page"), _T("Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	break;
	case SP_USERABORT:
	MessageBox(NULL,_T("Printing Process: SP_USERABORT before end of page"), _T("Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	break;
	case SP_OUTOFDISK:
	MessageBox(NULL,_T("Printing Process: SP_OUTOFDISK before end of page"), _T("Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	break;
	case SP_OUTOFMEMORY:
	MessageBox(NULL,_T("Printing Process: SP_OUTOFMEMORY before end of page"), _T("Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	break;
	}

	if (m_dc.EndPage () >= 0)
		return TRUE;
	else return FALSE;
}


CDC* CUSLPrint::StartDoc(CString PrintingName)
{

	//Create CPrintDialog Object
  	m_dlgPtr = new CPrintDialog(TRUE);
	ASSERT(m_dlgPtr);

	try{
	m_dlgPtr->GetDefaults ();
    DEVMODE FAR *lpDevMode =
   (DEVMODE FAR *)::GlobalLock(m_dlgPtr->m_pd.hDevMode);
   *lpDevMode = m_DevMode;
	}catch(...)
	{return NULL;}
  

//Be Carefull! canceled the settings in the devmode strucut intialized previously
/*
	CWinApp* app = AfxGetApp(); 

	
	if (!theApp.GetPrinterDeviceDefaults(&m_dlgPtr->m_pd)) 
		{
		// Fails to create a printer device context for printing because
		// no printer is being installed. Bring up dialog box to alert the
		// users that they need to install a printer.  This step is optional.
		if (theApp.DoPrintDialog(m_dlgPtr) != IDOK)
		return NULL ;
		}
		*/




	// Creates a printer DC from the DEVMODE and DEVNAMES structures.
	if (m_dlgPtr->CreatePrinterDC() == NULL) return NULL ;

	// A printer device context (DC) has been created.
	// Attach it to a CDC object. The DC is deleted when the CDC
	// object goes out of scope.

	m_dc.Attach (m_dlgPtr->m_pd.hDC);

   int PrintingAreaWidth	= m_dc.GetDeviceCaps(HORZRES);
   int PrintingAreaHeight  = m_dc.GetDeviceCaps(VERTRES);


	//Name of the print job
	DOCINFO di;
	::ZeroMemory (&di, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = PrintingName.GetBuffer(20);

	if (m_dc.StartDoc(&di)==-1)
	{
  	MessageBox(NULL, _T("Printing Process: Error at Start Doc"), _T("Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	}

	

  GlobalUnlock(m_dlgPtr->m_pd.hDevMode);

  


  return &m_dc;
}

void  CUSLPrint::CreateFont(WCHAR *Name, int Size,int lfEscapment)
{
	if (m_FontPtr!= NULL) 
		
	{
		m_dc.SelectObject(m_pOldFont);
		delete m_FontPtr;
		m_FontPtr= NULL;
	}

	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = Size;
	lf.lfEscapement  = lfEscapment;
	wcscpy_s(lf.lfFaceName, Name);

	m_FontPtr = new CFont;

	m_FontPtr->CreateFontIndirect(&lf);
	m_pOldFont = m_dc.SelectObject(m_FontPtr);
//	pDCPrinter->SetTextColor(RGB(255,0,0));
	m_dc.SetBkMode(TRANSPARENT);
}

void CUSLPrint::DeSelectCurrentFont()
{
	if (m_pOldFont!= NULL) m_dc.SelectObject(m_pOldFont);
}




bool CUSLPrint::EndDoc()
{

	switch(m_dc.EndDoc())
	{
	case SP_ERROR:  
	MessageBox(NULL, _T("General error"), _T("Printing Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	return FALSE;
	break;
	case SP_OUTOFMEMORY:
	MessageBox(NULL, _T("Out of Memory"), _T("Printing Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	return FALSE;
	break;
	case SP_USERABORT:
	MessageBox(NULL, _T("User Abort"), _T("Printing Error"),MB_ICONERROR|MB_OK);
	m_dc.AbortDoc ();
	return FALSE;
	break;
	};

	
 
  m_dc.Detach( );
  DeleteDC(m_dlgPtr->m_pd.hDC);
  GlobalFree(m_dlgPtr->m_pd.hDevMode);
  GlobalFree(m_dlgPtr->m_pd.hDevNames);
  delete m_dlgPtr;
  m_dlgPtr = NULL;

  return TRUE;
}
