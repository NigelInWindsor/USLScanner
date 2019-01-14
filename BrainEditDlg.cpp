// BrainEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BrainEditDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04

/////////////////////////////////////////////////////////////////////////////
// CBrainEditDlg dialog


CBrainEditDlg::CBrainEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop)
	: CDialog(CBrainEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrainEditDlg)
	//}}AFX_DATA_INIT
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pBrainTools=NULL;
}


void CBrainEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrainEditDlg)
	DDX_Control(pDX, IDC_CHECK_VIA_DANGER_PLANE, m_checkViaDangerPlane);
	DDX_Control(pDX, IDC_CHECK_WP_0, m_checkWaterPath);
	DDX_Control(pDX, IDC_CHECK_MOVE_RIGHT, m_checkMoveRight);
	DDX_Control(pDX, IDC_CHECK_NORMALIZE, m_checkNormalize);
	DDX_Control(pDX, IDC_CHECK_PEAKRX, m_checkPeakOpposite);
	DDX_Control(pDX, IDC_COMBO_OPTOMIZE_WHAT, m_comboOptomizeWhat);
	DDX_Control(pDX, IDC_COMBO_MOVETO_USING, m_comboMovetoUsing);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrainEditDlg, CDialog)
	//{{AFX_MSG_MAP(CBrainEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TOOLS, OnButtonTools)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTOMIZE_WHAT, OnSelchangeComboOptomizeWhat)
	ON_CBN_SELCHANGE(IDC_COMBO_MOVETO_USING, OnSelchangeComboMovetoUsing)
	ON_BN_CLICKED(IDC_CHECK_PEAKRX, OnCheckPeakOpposite)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_IT_SO, OnButtonMakeItSo)
	ON_BN_CLICKED(IDC_CHECK_WP_0, OnCheckWp0)
	ON_BN_CLICKED(IDC_CHECK_NORMALIZE, OnCheckNormalize)
	ON_BN_CLICKED(IDC_CHECK_MOVE_RIGHT, OnCheckMoveRight)
	ON_BN_CLICKED(IDC_CHECK_VIA_DANGER_PLANE, OnCheckViaDangerPlane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrainEditDlg message handlers

void CBrainEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CBrainEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CBrainEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CBrainEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	CWnd* pWnd;
	if(theApp.m_nSide0Orientation == 1) {
		pWnd = (CWnd*)GetDlgItem(IDC_CHECK_WP_0);
		Buff.LoadString(IDS_Water_Right);	pWnd->SetWindowText(Buff);
		pWnd = (CWnd*)GetDlgItem(IDC_CHECK_MOVE_RIGHT);
		Buff.LoadString(IDS_Left_to_Right);	pWnd->SetWindowText(Buff);
	}

	Buff.LoadString(IDS_Current);	m_comboOptomizeWhat.AddString(Buff);
	Buff.LoadString(IDS_Line);		m_comboOptomizeWhat.AddString(Buff);
	Buff.LoadString(IDS_All_Lines);	m_comboOptomizeWhat.AddString(Buff);

	Buff.LoadString(IDS_Move);				m_comboMovetoUsing.AddString(_T("Move"));
	Buff.LoadString(IDS_Move_Plus_Brane);	m_comboMovetoUsing.AddString(_T("Move + Brain"));


	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrainEditDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_nLine+1);
	m_editLineNumber.SetWindowText(Buff);

	if(PROFILE->m_nBrainLineL<=0) {
		m_spinLineNumber.SetRange(0,0);
		m_spinLineNumber.SetPos(0);
	}else{
		m_spinLineNumber.SetRange(0,PROFILE->m_nBrainLineL-1);
		m_spinLineNumber.SetPos(theApp.m_nLine);
	}

	m_comboOptomizeWhat.SetCurSel(theApp.m_LastSettings.nBrainOptomizeOption);
	m_comboMovetoUsing.SetCurSel(theApp.m_LastSettings.nBrainMoveToOption);


	m_checkNormalize.SetCheck(theApp.m_LastSettings.nRelearnNormals[0] & 1);
	m_checkWaterPath.SetCheck(theApp.m_LastSettings.nRelearnSurfacePos[0] & 1);
	m_checkMoveRight.SetCheck(theApp.m_LastSettings.nRelearnSurfacePos[1] & 1);
	m_checkPeakOpposite.SetCheck(theApp.m_LastSettings.nRelearnPeak[1]);
	m_checkViaDangerPlane.SetCheck(theApp.m_LastSettings.nBraneUseDangerPlane & 1);


	SetAccessPrivelages();
}



void CBrainEditDlg::OnChangeEditLineNumber() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CBrainEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	CString Buff;


	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_NUMBER:
		nTemp = theApp.m_nLine;
		theApp.m_nLine=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_nLine) {
			Buff.Format(_T("%d"),theApp.m_nLine+1);
			m_editLineNumber.SetWindowText(Buff);
			UpdateParent(INVALIDATE|FILLLIST);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBrainEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	HANDLE hWnd;
	CString	Buff;
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:
			hWnd = ::GetFocus();
			if(hWnd == m_editLineNumber.m_hWnd) {
				m_editLineNumber.GetWindowText(Buff);
				theApp.m_nLine = _ttoi(Buff) -1;
				if(theApp.m_nLine<0) theApp.m_nLine=0;
				m_spinLineNumber.SetPos(theApp.m_nLine);
				m_editLineNumber.SetWindowText(Buff);
				UpdateParent(FILLLIST|INVALIDATE);
				::SetFocus(m_editLineNumber.m_hWnd);
				return TRUE;
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CBrainEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);

	UpdateAllControls();

}

void CBrainEditDlg::OnButtonTools() 
{
	if (m_pBrainTools == NULL) {
		m_pBrainTools = new CBrainToolsDlg(this, (CWnd**)&m_pBrainTools);
		m_pBrainTools->Create(CBrainToolsDlg::IDD,this);
	} else {
		m_pBrainTools->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}		
	
}

void CBrainEditDlg::OnSelchangeComboOptomizeWhat() 
{
	theApp.m_LastSettings.nBrainOptomizeOption = m_comboOptomizeWhat.GetCurSel();
}

void CBrainEditDlg::OnSelchangeComboMovetoUsing() 
{
	theApp.m_LastSettings.nBrainMoveToOption = m_comboMovetoUsing.GetCurSel();
}


void CBrainEditDlg::OnButtonMakeItSo() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nLine;

	for(nLine=0;nLine<PROFILE->m_nBrainLineL;nLine++) {
		PROFILE->m_BrainLine[nLine].SetAllModifiy(FALSE);
	}

	switch(theApp.m_LastSettings.nBrainOptomizeOption) {
	case 0: PROFILE->m_BrainLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].bModified = TRUE;
		break;
	case 1: PROFILE->m_BrainLine[theApp.m_nLine].SetAllModifiy(TRUE);
		break;
	case 2:
		for(nLine=0;nLine<PROFILE->m_nBrainLineL;nLine++) {
			PROFILE->m_BrainLine[nLine].SetAllModifiy(TRUE);
		}
		break;
	}

	pFrame->StopCurrentTimerAction();
	pFrame->m_nTimerAction = RELEARN_BRAIN;

	
}

void CBrainEditDlg::OnCheckWp0() 
{
	theApp.m_LastSettings.nRelearnSurfacePos[0] = m_checkWaterPath.GetCheck() & 1;
}

void CBrainEditDlg::OnCheckNormalize() 
{
	theApp.m_LastSettings.nRelearnNormals[0] = m_checkNormalize.GetCheck() & 1;
}

void CBrainEditDlg::OnCheckMoveRight() 
{
	theApp.m_LastSettings.nRelearnSurfacePos[1] = m_checkMoveRight.GetCheck() & 1;
}

void CBrainEditDlg::OnCheckPeakOpposite() 
{
	theApp.m_LastSettings.nRelearnPeak[1] = m_checkPeakOpposite.GetCheck() & 1;
}

void CBrainEditDlg::OnCheckViaDangerPlane() 
{
	theApp.m_LastSettings.nBraneUseDangerPlane = m_checkViaDangerPlane.GetCheck() & 1;
}

void CBrainEditDlg::SetAccessPrivelages()
{
	bool bFlag;
	CWnd* pWnd;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		bFlag = TRUE;
		break;
	case SINGLE_BRIDGE_TANK:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TT_ROLLERS:
		theApp.m_LastSettings.nRelearnSurfacePos[1] = 0;
		theApp.m_LastSettings.nRelearnPeak[1] = 0;
		bFlag = FALSE;
		break;
	}


	pWnd = GetDlgItem(IDC_CHECK_MOVE_RIGHT); pWnd->ShowWindow(bFlag);
	pWnd = GetDlgItem(IDC_CHECK_PEAKRX); pWnd->ShowWindow(bFlag);

}

