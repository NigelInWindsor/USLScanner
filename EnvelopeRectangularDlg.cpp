// EnvelopeRectangularDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EnvelopeRectangularDlg.h"
#include "3DViewEditDlg.h"
//#include "EnvelopeEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeRectangularDlg dialog


CEnvelopeRectangularDlg::CEnvelopeRectangularDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CEnvelopeRectangularDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnvelopeRectangularDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
}


void CEnvelopeRectangularDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvelopeRectangularDlg)
	DDX_Control(pDX, IDC_COMBO_GOTO_SIDE, m_comboGotoSide);
	DDX_Control(pDX, IDC_EDIT_START, m_editStart);
	DDX_Control(pDX, IDC_EDIT_FINISH, m_editFinish);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvelopeRectangularDlg, CDialog)
	//{{AFX_MSG_MAP(CEnvelopeRectangularDlg)
	ON_EN_CHANGE(IDC_EDIT_START, OnChangeEditStart)
	ON_EN_CHANGE(IDC_EDIT_FINISH, OnChangeEditFinish)
	ON_BN_CLICKED(IDC_BUTTON_STORE_START, OnButtonStoreStart)
	ON_BN_CLICKED(IDC_BUTTON_STORE_FINISH, OnButtonStoreFinish)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_GOTO_SIDE, OnSelchangeComboGotoSide)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_START, OnButtonGotoStart)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_FINISH, OnButtonGotoFinish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeRectangularDlg message handlers
void CEnvelopeRectangularDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CEnvelopeRectangularDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CEnvelopeRectangularDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}

void CEnvelopeRectangularDlg::OnDestroy() 
{
	if (CEnvelopeRectangularDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CEnvelopeRectangularDlg::IDD]);
	}

	CDialog::OnDestroy();
	if(m_pDlg) *m_pDlg = NULL;			
}

BOOL CEnvelopeRectangularDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	if (CEnvelopeRectangularDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CEnvelopeRectangularDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}
	

	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.LoadString(IDS_Left); m_comboGotoSide.AddString(Buff);
		Buff.LoadString(IDS_Right); m_comboGotoSide.AddString(Buff);
		break;
	case 1:
		Buff.LoadString(IDS_Right); m_comboGotoSide.AddString(Buff);
		Buff.LoadString(IDS_Left); m_comboGotoSide.AddString(Buff);
		break;
	}
	Buff.LoadString(IDS_Both); m_comboGotoSide.AddString(Buff);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnvelopeRectangularDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboGotoSide.SetCurSel(PROFILE->m_nScanSide);

	Buff.Format(_T("X:%.01f Y:%.01f Z:%.01f"),PROFILE->m_CpRect[0].Side[theApp.m_nPrimarySide].fX,
		PROFILE->m_CpRect[0].Side[theApp.m_nPrimarySide].fY,PROFILE->m_CpRect[0].Side[theApp.m_nPrimarySide].fZ);
	m_editStart.SetWindowText(Buff);

	Buff.Format(_T("X:%.01f Y:%.01f Z:%.01f"),PROFILE->m_CpRect[1].Side[theApp.m_nPrimarySide].fX,
		PROFILE->m_CpRect[1].Side[theApp.m_nPrimarySide].fY,PROFILE->m_CpRect[1].Side[theApp.m_nPrimarySide].fZ);
	m_editFinish.SetWindowText(Buff);

}

void CEnvelopeRectangularDlg::OnChangeEditStart() 
{
	
}

void CEnvelopeRectangularDlg::OnChangeEditFinish() 
{
	
}

void CEnvelopeRectangularDlg::OnButtonStoreStart() 
{
	CCoord	CpHead,CpSurface;

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);

	PROFILE->m_CpRect[0] = CpSurface;
	UpdateAllControls();

	if(PROFILE->CalculateEnvelopeFromRect()==TRUE) {
//		theApp.SuspendThreads();
		PROFILE->GenerateScanLines();
//		theApp.ResumeThreads();
	}

	CEnvelopeEditDlg* pParent = (CEnvelopeEditDlg*)GetParent();
	pParent->UpdateParent(INVALIDATE);
	
}

void CEnvelopeRectangularDlg::OnButtonStoreFinish() 
{
	CCoord	CpHead,CpSurface;

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);

	PROFILE->m_CpRect[1] = CpSurface;
	UpdateAllControls();

	if(PROFILE->CalculateEnvelopeFromRect()== TRUE) {
//		theApp.SuspendThreads();
		PROFILE->GenerateScanLines();
//		theApp.ResumeThreads();
	}

	CEnvelopeEditDlg* pParent = (CEnvelopeEditDlg*)GetParent();
	pParent->UpdateParent(INVALIDATE);

}


void CEnvelopeRectangularDlg::OnSelchangeComboGotoSide() 
{
	PROFILE->m_nScanSide = m_comboGotoSide.GetCurSel();
}

void CEnvelopeRectangularDlg::OnButtonGotoStart() 
{
	theApp.m_Motors.GotoSurfaceCoord(&PROFILE->m_CpRect[0],PROFILE->m_nScanSide);
}

void CEnvelopeRectangularDlg::OnButtonGotoFinish() 
{
	theApp.m_Motors.GotoSurfaceCoord(&PROFILE->m_CpRect[1],PROFILE->m_nScanSide);
}
