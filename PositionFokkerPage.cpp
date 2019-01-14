// PositionFokkerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PositionFokkerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionFokkerPage property page

IMPLEMENT_DYNCREATE(CPositionFokkerPage, CResizablePage)

CPositionFokkerPage::CPositionFokkerPage() : CResizablePage(CPositionFokkerPage::IDD)
{
	//{{AFX_DATA_INIT(CPositionFokkerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDlgCreated = FALSE;
}

CPositionFokkerPage::~CPositionFokkerPage()
{
	delete m_pFont;
}

void CPositionFokkerPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionFokkerPage)
	DDX_Control(pDX, IDC_COMBO_POSITION_MODE, m_comboPositionMode);
	DDX_Control(pDX, IDC_EDIT_POS_XRIGHT, m_editPosXRight);
	DDX_Control(pDX, IDC_EDIT_POS_YRIGHT, m_editPosYRight);
	DDX_Control(pDX, IDC_EDIT_POS_ZRIGHT, m_editPosZRight);
	DDX_Control(pDX, IDC_EDIT_POS_XTRIGHT, m_editPosXtRight);
	DDX_Control(pDX, IDC_EDIT_POS_ZTRIGHT, m_editPosZtRight);
	DDX_Control(pDX, IDC_EDIT_POS_XLEFT, m_editPosXLeft);
	DDX_Control(pDX, IDC_EDIT_POS_YLEFT, m_editPosYLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZLEFT, m_editPosZLeft);
	DDX_Control(pDX, IDC_EDIT_POS_XTLEFT, m_editPosXtLeft);
	DDX_Control(pDX, IDC_EDIT_POS_ZTLEFT, m_editPosZtLeft);
	DDX_Control(pDX, IDC_STATIC_XRIGHT, m_staticXRight);
	DDX_Control(pDX, IDC_STATIC_YRIGHT, m_staticYRight);
	DDX_Control(pDX, IDC_STATIC_ZRIGHT, m_staticZRight);
	DDX_Control(pDX, IDC_STATIC_XTRIGHT, m_staticXtRight);
	DDX_Control(pDX, IDC_STATIC_ZTRIGHT, m_staticZtRight);
	DDX_Control(pDX, IDC_STATIC_XLEFT, m_staticXLeft);
	DDX_Control(pDX, IDC_STATIC_YLEFT, m_staticYLeft);
	DDX_Control(pDX, IDC_STATIC_ZLEFT, m_staticZLeft);
	DDX_Control(pDX, IDC_STATIC_XTLEFT, m_staticXtLeft);
	DDX_Control(pDX, IDC_STATIC_ZTLEFT, m_staticZtLeft);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionFokkerPage, CResizablePage)
	//{{AFX_MSG_MAP(CPositionFokkerPage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION_MODE, OnSelchangeComboPositionMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionFokkerPage message handlers

BOOL CPositionFokkerPage::OnInitDialog() 
{
	APP App = (APP) AfxGetApp();
	CResizablePage::OnInitDialog();

	GetClientRect(m_rrClient);

	AddAnchor(&m_editPosXRight);
	AddAnchor(&m_editPosYRight);
	AddAnchor(&m_editPosZRight);
	AddAnchor(&m_editPosXtRight);
	AddAnchor(&m_editPosZtRight);
	AddAnchor(&m_editPosXLeft);
	AddAnchor(&m_editPosYLeft);
	AddAnchor(&m_editPosZLeft);
	AddAnchor(&m_editPosXtLeft);
	AddAnchor(&m_editPosZtLeft);
	AddAnchor(&m_staticXRight);
	AddAnchor(&m_staticYRight);
	AddAnchor(&m_staticZRight);
	AddAnchor(&m_staticXtRight);
	AddAnchor(&m_staticZtRight);
	AddAnchor(&m_staticXLeft);
	AddAnchor(&m_staticYLeft);
	AddAnchor(&m_staticZLeft);
	AddAnchor(&m_staticXtLeft);
	AddAnchor(&m_staticZtLeft);
	AddAnchor(&m_staticRight);
	AddAnchor(&m_staticLeft);
	AddAnchor(&m_comboPositionMode);

	m_comboPositionMode.AddString("Encoder");
	m_comboPositionMode.AddString("Head");
	m_comboPositionMode.AddString("Surface");
	m_comboPositionMode.AddString("Surface Water");
	m_comboPositionMode.SetCurSel(App->m_LastSettings.nPositionDisplayMode);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	m_bDlgCreated = TRUE;

	SetTimer(1,300,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPositionFokkerPage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CPositionFokkerPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr;
	LOGFONT	lf;

	if(m_bDlgCreated == TRUE) {
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		delete m_pFont;
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);
		m_editPosXRight.SetFont(m_pFont);
		m_editPosYRight.SetFont(m_pFont);
		m_editPosZRight.SetFont(m_pFont);
		m_editPosXtRight.SetFont(m_pFont);
		m_editPosZtRight.SetFont(m_pFont);
		m_editPosXLeft.SetFont(m_pFont);
		m_editPosYLeft.SetFont(m_pFont);
		m_editPosZLeft.SetFont(m_pFont);
		m_editPosXtLeft.SetFont(m_pFont);
		m_editPosZtLeft.SetFont(m_pFont);
		m_staticXRight.SetFont(m_pFont);
		m_staticYRight.SetFont(m_pFont);
		m_staticZRight.SetFont(m_pFont);
		m_staticXtRight.SetFont(m_pFont);
		m_staticZtRight.SetFont(m_pFont);
		m_staticXLeft.SetFont(m_pFont);
		m_staticYLeft.SetFont(m_pFont);
		m_staticZLeft.SetFont(m_pFont);
		m_staticXtLeft.SetFont(m_pFont);
		m_staticZtLeft.SetFont(m_pFont);
		m_staticRight.SetFont(m_pFont);
		m_staticLeft.SetFont(m_pFont);
		m_comboPositionMode.SetFont(m_pFont);
	}

	memset(nOldPos,0xcc,sizeof nOldPos);
	
}

void CPositionFokkerPage::OnTimer(UINT nIDEvent) 
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	CString Unit[] = {"mm", "\x022", "\x0b0"};
	CCoord CpHead,CpSurface;

	switch(App->m_LastSettings.nPositionDisplayMode) {
	case 0: App->m_Motors.GetEncoderPos(&CpSurface);
		break;
	case 1:	App->m_Motors.GetHeadPos(&CpSurface);
		break;
	case 2:	App->m_Motors.GetHeadPos(&CpHead);
		App->m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		App->m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case 3:	App->m_Motors.GetHeadPos(&CpHead);
		App->m_Kinematics.CheckValidWaterPath(PORTSIDE,App->m_LastSettings.nWaterPathTS[PORTSIDE]);
		App->m_Kinematics.CheckValidWaterPath(STARBOARD,App->m_LastSettings.nWaterPathTS[STARBOARD]);
		App->m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		App->m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
		break;
	}
	
	if(m_CpOld.Side0.fX - CpSurface.Side0.fX) {
		Buff.Format("%.1f mm",CpSurface.Side0.fX);
		m_editPosXLeft.SetWindowText(Buff);
	}
	if(m_CpOld.Side0.fY - CpSurface.Side0.fY) {
		Buff.Format("%.1f mm",CpSurface.Side0.fY);
		m_editPosYLeft.SetWindowText(Buff);
	}
	if(m_CpOld.Side0.fZ - CpSurface.Side0.fZ) {
		Buff.Format("%.1f mm",CpSurface.Side0.fZ);
		m_editPosZLeft.SetWindowText(Buff);
	}
	if(m_CpOld.Side0.fXt - CpSurface.Side0.fXt) {
		Buff.Format("%.1f%s",CpSurface.Side0.fXt,DEGREES);
		m_editPosXtLeft.SetWindowText(Buff);
	}
	if(m_CpOld.Side0.fYt - CpSurface.Side0.fYt) {
		Buff.Format("%.1f%s",CpSurface.Side0.fYt,DEGREES);
		m_editPosZtLeft.SetWindowText(Buff);
	}

	if(m_CpOld.Side1.fX - CpSurface.Side1.fX) {
		Buff.Format("%.1f mm",CpSurface.Side1.fX);
		m_editPosXRight.SetWindowText(Buff);
	}
	if(m_CpOld.Side1.fY - CpSurface.Side1.fY) {
		Buff.Format("%.1f mm",CpSurface.Side1.fY);
		m_editPosYRight.SetWindowText(Buff);
	}
	if(m_CpOld.Side1.fZ - CpSurface.Side1.fZ) {
		Buff.Format("%.1f mm",CpSurface.Side1.fZ);
		m_editPosZRight.SetWindowText(Buff);
	}
	if(m_CpOld.Side0.fXt - CpSurface.Side1.fXt) {
		Buff.Format("%.1f%s",CpSurface.Side1.fXt,DEGREES);
		m_editPosXtRight.SetWindowText(Buff);
	}
	if(m_CpOld.Side1.fYt - CpSurface.Side1.fYt) {
		Buff.Format("%.1f%s",CpSurface.Side1.fYt,DEGREES);
		m_editPosZtRight.SetWindowText(Buff);
	}


	m_CpOld = CpSurface;

	CResizablePage::OnTimer(nIDEvent);
}

inline void CPositionFokkerPage::MinMax(int *nn, int nMin, int nMax)
{
	if(*nn<nMin) *nn=nMin;
	if(*nn>nMax) *nn=nMax;
}

void CPositionFokkerPage::OnSelchangeComboPositionMode() 
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.nPositionDisplayMode =m_comboPositionMode.GetCurSel();
}
