// WayPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "WayPointsDlg.h"
#include "3DViewEditDlg.h"
#include "PmacTrajectoryPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWayPointsDlg dialog


CWayPointsDlg::CWayPointsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CWayPointsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWayPointsDlg)
	//}}AFX_DATA_INIT
	
	m_pDlg = pDlg;
	m_pParent = pParent;
}


void CWayPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWayPointsDlg)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_ProgressBar);
	DDX_Control(pDX, IDC_CHECK_WITH_LIFT_OFF, m_checkWithLiftOff);
	DDX_Control(pDX, IDC_EDIT_MAX_WAY_PTS, m_editMaxWayPoints);
	DDX_Control(pDX, IDC_EDIT_NUMBER_SEGMENTS, m_editNumberWayPoints);
	DDX_Control(pDX, IDC_EDIT_CHANGE_XYZ, m_editChangeXYZ);
	DDX_Control(pDX, IDC_EDIT_CHANGE_IJK, m_editChangeIJK);
	DDX_Control(pDX, IDC_COMBO_WAYPOINTS_MODE, m_comboWaypointsMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWayPointsDlg, CDialog)
	//{{AFX_MSG_MAP(CWayPointsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_WAYPOINTS_MODE, OnSelchangeComboWaypointsMode)
	ON_EN_CHANGE(IDC_EDIT_CHANGE_XYZ, OnChangeEditChangeXyz)
	ON_EN_CHANGE(IDC_EDIT_CHANGE_IJK, OnChangeEditChangeIjk)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_SEGMENTS, OnChangeEditNumberSegments)
	ON_BN_CLICKED(IDC_BUTTON_RECALCULATE, OnButtonRecalculate)
	ON_BN_CLICKED(IDC_CHECK_WITH_LIFT_OFF, OnCheckWithLiftOff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWayPointsDlg message handlers

void CWayPointsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CWayPointsDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CWayPointsDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}

void CWayPointsDlg::OnDestroy() 
{
	if (CWayPointsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CWayPointsDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);
	}

	CDialog::OnDestroy();
	*m_pDlg = NULL;			
}

BOOL CWayPointsDlg::OnInitDialog() 
{
	
	CString	Buff;
	CDialog::OnInitDialog();
	
	if (CWayPointsDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CWayPointsDlg::IDD % MAXIMUMWINDOWSIDNUMBER];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	Buff.LoadString(IDS_Even_Segments);		m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_Absolute_Change);	m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_Changed_Vector);	m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_All_Scan_Points);	m_comboWaypointsMode.AddString(Buff);
	Buff.LoadString(IDS_Robot_facets);		m_comboWaypointsMode.AddString(Buff);
//	Buff.LoadString(IDS_Changed_Vector_v2);	m_comboWaypointsMode.AddString(Buff);
//	Buff.LoadString(IDS_Vector_differential);	m_comboWaypointsMode.AddString(Buff);
	
	if(PROFILE->m_fWaypointXYZChange < 0.5f) PROFILE->m_fWaypointXYZChange=0.5f;
	if(PROFILE->m_fWaypointIJKChange < 0.1f) PROFILE->m_fWaypointIJKChange=0.1f;

	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWayPointsDlg::UpdateAllControls()
{
	
	CString Buff;
	CString Micro = L"\x0b5";

	m_comboWaypointsMode.SetCurSel(PROFILE->m_nWaypointMode);

	Buff.Format(_T("%.01fmm"),PROFILE->m_fWaypointXYZChange);
	m_editChangeXYZ.SetWindowText(Buff);

	Buff.Format(_T("%.01f%s"),PROFILE->m_fWaypointIJKChange,DEGREES);
	m_editChangeIJK.SetWindowText(Buff);

	Buff.Format(_T("%d"),PROFILE->m_nWayPointNumber);
	m_editNumberWayPoints.SetWindowText(Buff);

	DisplayMaxWayPoints();

	switch(PROFILE->m_nWaypointMode) {
	case 0: m_editChangeXYZ.EnableWindow(FALSE);
		m_editChangeIJK.EnableWindow(FALSE);
		m_editNumberWayPoints.EnableWindow(TRUE);
		break;
	case 1:
		m_editChangeXYZ.EnableWindow(TRUE);
		m_editChangeIJK.EnableWindow(TRUE);
		m_editNumberWayPoints.EnableWindow(FALSE);
		break;
	case 3: m_editChangeXYZ.EnableWindow(FALSE);
		m_editChangeIJK.EnableWindow(FALSE);
		m_editNumberWayPoints.EnableWindow(FALSE);
		break;
	case 4: m_editChangeXYZ.EnableWindow(FALSE);
		m_editChangeIJK.EnableWindow(TRUE);
		m_editNumberWayPoints.EnableWindow(FALSE);
		break;
	case 2:
		m_editChangeXYZ.EnableWindow(TRUE);
		m_editChangeIJK.EnableWindow(TRUE);
		m_editNumberWayPoints.EnableWindow(FALSE);
		break;
	case 5:
	case 6:
		m_editChangeXYZ.EnableWindow(FALSE);
		m_editChangeIJK.EnableWindow(TRUE);
		m_editNumberWayPoints.EnableWindow(FALSE);
		break;
	}




	m_editMaxWayPoints.EnableWindow(FALSE);

}

void CWayPointsDlg::OnSelchangeComboWaypointsMode() 
{
	
	int nTemp = PROFILE->m_nWaypointMode;

	PROFILE->m_nWaypointMode = m_comboWaypointsMode.GetCurSel();
	PROFILE->CalculateWayPoints(&m_ProgressBar);
	DisplayMaxWayPoints();
	UpdateAllControls();
	UpdateParent(INVALIDATE|FILLLIST);
}

void CWayPointsDlg::OnChangeEditChangeXyz() 
{
	
	float fTemp = PROFILE->m_fWaypointXYZChange;
	CString Buff;

	m_editChangeXYZ.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fWaypointXYZChange);
	if(fTemp - PROFILE->m_fWaypointXYZChange) {
		PROFILE->CalculateWayPoints(&m_ProgressBar);
		DisplayMaxWayPoints();
		UpdateParent(INVALIDATE|FILLLIST);
	}

	
}

void CWayPointsDlg::OnChangeEditChangeIjk() 
{
	
	float fTemp = PROFILE->m_fWaypointIJKChange;
	CString Buff;

	m_editChangeIJK.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fWaypointIJKChange);
	if(fTemp - PROFILE->m_fWaypointIJKChange) {
		PROFILE->CalculateWayPoints(&m_ProgressBar);
		DisplayMaxWayPoints();
		UpdateParent(INVALIDATE|FILLLIST);
	}
}

void CWayPointsDlg::OnChangeEditNumberSegments() 
{
	
	int nTemp = PROFILE->m_nWayPointNumber;
	CString Buff;

	m_editNumberWayPoints.GetWindowText(Buff);
	PROFILE->m_nWayPointNumber = _ttoi(Buff);
	if(nTemp - PROFILE->m_nWayPointNumber) {
		PROFILE->CalculateWayPoints(&m_ProgressBar);
		DisplayMaxWayPoints();
		UpdateParent(INVALIDATE|FILLLIST);
	}


}


void CWayPointsDlg::DisplayMaxWayPoints()
{
	
	CString Buff;

	int nMax = PROFILE->GetMaxWayPoints(&Buff);
	m_editMaxWayPoints.SetWindowText(Buff);

	if(nMax > 600) {
		MessageBox(L"Too many wap points\r\nReduce to less than 600",L"Error",MB_ICONERROR);
	}
}


void CWayPointsDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	pParent->InitializeEditLine();
	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) {
		pParent->RefreshView();
	}

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPmacTrajectoryPage* pPage;

	if(pFrame->m_pPmacSheet != NULL) {
		pPage = (CPmacTrajectoryPage*)pFrame->m_pPmacSheet->GetActivePage();
		pPage->RefreshFromWayPoints();
	}

}

void CWayPointsDlg::OnButtonRecalculate() 
{
	

	PROFILE->CalculateWayPoints(&m_ProgressBar);
	DisplayMaxWayPoints();
	UpdateParent(INVALIDATE|FILLLIST);
}

void CWayPointsDlg::OnCheckWithLiftOff() 
{
	
	
	PROFILE->m_nWayPointsWithLiftOff = m_checkWithLiftOff.GetCheck();
	PROFILE->CalculateWayPoints(&m_ProgressBar);
	DisplayMaxWayPoints();
	UpdateAllControls();
	UpdateParent(INVALIDATE|FILLLIST);
}
