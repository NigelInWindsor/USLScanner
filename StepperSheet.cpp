// StepperSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "StepperSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStepperSheet

IMPLEMENT_DYNAMIC(CStepperSheet, CPropertySheet)

inline void CStepperSheet::Construct(CWnd** pDlg)
{
	
	int	nn;

	m_pDlg = pDlg;
	
	for(nn=0;nn<16;nn++) m_pAxis[nn] = NULL;

	nn=0;
	m_pAxis[nn] = new CAxisDlg(IDS_X_MOVING, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXStep,nn);
	nn++;
	m_pAxis[nn] = new CAxisDlg(IDS_Y_MOVING, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYStep,nn);
	nn++;
	m_pAxis[nn] = new CAxisDlg(IDS_Z_MOVING, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nZStep,nn);
	nn++;
	m_pAxis[nn] = new CAxisDlg(IDS_X_FIXED, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nRStep,nn);
	nn++;
	m_pAxis[nn] = new CAxisDlg(IDS_Y_FIXED, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXtStep,nn);
	nn++;
	m_pAxis[nn] = new CAxisDlg(IDS_Z_FIXED, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYtStep,nn);
	nn++;

	for(nn=0;nn<16;nn++) {
		if(m_pAxis[nn]) AddPage(m_pAxis[nn]);
	}

}

CStepperSheet::CStepperSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CStepperSheet::CStepperSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CStepperSheet::~CStepperSheet()
{
	for(int nn=0;nn<16;nn++) {
		SAFE_DELETE( m_pAxis[nn] );
	}
}


BEGIN_MESSAGE_MAP(CStepperSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CStepperSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStepperSheet message handlers

void CStepperSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}

void CStepperSheet::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_STEPPER_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}
void CStepperSheet::OnDestroy() 
{
	
	


	if (IDD_STEPPER_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_STEPPER_SHEET]);

	CPropertySheet::OnDestroy();
	*m_pDlg	= NULL;		
	
}

void CStepperSheet::UpdateAllPages()
{

}

BOOL CStepperSheet::OnInitDialog()
{
	
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	if (IDD_STEPPER_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_STEPPER_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	return bResult;	
}
