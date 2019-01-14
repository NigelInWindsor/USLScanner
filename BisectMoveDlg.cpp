// BisectMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BisectMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBisectMoveDlg dialog


CBisectMoveDlg::CBisectMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBisectMoveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBisectMoveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


}


void CBisectMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBisectMoveDlg)
	DDX_Control(pDX, IDC_COMBO_SIDE, m_comboSide);
	DDX_Control(pDX, IDC_BUTTON_STORE_1, m_buttonStore1);
	DDX_Control(pDX, IDC_BUTTON_STORE_0, m_buttonStore0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBisectMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CBisectMoveDlg)
	ON_BN_CLICKED(IDC_BUTTON_STORE_0, OnButtonStore0)
	ON_BN_CLICKED(IDC_BUTTON_STORE_1, OnButtonStore1)
	ON_BN_CLICKED(IDC_BUTTON_BISECT, OnButtonBisect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBisectMoveDlg message handlers

BOOL CBisectMoveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.LoadString(IDS_Left);	m_comboSide.AddString(Buff);
		Buff.LoadString(IDS_Right);	m_comboSide.AddString(Buff);
		break;
	case 1:
		Buff.LoadString(IDS_Right);	m_comboSide.AddString(Buff);
		Buff.LoadString(IDS_Left);	m_comboSide.AddString(Buff);
		break;
	}

	m_nSide=0;
	m_comboSide.SetCurSel(m_nSide);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBisectMoveDlg::OnButtonStore0() 
{
	CString Buff;

	theApp.m_Motors.GetSurfacePos(&m_Cp[0]);
	theApp.m_Kinematics.NormalsFromTips(&m_Cp[0],m_nSide);

	Buff.Format(_T("%.0f %.0f %.0f"),m_Cp[0].Side[m_nSide].fX,m_Cp[0].Side[m_nSide].fY,m_Cp[0].Side[m_nSide].fZ);
	m_buttonStore0.SetWindowText(Buff);
}

void CBisectMoveDlg::OnButtonStore1() 
{
	CString Buff;

	theApp.m_Motors.GetSurfacePos(&m_Cp[1]);
	theApp.m_Kinematics.NormalsFromTips(&m_Cp[0],m_nSide);

	Buff.Format(_T("%.0f %.0f %.0f"),m_Cp[1].Side[m_nSide].fX,m_Cp[1].Side[m_nSide].fY,m_Cp[1].Side[m_nSide].fZ);
	m_buttonStore1.SetWindowText(Buff);
}

void CBisectMoveDlg::OnButtonBisect() 
{
	CCoord CpSurface;

	for(int ii=0;ii<30;ii++) {
		CpSurface.fPos[ii] = (m_Cp[0].fPos[ii] + m_Cp[1].fPos[ii]) / 2.0f;
	}

	theApp.m_Motors.GotoSurfaceCoord(&CpSurface,m_nSide);
}
