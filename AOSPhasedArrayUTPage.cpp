#include "stdafx.h"
#include "USLScanner.h"
#include "AOSPhasedArrayUTPage.h"

#define U_ANALOGUE_GAIN	0x0001
#define U_DIGITAL_GAIN	0x0002
#define U_PULSE_WIDTH	0x0004
#define U_PRF			0x0008
#define	U_RECTIFY		0x0010
#define U_DAC			0x0020

IMPLEMENT_DYNCREATE(CAOSPhasedArrayUTPage, CPropertyPage)

extern UINT AOSUTThread(LPVOID pParam);

float m_fMaxFilterFrequency;

CAOSPhasedArrayUTPage::CAOSPhasedArrayUTPage() : CPropertyPage(CAOSPhasedArrayUTPage::IDD)
{
	//{{AFX_DATA_INIT(CAOSPhasedArrayPage)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nUpdateHardware = 0;
	m_bThreadEnabled = false;
}

CAOSPhasedArrayUTPage::~CAOSPhasedArrayUTPage()
{
	SuspendThread();
}

void CAOSPhasedArrayUTPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAOSPhasedArrayPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SPIN_ANALOGUE_GAIN, m_editspinAnalogueGain);
	DDX_Control(pDX, IDC_SPIN_DIGITAL_GAIN, m_editspinDigitalGain);
	DDX_Control(pDX, IDC_SPIN_PULSE_WIDTH, m_editspinPulseWidth);
	DDX_Control(pDX, IDC_SPIN_PRF, m_editspinPRF);
	DDX_Control(pDX, IDC_COMBO_RECTIFY, m_comboRectify);
	DDX_Control(pDX, IDC_SPIN_MAX_FILTER_FREQUENCY, m_editspinMaxFilterFrequency);
	DDX_Control(pDX, IDC_SPIN_MIN_FILTER_FREQUENCY, m_editspinMinFilterFrequency);
	DDX_Control(pDX, IDC_SPIN_RIPPLE, m_editspinRipple);
	DDX_Control(pDX, IDC_SPIN_STOP_GAIN, m_editspinStopGain);
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_comboFilterType);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_MODE, m_comboDisplayMode);
	DDX_Control(pDX, IDC_STATIC_FREQUENCY, m_staticView);
	DDX_Control(pDX, IDC_SPIN_FILTER_GAIN, m_editspinFilterGain);
	DDX_Control(pDX, IDC_LIST_DAC_LIST, m_listDAC);
	DDX_Control(pDX, IDC_COMBO_DAC_MODE, m_comboDacMode);
}


BEGIN_MESSAGE_MAP(CAOSPhasedArrayUTPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAOSPhasedArrayPage)
	ON_WM_CTLCOLOR()
//	ON_EN_CHANGE(IDC_EDIT_MAX_FILTER_FREQUENCY, &CAOSPhasedArrayUTPage::OnEnChangeEditMaxFilterFrequency)
	ON_EN_CHANGE(IDC_EDIT_ANALOGUE_GAIN, &CAOSPhasedArrayUTPage::OnEnChangeEditAnalogueGain)
	ON_EN_CHANGE(IDC_EDIT_DIGITAL_GAIN, &CAOSPhasedArrayUTPage::OnEnChangeEditDigitalGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ANALOGUE_GAIN, &CAOSPhasedArrayUTPage::OnDeltaposSpinAnalogueGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIGITAL_GAIN, &CAOSPhasedArrayUTPage::OnDeltaposSpinDigitalGain)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_PULSE_WIDTH, &CAOSPhasedArrayUTPage::OnEnChangeEditPulseWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PULSE_WIDTH, &CAOSPhasedArrayUTPage::OnDeltaposSpinPulseWidth)
	ON_EN_CHANGE(IDC_EDIT_PRF, &CAOSPhasedArrayUTPage::OnEnChangeEditPrf)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRF, &CAOSPhasedArrayUTPage::OnDeltaposSpinPrf)
	ON_CBN_SELCHANGE(IDC_COMBO_RECTIFY, &CAOSPhasedArrayUTPage::OnCbnSelchangeComboRectify)
	ON_EN_CHANGE(IDC_EDIT_MAX_FILTER_FREQUENCY, &CAOSPhasedArrayUTPage::OnEnChangeEditMaxFilterFrequency)
	ON_EN_CHANGE(IDC_EDIT_MIN_FILTER_FREQUENCY, &CAOSPhasedArrayUTPage::OnEnChangeEditMinFilterFrequency)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, &CAOSPhasedArrayUTPage::OnCbnSelchangeComboFilterType)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CAOSPhasedArrayUTPage::OnBnClickedButtonApply)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, &CAOSPhasedArrayUTPage::OnCbnSelchangeComboDisplayMode)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MIN_FILTER_FREQUENCY, &CAOSPhasedArrayUTPage::OnDeltaposSpinMinFilterFrequency)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAX_FILTER_FREQUENCY, &CAOSPhasedArrayUTPage::OnDeltaposSpinMaxFilterFrequency)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIPPLE, &CAOSPhasedArrayUTPage::OnDeltaposSpinRipple)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STOP_GAIN, &CAOSPhasedArrayUTPage::OnDeltaposSpinStopGain)
ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FILTER_GAIN, &CAOSPhasedArrayUTPage::OnDeltaposSpinFilterGain)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DAC_LIST, &CAOSPhasedArrayUTPage::OnLvnGetdispinfoListDacList)
	ON_CBN_SELCHANGE(IDC_COMBO_DAC_MODE, &CAOSPhasedArrayUTPage::OnCbnSelchangeComboDacMode)
END_MESSAGE_MAP()

void CAOSPhasedArrayUTPage::OnClose()
{
	theApp.SuspendThread(this);

	CPropertyPage::OnClose();
}


void CAOSPhasedArrayUTPage::OnDestroy()
{
	CPropertyPage::OnDestroy();

	theApp.SuspendThread(this);
}

BOOL CAOSPhasedArrayUTPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	m_comboRectify.AddString(L"RF");
	m_comboRectify.AddString(L"Full wave");
	m_comboRectify.AddString(L"+ve half wave");
	m_comboRectify.AddString(L"-ve half wave");

	Buff.LoadStringW(IDS_Off);			m_comboFilterType.AddString(Buff);
	Buff.LoadStringW(IDS_Lowpass);		m_comboFilterType.AddString(Buff);
	Buff.LoadStringW(IDS_Highpass);		m_comboFilterType.AddString(Buff);
	Buff.LoadStringW(IDS_Bandpass);		m_comboFilterType.AddString(Buff);
	Buff.LoadStringW(IDS_Bandreject);	m_comboFilterType.AddString(Buff);

	Buff.LoadStringW(IDS_Frequency);	m_comboDisplayMode.AddString(Buff);
	Buff.LoadStringW(IDS_ImpulseResponce);	m_comboDisplayMode.AddString(Buff);

	Buff.LoadStringW(IDS_Off);			m_comboDacMode.AddString(Buff);
	Buff.LoadStringW(IDS_Main_Bang);	m_comboDacMode.AddString(Buff);
	Buff.LoadStringW(IDS_Interface);	m_comboDacMode.AddString(Buff);

	m_editspinPulseWidth.Init(theApp.m_PhasedArray[PORTSIDE].getPulseWidth(), 0.0f, 1000.0e-9f, 5.0e-9f, 1.0e-9f, L"%.0f ns");
	m_editspinPRF.Init((float)theApp.m_UtUser.m_Global.nPrf, 0.0f, 10000.0f, 10.0f, 1.0f, L"%.0f Hz");
	m_editspinAnalogueGain.Init(theApp.m_PhasedArray[PORTSIDE].getAnalogueGain(), 0.0f, 47.7f, 0.1f, 1.0f, L"%.01f dB");
	m_editspinDigitalGain.Init(theApp.m_PhasedArray[PORTSIDE].getDigitalGain(), 0.0f, 80.0f, 0.1f, 1.0f, L"%.01f dB");
	m_editspinMinFilterFrequency.Init(theApp.m_PhasedArray[PORTSIDE].m_fFilterFreq[0], 1.0e6f, 25.0e6f, 0.1e6f, 1e6f, L"%.01f MHz");
	m_editspinMaxFilterFrequency.Init(theApp.m_PhasedArray[PORTSIDE].m_fFilterFreq[1], 1.0e6f, 25.0e6f, 0.1e6f, 1e6f, L"%.01f MHz");
	m_editspinRipple.Init(theApp.m_PhasedArray[PORTSIDE].m_fRipple, 0.5f, 1.0f, 0.1f, 1.0f, L"%.01f dB");
	m_editspinStopGain.Init(theApp.m_PhasedArray[PORTSIDE].m_fStopGain, 0.0f, 50.0f, 0.5f, 1.0f, L"%.01f dB");
	m_editspinFilterGain.Init((float)theApp.m_PhasedArray[PORTSIDE].m_nFilterGain,1.0f,24.0f,1.0f,1.0f,L"%.0f");

	CreateColumnsDAC();
	FillListDAC();

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAOSPhasedArrayUTPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAOSPhasedArrayUTPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	bool bFlag = true;

	m_comboDisplayMode.SetCurSel(theApp.m_LastSettings.nPAFilterDisplayMode &= 1);
	m_comboRectify.SetCurSel(theApp.m_PhasedArray[PORTSIDE].getRectification());
	m_comboFilterType.SetCurSel(theApp.m_PhasedArray[PORTSIDE].m_eFilterType);
	m_comboDacMode.SetCurSel(theApp.m_PhasedArray[PORTSIDE].getDacMode());

	theApp.m_PhasedArray[PORTSIDE].getFilterType() < BANDPASS ? bFlag = true : bFlag = false;
	((CEditSpinCtrl*)GetDlgItem(IDC_SPIN_MAX_FILTER_FREQUENCY))->SetReadOnly(bFlag);
	((CEditSpinCtrl*)GetDlgItem(IDC_SPIN_RIPPLE))->SetReadOnly(true);

	m_listDAC.Invalidate(false);
}

BOOL CAOSPhasedArrayUTPage::OnSetActive()
{
	theApp.m_LastSettings.nLastPhasedArrayTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	StartThread();
	UpdateAllControls();

	return CPropertyPage::OnSetActive();
}

BOOL CAOSPhasedArrayUTPage::OnKillActive()
{

	SuspendThread();

	return CPropertyPage::OnKillActive();
}

void CAOSPhasedArrayUTPage::StartThread()
{
	theApp.StartThread(L"AOS Ultrasonics Dlg", &AOSUTThread, this, 200, THREAD_PRIORITY_NORMAL);
}

void CAOSPhasedArrayUTPage::SuspendThread()
{
	theApp.SuspendThread(this);
}

UINT AOSUTThread(LPVOID pParam)
{
	CAOSPhasedArrayUTPage* pParent = (CAOSPhasedArrayUTPage*)pParam;

	TRACE0("AOS Thread waiting\n");
	if (WaitForSingleObject(pParent->m_hSemaphore, 10) == WAIT_OBJECT_0) {

		if (pParent->GetSafeHwnd() && pParent->IsWindowVisible() == TRUE) {

			if (pParent->m_nUpdateHardware & U_ANALOGUE_GAIN) {
				theApp.m_PhasedArray[PORTSIDE].setAnalogueGain(theApp.m_PhasedArray[PORTSIDE].getAnalogueGain(), true);
				pParent->m_nUpdateHardware &= ~U_ANALOGUE_GAIN;
			}

			if (pParent->m_nUpdateHardware & U_DIGITAL_GAIN) {
				theApp.m_PhasedArray[PORTSIDE].setDigitalGain(theApp.m_PhasedArray[PORTSIDE].getDigitalGain(), true);
				pParent->m_nUpdateHardware &= ~U_DIGITAL_GAIN;
			}

			if (pParent->m_nUpdateHardware & U_PULSE_WIDTH) {
				theApp.m_PhasedArray[PORTSIDE].setPulseWidth(theApp.m_PhasedArray[PORTSIDE].getPulseWidth(), true);
				pParent->m_nUpdateHardware &= ~U_PULSE_WIDTH;
			}

			if (pParent->m_nUpdateHardware & U_PRF) {
				theApp.m_PhasedArray[PORTSIDE].setPRF(theApp.m_UtUser.m_Global.nPrf, true);
				pParent->m_nUpdateHardware &= ~U_PRF;
			}

			if (pParent->m_nUpdateHardware & U_RECTIFY) {
				theApp.m_PhasedArray[PORTSIDE].setRectification(theApp.m_PhasedArray[PORTSIDE].getRectification(), true);
				pParent->m_nUpdateHardware &= ~U_RECTIFY;
			}

			if (pParent->m_nUpdateHardware & U_DAC) {
				theApp.m_PhasedArray[PORTSIDE].setAllDacVariables();
				pParent->m_nUpdateHardware &= ~U_DAC;
			}
		}
		ReleaseSemaphore(pParent->m_hSemaphore, 1, NULL);
	}
	TRACE0("AOS Thread waiting completed\n");

	return THREAD_CONTINUE;
}




void CAOSPhasedArrayUTPage::OnEnChangeEditAnalogueGain()
{
	theApp.m_PhasedArray[PORTSIDE].setAnalogueGain(m_editspinAnalogueGain.GetValue(), false);
	m_nUpdateHardware |= U_ANALOGUE_GAIN;
}

void CAOSPhasedArrayUTPage::OnEnChangeEditDigitalGain()
{
	theApp.m_PhasedArray[PORTSIDE].setDigitalGain(m_editspinDigitalGain.GetValue(), false);
	m_nUpdateHardware |= U_DIGITAL_GAIN;

}


void CAOSPhasedArrayUTPage::OnDeltaposSpinAnalogueGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_PhasedArray[PORTSIDE].setAnalogueGain(m_editspinAnalogueGain.GetValue(), false);
	m_nUpdateHardware |= U_ANALOGUE_GAIN;

	*pResult = 0;
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinDigitalGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_PhasedArray[PORTSIDE].setDigitalGain(m_editspinDigitalGain.GetValue(), false);
	m_nUpdateHardware |= U_DIGITAL_GAIN;

	*pResult = 0;
}


void CAOSPhasedArrayUTPage::OnTimer(UINT_PTR nIDEvent)
{

	CPropertyPage::OnTimer(nIDEvent);
}


void CAOSPhasedArrayUTPage::OnEnChangeEditPulseWidth()
{
	theApp.m_PhasedArray[PORTSIDE].setPulseWidth(m_editspinPulseWidth.GetValue() , false);
	m_nUpdateHardware |= U_PULSE_WIDTH;
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinPulseWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_PhasedArray[PORTSIDE].setPulseWidth(m_editspinPulseWidth.GetValue(), false);
	m_nUpdateHardware |= U_PULSE_WIDTH;

	*pResult = 0;
}


void CAOSPhasedArrayUTPage::OnEnChangeEditPrf()
{
	theApp.m_UtUser.m_Global.nPrf = (int)m_editspinPRF.GetValue();
	m_nUpdateHardware |= U_PRF;
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinPrf(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_UtUser.m_Global.nPrf = (int)m_editspinPRF.GetValue();
	m_nUpdateHardware |= U_PRF;

	*pResult = 0;
}


void CAOSPhasedArrayUTPage::OnCbnSelchangeComboRectify()
{
	theApp.m_PhasedArray[PORTSIDE].setRectification(m_comboRectify.GetCurSel(), false);
	m_nUpdateHardware |= U_RECTIFY;
}


void CAOSPhasedArrayUTPage::OnEnChangeEditMaxFilterFrequency()
{
	theApp.m_PhasedArray->m_fFilterFreq[1] = m_editspinMaxFilterFrequency.GetValue();
}


void CAOSPhasedArrayUTPage::OnEnChangeEditMinFilterFrequency()
{
	theApp.m_PhasedArray->m_fFilterFreq[0] = m_editspinMinFilterFrequency.GetValue();
}


void CAOSPhasedArrayUTPage::OnCbnSelchangeComboFilterType()
{
	theApp.m_PhasedArray[PORTSIDE].m_eFilterType = (FrequencyFilterType)m_comboFilterType.GetCurSel();
	UpdateAllControls();
	Invalidate(false);
}


void CAOSPhasedArrayUTPage::OnBnClickedButtonApply()
{
	theApp.m_PhasedArray[PORTSIDE].ApplyFilter();
	Invalidate(false);
}


void CAOSPhasedArrayUTPage::OnCbnSelchangeComboDisplayMode()
{
	theApp.m_LastSettings.nPAFilterDisplayMode = m_comboDisplayMode.GetCurSel();
	Invalidate(false);

}


void CAOSPhasedArrayUTPage::OnPaint()
{
	CPaintDC dc(this);

	theApp.m_PhasedArray[PORTSIDE].ApplyFilter();

	switch (theApp.m_LastSettings.nPAFilterDisplayMode) {
	default:
		RenderFrequency(&dc);
		break;
	case 1:	RenderPhase(&dc);
		break;
	}
}


void CAOSPhasedArrayUTPage::RenderFrequency(CPaintDC* pDC)
{
	//	CDC* pDC = GetDC();
	CRect rr;
	CBrush Brush(RGB(255, 255, 255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID, 1, RGB(255, 0, 0));
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen penGrey(PS_SOLID, 1, RGB(50, 50, 50));
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penDotGrey(PS_DOT, 1, RGB(180, 180, 180));
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));
	double dY, dX;
	int nPnY, nPnX, ii;

	LOGFONT lf;
	ZeroMemory(&lf, sizeof lf);
	CFont FontHorizontal;
	CFont FontVertical;
	CFont* pOldFont;
	theApp.SetFontLanguage(&lf);
	lf.lfHeight = 12;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	FontVertical.CreateFontIndirect(&lf);


	Buff = "Hello";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top = rr.bottom;
	rr.bottom = rr.top + 3 + size.cy;
	rr.left -= size.cx;
	rr.right += size.cx;
	pDC->FillRect(rr, &brushBK);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2, 2);
	pDC->FillRect(&rr, &Brush);


	int freqPoints = 200;
	double* f = new double[freqPoints + 1];
	CFIR* pFIR = &theApp.m_PhasedArray[PORTSIDE].m_FIR;
	pFIR->FilterGain(pFIR->nNumTaps, pFIR->m_dAmpFilter, freqPoints, f);


	//Find Min and Max
	double dMax = -120.0;
	double dMin = 120.0;

	for (ii = 0; ii<freqPoints; ii++) {
		if (f[ii]>dMax) dMax = f[ii];
		if (f[ii]<dMin) dMin = f[ii];
	}

	dMin = -70.0;
	dMax = (double)((int)dMax / 10 + 1) * 10.0;
	dMin = (double)((int)dMin / 10 - 1) * 10.0;

	//Draw graticule & text
	rr.DeflateRect(size.cx, 2 * size.cy);
	rr.right += (size.cx / 2);

	pDC->SelectObject(&penDotGrey);
	for (dY = dMin; dY <= dMax; dY += 10.0) {
		nPnY = rr.bottom - (int)((dY - dMin) * (double)rr.Height() / (dMax - dMin));
		pDC->MoveTo(rr.left + 1, nPnY);
		pDC->LineTo(rr.right + 1, nPnY);
		Buff.Format(_T("%.0f"), dY);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.left - size.cx - 5, nPnY - size.cy / 2, Buff);
	}

	m_fMaxFilterFrequency = 50.0f;

	for (dX = 0.0; dX <= m_fMaxFilterFrequency; dX += (m_fMaxFilterFrequency / 10)) {
		nPnX = (int)(dX * (double)rr.Width() / m_fMaxFilterFrequency) + rr.left;
		pDC->MoveTo(nPnX, rr.bottom - 1);
		pDC->LineTo(nPnX, rr.top);
		Buff.Format(_T("%.0f"), dX);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nPnX - size.cx / 2, rr.bottom + 2, Buff);
	}

	pDC->SelectObject(&penBlack);
	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.left, rr.bottom);
	pDC->LineTo(rr.BottomRight());

	Buff.LoadString(IDS_Inphase_Filter_Frequency_Responce);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x - size.cx / 2, rr.top - (size.cy * 3 / 2), Buff);

	Buff.LoadString(IDS_Frequency_In_MHz);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x - size.cx / 2, rr.bottom + (size.cy * 3 / 2), Buff);

	pDC->SelectObject(&FontVertical);
	Buff.LoadString(IDS_Magnitude_In_dB);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.left - size.cy * 2 - 5, rr.CenterPoint().y + (size.cx / 2), Buff);

	//Draw Trace
	rr.DeflateRect(1, 1);
	pDC->SelectObject(&penRed);
	ii = 0;
	nPnY = rr.bottom - (int)((f[ii++] - dMin) * (double)rr.Height() / (dMax - dMin));
	pDC->MoveTo(rr.left, nPnY);
	for (ii; ii<freqPoints; ii++) {
		nPnX = MulDiv(ii, rr.Width() - 1, freqPoints - 1) + rr.left;
		nPnY = rr.bottom - (int)((f[ii] - dMin) * (double)rr.Height() / (dMax - dMin));
		pDC->LineTo(nPnX, nPnY);
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

	delete f;
}


void CAOSPhasedArrayUTPage::RenderPhase(CPaintDC* pDC)
{
	CRect rr;
	CBrush Brush(RGB(255, 255, 255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID, 1, RGB(255, 0, 0));
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen penGrey(PS_SOLID, 1, RGB(50, 50, 50));
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penDotGrey(PS_DOT, 1, RGB(180, 180, 180));
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));
	double dY, dX;
	int nPnY, nPnX;

	LOGFONT lf;
	ZeroMemory(&lf, sizeof lf);
	CFont FontHorizontal;
	CFont FontVertical;
	CFont* pOldFont;
	theApp.SetFontLanguage(&lf);
	lf.lfHeight = 12;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	FontVertical.CreateFontIndirect(&lf);


	Buff = "HelloAll";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);



	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top = rr.bottom;
	rr.bottom = rr.top + 3 + size.cy;
	rr.left -= size.cx;
	rr.right += size.cx;
	pDC->FillRect(rr, &brushBK);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2, 2);
	pDC->FillRect(&rr, &Brush);

	//Find Min and Max
	double dMax = -1.0;
	double dMin = 1.0;
	double dStep = 0.01;
	int nPrecision = 1;
	int ii;

	CFIR* pFIR = &theApp.m_PhasedArray[PORTSIDE].m_FIR;
	if (pFIR->m_dAmpFilter != NULL) {
		for (ii = 0; ii<pFIR->nNumTaps; ii++) {
			if (pFIR->m_dAmpFilter[ii]>dMax) dMax = pFIR->m_dAmpFilter[ii];
			if (pFIR->m_dAmpFilter[ii]<dMin) dMin = pFIR->m_dAmpFilter[ii];
		}


		if ((dMax - dMin) < 0.3) {
			dMax = (double)((int)(dMax*100.0) + 1) / 100.0;
			dMin = (double)((int)(dMin*100.0) - 1) / 100.0;
			dStep = 0.01;
			nPrecision = 2;
		}
		else {
			dMax = (double)((int)(dMax*10.0) + 1) / 10.0;
			dMin = (double)((int)(dMin*10.0) - 1) / 10.0;
			dStep = 0.1;
			nPrecision = 1;
		}
		if (dMax <= dMin) dMax = dMin + 0.1;

		//Draw graticule & text
		rr.DeflateRect(size.cx, 2 * size.cy);
		rr.right += (size.cx * 2 / 3);

		pDC->SelectObject(&penDotGrey);
		for (dY = dMin; dY <= (dMax + 0.001); dY += dStep) {
			nPnY = rr.bottom - (int)((dY - dMin) * (double)rr.Height() / (dMax - dMin));
			pDC->MoveTo(rr.left + 1, nPnY);
			pDC->LineTo(rr.right + 1, nPnY);
			Buff.Format(_T("%.*f"), nPrecision, dY);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(rr.left - size.cx - 5, nPnY - size.cy / 2, Buff);
		}

		double dMaxTime = 1.0 / (m_fMaxFilterFrequency * 2.0) * (double)theApp.Fir.nNumTaps;
		dStep = 0.2;
		if (dMaxTime <= 1.0) dStep = 0.1;
		for (dX = 0.0; dX <= dMaxTime; dX += dStep) {
			nPnX = (int)(dX * (double)rr.Width() / dMaxTime) + rr.left;
			pDC->MoveTo(nPnX, rr.bottom - 1);
			pDC->LineTo(nPnX, rr.top);
			Buff.Format(_T("%.01f"), dX);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnX - size.cx / 2, rr.bottom + 2, Buff);
		}

		pDC->SelectObject(&penBlack);
		pDC->MoveTo(rr.TopLeft());
		pDC->LineTo(rr.left, rr.bottom);
		pDC->LineTo(rr.BottomRight());

		Buff.LoadString(IDS_Impulse_Responce);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x - size.cx / 2, rr.top - (size.cy * 3 / 2), Buff);

		Buff.LoadString(IDS_Time_in_us);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x - size.cx / 2, rr.bottom + (size.cy * 3 / 2), Buff);

		pDC->SelectObject(&FontVertical);
		Buff.LoadString(IDS_Inphase_Amplitude);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.left - size.cy * 3 - 4, rr.CenterPoint().y + (size.cx / 2), Buff);



		//Draw Trace
		rr.DeflateRect(1, 1);
		pDC->SelectObject(&penRed);
		ii = 0;
		nPnY = rr.bottom - (int)((pFIR->m_dAmpFilter[ii++] - dMin) * (double)rr.Height() / (dMax - dMin));
		pDC->MoveTo(rr.left, nPnY);
		for (ii; ii<pFIR->nNumTaps; ii++) {
			nPnX = MulDiv(ii, rr.Width() - 1, pFIR->nNumTaps - 1) + rr.left;
			nPnY = rr.bottom - (int)((pFIR->m_dAmpFilter[ii] - dMin) * (double)rr.Height() / (dMax - dMin));
			pDC->LineTo(nPnX, nPnY);
		}

	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

	//	ReleaseDC(pDC);
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinMinFilterFrequency(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_PhasedArray[PORTSIDE].m_fFilterFreq[0] = m_editspinMinFilterFrequency.GetPos();
	*pResult = 0;
	Invalidate(false);
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinMaxFilterFrequency(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_PhasedArray[PORTSIDE].m_fFilterFreq[0] = m_editspinMaxFilterFrequency.GetPos();
	*pResult = 0;
	Invalidate(false);
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinRipple(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_PhasedArray[PORTSIDE].m_fRipple = m_editspinRipple.GetPos();
	*pResult = 0;
	Invalidate(false);
}


void CAOSPhasedArrayUTPage::OnDeltaposSpinStopGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	theApp.m_PhasedArray[PORTSIDE].m_fStopGain = m_editspinStopGain.GetPos();
	*pResult = 0;
	Invalidate(false);
}




void CAOSPhasedArrayUTPage::OnDeltaposSpinFilterGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	theApp.m_PhasedArray[PORTSIDE].m_nFilterGain = (int)m_editspinFilterGain.GetValue();
	*pResult = 0;
	Invalidate(false);
}


void CAOSPhasedArrayUTPage::CreateColumnsDAC()
{
	CString ColumnName[3] = { L"#",L"Delay mm", L"Gain" };
	int ColumnWidth[3] = { 0,60,50 };
	CRect rr;

	ColumnName[1] = L"\x0b5s";
	ColumnName[2] = L"dB";

	m_listDAC.GetWindowRect(&rr);
	rr.right -= 22;

	// Delete all of the columns.
	int nColumnCount = m_listDAC.GetHeaderCtrl()->GetItemCount();
	for (int ii = 0; ii < nColumnCount; ii++) {
		m_listDAC.DeleteColumn(0);
	}

	m_listDAC.InsertColumn(0, ColumnName[0], LVCFMT_LEFT, 0);
	m_listDAC.InsertColumn(1, ColumnName[1], LVCFMT_LEFT, rr.Width() / 2 - 1);
	m_listDAC.InsertColumn(2, ColumnName[2], LVCFMT_LEFT, rr.Width() / 2 - 2);

	m_listDAC.SetExtendedStyle(LVS_EX_FULLROWSELECT);

}

void CAOSPhasedArrayUTPage::FillListDAC()
{
	CString	Buff;

	m_listDAC.DeleteAllItems();
	for (int ii = 0; ii < 16; ii++) {
		Buff.Format(_T("%d"), ii + 1);
		m_listDAC.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listDAC.EnsureVisible(m_nDacIndex, FALSE);
	m_listDAC.SetItemState(m_nDacIndex, LVIS_SELECTED, LVIS_SELECTED);
	m_listDAC.SetFocus();
}

void CAOSPhasedArrayUTPage::OnLvnGetdispinfoListDacList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	static	WCHAR str[100];

	str[0] = 0;
	switch (pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str, _T("%d"), pDispInfo->item.iItem + 1);
		break;
	case 1:	swprintf_s(str, _T("%.02f"), theApp.m_PhasedArray[PORTSIDE].m_fDacDelay[0][pDispInfo->item.iItem]);
		break;
	case 2:	swprintf_s(str, _T("%.02f"), theApp.m_PhasedArray[PORTSIDE].m_fDacGain[0][pDispInfo->item.iItem]);
		break;
	}
	pDispInfo->item.pszText = str;
	*pResult = 0;
}


void CAOSPhasedArrayUTPage::OnCbnSelchangeComboDacMode()
{
	theApp.m_PhasedArray[PORTSIDE].setDacMode(m_comboDacMode.GetCurSel());
	m_nUpdateHardware |= U_DAC;
}
