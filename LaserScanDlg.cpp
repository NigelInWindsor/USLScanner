// LaserScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LaserScanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaserScanDlg dialog


CLaserScanDlg::CLaserScanDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CLaserScanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaserScanDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_bDlgCreated = FALSE;
	m_nLeft = nLeft;
	m_nTop = nTop;
}


void CLaserScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserScanDlg)
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS_SCAN, m_checkContinuous);
	DDX_Control(pDX, IDC_EDIT_START_POINT, m_editStartPoint);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_FINISH_POINT, m_editFinishPoint);
	DDX_Control(pDX, IDC_EDIT_FAST_INCREMENT, m_editFastIncrement);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLaserScanDlg, CDialog)
	//{{AFX_MSG_MAP(CLaserScanDlg)
	ON_EN_CHANGE(IDC_EDIT_START_POINT, OnChangeEditStartPoint)
	ON_EN_CHANGE(IDC_EDIT_FINISH_POINT, OnChangeEditFinishPoint)
	ON_EN_CHANGE(IDC_EDIT_FAST_INCREMENT, OnChangeEditFastIncrement)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_STORE_START, OnButtonStoreStart)
	ON_BN_CLICKED(IDC_BUTTON_STORE_FINISH, OnButtonStoreFinish)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_FINISH, OnButtonGotoFinish)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_START, OnButtonGotoStart)
	ON_BN_CLICKED(IDC_CHECK_CONTINUOUS_SCAN, OnCheckContinuousScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserScanDlg message handlers

void CLaserScanDlg::OnClose() 
{
	CDialog::OnClose();
}

void CLaserScanDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

BOOL CLaserScanDlg::OnInitDialog() 
{
	APP App = (APP) AfxGetApp();
	CDialog::OnInitDialog();
	CString Buff;

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	CLazerTopographyPage* pParent = (CLazerTopographyPage*)m_pParent;
	
	Buff.Format(_T("X:%.1fmm Z:%.1fmm"),App->m_LaserUser.m_CpRect[0].Side0.fX,App->m_LaserUser.m_CpRect[0].Side0.fZ);
	m_editStartPoint.SetWindowText(Buff);

	Buff.Format(_T("X:%.1fmm Z:%.1fmm"),App->m_LaserUser.m_CpRect[1].Side0.fX,App->m_LaserUser.m_CpRect[1].Side0.fZ);
	m_editFinishPoint.SetWindowText(Buff);

	Buff.Format(_T("%.1fmm"),App->m_LastSettings.fLaserFastIncrement);
	m_editFastIncrement.SetWindowText(Buff);

	Buff.Format(_T("%.1fmm"),App->m_LastSettings.fLaserSlowIncrement);
	m_editSlowIncrement.SetWindowText(Buff);

	m_checkContinuous.SetCheck(App->m_LastSettings.bContinuousLaserScan);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CLaserScanDlg::OnChangeEditStartPoint() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	float fX,fZ;

	m_editStartPoint.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%fmm Z:%fmm"),&fX,&fZ);

	App->m_LaserUser.m_CpRect[0].Side0.fX = fX;
	App->m_LaserUser.m_CpRect[0].Side0.fZ = fZ;

}

void CLaserScanDlg::OnChangeEditFinishPoint() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	float fX,fZ;

	m_editFinishPoint.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%fmm Z:%fmm"),&fX,&fZ);

	App->m_LaserUser.m_CpRect[1].Side0.fX = fX;
	App->m_LaserUser.m_CpRect[1].Side0.fZ = fZ;

	
}

void CLaserScanDlg::OnChangeEditFastIncrement() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,App->m_LastSettings.fLaserFastIncrement);

	
}

void CLaserScanDlg::OnChangeEditSlowIncrement() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,App->m_LastSettings.fLaserSlowIncrement);
	
}




void CLaserScanDlg::OnButtonScan() 
{
	APP App = (APP) AfxGetApp();
	float fTemp;

	//Check start is BL and fnish is TR if not swap around coordinates
	if(App->m_LaserUser.m_CpRect[0].Side0.fX>App->m_LaserUser.m_CpRect[1].Side0.fX){
		fTemp = App->m_LaserUser.m_CpRect[0].Side0.fX;
		App->m_LaserUser.m_CpRect[0].Side0.fX = App->m_LaserUser.m_CpRect[1].Side0.fX;
		App->m_LaserUser.m_CpRect[1].Side0.fX = fTemp;
	}

	if(App->m_LaserUser.m_CpRect[0].Side0.fZ>App->m_LaserUser.m_CpRect[1].Side0.fZ){
		fTemp = App->m_LaserUser.m_CpRect[0].Side0.fZ;
		App->m_LaserUser.m_CpRect[0].Side0.fZ = App->m_LaserUser.m_CpRect[1].Side0.fZ;
		App->m_LaserUser.m_CpRect[1].Side0.fZ = fTemp;
	}

	App->m_Thread.StopCurrentThreadAction();
	App->m_Thread.m_nThreadAction = LASER_TAUGHT_LINE_SCAN;

	
}

void CLaserScanDlg::OnButtonStoreStart() 
{
	APP App = (APP) AfxGetApp();
	CCoord CpHead;
	CString Buff;

	
	App->m_Motors.GetHeadPos(&CpHead);
	App->m_LaserUser.m_CpRect[0].Side0.fX = CpHead.Side0.fX;
	App->m_LaserUser.m_CpRect[0].Side0.fZ = CpHead.Side0.fZ;

		
	Buff.Format(_T("X:%.1fmm Z:%.1fmm"),App->m_LaserUser.m_CpRect[0].Side0.fX,App->m_LaserUser.m_CpRect[0].Side0.fZ);
	m_editStartPoint.SetWindowText(Buff);
	
}

void CLaserScanDlg::OnButtonStoreFinish() 
{
	APP App = (APP) AfxGetApp();
	CCoord CpHead;
	CString Buff;

	
	App->m_Motors.GetHeadPos(&CpHead);
	App->m_LaserUser.m_CpRect[1].Side0.fX = CpHead.Side0.fX;
	App->m_LaserUser.m_CpRect[1].Side0.fZ = CpHead.Side0.fZ;

		
	Buff.Format(_T("X:%.1fmm Z:%.1fmm"),App->m_LaserUser.m_CpRect[1].Side0.fX,App->m_LaserUser.m_CpRect[1].Side0.fZ);
	m_editFinishPoint.SetWindowText(Buff);
}

void CLaserScanDlg::OnButtonGotoFinish() 
{
	APP App = (APP) AfxGetApp();
	float fX, fZ;
	CCoord CpHead;
	CString Buff;

	m_editFinishPoint.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%fmm Z:%fmm"),&fX,&fZ);

	App->m_Motors.GetHeadPos(&CpHead);
	CpHead.Side0.fX = fX;
	CpHead.Side0.fZ = fZ;

	App->m_Thread.StopCurrentThreadAction();
	App->m_Thread.m_CpMoveTo = CpHead;
	App->m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
	
}

void CLaserScanDlg::OnButtonGotoStart() 
{
	APP App = (APP) AfxGetApp();
	float fX, fZ;
	CCoord CpHead;
	CString Buff;

	m_editStartPoint.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%fmm Z:%fmm"),&fX,&fZ);

	App->m_Motors.GetHeadPos(&CpHead);
	CpHead.Side0.fX = fX;
	CpHead.Side0.fZ = fZ;

	App->m_Thread.StopCurrentThreadAction();
	App->m_Thread.m_CpMoveTo = CpHead;
	App->m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

}

void CLaserScanDlg::OnOK() 
{
}

void CLaserScanDlg::OnCheckContinuousScan() 
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.bContinuousLaserScan = m_checkContinuous.GetCheck() & 1;
}
