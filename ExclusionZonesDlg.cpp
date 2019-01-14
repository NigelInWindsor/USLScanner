// ExclusionZonesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ExclusionZonesDlg.h"
//#include "HolesEditDlg.h"
#include "3DViewEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExclusionZonesDlg dialog


CExclusionZonesDlg::CExclusionZonesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CExclusionZonesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExclusionZonesDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pParent = pParent;

	for(int ii=0;ii<3;ii++) {
		for(int gg=0;gg<2;gg++) {
			m_CpExclusion[ii][gg] = PROFILE->m_CpExclusion[ii][gg];
		}
	}
}


void CExclusionZonesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExclusionZonesDlg)
	DDX_Control(pDX, IDC_EDIT_AUTO_Z, m_editAutoZ);
	DDX_Control(pDX, IDC_EDIT_AUTO_Y, m_editAutoY);
	DDX_Control(pDX, IDC_EDIT_AUTO_X, m_editAutoX);
	DDX_Control(pDX, IDC_CHECK_EXCLUSION, m_checkExclusionZones0);
	DDX_Control(pDX, IDC_CHECK_EXCLUSION2, m_checkExclusionZones1);
	DDX_Control(pDX, IDC_EDIT_FLOATING_1, m_editFloating1);
	DDX_Control(pDX, IDC_EDIT_FLOATING_0, m_editFloating0);
	DDX_Control(pDX, IDC_EDIT_FIXED_1, m_editFixed1);
	DDX_Control(pDX, IDC_EDIT_FIXED_0, m_editFixed0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExclusionZonesDlg, CDialog)
	//{{AFX_MSG_MAP(CExclusionZonesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_FIXED_0, OnChangeEditFixed0)
	ON_EN_CHANGE(IDC_EDIT_FIXED_1, OnChangeEditFixed1)
	ON_EN_CHANGE(IDC_EDIT_FLOATING_0, OnChangeEditFloating0)
	ON_EN_CHANGE(IDC_EDIT_FLOATING_1, OnChangeEditFloating1)
	ON_BN_CLICKED(IDC_CHECK_EXCLUSION, OnCheckExclusion)
	ON_BN_CLICKED(IDC_BUTTON_AUTOMATIC, OnButtonAutomatic)
	ON_BN_CLICKED(IDC_BUTTON_FIXED_0, OnButtonFixed0)
	ON_BN_CLICKED(IDC_BUTTON_FIXED_1, OnButtonFixed1)
	ON_BN_CLICKED(IDC_BUTTON_FLOATING_0, OnButtonFloating0)
	ON_BN_CLICKED(IDC_BUTTON_FLOATING_1, OnButtonFloating1)
	ON_BN_CLICKED(IDC_CHECK_EXCLUSION2, OnCheckExclusion2)
	ON_EN_CHANGE(IDC_EDIT_AUTO_X, OnChangeEditAutoX)
	ON_EN_CHANGE(IDC_EDIT_AUTO_Y, OnChangeEditAutoY)
	ON_EN_CHANGE(IDC_EDIT_AUTO_Z, OnChangeEditAutoZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExclusionZonesDlg message handlers

void CExclusionZonesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CExclusionZonesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CExclusionZonesDlg::IDD]=FALSE;
		
	CDialog::OnClose();
	DestroyWindow();
}

void CExclusionZonesDlg::OnDestroy() 
{
	if (CExclusionZonesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CExclusionZonesDlg::IDD]);
	}

	CDialog::OnDestroy();
	if(m_pDlg) *m_pDlg = NULL;			
	
}

BOOL CExclusionZonesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	if (CExclusionZonesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CExclusionZonesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExclusionZonesDlg::UpdateAllControls()
{
	CString Buff;

	m_checkExclusionZones0.SetCheck(PROFILE->m_bExclusionZones[0] & 1);
	m_checkExclusionZones1.SetCheck(PROFILE->m_bExclusionZones[1] & 1);


	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fX,m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fY,m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fZ);
	m_editFixed0.SetWindowText(Buff);
	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX,m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fY,m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fZ);
	m_editFixed1.SetWindowText(Buff);

	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fX,m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fY,m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fZ);
	m_editFloating0.SetWindowText(Buff);
	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fX,m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fY,m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fZ);
	m_editFloating1.SetWindowText(Buff);

	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fAutoExclusionX);
	m_editAutoX.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fAutoExclusionY);
	m_editAutoY.SetWindowText(Buff);
	Buff.Format(_T("%.1f mm"),theApp.m_LastSettings.fAutoExclusionZ);
	m_editAutoZ.SetWindowText(Buff);

}

void CExclusionZonesDlg::OnChangeEditFixed0() 
{
	CString Buff;

	m_editFixed0.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fX,&m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fY,&m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fZ);
	UpdateProfile();

}

void CExclusionZonesDlg::OnChangeEditFixed1() 
{
	CString Buff;

	m_editFixed1.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX,&m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fY,&m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fZ);
	UpdateProfile();
}

void CExclusionZonesDlg::OnChangeEditFloating0() 
{
	CString Buff;

	m_editFloating0.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fX,&m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fY,&m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fZ);
	UpdateProfile();
}

void CExclusionZonesDlg::OnChangeEditFloating1() 
{
	CString Buff;

	m_editFloating1.GetWindowText(Buff);
	swscanf_s(Buff,_T("X:%f Y:%f Z:%f"),&m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fX,&m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fY,&m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fZ);
	UpdateProfile();
}


//This works on a TopLeft BottomRight basis

void CExclusionZonesDlg::UpdateProfile()
{
	int ii;

	ZeroMemory(&PROFILE->m_CpExclusion,sizeof PROFILE->m_CpExclusion);

	for(ii=0;ii<2;ii++) {
		if(m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fX<=m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX) {
			PROFILE->m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fX = m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fX;
			PROFILE->m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fX = m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fX;
		} else {
			PROFILE->m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fX = m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fX;
			PROFILE->m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fX = m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fX;
		}

		if(m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fZ>=m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fZ) {
			PROFILE->m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fZ = m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fZ;
			PROFILE->m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fZ = m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fZ;
		} else {
			PROFILE->m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fZ = m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fZ;
			PROFILE->m_CpExclusion[ii][1].Side[theApp.m_nPrimarySide].fZ = m_CpExclusion[ii][0].Side[theApp.m_nPrimarySide].fZ;
		}
	}
	CHolesEditDlg* pDlg = (CHolesEditDlg*)m_pParent;
	pDlg->UpdateParent(INVALIDATE);

}

void CExclusionZonesDlg::OnButtonAutomatic() 
{
	m_CpExclusion[0][0] = m_CpExclusion[0][1]= PROFILE->m_CpCadHole[0];
	m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fX-=150.0f;
	m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fZ+=theApp.m_LastSettings.fAutoExclusionZ;
	m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX+=theApp.m_LastSettings.fAutoExclusionX;
	m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fZ-=theApp.m_LastSettings.fAutoExclusionZ;
	
	m_CpExclusion[1][0] = m_CpExclusion[1][1]= PROFILE->m_CpCadHole[1];
	m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fX-=theApp.m_LastSettings.fAutoExclusionX;
	m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fZ+=theApp.m_LastSettings.fAutoExclusionZ;
	m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fX+=150.0f;
	m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fZ-=theApp.m_LastSettings.fAutoExclusionZ;
	


	UpdateProfile();
	UpdateAllControls();
}

void CExclusionZonesDlg::OnButtonFixed0() 
{
	CCoord CpSurface;
	
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_CpExclusion[0][0] = CpSurface;
	UpdateProfile();
	UpdateAllControls();
}

void CExclusionZonesDlg::OnButtonFixed1() 
{
	CCoord CpSurface;
	
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_CpExclusion[0][1] = CpSurface;
	UpdateProfile();
	UpdateAllControls();
}

void CExclusionZonesDlg::OnButtonFloating0() 
{
	CCoord CpSurface;
	
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_CpExclusion[1][0] = CpSurface;
	UpdateProfile();
	UpdateAllControls();
}

void CExclusionZonesDlg::OnButtonFloating1() 
{
	CCoord CpSurface;
	
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_CpExclusion[1][1] = CpSurface;
	UpdateProfile();
	UpdateAllControls();
}

void CExclusionZonesDlg::OnCheckExclusion() 
{
	PROFILE->m_bExclusionZones[0] = m_checkExclusionZones0.GetCheck() & 1;
	UpdateProfile();
}

void CExclusionZonesDlg::OnCheckExclusion2() 
{
	PROFILE->m_bExclusionZones[1] = m_checkExclusionZones1.GetCheck() & 1;
	UpdateProfile();
}

void CExclusionZonesDlg::OnChangeEditAutoX() 
{
	CString Buff;

	m_editAutoX.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fAutoExclusionX);
}

void CExclusionZonesDlg::OnChangeEditAutoY() 
{
	CString Buff;

	m_editAutoY.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fAutoExclusionY);
}

void CExclusionZonesDlg::OnChangeEditAutoZ() 
{
	CString Buff;

	m_editAutoZ.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fAutoExclusionZ);
}
