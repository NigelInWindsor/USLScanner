// VolumeLibraryPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "VolumeLibraryPage.h"
#include "VolumeViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeLibraryPage property page

IMPLEMENT_DYNCREATE(CVolumeLibraryPage, CPropertyPage)


CVolumeLibraryPage::CVolumeLibraryPage(CWnd* pParent,GatesData* pGate) : CPropertyPage(CVolumeLibraryPage::IDD)
{
	//{{AFX_DATA_INIT(CVolumeLibraryPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
	
	m_pParent= pParent;
	m_pGate = pGate;
	
}

CVolumeLibraryPage::~CVolumeLibraryPage()
{
}

void CVolumeLibraryPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeLibraryPage)
	DDX_Control(pDX, IDC_SPIN_GATE_INTERVAL, m_spinGateInterval);
	DDX_Control(pDX, IDC_SPIN_START, m_spinStart);
	DDX_Control(pDX, IDC_SPIN_NUMBER_SLICES, m_spinNumberSlices);
	DDX_Control(pDX, IDC_SPIN_GATE_WIDTH, m_spinGateWidth);
	DDX_Control(pDX, IDC_COMBO_GATE_NUMBER, m_comboGateNumber);
	DDX_Control(pDX, IDC_EDIT_START, m_editStartDepth);
	DDX_Control(pDX, IDC_EDIT_TOTAL_DEPTH, m_editTotalDepth);
	DDX_Control(pDX, IDC_EDIT_NUM_OF_GATES, m_editNumberofGates);
	DDX_Control(pDX, IDC_EDIT_GATE_WIDTH, m_editGateWidth);
	DDX_Control(pDX, IDC_EDIT_GATE_INTERVAL, m_editGateInterval);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_START2, m_editStopDepth);
	DDX_Control(pDX, IDC_SPIN_START2, m_spinStopDepth);
}


BEGIN_MESSAGE_MAP(CVolumeLibraryPage, CPropertyPage)
	//{{AFX_MSG_MAP(CVolumeLibraryPage)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_GATE_WIDTH, OnChangeEditGateWidth)
	ON_EN_CHANGE(IDC_EDIT_GATE_INTERVAL, OnChangeEditGateInterval)
	ON_EN_CHANGE(IDC_EDIT_NUM_OF_GATES, OnChangeEditNumOfGates)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonCreate)
	ON_EN_CHANGE(IDC_EDIT_START, OnChangeEditStart)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE_NUMBER, OnSelchangeComboGateNumber)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GATE_INTERVAL, OnDeltaposSpinGateInterval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GATE_WIDTH, OnDeltaposSpinGateWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMBER_SLICES, OnDeltaposSpinNumberSlices)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_START, OnDeltaposSpinStart)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CVolumeLibraryPage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CVolumeLibraryPage::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_GATE_WIDTH, &CVolumeLibraryPage::OnBnClickedButtonGateWidth)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeLibraryPage message handlers

BOOL CVolumeLibraryPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_spinGateInterval.SetRange(0,10000);
	m_spinStart.SetRange(0,10000);
	m_spinNumberSlices.SetRange(0,10000);
	m_spinGateWidth.SetRange(0,10000);
	
	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CVolumeLibraryPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CVolumeLibraryPage::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertyPage::OnClose();
}


void CVolumeLibraryPage::UpdateAllControls()
{
	
	CString Buff;
	if (GetSafeHwnd() == NULL) return;

	m_comboGateNumber.ResetContent();
	m_comboGateNumber.LimitText(sizeof m_pGate->cName[0] - 1);
	for(int gg=0;gg<8;gg++) {
		m_comboGateNumber.AddString(m_pGate->cName[gg]);
	}
	m_comboGateNumber.SetCurSel(theApp.m_LastSettings.nVolumeCScanGate);

	switch(theApp.m_Scope.m_nGateDelayUnits ){
	case 0: //mm
		Buff.Format(_T("%d"),theApp.m_LastSettings.nVolGatesL);
		m_editNumberofGates.SetWindowText(Buff);

		Buff.Format(L"%0.2f mm", TimeToDistance(theApp.m_LastSettings.fVolGateWidthNs));
		m_editGateWidth.SetWindowText(Buff);

		Buff.Format(L"%0.2f mm",TimeToDistance(theApp.m_LastSettings.fVolGateIntervalNs));
		m_editGateInterval.SetWindowText(Buff);

		Buff.Format(L"%0.2f mm", TimeToDistance(theApp.m_LastSettings.fVolStartDepthNs));
		m_editStartDepth.SetWindowText(Buff);

		Buff.Format(L"%0.2f mm", TimeToDistance(theApp.m_LastSettings.fVolStopDepthNs));
		m_editStopDepth.SetWindowText(Buff);

		break;
	case 1: //us
		Buff.Format(_T("%d"),theApp.m_LastSettings.nVolGatesL);
		m_editNumberofGates.SetWindowText(Buff);

		Buff.Format(_T("%0.2fus"),(double)theApp.m_LastSettings.fVolGateWidthNs/1000.0f);
		m_editGateWidth.SetWindowText(Buff);

		Buff.Format(_T("%0.2fus"),(double)theApp.m_LastSettings.fVolGateIntervalNs/1000.0f);
		m_editGateInterval.SetWindowText(Buff);

		Buff.Format(_T("%0.2fus"), ((double)theApp.m_LastSettings.fVolStartDepthNs / 1000.0f));
		m_editStartDepth.SetWindowText(Buff);

		Buff.Format(_T("%0.2fus"), ((double)theApp.m_LastSettings.fVolStopDepthNs / 1000.0f));
		m_editStopDepth.SetWindowText(Buff);

		break;
	}

	float fTotalTime = theApp.m_LastSettings.fVolStartDepthNs + (theApp.m_LastSettings.fVolGateIntervalNs * (float)theApp.m_LastSettings.nVolGatesL) + theApp.m_LastSettings.fVolGateWidthNs;
	Buff.Format(L"%0.1fmm", TimeToDistance(fTotalTime));
	m_editTotalDepth.SetWindowText(Buff);

}

void CVolumeLibraryPage::OnChangeEditGateWidth() 
{
	CString Buff;
	float fWidthns = theApp.m_LastSettings.fVolGateWidthNs;

	m_editGateWidth.GetWindowText(Buff);
	theApp.m_LastSettings.fVolGateWidthNs = (float)theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateWidthUnits);

	if(fWidthns - theApp.m_LastSettings.fVolGateWidthNs) {
		m_pGate->nNsWidth[theApp.m_LastSettings.nVolumeCScanGate] = (int)theApp.m_LastSettings.fVolGateWidthNs;
		InvalidateParent();
	}

	
}

void CVolumeLibraryPage::OnChangeEditGateInterval() 
{
	
	CString Buff;
	float fTemp = theApp.m_LastSettings.fVolGateIntervalNs;

	m_editGateInterval.GetWindowText(Buff);
	theApp.m_LastSettings.fVolGateIntervalNs = (float)theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateDelayUnits);

	if(fTemp - theApp.m_LastSettings.fVolGateIntervalNs) {
		UpdateAllControls();
	}

	
}

void CVolumeLibraryPage::OnChangeEditNumOfGates() 
{
	
	CString Buff;
	int nTemp = theApp.m_LastSettings.nVolGatesL;

	m_editNumberofGates.GetWindowText(Buff);
	theApp.m_LastSettings.nVolGatesL = _ttoi(Buff);

	if(nTemp - theApp.m_LastSettings.nVolGatesL) {
		UpdateAllControls();
	}
}

void CVolumeLibraryPage::OnButtonCreate() 
{
	CVolumeViewDlg* pParent = (CVolumeViewDlg*)m_pParent;
	CString ImageName;

	int nGate = theApp.m_LastSettings.nVolumeCScanGate;

	for(int ii=0; ii<theApp.m_LastSettings.nVolGatesL; ii++) {
		m_pGate->nNsDelay[ nGate ]=(int)((theApp.m_LastSettings.fVolGateIntervalNs*ii)+theApp.m_LastSettings.fVolStartDepthNs);
		m_pGate->nNsWidth[ nGate ]=(int)theApp.m_LastSettings.fVolGateWidthNs;

		float fStartTime = (theApp.m_LastSettings.fVolGateIntervalNs * (float)ii) + theApp.m_LastSettings.fVolStartDepthNs;
		float fFinishTime = fStartTime + theApp.m_LastSettings.fVolGateWidthNs;

		switch(theApp.m_Scope.m_nGateDelayUnits){
		case 0:
		case 1:
			ImageName.Format(L"Amp %0.2fus->%0.2fus",fStartTime / 1000.0f, fFinishTime / 1000.0f);
			break;
		default:
			ImageName.Format(L"Amp %0.1fmm->%0.1fmm", TimeToDistance(fStartTime), TimeToDistance(fFinishTime));
			break;
		}
		pParent->CreateAmpCscan(ImageName);
	}
}

void CVolumeLibraryPage::OnChangeEditStart() 
{
	CString Buff;
	float fStartDepthns = theApp.m_LastSettings.fVolStartDepthNs;

	m_editStartDepth.GetWindowText(Buff);
	theApp.m_LastSettings.fVolStartDepthNs = (float)theApp.ScanfDistanceTimens(Buff, theApp.m_Scope.m_nGateDelayUnits);
	if(fStartDepthns - theApp.m_LastSettings.fVolStartDepthNs) {
		InvalidateNumberSlices();
	}
}

void CVolumeLibraryPage::OnSelchangeComboGateNumber() 
{
	theApp.m_LastSettings.nVolumeCScanGate=m_comboGateNumber.GetCurSel();
	UpdateAllControls();
}

void CVolumeLibraryPage::InvalidateParent()
{
	CVolumeViewDlg* pVolumeView = (CVolumeViewDlg*)m_pParent;

	pVolumeView->SendMessage(WM_PAINT);
}


void CVolumeLibraryPage::OnDeltaposSpinGateInterval(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fStepns;

	switch (theApp.m_Scope.m_nGateDelayUnits) {
	default:	fStepns = 100.0f;
		break;
	case 2:
	case 4:	fStepns = 0.01f / (float)(theApp.m_LastSettings.nWaterVelocity / 1000) * 2000.0f;
		break;
	case 3:
	case 5:	fStepns = 0.01f / (float)(theApp.m_LastSettings.nMaterialVelocity / 1000) * 2000.0f;
		break;
	}
	theApp.m_LastSettings.fVolGateIntervalNs += (fStepns * (float)pNMUpDown->iDelta);
	InvalidateNumberSlices();
	UpdateAllControls();
	InvalidateParent();
	
	*pResult = 0;
}

void CVolumeLibraryPage::OnDeltaposSpinGateWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fStepns;

	switch(theApp.m_Scope.m_nGateDelayUnits){
	default:	fStepns = 100.0f;
		break;
	case 2:
	case 4:	fStepns = 0.01f / (float)(theApp.m_LastSettings.nWaterVelocity / 1000) * 2000.0f;
		break;
	case 3:
	case 5:	fStepns = 0.01f / (float)(theApp.m_LastSettings.nMaterialVelocity / 1000) * 2000.0f;
		break;
	}
	theApp.m_LastSettings.fVolGateWidthNs += (fStepns * (float)pNMUpDown->iDelta);
	m_pGate->nNsWidth[theApp.m_LastSettings.nVolumeCScanGate] = (int)theApp.m_LastSettings.fVolGateWidthNs;
	UpdateAllControls();
	InvalidateParent();
	
	*pResult = 0;
}

void CVolumeLibraryPage::OnDeltaposSpinNumberSlices(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	theApp.m_LastSettings.nVolGatesL  += pNMUpDown->iDelta;
	InvalidateGateInterval();
	UpdateAllControls();
	
	*pResult = 0;
}

void CVolumeLibraryPage::OnDeltaposSpinStart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fStepns;

	switch (theApp.m_Scope.m_nGateDelayUnits) {
	default:	fStepns = 10.0f;
		break;
	case 2:
	case 4:	fStepns = 0.01f / (float)(theApp.m_LastSettings.nWaterVelocity / 1000) * 2000.0f;
		break;
	case 3:
	case 5:	fStepns = 0.01f / (float)(theApp.m_LastSettings.nMaterialVelocity / 1000) * 2000.0f;
		break;
	}
	theApp.m_LastSettings.fVolStartDepthNs += (fStepns * (float)pNMUpDown->iDelta);
	InvalidateNumberSlices();
	UpdateAllControls();
	
	*pResult = 0;
}

float CVolumeLibraryPage::TimeToDistance(float fTime)
{
	return (fTime / 1000.0f) * ((float)theApp.m_LastSettings.nMaterialVelocity / 1000.0f) / 2.0f;
}


void CVolumeLibraryPage::OnBnClickedButtonStart()
{
	theApp.m_LastSettings.fVolStartDepthNs = (float)m_pGate->nNsDelay[theApp.m_LastSettings.nVolumeCScanGate];
	InvalidateNumberSlices();
	UpdateAllControls();
	InvalidateParent();
}


void CVolumeLibraryPage::OnBnClickedButtonStop()
{
	theApp.m_LastSettings.fVolStopDepthNs = (float)m_pGate->nNsDelay[theApp.m_LastSettings.nVolumeCScanGate];
	InvalidateNumberSlices();
	UpdateAllControls();
	InvalidateParent();
}


void CVolumeLibraryPage::OnBnClickedButtonGateWidth()
{
	theApp.m_LastSettings.fVolGateWidthNs = (float)m_pGate->nNsWidth[theApp.m_LastSettings.nVolumeCScanGate];
	UpdateAllControls();
	InvalidateParent();
}

void CVolumeLibraryPage::InvalidateNumberSlices()
{
	float fInterval = theApp.m_LastSettings.fVolGateIntervalNs;
	if (fInterval <= 0.0f) fInterval = 100;
	theApp.m_LastSettings.nVolGatesL = (int)(fabsf(theApp.m_LastSettings.fVolStopDepthNs - theApp.m_LastSettings.fVolStartDepthNs) / fInterval);
}

void CVolumeLibraryPage::InvalidateGateInterval()
{
	int nSize = theApp.m_LastSettings.nVolGatesL;
	if (nSize <= 0) nSize = 1;
	theApp.m_LastSettings.fVolGateIntervalNs = fabsf(theApp.m_LastSettings.fVolStopDepthNs - theApp.m_LastSettings.fVolStartDepthNs) / (float)nSize;
}