// LaserToolsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "LaserToolsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaserToolsPage property page

IMPLEMENT_DYNCREATE(CLaserToolsPage, CPropertyPage)

CLaserToolsPage::CLaserToolsPage(CPropertySheet* pToolSheet, CPropertyPage* pViewPage) : CPropertyPage(CLaserToolsPage::IDD)
{
	//{{AFX_DATA_INIT(CLaserToolsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewPage = pViewPage;
	m_pToolSheet = pToolSheet;

}

CLaserToolsPage::~CLaserToolsPage()
{
}

void CLaserToolsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserToolsPage)
	DDX_Control(pDX, IDC_EDIT_Z_RANGE, m_editZRange);
	DDX_Control(pDX, IDC_EDIT_Y_RANGE, m_editYRange);
	DDX_Control(pDX, IDC_EDIT_TRANSLATION, m_editTranslation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLaserToolsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLaserToolsPage)
	ON_EN_CHANGE(IDC_EDIT_TRANSLATION, OnChangeEditTranslation)
	ON_EN_CHANGE(IDC_EDIT_Z_RANGE, OnChangeEditZRange)
	ON_EN_CHANGE(IDC_EDIT_Y_RANGE, OnChangeEditYRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserToolsPage message handlers

BOOL CLaserToolsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.Format(L"X=%.01f Y=%.01f Z=%.01f",theApp.m_LastSettings.vLaserTranslation.x,theApp.m_LastSettings.vLaserTranslation.y,theApp.m_LastSettings.vLaserTranslation.z);
	m_editTranslation.SetWindowText(Buff);

	Buff.Format(L"Min:%.0f mm Max:%.0f mm",theApp.m_LastSettings.fLaserMinY,theApp.m_LastSettings.fLaserMaxY);
	m_editYRange.SetWindowText(Buff);

	Buff.Format(L"Min:%.0f mm Max:%.0f mm",theApp.m_LastSettings.fLaserMinZ,theApp.m_LastSettings.fLaserMaxZ);
	m_editZRange.SetWindowText(Buff);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLaserToolsPage::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CLaserToolsPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CLaserToolsPage::OnChangeEditTranslation() 
{
	CString Buff;
	int nStart;

	m_editTranslation.GetWindowText(Buff);

	Buff.MakeUpper();
	
	if((nStart=Buff.Find(L"X=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X=%f",&theApp.m_LastSettings.vLaserTranslation.x);
	if((nStart=Buff.Find(L"Y=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y=%f",&theApp.m_LastSettings.vLaserTranslation.y);
	if((nStart=Buff.Find(L"Z=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z=%f",&theApp.m_LastSettings.vLaserTranslation.z);

}

void CLaserToolsPage::OnChangeEditZRange() 
{
	CString Buff;
	float fZ0,fZ1;

	m_editZRange.GetWindowText(Buff);
	Buff.Replace(L"Min:",L"");
	Buff.Replace(L"Max:",L"");
	Buff.Replace(L"mm",L"");
	swscanf_s(Buff,L"%f %f",&fZ0,&fZ1);

	if(fZ0 < fZ1) {
		theApp.m_LastSettings.fLaserMinZ = fZ0;
		theApp.m_LastSettings.fLaserMaxZ = fZ1;
	} else {
		theApp.m_LastSettings.fLaserMaxZ = fZ0;
		theApp.m_LastSettings.fLaserMinZ = fZ1;
	}
}

void CLaserToolsPage::OnChangeEditYRange() 
{
	CString Buff;
	float fY0,fY1;

	m_editYRange.GetWindowText(Buff);
	Buff.Replace(L"Min:",L"");
	Buff.Replace(L"Max:",L"");
	Buff.Replace(L"mm",L"");
	swscanf_s(Buff,L"%f %f",&fY0,&fY1);

	if(fY0 < fY1) {
		theApp.m_LastSettings.fLaserMinY = fY0;
		theApp.m_LastSettings.fLaserMaxY = fY1;
	} else {
		theApp.m_LastSettings.fLaserMaxY = fY0;
		theApp.m_LastSettings.fLaserMinY = fY1;
	}
}
