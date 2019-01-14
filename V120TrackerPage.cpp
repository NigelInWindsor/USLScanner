// V120TrackerPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "V120TrackerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CV120TrackerPage property page

IMPLEMENT_DYNCREATE(CV120TrackerPage, CPropertyPage)

CV120TrackerPage::CV120TrackerPage() : CPropertyPage(CV120TrackerPage::IDD)
{
	//{{AFX_DATA_INIT(CV120TrackerPage)
	//}}AFX_DATA_INIT
}

CV120TrackerPage::~CV120TrackerPage()
{
}

void CV120TrackerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CV120TrackerPage)
	DDX_Control(pDX, IDC_EDIT_Z, m_editZ);
	DDX_Control(pDX, IDC_EDIT_YAW, m_editYaw);
	DDX_Control(pDX, IDC_EDIT_Y, m_editY);
	DDX_Control(pDX, IDC_EDIT_ROLL, m_editRoll);
	DDX_Control(pDX, IDC_EDIT_QZ, m_editQZ);
	DDX_Control(pDX, IDC_EDIT_QY, m_editQY);
	DDX_Control(pDX, IDC_EDIT_QX, m_editQX);
	DDX_Control(pDX, IDC_EDIT_QW, m_editQW);
	DDX_Control(pDX, IDC_EDIT_PITCH, m_editPitch);
	DDX_Control(pDX, IDC_EDIT_X, m_editX);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_buttonDisconnect);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CV120TrackerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CV120TrackerPage)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CV120TrackerPage message handlers

void CV120TrackerPage::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	
}

void CV120TrackerPage::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CV120TrackerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	const wchar_t* sGUID = L"{0AAA8B22-8324-4F01-9248-F020731E398F}"; //unique ID for shared memory object.
	m_pUSM = new usm<float>(sGUID, false, 1000, 100);
	int result = m_pUSM->Initialize();

	size_t floatSize = sizeof(float);
	int iOffset = 0;
	size_t offset = iOffset * floatSize;
	m_x=0;
	unsigned long smResult = m_pUSM->ReadData(&m_x, floatSize, offset);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CV120TrackerPage::OnTimer(UINT nIDEvent) 
{
	Update();
	
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CV120TrackerPage::OnSetActive() 
{
	SetTimer(1, 100, NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CV120TrackerPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CV120TrackerPage::Update(){
	float x,y,z,yaw,pitch,roll,qx,qy,qz,qw;
	size_t floatSize = sizeof(float);
	size_t offset = 0;
	unsigned long smResult = m_pUSM->ReadData(&x, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&y, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&z, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&qx, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&qy, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&qz, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&qw, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&yaw, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&pitch, floatSize, offset);
	offset += floatSize;
	smResult = m_pUSM->ReadData(&roll, floatSize, offset);
	
	CString Buff;	
	Buff.Format(L"%f", x);
	m_editX.SetWindowText(Buff);
	Buff.Format(L"%f", y);
	m_editY.SetWindowText(Buff);
	Buff.Format(L"%f", z);
	m_editZ.SetWindowText(Buff);
	
	Buff.Format(L"%f", yaw);
	m_editYaw.SetWindowText(Buff);
	Buff.Format(L"%f", pitch);
	m_editPitch.SetWindowText(Buff);
	Buff.Format(L"%f", roll);
	m_editRoll.SetWindowText(Buff);
	
	Buff.Format(L"%f", qx);
	m_editQX.SetWindowText(Buff);
	Buff.Format(L"%f", qy);
	m_editQY.SetWindowText(Buff);
	Buff.Format(L"%f", qz);
	m_editQZ.SetWindowText(Buff); 
	Buff.Format(L"%f", qw);
	m_editQW.SetWindowText(Buff);	
}

