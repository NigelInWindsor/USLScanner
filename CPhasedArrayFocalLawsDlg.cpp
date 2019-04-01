#include "stdafx.h"
#include "USLScanner.h"
#include "CPhasedArrayFocalLawsDlg.h"

#define U_APPLY_FOCAL_LAWS		0x0000100
#define U_BEAM_CORRECTION_GAIN	0x0000200

extern UINT FocalLawsThread(LPVOID pParam);

void CPhasedArrayFocalLawsDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TX_PITCH, m_comboTXFocalLawPitch);
	DDX_Control(pDX, IDC_SPIN_TX_APERTURE, m_spinTxAperture);
	DDX_Control(pDX, IDC_EDIT_TX_APERTURE, m_editTxAperture);

	DDX_Control(pDX, IDC_COMBO_RX_PITCH, m_comboRXFocalLawPitch);
	DDX_Control(pDX, IDC_SPIN_RX_APERTURE, m_spinRxAperture);
	DDX_Control(pDX, IDC_EDIT_RX_APERTURE, m_editRxAperture);



	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_TX_BEAM_ANGLE, m_editTxBeamAngle);
	DDX_Control(pDX, IDC_SPIN_TX_BEAM_ANGLE, m_spinTxBeamAngle);
	DDX_Control(pDX, IDC_EDIT_RX_BEAM_ANGLE, m_editRxBeamAngle);
	DDX_Control(pDX, IDC_SPIN_RX_BEAM_ANGLE, m_spinRxBeamAngle);
	DDX_Control(pDX, IDC_EDIT_TX_FOCAL_LENGTH, m_editTxFocalLength);
	DDX_Control(pDX, IDC_SPIN_TX_FOCAL_LENGTH, m_spinTxFocalLength);
	DDX_Control(pDX, IDC_EDIT_RX_FOCAL_LENGTH_0, m_editRxFocalLength[0]);
	DDX_Control(pDX, IDC_SPIN_RX_FOCAL_LENGTH_0, m_spinRxFocalLength[0]);
	DDX_Control(pDX, IDC_EDIT_RX_FOCAL_LENGTH_1, m_editRxFocalLength[1]);
	DDX_Control(pDX, IDC_SPIN_RX_FOCAL_LENGTH_1, m_spinRxFocalLength[1]);
	DDX_Control(pDX, IDC_EDIT_RX_FOCAL_LENGTH_2, m_editRxFocalLength[2]);
	DDX_Control(pDX, IDC_SPIN_RX_FOCAL_LENGTH_2, m_spinRxFocalLength[2]);
	DDX_Control(pDX, IDC_EDIT_RX_FOCAL_LENGTH_3, m_editRxFocalLength[3]);
	DDX_Control(pDX, IDC_SPIN_RX_FOCAL_LENGTH_3, m_spinRxFocalLength[3]);
	DDX_Control(pDX, IDC_COMBO_FIRING_ORDER, m_comboFiringOrder);

	DDX_Control(pDX, IDC_EDIT_FOCAL_LAW, m_editFocalLaw);
	DDX_Control(pDX, IDC_SPIN_FOCAL_LAW, m_spinFocalLaw);
	DDX_Control(pDX, IDC_CHECK_TX_EQUALS_RX, m_checkRxEqualsTx);
	DDX_Control(pDX, IDC_CHECK_REVERSE_ELEMENTS, m_checkReverseElements);
	DDX_Control(pDX, IDC_COMBO_FIRST_ELEMENT_TX, m_comboFirstElementTx);
	DDX_Control(pDX, IDC_COMBO_LAST_ELEMENT_TX, m_comboLastElementTx);
	DDX_Control(pDX, IDC_COMBO_FIRST_ELEMENT_RX, m_comboFirstElementRx);
	DDX_Control(pDX, IDC_COMBO_LAST_ELEMENT_RX, m_comboLastElementRx);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_editGain);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_spinGain);
}

CPhasedArrayFocalLawsDlg::CPhasedArrayFocalLawsDlg()
	: CResizablePage(CPhasedArrayFocalLawsDlg::IDD)

{
	m_nSide = 0;
	m_nUpdateHardware = 0;
}


CPhasedArrayFocalLawsDlg::~CPhasedArrayFocalLawsDlg()
{
	theApp.SuspendThread(this);

}

BEGIN_MESSAGE_MAP(CPhasedArrayFocalLawsDlg, CResizablePage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_TX_APERTURE, &CPhasedArrayFocalLawsDlg::OnEnChangeEditTxAperture)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_RX_APERTURE, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxAperture)
	ON_CBN_SELCHANGE(IDC_COMBO_TX_PITCH, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboTxPitch)
	ON_CBN_SELCHANGE(IDC_COMBO_RX_PITCH, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboRxPitch)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRING_ORDER, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFiringOrder)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TX_BEAM_ANGLE, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinTxBeamAngle)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TX_FOCAL_LENGTH, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinTxFocalLength)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RX_BEAM_ANGLE, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxBeamAngle)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RX_FOCAL_LENGTH_0, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RX_FOCAL_LENGTH_1, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RX_FOCAL_LENGTH_2, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RX_FOCAL_LENGTH_3, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength3)
	ON_EN_CHANGE(IDC_EDIT_RX_FOCAL_LENGTH_0, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength0)
	ON_EN_CHANGE(IDC_EDIT_RX_FOCAL_LENGTH_1, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength1)
	ON_EN_CHANGE(IDC_EDIT_RX_FOCAL_LENGTH_2, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength2)
	ON_EN_CHANGE(IDC_EDIT_RX_FOCAL_LENGTH_3, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength3)
	ON_EN_CHANGE(IDC_EDIT_TX_BEAM_ANGLE, &CPhasedArrayFocalLawsDlg::OnEnChangeEditTxBeamAngle)
	ON_EN_CHANGE(IDC_EDIT_RX_BEAM_ANGLE, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxBeamAngle)
	ON_EN_CHANGE(IDC_EDIT_FOCAL_LAW, &CPhasedArrayFocalLawsDlg::OnEnChangeEditFocalLaw)
	ON_BN_CLICKED(IDC_CHECK_TX_EQUALS_RX, &CPhasedArrayFocalLawsDlg::OnBnClickedCheckTxEqualsRx)
	ON_BN_CLICKED(IDC_CHECK_REVERSE_ELEMENTS, &CPhasedArrayFocalLawsDlg::OnBnClickedCheckReverseElements)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRST_ELEMENT_TX, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFirstElementTx)
	ON_CBN_SELCHANGE(IDC_COMBO_LAST_ELEMENT_TX, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboLastElementTx)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRST_ELEMENT_RX, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFirstElementRx)
	ON_CBN_SELCHANGE(IDC_COMBO_LAST_ELEMENT_RX, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboLastElementRx)
	ON_EN_CHANGE(IDC_EDIT_TX_FOCAL_LENGTH, &CPhasedArrayFocalLawsDlg::OnEnChangeEditTxFocalLength)
	ON_EN_CHANGE(IDC_EDIT_GAIN, &CPhasedArrayFocalLawsDlg::OnEnChangeEditGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAIN, &CPhasedArrayFocalLawsDlg::OnDeltaposSpinGain)
END_MESSAGE_MAP()


HBRUSH CPhasedArrayFocalLawsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

BOOL CPhasedArrayFocalLawsDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	m_spinFocalLaw.SetRange(0, 128);

	m_spinTxAperture.SetRange(1, 16);
	m_spinRxAperture.SetRange(1, 16);
	m_spinTxBeamAngle.SetRange(0, 7200);
	m_spinRxBeamAngle.SetRange(0, 7200);
	m_spinTxFocalLength.SetRange(0, 1000);
	m_spinGain.SetRange(0,100);

	for (int nTOF = 0; nTOF < 4; nTOF++)
		m_spinRxFocalLength[nTOF].SetRange(0, 1000);

	m_comboFiringOrder.AddString(L"Sequential");
	m_comboFiringOrder.AddString(L"Half");
	m_comboFiringOrder.AddString(L"Third");
	m_comboFiringOrder.AddString(L"Quarter");
	m_comboFiringOrder.AddString(L"Fith");
	m_comboFiringOrder.AddString(L"Sixth");
	m_comboFiringOrder.AddString(L"Seventh");


	UpdateAllControls();


	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CPhasedArrayFocalLawsDlg::OnSetActive()
{
	CString Buff;
	theApp.m_LastSettings.nLastPhasedArrayTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	m_comboTXFocalLawPitch.ResetContent();
	m_comboRXFocalLawPitch.ResetContent();
	for (int ii = 0; ii < 16; ii++) {
		Buff.Format(L"%.02f mm", theApp.m_PhasedArray[m_nSide].getElementPitch() * (ii + 1));
		m_comboTXFocalLawPitch.AddString(Buff);
		m_comboRXFocalLawPitch.AddString(Buff);
	}

	StartThread();

	UpdateAllControls();

	return CResizablePage::OnSetActive();
}


BOOL CPhasedArrayFocalLawsDlg::OnKillActive()
{
	SuspendThread();

	return CResizablePage::OnKillActive();
}

void CPhasedArrayFocalLawsDlg::StartThread()
{
	theApp.StartThread(L"Phased array focal laws Dlg", &FocalLawsThread, this, 200, THREAD_PRIORITY_NORMAL);
}

void CPhasedArrayFocalLawsDlg::SuspendThread()
{
	theApp.SuspendThread(this);
}

UINT FocalLawsThread(LPVOID pParam)
{
	CPhasedArrayFocalLawsDlg* pParent = (CPhasedArrayFocalLawsDlg*)pParam;

	TRACE0("Focal laws waiting\n");
	if (WaitForSingleObject(pParent->m_hSemaphore, 10) == WAIT_OBJECT_0) {

		if (pParent->GetSafeHwnd() && pParent->IsWindowVisible() == TRUE) {

			int nFL = theApp.m_LSA.m_nScopeViewLaw;

			if (pParent->m_nUpdateHardware & U_APPLY_FOCAL_LAWS) {
				pParent->ApplyFocalLaws();
				pParent->m_nUpdateHardware &= ~U_APPLY_FOCAL_LAWS;
			}

			if (pParent->m_nUpdateHardware & U_BEAM_CORRECTION_GAIN) {
				theApp.m_PhasedArray[PORTSIDE].setBeamCorrectionGain(nFL, true);
				pParent->m_nUpdateHardware &= ~U_BEAM_CORRECTION_GAIN;
			}
		}
		ReleaseSemaphore(pParent->m_hSemaphore, 1, NULL);
	}
	TRACE0("Focal laws waiting completed\n");

	return THREAD_CONTINUE;
}

void CPhasedArrayFocalLawsDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	bool bReadOnly;

	if (theApp.m_PhasedArray[PORTSIDE].getNumberElements() - m_comboFirstElementTx.GetCount()) {
		m_comboFirstElementTx.ResetContent();
		m_comboFirstElementRx.ResetContent();
		m_comboLastElementTx.ResetContent();
		m_comboLastElementRx.ResetContent();
		for (int ii = 0; ii < theApp.m_PhasedArray[PORTSIDE].getNumberElements(); ii++) {
			Buff.Format(L"%d", ii + 1);
			m_comboFirstElementTx.AddString(Buff);
			m_comboFirstElementRx.AddString(Buff);
			m_comboLastElementTx.AddString(Buff);
			m_comboLastElementRx.AddString(Buff);
		}
	}
	m_comboFirstElementTx.SetCurSel(theApp.m_PhasedArray[m_nSide].getTxFirstElement());
	m_comboFirstElementRx.SetCurSel(theApp.m_PhasedArray[m_nSide].getRxFirstElement());
	m_comboLastElementTx.SetCurSel(theApp.m_PhasedArray[m_nSide].getTxLastElement());
	m_comboLastElementRx.SetCurSel(theApp.m_PhasedArray[m_nSide].getRxLastElement());

	Buff.Format(_T("%d"), theApp.m_LSA.m_nScopeViewLaw + 1);
	m_editFocalLaw.SetWindowText(Buff);
	m_spinFocalLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw );


	int nFL = theApp.m_LSA.m_nScopeViewLaw;
	Buff.Format(L"%.01f dB", theApp.m_PhasedArray[PORTSIDE].m_FLRx[nFL].getGain());
	m_editGain.SetWindowTextW(Buff);
	m_spinGain.SetPos(50);

	m_comboFiringOrder.SetCurSel(theApp.m_PhasedArray[PORTSIDE].getFiringOrder());

	Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxAperture());
	m_editTxAperture.SetWindowText(Buff);
	m_spinTxAperture.SetPos(theApp.m_PhasedArray[m_nSide].getTxAperture());

	Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getRxAperture());
	m_editRxAperture.SetWindowText(Buff);
	m_spinRxAperture.SetPos(theApp.m_PhasedArray[m_nSide].getRxAperture());
	
	m_comboTXFocalLawPitch.SetCurSel(theApp.m_PhasedArray[m_nSide].getIntTxFocalLawPitch() - 1);
	m_comboRXFocalLawPitch.SetCurSel(theApp.m_PhasedArray[m_nSide].getIntRxFocalLawPitch() - 1);


	Buff.Format(L"%.01f%s", theApp.m_PhasedArray[PORTSIDE].getTxBeamAngle(0), DEGREES);
	m_editTxBeamAngle.SetWindowTextW(Buff);
	m_spinTxBeamAngle.SetPos((int)(theApp.m_PhasedArray[PORTSIDE].getTxBeamAngle(0) * 10.0f));

	Buff.Format(L"%.01f%s", theApp.m_PhasedArray[PORTSIDE].getRxBeamAngle(0), DEGREES);
	m_editRxBeamAngle.SetWindowTextW(Buff);
	m_spinRxBeamAngle.SetPos((int)(theApp.m_PhasedArray[PORTSIDE].getRxBeamAngle(0) * 10.0f));

	if (theApp.m_PhasedArray[PORTSIDE].getTxFocalLength() <= 0.0f) {
		Buff = L"Infinity";
	}
	else {
		Buff.Format(L"%.0f", theApp.m_PhasedArray[PORTSIDE].getTxFocalLength());
	}
	m_editTxFocalLength.SetWindowTextW(Buff);

	for (int nTOF = 0; nTOF < 4; nTOF++) {
		if (theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF) <= 0.0f) {
			Buff = L"Infinity";
		}
		else {
			Buff.Format(L"%.01f mm", theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF));
		}
		m_editRxFocalLength[nTOF].SetWindowTextW(Buff);
	}

	m_checkReverseElements.SetCheck( theApp.m_PhasedArray[PORTSIDE].getReverseArray() );


	theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX ? bReadOnly = true : bReadOnly = false;
	m_checkRxEqualsTx.SetCheck(bReadOnly);
	m_comboRXFocalLawPitch.EnableWindow(bReadOnly ^ 1);
	m_editRxBeamAngle.SetReadOnly(bReadOnly);
	m_editRxAperture.SetReadOnly(bReadOnly);
	m_comboFirstElementRx.EnableWindow(bReadOnly ^ 1);
	m_comboLastElementRx.EnableWindow(bReadOnly ^ 1);

}



void CPhasedArrayFocalLawsDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int	nTemp;

	switch (pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_FOCAL_LAW:
		nTemp = (pSpin->GetPos() & 0xffff);
		if (nTemp - theApp.m_LSA.m_nScopeViewLaw) {
			theApp.m_LSA.m_nScopeViewLaw = MinMax(&nTemp, 0, theApp.m_LSA.nFocalLawsL - 1);
			Buff.Format(_T("%d"), theApp.m_LSA.m_nScopeViewLaw + 1);
			m_editFocalLaw.SetWindowText(Buff);
			m_spinFocalLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw);
			int nFL = theApp.m_LSA.m_nScopeViewLaw;
			Buff.Format(L"%.01f dB", theApp.m_PhasedArray[PORTSIDE].m_FLRx[nFL].getGain());
			m_editGain.SetWindowTextW(Buff);
		}
		break;
	case IDC_SPIN_TX_APERTURE:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxAperture()) {
			theApp.m_PhasedArray[m_nSide].setTxAperture(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxAperture());
			m_editTxAperture.SetWindowText(Buff);
			m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
			UpdateAllControls();
		};
		break;
	case IDC_SPIN_RX_APERTURE:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxAperture()) {
			theApp.m_PhasedArray[m_nSide].setRxAperture(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getRxAperture());
			m_editRxAperture.SetWindowText(Buff);
			m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
			UpdateAllControls();
		};
		break;
	};

	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPhasedArrayFocalLawsDlg::ApplyFocalLaws()
{
	int nScanPitch = theApp.m_PhasedArray[m_nSide].getIntTxFocalLawPitch();
	int nNumberFocalLaws = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();
	theApp.m_LSA.ApplyFocalLaws(nNumberFocalLaws, theApp.m_PhasedArray[PORTSIDE].getTxAperture(), theApp.m_PhasedArray[PORTSIDE].getTxFirstElement(), false, 0, nScanPitch, theApp.m_PhasedArray[PORTSIDE].getElementPitch());
	theApp.m_PhasedArray[PORTSIDE].setNumberFocalLaws(nNumberFocalLaws);
	if (CURRENT_FL >= theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws()) {
		CURRENT_FL = theApp.m_UtUser.m_Global.nTimeSlot = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws() - 1;
	}
	if (theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX) 	theApp.m_PhasedArray[PORTSIDE].setRxEqualTx();
	theApp.m_PhasedArray[PORTSIDE].CalculateTxFocalLaws();
	theApp.m_PhasedArray[PORTSIDE].CalculateRxFocalLaws();

	theApp.m_PhasedArray[PORTSIDE].DownloadAllToHardware(0);
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditTxAperture()
{
	CString Buff;
	int nTemp = theApp.m_PhasedArray[PORTSIDE].getTxAperture();

	m_editTxAperture.GetWindowText(Buff);
	theApp.m_PhasedArray[PORTSIDE].setTxAperture(_ttoi(Buff));

	if (nTemp - theApp.m_PhasedArray[PORTSIDE].getTxAperture()) {
		m_spinTxAperture.SetPos(theApp.m_PhasedArray[PORTSIDE].getTxAperture());
		m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
		if (theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX) {
			Buff.Format(L"%d", theApp.m_PhasedArray[PORTSIDE].getRxAperture());
			m_editRxAperture.SetWindowText(Buff);
		}
	};
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditRxAperture()
{
	CString Buff;
	int nTemp = theApp.m_PhasedArray[PORTSIDE].getRxAperture();

	m_editRxAperture.GetWindowText(Buff);
	theApp.m_PhasedArray[PORTSIDE].setRxAperture(_ttoi(Buff));

	if (nTemp - theApp.m_PhasedArray[PORTSIDE].getRxAperture()) {
		m_spinRxAperture.SetPos(theApp.m_PhasedArray[PORTSIDE].getRxAperture());
		m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	};
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboTxPitch()
{
	theApp.m_PhasedArray[m_nSide].setTxFocalLawPitch(m_comboTXFocalLawPitch.GetCurSel() + 1);
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboRxPitch()
{
	theApp.m_PhasedArray[m_nSide].setRxFocalLawPitch(m_comboRXFocalLawPitch.GetCurSel() + 1);
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
}




void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFiringOrder()
{
	theApp.m_PhasedArray[PORTSIDE].setFiringOrder(m_comboFiringOrder.GetCurSel());
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
}





void CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxBeamAngle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fAngle = theApp.m_PhasedArray[PORTSIDE].getRxBeamAngle(0);

	fAngle += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setRxBeamAngle(fAngle, 0);
	Buff.Format(L"%.01f%s", theApp.m_PhasedArray[PORTSIDE].getRxBeamAngle(0), DEGREES);
	m_editRxBeamAngle.SetWindowText(Buff);
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;

	*pResult = 0;
}

void CPhasedArrayFocalLawsDlg::OnDeltaposSpinTxBeamAngle(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fAngle = theApp.m_PhasedArray[PORTSIDE].getTxBeamAngle(0);

	fAngle += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setTxBeamAngle(fAngle, 0);
	Buff.Format(L"%.01f%s", theApp.m_PhasedArray[PORTSIDE].getTxBeamAngle(0), DEGREES);
	m_editTxBeamAngle.SetWindowText(Buff);
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();


	*pResult = 0;
}



void CPhasedArrayFocalLawsDlg::OnEnChangeEditTxBeamAngle()
{
	CString Buff;
	float fTemp;

	m_editTxBeamAngle.GetWindowTextW(Buff);
	_WTOF(Buff, fTemp);
	if (fTemp - theApp.m_PhasedArray[PORTSIDE].getTxBeamAngle(0)) {
		theApp.m_PhasedArray[PORTSIDE].setTxBeamAngle(fTemp, 0);
		m_spinTxBeamAngle.SetPos((int)(fTemp * 10));
		m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditRxBeamAngle()
{
	CString Buff;
	float fTemp;

	m_editRxBeamAngle.GetWindowTextW(Buff);
	_WTOF(Buff, fTemp);
	if (fTemp - theApp.m_PhasedArray[PORTSIDE].getRxBeamAngle(0)) {
		theApp.m_PhasedArray[PORTSIDE].setRxBeamAngle(fTemp, 0);
		m_spinRxBeamAngle.SetPos((int)(fTemp * 10));
		m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditFocalLaw()
{
	CString Buff;
	int nTemp = theApp.m_LSA.m_nScopeViewLaw;

	m_editFocalLaw.GetWindowText(Buff);
	nTemp = _ttoi(Buff) - 1;

	if (nTemp - theApp.m_LSA.m_nScopeViewLaw) {
		theApp.m_LSA.m_nScopeViewLaw = MinMax(&nTemp, 0, theApp.m_LSA.nFocalLawsL - 1);

		m_spinFocalLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw);
	}
}


void CPhasedArrayFocalLawsDlg::OnBnClickedCheckTxEqualsRx()
{
	theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX ? theApp.m_LastSettings.nPhasedArrayMask &= ~PA_RX_EQUAL_TX : theApp.m_LastSettings.nPhasedArrayMask |= PA_RX_EQUAL_TX;
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();
}


void CPhasedArrayFocalLawsDlg::OnBnClickedCheckReverseElements()
{
	m_checkReverseElements.GetCheck() == false ? theApp.m_PhasedArray[PORTSIDE].setReverseArray(false) : theApp.m_PhasedArray[PORTSIDE].setReverseArray(true);
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFirstElementTx()
{
	theApp.m_PhasedArray[m_nSide].setTxFirstElement(m_comboFirstElementTx.GetCurSel());
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboLastElementTx()
{
	theApp.m_PhasedArray[m_nSide].setTxLastElement(m_comboLastElementTx.GetCurSel());
	m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFirstElementRx()
{
	theApp.m_PhasedArray[m_nSide].setRxFirstElement(m_comboFirstElementRx.GetCurSel());
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboLastElementRx()
{
	theApp.m_PhasedArray[m_nSide].setRxLastElement(m_comboLastElementRx.GetCurSel());
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();
}


void CPhasedArrayFocalLawsDlg::OnDeltaposSpinTxFocalLength(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString Buff;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fDistance = theApp.m_PhasedArray[PORTSIDE].getTxFocalLength();

	fDistance += ((float)pNMUpDown->iDelta * 2.0f);
	theApp.m_PhasedArray[PORTSIDE].setTxFocalLength(fDistance);
	Buff.Format(L"%.01f mm", theApp.m_PhasedArray[PORTSIDE].getTxFocalLength());
	m_editTxFocalLength.SetWindowText(Buff);
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	UpdateAllControls();

	*pResult = 0;
}

void CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength0(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTOF = 0;
	float fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF);

	fLength += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, nTOF);
	if ((fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF)) > 0.0f) {
		Buff.Format(L"%.01f mm", fLength, DEGREES);
	}
	else {
		Buff = L"Infinity";
	}
	m_editRxFocalLength[nTOF].SetWindowText(Buff);
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	*pResult = 0;
}

void CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTOF = 1;
	float fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF);

	fLength += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, nTOF);
	if ((fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF)) > 0.0f) {
		Buff.Format(L"%.01f mm", fLength, DEGREES);
	}
	else {
		Buff = L"Infinity";
	}
	m_editRxFocalLength[nTOF].SetWindowText(Buff);
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	*pResult = 0;
}

void CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTOF = 2;
	float fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF);

	fLength += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, nTOF);
	if ((fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF)) > 0.0f) {
		Buff.Format(L"%.01f mm", fLength, DEGREES);
	}
	else {
		Buff = L"Infinity";
	}
	m_editRxFocalLength[nTOF].SetWindowText(Buff);
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	*pResult = 0;
}

void CPhasedArrayFocalLawsDlg::OnDeltaposSpinRxFocalLength3(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTOF = 3;
	float fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF);

	fLength += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, nTOF);
	if ((fLength = theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF)) > 0.0f) {
		Buff.Format(L"%.01f mm", fLength, DEGREES);
	}
	else {
		Buff = L"Infinity";
	}
	m_editRxFocalLength[nTOF].SetWindowText(Buff);
	   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	*pResult = 0;
}

void CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength0()
{
	CString Buff;
	float fLength;

	m_editRxFocalLength[0].GetWindowTextW(Buff);
	_WTOF(Buff, fLength);
	if (fLength > 1000.0f) fLength = 0.0f;
	if (fLength - theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(0)) {
		theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, 0);
		   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}
}

void CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength1()
{
	CString Buff;
	float fLength;

	m_editRxFocalLength[1].GetWindowTextW(Buff);
	_WTOF(Buff, fLength);
	if (fLength > 1000.0f) fLength = 0.0f;
	if (fLength - theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(1)) {
		theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, 1);
		   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}
}

void CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength2()
{
	CString Buff;
	float fLength;

	m_editRxFocalLength[2].GetWindowTextW(Buff);
	_WTOF(Buff, fLength);
	if (fLength > 1000.0f) fLength = 0.0f;
	if (fLength - theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(2)) {
		theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, 2);
		   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}
}

void CPhasedArrayFocalLawsDlg::OnEnChangeEditRxFocalLength3()
{
	CString Buff;
	float fLength;

	m_editRxFocalLength[3].GetWindowTextW(Buff);
	_WTOF(Buff, fLength);
	if (fLength > 1000.0f) fLength = 0.0f;
	if (fLength - theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(2)) {
		theApp.m_PhasedArray[PORTSIDE].setRxFocalLength(fLength, 3);
		   m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}

}

void CPhasedArrayFocalLawsDlg::OnEnChangeEditTxFocalLength()
{
	CString Buff;
	float fLength;

	m_editTxFocalLength.GetWindowTextW(Buff);
	_WTOF(Buff, fLength);
	if (fLength > 1000.0f) fLength = 0.0f;
	if (fLength - theApp.m_PhasedArray[PORTSIDE].getTxFocalLength()) {
		theApp.m_PhasedArray[PORTSIDE].setTxFocalLength(fLength);
		m_nUpdateHardware |= U_APPLY_FOCAL_LAWS;
	}
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditGain()
{
	CString Buff;
	float fGain;
	int nFL = theApp.m_LSA.m_nScopeViewLaw;

	m_editGain.GetWindowTextW(Buff);
	_WTOF(Buff, fGain);

	if (fGain - theApp.m_PhasedArray[PORTSIDE].m_FLRx[nFL].getGain()) {
		theApp.m_PhasedArray[PORTSIDE].m_FLRx[nFL].setGain(fGain);
		m_nUpdateHardware |= U_BEAM_CORRECTION_GAIN;
	}
}


void CPhasedArrayFocalLawsDlg::OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float fGain;
	int nFL = theApp.m_LSA.m_nScopeViewLaw;
	CString Buff;

	fGain = theApp.m_PhasedArray[PORTSIDE].m_FLRx[nFL].getGain();
	fGain += ((float)pNMUpDown->iDelta * 0.5f);

	theApp.m_PhasedArray[PORTSIDE].m_FLRx[nFL].setGain(fGain);
	m_nUpdateHardware |= U_BEAM_CORRECTION_GAIN;
	Buff.Format(L"%.01f dB", fGain);
	m_editGain.SetWindowTextW(Buff);

	*pResult = 0;
}
