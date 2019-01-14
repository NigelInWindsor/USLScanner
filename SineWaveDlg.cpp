// SineWaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SineWaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSineWaveDlg dialog


CSineWaveDlg::CSineWaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSineWaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSineWaveDlg)
	//}}AFX_DATA_INIT
	
}


void CSineWaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSineWaveDlg)
	DDX_Control(pDX, IDC_EDIT_CYCLES, m_editCycles);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_EDIT_SEGMENT, m_editSegment);
	DDX_Control(pDX, IDC_EDIT_FINISH, m_editFinish);
	DDX_Control(pDX, IDC_EDIT_START, m_editStart);
	DDX_Control(pDX, IDC_EDIT_PITCH, m_editPitch);
	DDX_Control(pDX, IDC_EDIT_PHASE, m_editPhase);
	DDX_Control(pDX, IDC_EDIT_AMPLITUDE, m_editAmplitude);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSineWaveDlg, CDialog)
	//{{AFX_MSG_MAP(CSineWaveDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_STORE_START, OnButtonStoreStart)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, OnButtonFinish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSineWaveDlg message handlers

void CSineWaveDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}

void CSineWaveDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CSineWaveDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CSineWaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_comboMode.AddString(_T("From To"));
	m_comboMode.AddString(_T("Number of Cycles"));

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSineWaveDlg::UpdateAllControls()
{
	CString Buff;
	CString Degrees = L"\x0b0";
	CString Micro = L"\x0b5";

	Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f"),m_Cp[0].Side[0].fX,
												m_Cp[0].Side[0].fY,
												m_Cp[0].Side[0].fZ);
	m_editStart.SetWindowText(Buff);

	Buff.Format(_T("X: %.1f Y: %.1f Z: %.1f"),m_Cp[1].Side[0].fX,
												m_Cp[1].Side[0].fY,
												m_Cp[1].Side[0].fZ);
	m_editFinish.SetWindowText(Buff);

	m_editPitch.SetWindowText(_T("100.0 mm"));
	m_editAmplitude.SetWindowText(_T("10.0 mm"));
	m_editPhase.SetWindowText(_T("0.0") + Degrees);
	m_editSegment.SetWindowText(_T("5.0") + Degrees);

	m_comboMode.SetCurSel(m_nMode);

	Buff.Format(_T("%.02f"),m_fNumberCycles);
	m_editCycles.SetWindowText(Buff);

}

void CSineWaveDlg::OnOK() 
{
	CString Buff;

	m_editPitch.GetWindowText(Buff);
	_WTOF(Buff,m_fPitch);
	m_editAmplitude.GetWindowText(Buff);
	_WTOF(Buff,m_fAmp);
	m_editPhase.GetWindowText(Buff);
	_WTOF(Buff,m_fPhase);
	m_editSegment.GetWindowText(Buff);
	_WTOF(Buff,m_fSeg);

	m_editStart.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&m_Cp[0].Side[0].fX,
										&m_Cp[0].Side[0].fY,
										&m_Cp[0].Side[0].fZ);
	m_editFinish.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&m_Cp[1].Side[0].fX,
										&m_Cp[1].Side[0].fY,
										&m_Cp[1].Side[0].fZ);

	m_nMode=m_comboMode.GetCurSel();
	m_editCycles.GetWindowText(Buff);
	_WTOF(Buff,m_fNumberCycles);


	CDialog::OnOK();
}


void CSineWaveDlg::OnButtonStoreStart() 
{
	
	CString Buff;

	theApp.m_Motors.GetSurfacePos(&m_Cp[0]);
	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_Cp[0].Side0.fX,m_Cp[0].Side0.fY,m_Cp[0].Side0.fZ);
	m_editStart.SetWindowText(Buff);

}

void CSineWaveDlg::OnButtonFinish() 
{
	
	CString Buff;

	theApp.m_Motors.GetSurfacePos(&m_Cp[1]);
	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_Cp[1].Side0.fX,m_Cp[1].Side0.fY,m_Cp[1].Side0.fZ);
	m_editStart.SetWindowText(Buff);
}
