// EnvelopeToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EnvelopeToolsDlg.h"
#include "3DViewEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeToolsDlg dialog


CEnvelopeToolsDlg::CEnvelopeToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnvelopeToolsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnvelopeToolsDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;

}


void CEnvelopeToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvelopeToolsDlg)
	DDX_Control(pDX, IDC_SPIN_TOP, m_spinTop);
	DDX_Control(pDX, IDC_SPIN_RIGHT, m_spinRight);
	DDX_Control(pDX, IDC_SPIN_LEFT, m_spinLeft);
	DDX_Control(pDX, IDC_SPIN_BOTTOM, m_spinBottom);
	DDX_Control(pDX, IDC_EDIT_TOP, m_editTop);
	DDX_Control(pDX, IDC_EDIT_RIGHT, m_editRight);
	DDX_Control(pDX, IDC_EDIT_LEFT, m_editLeft);
	DDX_Control(pDX, IDC_EDIT_BOTTOM, m_editBottom);
	DDX_Control(pDX, IDC_COMBO_WHICHLINES, m_comboWhichLines);
	DDX_Control(pDX, IDC_COMBO_ALOGORITHM, m_comboAlgorithm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvelopeToolsDlg, CDialog)
	//{{AFX_MSG_MAP(CEnvelopeToolsDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TOP, OnDeltaposSpinTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIGHT, OnDeltaposSpinRight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFT, OnDeltaposSpinLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BOTTOM, OnDeltaposSpinBottom)
	ON_EN_CHANGE(IDC_EDIT_TOP, OnChangeEditTop)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, OnChangeEditRight)
	ON_EN_CHANGE(IDC_EDIT_LEFT, OnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_BOTTOM, OnChangeEditBottom)
	ON_CBN_SELCHANGE(IDC_COMBO_ALOGORITHM, OnSelchangeComboAlogorithm)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICHLINES, OnSelchangeComboWhichlines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeToolsDlg message handlers

BOOL CEnvelopeToolsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	
	Buff.LoadString(IDS_Taught); m_comboWhichLines.AddString(Buff);
	Buff.LoadString(IDS_Scan); m_comboWhichLines.AddString(Buff);
	Buff.LoadString(IDS_Brain); m_comboWhichLines.AddString(Buff);
	m_comboWhichLines.SetCurSel(0);


	Buff.LoadString(IDS_Edge_of_part); m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_Rectangular); m_comboAlgorithm.AddString(Buff);
	m_comboAlgorithm.SetCurSel(theApp.m_LastSettings.nEnvelopeAlgorithm);

	m_spinBottom.SetRange(0,1000);
	m_spinTop.SetRange(0,1000);
	m_spinLeft.SetRange(0,1000);
	m_spinRight.SetRange(0,1000);
	
	m_spinBottom.SetPos(500);
	m_spinTop.SetPos(500);
	m_spinLeft.SetPos(500);
	m_spinRight.SetPos(500);

	UpdateAllControls();

	Process();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEnvelopeToolsDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fLeftEnlarge);
	m_editLeft.SetWindowText(Buff);
	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fRightEnlarge);
	m_editRight.SetWindowText(Buff);
	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fTopEnlarge);
	m_editTop.SetWindowText(Buff);
	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fBottomEnlarge);
	m_editBottom.SetWindowText(Buff);
}


void CEnvelopeToolsDlg::OnOK() 
{

	CDialog::OnOK();
}

void CEnvelopeToolsDlg::Process()
{
	float fTemp;
	float fLeft,fRight,fTop,fBottom;

	fLeft		=	theApp.m_LastSettings.fLeftEnlarge;
	fRight		=	theApp.m_LastSettings.fRightEnlarge;
	fTop		=	theApp.m_LastSettings.fTopEnlarge;
	fBottom	=	theApp.m_LastSettings.fBottomEnlarge;

	if((theApp.m_LastSettings.nOrientation3DEditImage  & 2) == 2) {
		fTemp = fRight;
		fRight = fLeft;
		fLeft = fTemp;
	}
	if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 1) {
		fTemp = fTop;
		fTop = fBottom;
		fBottom = fTemp;
	}

	int nWhichLines = m_comboWhichLines.GetCurSel();

	switch(theApp.m_LastSettings.nEnvelopeAlgorithm) {
	case 0: PROFILE->AutomaticEnvelope(nWhichLines,fTop,fLeft,fBottom,fRight);
		break;
	case 1:	PROFILE->AutomaticRectangleEnvelope(nWhichLines,fTop,fLeft,fBottom,fRight);
		break;
	}
	UpdateParent(INVALIDATE|FILLLIST);

}


void CEnvelopeToolsDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	pParent->InitializeEditLine();
	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) {
		pParent->RefreshView();
	}

}

void CEnvelopeToolsDlg::OnDeltaposSpinTop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.fTopEnlarge += ((float)pNMUpDown->iDelta * 2.0f);
	UpdateAllControls();
	Process();
	
	*pResult = 0;
}

void CEnvelopeToolsDlg::OnDeltaposSpinRight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.fRightEnlarge += ((float)pNMUpDown->iDelta * 2.0f);
	UpdateAllControls();
	Process();
	
	*pResult = 0;
}

void CEnvelopeToolsDlg::OnDeltaposSpinLeft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.fLeftEnlarge += ((float)pNMUpDown->iDelta * 2.0f);
	UpdateAllControls();
	Process();
	
	*pResult = 0;
}

void CEnvelopeToolsDlg::OnDeltaposSpinBottom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.fBottomEnlarge += ((float)pNMUpDown->iDelta * 2.0f);
	UpdateAllControls();
	Process();

	*pResult = 0;
}

void CEnvelopeToolsDlg::OnChangeEditTop() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fTopEnlarge;

	m_editTop.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fTopEnlarge);
	if(fTemp - theApp.m_LastSettings.fTopEnlarge) Process();
}

void CEnvelopeToolsDlg::OnChangeEditRight() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fRightEnlarge;

	m_editRight.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fRightEnlarge);
	if(fTemp - theApp.m_LastSettings.fRightEnlarge) Process();
}

void CEnvelopeToolsDlg::OnChangeEditLeft() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fLeftEnlarge;

	m_editLeft.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fLeftEnlarge);
	if(fTemp - theApp.m_LastSettings.fLeftEnlarge) Process();
}

void CEnvelopeToolsDlg::OnChangeEditBottom() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fBottomEnlarge;

	m_editBottom.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fBottomEnlarge);
	if(fTemp - theApp.m_LastSettings.fBottomEnlarge) Process();
}

void CEnvelopeToolsDlg::OnSelchangeComboAlogorithm() 
{
	theApp.m_LastSettings.nEnvelopeAlgorithm = m_comboAlgorithm.GetCurSel();
	Process();
}

void CEnvelopeToolsDlg::OnSelchangeComboWhichlines() 
{
	Process();
}
