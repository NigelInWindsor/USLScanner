// HistogramThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "HistogramThresholdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogramThresholdDlg dialog


CHistogramThresholdDlg::CHistogramThresholdDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, CData* pData)
	: CDialog(CHistogramThresholdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramThresholdDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;
	

}


void CHistogramThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramThresholdDlg)
	DDX_Control(pDX, IDC_SPIN_THRESHOLD_1, m_spinThreshold1);
	DDX_Control(pDX, IDC_SPIN_THRESHOLD_0, m_spinThreshold0);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_1, m_editThreshold1);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_0, m_editThreshold0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramThresholdDlg, CDialog)
	//{{AFX_MSG_MAP(CHistogramThresholdDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD_0, OnChangeEditThreshold0)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD_1, OnChangeEditThreshold1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THRESHOLD_0, OnDeltaposSpinThreshold0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THRESHOLD_1, OnDeltaposSpinThreshold1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramThresholdDlg message handlers

void CHistogramThresholdDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CHistogramThresholdDlg::OnClose() 
{
	CDialog::OnClose();
	DestroyWindow();
}

void CHistogramThresholdDlg::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CHistogramThresholdDlg::IDD & MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CHistogramThresholdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	m_spinThreshold0.SetRange(0,1000);
	m_spinThreshold1.SetRange(0,1000);
	
	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistogramThresholdDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"%.01f dB",theApp.m_LastSettings.fHistogramRelativedB[0]);
	m_editThreshold0.SetWindowText(Buff);

	Buff.Format(L"%.01f dB",theApp.m_LastSettings.fHistogramRelativedB[1]);
	m_editThreshold1.SetWindowText(Buff);

	m_spinThreshold0.SetPos((int)(fabsf(theApp.m_LastSettings.fHistogramRelativedB[0]) * 10.0f));
	m_spinThreshold1.SetPos((int)(fabsf(theApp.m_LastSettings.fHistogramRelativedB[1]) * 10.0f));
}

void CHistogramThresholdDlg::OnChangeEditThreshold0() 
{
	CString Buff;
	float fTemp;

	m_editThreshold0.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	MinMax(&fTemp,-120.0f,0.0f);
	theApp.m_LastSettings.fHistogramRelativedB[0] = fTemp;

	m_pData->ReBuildHistogramData();
	m_pParent->Invalidate(FALSE);

}

void CHistogramThresholdDlg::OnChangeEditThreshold1() 
{
	CString Buff;
	float fTemp;

	m_editThreshold1.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	MinMax(&fTemp,-120.0f,0.0f);
	theApp.m_LastSettings.fHistogramRelativedB[1] = fTemp;

	m_pData->ReBuildHistogramData();
	m_pParent->Invalidate(FALSE);
}

void CHistogramThresholdDlg::OnDeltaposSpinThreshold0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString Buff;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.fHistogramRelativedB[0] += ((float)pNMUpDown->iDelta * 0.1f);
	Buff.Format(L"%.01f dB",theApp.m_LastSettings.fHistogramRelativedB[0]);
	m_editThreshold0.SetWindowText(Buff);

	m_pData->ReBuildHistogramData();
	m_pParent->Invalidate(FALSE);

	
	*pResult = 0;
}

void CHistogramThresholdDlg::OnDeltaposSpinThreshold1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString Buff;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.fHistogramRelativedB[1] += ((float)pNMUpDown->iDelta * 0.1f);
	Buff.Format(L"%.01f dB",theApp.m_LastSettings.fHistogramRelativedB[1]);
	m_editThreshold1.SetWindowText(Buff);

	m_pData->ReBuildHistogramData();
	m_pParent->Invalidate(FALSE);

	
	*pResult = 0;
}

