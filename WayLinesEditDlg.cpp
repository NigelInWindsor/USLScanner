// WayLinesEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "WayLinesEditDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"
#include "PmacTrajectoryPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04
#define UPDATE_CHILDREN 0x08

/////////////////////////////////////////////////////////////////////////////
// CWayLinesEditDlg dialog


CWayLinesEditDlg::CWayLinesEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CWayLinesEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWayLinesEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
	m_nLeft = nLeft;
	m_nTop = nTop;

}


void CWayLinesEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWayLinesEditDlg)
	DDX_Control(pDX, IDC_BUTTON_RECALCULATE, m_buttonCalculate);
	DDX_Control(pDX, IDC_PROGRESS_WAYPOINTS, m_progressBar);
	DDX_Control(pDX, IDC_EDIT_MAX_WAY_POINTS, m_editMaxWayPoints);
	DDX_Control(pDX, IDC_COMBO_WAYPOINTS_MODE, m_comboMode);
	DDX_Control(pDX, IDC_EDIT_SEGMENT_COUNT, m_editSegmentCount);
	DDX_Control(pDX, IDC_EDIT_DEV_VECTOR, m_editDevVector);
	DDX_Control(pDX, IDC_EDIT_DEV_NORM, m_editDevNorm);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWayLinesEditDlg, CDialog)
	//{{AFX_MSG_MAP(CWayLinesEditDlg)
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LINE_NUMBER, OnDeltaposSpinLineNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_WAYPOINTS_MODE, OnSelchangeComboWaypointsMode)
	ON_EN_CHANGE(IDC_EDIT_DEV_NORM, OnChangeEditDevNorm)
	ON_EN_CHANGE(IDC_EDIT_DEV_VECTOR, OnChangeEditDevVector)
	ON_EN_CHANGE(IDC_EDIT_SEGMENT_COUNT, OnChangeEditSegmentCount)
	ON_BN_CLICKED(IDC_BUTTON_RECALCULATE, OnButtonRecalculate)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_CALCULATE_WAYPOINTS_FINISHED, UICalculateWayPointsFinished)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWayLinesEditDlg message handlers
void CWayLinesEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CWayLinesEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CWayLinesEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
	
}
BOOL CWayLinesEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	
	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	Buff.LoadString(IDS_Even_Segments);			m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Absolute_Change);		m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Changed_Vector);		m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_All_Scan_Points);		m_comboMode.AddString(Buff);
	Buff.LoadString(IDS_Robot_facets);			m_comboMode.AddString(Buff);
//	Buff.LoadString(IDS_Changed_Vector_v2);		m_comboMode.AddString(Buff);
//	Buff.LoadString(IDS_Vector_differential);	m_comboMode.AddString(Buff);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWayLinesEditDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	m_spinLineNumber.SetRange(0,PROFILE->m_nWayPointsL-1);
	m_spinLineNumber.SetPos(theApp.m_nLine);

	Buff.Format(L"%d",theApp.m_nLine+1);
	m_editLineNumber.SetWindowText(Buff);

	m_comboMode.SetCurSel(PROFILE->m_nWaypointMode);

	Buff.Format(_T("%.01fmm"),PROFILE->m_fWaypointXYZChange);
	m_editDevVector.SetWindowText(Buff);

	Buff.Format(_T("%.01f%s"),PROFILE->m_fWaypointIJKChange,DEGREES);
	m_editDevNorm.SetWindowText(Buff);

	Buff.Format(_T("%d"),PROFILE->m_nWayPointNumber);
	m_editSegmentCount.SetWindowText(Buff);


	SetAccessPrivelages();
}

void CWayLinesEditDlg::OnChangeEditLineNumber() 
{
	CString Buff;
	
	int nTemp = theApp.m_nLine;

	m_editLineNumber.GetWindowText(Buff);
	theApp.m_nLine = _ttoi(Buff) - 1;
	MinMax(&theApp.m_nLine,0,PROFILE->m_nWayPointsL-1);
	if(nTemp - theApp.m_nLine) {
		m_spinLineNumber.SetPos(theApp.m_nLine);
		UpdateParent(INVALIDATE|FILLLIST|UPDATE_CHILDREN);
		UpdatePmacTrajectory();
		::SetFocus(m_editLineNumber.m_hWnd);
	}
}



void CWayLinesEditDlg::OnDeltaposSpinLineNumber(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int nTemp = theApp.m_nLine;
	CString Buff;

	theApp.m_nLine += pNMUpDown->iDelta;
	MinMax(&theApp.m_nLine,0,PROFILE->m_nWayPointsL-1);
	if(nTemp - theApp.m_nLine) {
		Buff.Format(L"%d",theApp.m_nLine+1);
		m_editLineNumber.SetWindowText(Buff);
		UpdateParent(INVALIDATE|FILLLIST|UPDATE_CHILDREN);
		UpdatePmacTrajectory();
		::SetFocus(m_editLineNumber.m_hWnd);
	}
	
	*pResult = 0;
}


void CWayLinesEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);
	if(nAction & UPDATE_CHILDREN) pParent->UpdateAllControls();

}

void CWayLinesEditDlg::UpdatePmacTrajectory()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CPmacTrajectoryPage* pPage;

	if(pFrame->m_pPmacSheet != NULL) {
		pPage = (CPmacTrajectoryPage*)pFrame->m_pPmacSheet->GetActivePage();
		pPage->RefreshFromWayPoints();
	}

}

void CWayLinesEditDlg::OnSelchangeComboWaypointsMode() 
{
	
	int nTemp = PROFILE->m_nWaypointMode;

	PROFILE->m_nWaypointMode = m_comboMode.GetCurSel();
	SetAccessPrivelages();
	
}

void CWayLinesEditDlg::OnChangeEditDevNorm() 
{
	
	float fTemp = PROFILE->m_fWaypointIJKChange;
	CString Buff;

	m_editDevNorm.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fWaypointIJKChange);
}

void CWayLinesEditDlg::OnChangeEditDevVector() 
{
	
	float fTemp = PROFILE->m_fWaypointXYZChange;
	CString Buff;

	m_editDevVector.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fWaypointXYZChange);

}

void CWayLinesEditDlg::OnChangeEditSegmentCount() 
{
	
	CString Buff;

	m_editSegmentCount.GetWindowText(Buff);
	PROFILE->m_nWayPointNumber = _ttoi(Buff);
}

void CWayLinesEditDlg::SetAccessPrivelages()
{
	

	switch(PROFILE->m_nWaypointMode) {
	case 0: m_editDevVector.EnableWindow(FALSE);
		m_editDevNorm.EnableWindow(FALSE);
		m_editSegmentCount.EnableWindow(TRUE);
		break;
	case 1:
		m_editDevVector.EnableWindow(TRUE);
		m_editDevNorm.EnableWindow(TRUE);
		m_editSegmentCount.EnableWindow(FALSE);
		break;
	case 3: m_editDevVector.EnableWindow(FALSE);
		m_editDevNorm.EnableWindow(FALSE);
		m_editSegmentCount.EnableWindow(FALSE);
		break;
	case 4: m_editDevVector.EnableWindow(FALSE);
		m_editDevNorm.EnableWindow(TRUE);
		m_editSegmentCount.EnableWindow(FALSE);
		break;
	case 2:
		m_editDevVector.EnableWindow(TRUE);
		m_editDevNorm.EnableWindow(TRUE);
		m_editSegmentCount.EnableWindow(FALSE);
		break;
	case 5:
	case 6:
		m_editDevVector.EnableWindow(FALSE);
		m_editDevNorm.EnableWindow(TRUE);
		m_editSegmentCount.EnableWindow(FALSE);
		break;
	}

	m_editMaxWayPoints.EnableWindow(FALSE);

	if(PROFILE->m_bCalculateWayPointsThreadEnabled==true) {
		m_buttonCalculate.SetWindowText(L"Stop");
	} else {
		m_buttonCalculate.SetWindowText(L"Recalculate");
	}

}

void CWayLinesEditDlg::DisplayMaxWayPoints()
{
	
	CString Buff;

	int nMax = PROFILE->GetMaxWayPoints(&Buff);
	m_editMaxWayPoints.SetWindowText(Buff);

	if(nMax > 600) {
		MessageBox(L"Too many wap points\r\nReduce to less than 600",L"Error",MB_ICONERROR);
	}
}

void CWayLinesEditDlg::OnButtonRecalculate() 
{
	

	PROFILE->CalculateWayPoints(this);
	SetAccessPrivelages();
}


HRESULT CWayLinesEditDlg::UICalculateWayPointsFinished(WPARAM wParam, LPARAM lParam)
{

	DisplayMaxWayPoints();
	UpdateParent(INVALIDATE|FILLLIST);
	SetAccessPrivelages();

	return NULL;
}
