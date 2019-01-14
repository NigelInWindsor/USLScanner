// PrintOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PrintOptionsDlg.h"
#include "PrintConesCylindersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg dialog


CPrintOptionsDlg::CPrintOptionsDlg(CWnd* pParent, CWnd** pDlg,CData* pData)
	: CDialog(CPrintOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pData = pData;
}


void CPrintOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintOptionsDlg)
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_CHECK_TRANSPARENT, m_checkTransparent);
	DDX_Control(pDX, IDC_SPIN_FONT_SIZE, m_spinFontSize);
	DDX_Control(pDX, IDC_EDIT_FONT_SIZE, m_editFontSize);
	DDX_Control(pDX, IDC_COMBO_R_AXIS_UNITS, m_comboRaxisMode);
	DDX_Control(pDX, IDC_SPIN_GRATICULE_START_ANGLE, m_spinGraticuleStartAngle);
	DDX_Control(pDX, IDC_EDIT_GRATICULE_START_ANGLE, m_editGraticuleStartAngle);
	DDX_Control(pDX, IDC_EDIT_PAPER_WIDTH, m_editPaperWidth);
	DDX_Control(pDX, IDC_SPIN_SCAN_HEIGHT, m_spinScanHeight);
	DDX_Control(pDX, IDC_SPIN_LOWER_DIAMETER, m_spinLowerDiameter);
	DDX_Control(pDX, IDC_SPIN_UPPER_DIAMETER, m_spinUpperDiameter);
	DDX_Control(pDX, IDC_EDIT_COMPONENT_HEIGHT, m_editComponentHeight);
	DDX_Control(pDX, IDC_EDIT_LOWER_DIAMETER, m_editLowerDiameter);
	DDX_Control(pDX, IDC_EDIT_UPPER_DIAMETER, m_editUpperDiameter);
	DDX_Control(pDX, IDC_LIST_OPTIONS, m_listOptions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintOptionsDlg)
	ON_LBN_SELCHANGE(IDC_LIST_OPTIONS, OnSelchangeListOptions)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCAN_HEIGHT, OnDeltaposSpinScanHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LOWER_DIAMETER, OnDeltaposSpinLowerDiameter)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_UPPER_DIAMETER, OnDeltaposSpinUpperDiameter)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GRATICULE_START_ANGLE, OnDeltaposSpinGraticuleStartAngle)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_CONE_1_1, OnButtonCone11)
	ON_CBN_SELCHANGE(IDC_COMBO_R_AXIS_UNITS, OnSelchangeComboRAxisUnits)
	ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, OnChangeEditFontSize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FONT_SIZE, OnDeltaposSpinFontSize)
	ON_BN_CLICKED(IDC_CHECK_TRANSPARENT, OnCheckTransparent)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_EDIT_GRATICULE_START_ANGLE, OnChangeEditGraticuleStartAngle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg message handlers
void CPrintOptionsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CPrintOptionsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[this->IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CPrintOptionsDlg::OnDestroy() 
{
	if (this->IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[this->IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	

}

BOOL CPrintOptionsDlg::OnInitDialog() 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	CDialog::OnInitDialog();
	CString Buff;
	int nPos;
	CString Degrees = L"\x0b0";

	if (this->IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[this->IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	m_listOptions.ResetContent();
	m_listOptions.SetCheckStyle(BS_AUTOCHECKBOX);

	Buff.Format(L"File Path: %s",m_pData->m_FilePath);				m_listOptions.AddString(Buff);
	if((nPos = m_pData->m_FileName.ReverseFind('.')) >0) {
		Buff.Format(L"File Name: %s", m_pData->m_FileName.Left(nPos));
	} else {
		Buff.Format(L"File Name: %s",m_pData->m_FileName);
	}
																	m_listOptions.AddString(Buff);
	Buff.Format(L"Profile: %s",m_pData->m_ProfileName);				m_listOptions.AddString(Buff);
	Buff.Format(L"Operator: %s",m_pData->m_Operator);				m_listOptions.AddString(Buff);
	CTime time(m_pData->m_osTestDateTime);
	Buff = time.Format(_T("%d/%m/%Y %Hh%M"));						m_listOptions.AddString(Buff);
	
	int nMask = 1;
	for(int nn=0;nn<5;nn++,nMask <<= 1) {
		theApp.m_LastSettings.nPrintOptionsMask & nMask ? m_listOptions.SetCheck(nn,true) : m_listOptions.SetCheck(nn,false);
	}


	m_spinScanHeight.SetRange(0,5000);
	m_spinLowerDiameter.SetRange(0,5000);
	m_spinUpperDiameter.SetRange(0,5000);
	m_spinScanHeight.SetPos(2500);
	m_spinLowerDiameter.SetPos(2500);
	m_spinUpperDiameter.SetPos(2500);

	m_spinGraticuleStartAngle.SetRange(0,720);
	m_spinGraticuleStartAngle.SetPos(0);

	Buff.LoadString(IDS_Degrees);		m_comboRaxisMode.AddString(Buff);
	Buff.LoadString(IDS_Length);		m_comboRaxisMode.AddString(Buff);
	
	m_spinFontSize.SetRange(1,30);
	m_spinFontSize.SetPos(theApp.m_LastSettings.nConeFontSize);

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintOptionsDlg::UpdateAllControls()
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	CString Buff;
	CString Degrees = L"\x0b0";

	Buff.Format(L"%.*f%s",theApp.m_nUnitsPrecision,pDlg->Cone.fR[CONE_TOP] * theApp.m_fUnits * 2.0f, theApp.m_Units);
	m_editUpperDiameter.SetWindowText(Buff);
	Buff.Format(L"%.*f%s",theApp.m_nUnitsPrecision,pDlg->Cone.fR[CONE_BOT] * theApp.m_fUnits * 2.0f, theApp.m_Units);
	m_editLowerDiameter.SetWindowText(Buff);
	Buff.Format(L"%.*f%s",theApp.m_nUnitsPrecision,pDlg->Cone.fScanHeight * theApp.m_fUnits, theApp.m_Units);
	m_editComponentHeight.SetWindowText(Buff);

	
	Buff.Format(L"%.*f%s",theApp.m_nUnitsPrecision,theApp.m_LastSettings.fPrintPaperWidth * theApp.m_fUnits, theApp.m_Units);
	m_editPaperWidth.SetWindowText(Buff);


	Buff.Format(L"%.01f%s",pDlg->m_fGratStartAngle,Degrees);
	m_editGraticuleStartAngle.SetWindowText(Buff);

	m_comboRaxisMode.SetCurSel(theApp.m_LastSettings.nRAxisGraticuleMode);

	Buff.Format(L"%d",theApp.m_LastSettings.nConeFontSize);
	m_editFontSize.SetWindowText(Buff);
	m_checkTransparent.SetCheck(theApp.m_LastSettings.nConeTextTransparent);

}

void CPrintOptionsDlg::OnSelchangeListOptions() 
{
	// TODO: Add your control notification handler code here
	
}


void CPrintOptionsDlg::OnOK() 
{
	theApp.m_LastSettings.nPrintOptionsMask = 0;
	int nMask = 1;
	for(int nn=0;nn<5;nn++, nMask<<=1) {
		if(m_listOptions.GetCheck(nn) == TRUE) theApp.m_LastSettings.nPrintOptionsMask |= nMask;
	}

	CDialog::OnOK();
}

void CPrintOptionsDlg::OnButtonApply() 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;

	CString Buff;
	float fTemp;

	m_editUpperDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	pDlg->Cone.fR[CONE_TOP] = fTemp / theApp.m_fUnits / 2.0f;

	m_editLowerDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	pDlg->Cone.fR[CONE_BOT] = fTemp / theApp.m_fUnits / 2.0f;

	m_editComponentHeight.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	pDlg->Cone.fScanHeight = fTemp / theApp.m_fUnits;

	m_editPaperWidth.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	theApp.m_LastSettings.fPrintPaperWidth = fTemp / theApp.m_fUnits;

	m_editGraticuleStartAngle.GetWindowText(Buff);
	_WTOF(Buff,pDlg->m_fGratStartAngle);

	theApp.m_LastSettings.nPrintOptionsMask = 0;
	int nMask = 1;
	for(int nn=0;nn<5;nn++, nMask<<=1) {
		if(m_listOptions.GetCheck(nn) == TRUE) theApp.m_LastSettings.nPrintOptionsMask |= nMask;
	}

	pDlg->m_fVertMag=1.0f;
	pDlg->CalculateConeGeometry();
	pDlg->SetToolBarCheckedState();
	pDlg->Invalidate(false);

}


void CPrintOptionsDlg::OnDeltaposSpinScanHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	pDlg->Cone.fScanHeight += ((float)pNMUpDown->iDelta * 12.5f);
	if(pDlg->Cone.fScanHeight<0.0f) pDlg->Cone.fScanHeight=0.0f;
	if(pDlg->Cone.fScanHeight>5000.0f) pDlg->Cone.fScanHeight=5000.0f;
	UpdateAllControls();
	
	*pResult = 0;
}

void CPrintOptionsDlg::OnDeltaposSpinLowerDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	pDlg->Cone.fR[CONE_BOT] += ((float)pNMUpDown->iDelta * 6.25f);
	if(pDlg->Cone.fR[CONE_BOT]<0.0f) pDlg->Cone.fR[CONE_BOT]=0.0f;
	if(pDlg->Cone.fR[CONE_BOT]>5000.0f) pDlg->Cone.fR[CONE_BOT]=5000.0f;
	UpdateAllControls();
	
	*pResult = 0;
}

void CPrintOptionsDlg::OnDeltaposSpinUpperDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	pDlg->Cone.fR[CONE_TOP] += ((float)pNMUpDown->iDelta * 6.25f);
	if(pDlg->Cone.fR[CONE_TOP]<0.0f) pDlg->Cone.fR[CONE_TOP]=0.0f;
	if(pDlg->Cone.fR[CONE_TOP]>5000.0f) pDlg->Cone.fR[CONE_TOP]=5000.0f;
	UpdateAllControls();
	
	*pResult = 0;
}

void CPrintOptionsDlg::OnDeltaposSpinGraticuleStartAngle(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	pDlg->m_fGratStartAngle += (float)pNMUpDown->iDelta;
	UpdateAllControls();
	InvalidateParent();
	
	*pResult = 0;
}


void CPrintOptionsDlg::OnButtonCone11() 
{

}

void CPrintOptionsDlg::OnSelchangeComboRAxisUnits() 
{
	theApp.m_LastSettings.nRAxisGraticuleMode = m_comboRaxisMode.GetCurSel();
	InvalidateParent();
}


void CPrintOptionsDlg::InvalidateParent()
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;

	pDlg->Invalidate(false);
}

void CPrintOptionsDlg::OnChangeEditFontSize() 
{
	CString Buff;
	int nTemp = theApp.m_LastSettings.nConeFontSize;

	m_editFontSize.GetWindowText(Buff);
	theApp.m_LastSettings.nConeFontSize = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nConeFontSize) InvalidateParent();
}

void CPrintOptionsDlg::OnDeltaposSpinFontSize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_LastSettings.nConeFontSize += pNMUpDown->iDelta;
	UpdateAllControls();
	InvalidateParent();
	
	*pResult = 0;
}

void CPrintOptionsDlg::OnCheckTransparent() 
{
	theApp.m_LastSettings.nConeTextTransparent = m_checkTransparent.GetCheck();
	InvalidateParent();
}



void CPrintOptionsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbConeTextColor));
}

void CPrintOptionsDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbConeTextColor=dlg.GetColor();
			Invalidate(FALSE);
			InvalidateParent();
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CPrintOptionsDlg::OnChangeEditGraticuleStartAngle() 
{
	CPrintConesCylindersDlg* pDlg = (CPrintConesCylindersDlg*)m_pParent;
	CString Buff;
	float fTemp = pDlg->m_fGratStartAngle;

	m_editGraticuleStartAngle.GetWindowText(Buff);
	_WTOF(Buff,pDlg->m_fGratStartAngle);
	if(fTemp - pDlg->m_fGratStartAngle) {
		m_spinGraticuleStartAngle.SetPos((int)pDlg->m_fGratStartAngle);
		InvalidateParent();
	}

}
