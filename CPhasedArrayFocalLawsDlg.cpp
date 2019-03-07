#include "stdafx.h"
#include "USLScanner.h"
#include "CPhasedArrayFocalLawsDlg.h"

void CPhasedArrayFocalLawsDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TX_PITCH, m_comboTXFocalLawPitch);
	DDX_Control(pDX, IDC_SPIN_LAST_TX_ELEMENT, m_spinTxLastElement);
	DDX_Control(pDX, IDC_EDIT_LAST_TX_ELEMENT, m_editTxLastElement);
	DDX_Control(pDX, IDC_SPIN_FIRST_TX_ELEMENT, m_spinTxFirstElement);
	DDX_Control(pDX, IDC_EDIT_FIRST_TX_ELEMENT, m_editTxFirstElement);
	DDX_Control(pDX, IDC_SPIN_TX_APERTURE, m_spinTxAperture);
	DDX_Control(pDX, IDC_EDIT_TX_APERTURE, m_editTxAperture);

	DDX_Control(pDX, IDC_COMBO_RX_PITCH, m_comboRXFocalLawPitch);
	DDX_Control(pDX, IDC_SPIN_LAST_RX_ELEMENT, m_spinRxLastElement);
	DDX_Control(pDX, IDC_EDIT_LAST_RX_ELEMENT, m_editRxLastElement);
	DDX_Control(pDX, IDC_SPIN_FIRST_RX_ELEMENT, m_spinRxFirstElement);
	DDX_Control(pDX, IDC_EDIT_FIRST_RX_ELEMENT, m_editRxFirstElement);
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
}

CPhasedArrayFocalLawsDlg::CPhasedArrayFocalLawsDlg()
	: CResizablePage(CPhasedArrayFocalLawsDlg::IDD)

{
	m_nSide = 0;
}


CPhasedArrayFocalLawsDlg::~CPhasedArrayFocalLawsDlg()
{
}

BEGIN_MESSAGE_MAP(CPhasedArrayFocalLawsDlg, CResizablePage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_TX_APERTURE, &CPhasedArrayFocalLawsDlg::OnEnChangeEditTxAperture)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_RX_APERTURE, &CPhasedArrayFocalLawsDlg::OnEnChangeEditRxAperture)
	ON_CBN_SELCHANGE(IDC_COMBO_TX_PITCH, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboTxPitch)
	ON_CBN_SELCHANGE(IDC_COMBO_RX_PITCH, &CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboRxPitch)
	ON_EN_CHANGE(IDC_EDIT_FIRST_TX_ELEMENT, &CPhasedArrayFocalLawsDlg::OnEnChangeEditFirstTxElement)
	ON_EN_CHANGE(IDC_EDIT_LAST_TX_ELEMENT, &CPhasedArrayFocalLawsDlg::OnEnChangeEditLastTxElement)
	ON_EN_CHANGE(IDC_EDIT_FIRST_RX_ELEMENT, &CPhasedArrayFocalLawsDlg::OnEnChangeEditFirstRxElement)
	ON_EN_CHANGE(IDC_EDIT_LAST_RX_ELEMENT, &CPhasedArrayFocalLawsDlg::OnEnChangeEditLastRxElement)
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

	m_spinFocalLaw.SetRange(0, 128);

	m_spinTxAperture.SetRange(1, 16);
	m_spinRxAperture.SetRange(1, 16);
	m_spinTxBeamAngle.SetRange(0, 7200);
	m_spinRxBeamAngle.SetRange(0, 7200);
	m_spinTxFocalLength.SetRange(0, 1000);
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
	m_comboTXFocalLawPitch.SetCurSel(theApp.m_PhasedArray[m_nSide].getIntTxFocalLawPitch() - 1);
	m_comboRXFocalLawPitch.SetCurSel(theApp.m_PhasedArray[m_nSide].getIntRxFocalLawPitch() - 1);

	return CResizablePage::OnSetActive();
}


BOOL CPhasedArrayFocalLawsDlg::OnKillActive()
{
	// TODO: Add your specialized code here and/or call the base class

	return CResizablePage::OnKillActive();
}

void CPhasedArrayFocalLawsDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	bool bReadOnly;

	Buff.Format(_T("%d"), theApp.m_LSA.m_nScopeViewLaw + 1);
	m_editFocalLaw.SetWindowText(Buff);
	m_spinFocalLaw.SetPos(theApp.m_LSA.m_nScopeViewLaw );

	m_spinTxFirstElement.SetRange(0, theApp.m_PhasedArray[PORTSIDE].getNumberElements() - 1);
	m_spinTxLastElement.SetRange(0, theApp.m_PhasedArray[PORTSIDE].getNumberElements() - 1);
	m_spinRxFirstElement.SetRange(0, theApp.m_PhasedArray[PORTSIDE].getNumberElements() - 1);
	m_spinRxLastElement.SetRange(0, theApp.m_PhasedArray[PORTSIDE].getNumberElements() - 1);


	m_comboFiringOrder.SetCurSel(theApp.m_PhasedArray[PORTSIDE].getFiringOrder());

	Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getTxFirstElement() + 1);
	m_editTxFirstElement.SetWindowText(Buff);
	m_spinTxFirstElement.SetPos(theApp.m_PhasedArray[m_nSide].getTxFirstElement());

	Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getTxLastElement() + 1);
	m_editTxLastElement.SetWindowText(Buff);
	m_spinTxLastElement.SetPos(theApp.m_PhasedArray[m_nSide].getTxLastElement());

	Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxAperture());
	m_editTxAperture.SetWindowText(Buff);
	m_spinTxAperture.SetPos(theApp.m_PhasedArray[m_nSide].getTxAperture());

	Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxFirstElement() + 1);
	m_editRxFirstElement.SetWindowText(Buff);
	m_spinRxFirstElement.SetPos(theApp.m_PhasedArray[m_nSide].getRxFirstElement());

	Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxLastElement() + 1);
	m_editRxLastElement.SetWindowText(Buff);
	m_spinRxLastElement.SetPos(theApp.m_PhasedArray[m_nSide].getRxLastElement());

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
	m_editRxFirstElement.SetReadOnly(bReadOnly);
	m_editRxLastElement.SetReadOnly(bReadOnly);

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
			theApp.m_LSA.CmdSetWriteFL(theApp.m_LSA.m_nScopeViewLaw + 1);

		}
		break;
	case IDC_SPIN_TX_APERTURE:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxAperture()) {
			theApp.m_PhasedArray[m_nSide].setTxAperture(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxAperture());
			m_editTxAperture.SetWindowText(Buff);
			ApplyFocalLaws();
			UpdateAllControls();
		};
		break;
	case IDC_SPIN_RX_APERTURE:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxAperture()) {
			theApp.m_PhasedArray[m_nSide].setRxAperture(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getRxAperture());
			m_editRxAperture.SetWindowText(Buff);
			ApplyFocalLaws();
			UpdateAllControls();
		};
		break;
	case IDC_SPIN_FIRST_TX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxFirstElement()) {
			theApp.m_PhasedArray[m_nSide].setTxFirstElement(nTemp);
			Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getTxFirstElement() + 1);
			m_editTxFirstElement.SetWindowText(Buff);
			ApplyFocalLaws();
			UpdateAllControls();
		}
		break;
	case IDC_SPIN_LAST_TX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxLastElement()) {
			theApp.m_PhasedArray[m_nSide].setTxLastElement(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxLastElement() + 1);
			m_editTxLastElement.SetWindowText(Buff);
			ApplyFocalLaws();
			UpdateAllControls();
		};
		break;
	case IDC_SPIN_FIRST_RX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxFirstElement()) {
			theApp.m_PhasedArray[m_nSide].setRxFirstElement(nTemp);
			Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxFirstElement() + 1);
			m_editRxFirstElement.SetWindowText(Buff);
			ApplyFocalLaws();
			UpdateAllControls();
		}
		break;
	case IDC_SPIN_LAST_RX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxLastElement()) {
			theApp.m_PhasedArray[m_nSide].setRxLastElement(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getRxLastElement() + 1);
			m_editRxLastElement.SetWindowText(Buff);
			ApplyFocalLaws();
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

	theApp.m_PhasedArray[PORTSIDE].DownloadAllToHardware();
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditTxAperture()
{
	CString Buff;
	int nTemp = theApp.m_PhasedArray[PORTSIDE].getTxAperture();

	m_editTxAperture.GetWindowText(Buff);
	theApp.m_PhasedArray[PORTSIDE].setTxAperture(_ttoi(Buff));

	if (nTemp - theApp.m_PhasedArray[PORTSIDE].getTxAperture()) {
		m_spinTxAperture.SetPos(theApp.m_PhasedArray[PORTSIDE].getTxAperture());
		ApplyFocalLaws();
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
		ApplyFocalLaws();
	};
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboTxPitch()
{
	theApp.m_PhasedArray[m_nSide].setTxFocalLawPitch(m_comboTXFocalLawPitch.GetCurSel() + 1);
	ApplyFocalLaws();
}


void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboRxPitch()
{
	theApp.m_PhasedArray[m_nSide].setRxFocalLawPitch(m_comboRXFocalLawPitch.GetCurSel() + 1);
	ApplyFocalLaws();
}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditLastTxElement()
{
	CString Buff;

	m_editTxLastElement.GetWindowText(Buff);
	int nTemp = _ttoi(Buff) - 1;

	if (nTemp - theApp.m_PhasedArray[m_nSide].getTxLastElement()) {
		theApp.m_PhasedArray[m_nSide].setTxLastElement(nTemp);
		m_spinTxLastElement.SetPos(theApp.m_PhasedArray[m_nSide].getTxLastElement());
		ApplyFocalLaws();
		if (theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX) {
			Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxLastElement() + 1);
			m_editRxLastElement.SetWindowText(Buff);
		}
	}

}

void CPhasedArrayFocalLawsDlg::OnEnChangeEditFirstTxElement()
{
	CString Buff;

	m_editTxFirstElement.GetWindowText(Buff);
	int nTemp = _ttoi(Buff) - 1;

	if (nTemp - theApp.m_PhasedArray[m_nSide].getTxFirstElement()) {
		theApp.m_PhasedArray[m_nSide].setTxFirstElement(nTemp);
		m_spinTxFirstElement.SetPos(theApp.m_PhasedArray[m_nSide].getTxFirstElement());
		Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getTxFirstElement() + 1);
		m_editTxFirstElement.SetWindowText(Buff);
		ApplyFocalLaws();
		if (theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX) {
			Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxFirstElement() + 1);
			m_editRxFirstElement.SetWindowText(Buff);
		}
	}

}


void CPhasedArrayFocalLawsDlg::OnEnChangeEditFirstRxElement()
{
	CString Buff;

	m_editRxFirstElement.GetWindowText(Buff);
	int nTemp = _ttoi(Buff) - 1;

	if (nTemp - theApp.m_PhasedArray[m_nSide].getRxFirstElement()) {
		theApp.m_PhasedArray[m_nSide].setRxFirstElement(nTemp);
		m_spinRxFirstElement.SetPos(theApp.m_PhasedArray[m_nSide].getRxFirstElement());
		Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxFirstElement() + 1);
		m_editRxFirstElement.SetWindowText(Buff);
		ApplyFocalLaws();
	}
}



void CPhasedArrayFocalLawsDlg::OnEnChangeEditLastRxElement()
{
	CString Buff;

	m_editRxLastElement.GetWindowText(Buff);
	int nTemp = _ttoi(Buff) - 1;

	if (nTemp - theApp.m_PhasedArray[m_nSide].getRxLastElement()) {
		theApp.m_PhasedArray[m_nSide].setRxLastElement(nTemp);
		m_spinRxLastElement.SetPos(theApp.m_PhasedArray[m_nSide].getRxLastElement());
		Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxLastElement() + 1);
		m_editRxLastElement.SetWindowText(Buff);
		ApplyFocalLaws();
	}

}





void CPhasedArrayFocalLawsDlg::OnCbnSelchangeComboFiringOrder()
{
	theApp.m_PhasedArray[PORTSIDE].setFiringOrder(m_comboFiringOrder.GetCurSel());
}




void CPhasedArrayFocalLawsDlg::OnDeltaposSpinTxFocalLength(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString Buff;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fDistance = theApp.m_PhasedArray[PORTSIDE].getTxFocalLength();

	fDistance += ((float)pNMUpDown->iDelta * 0.1f);
	theApp.m_PhasedArray[PORTSIDE].setTxFocalLength(fDistance);
	Buff.Format(L"%.01f mm", theApp.m_PhasedArray[PORTSIDE].getTxFocalLength());
	m_editTxFocalLength.SetWindowText(Buff);
	ApplyFocalLaws();

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
	ApplyFocalLaws();
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
	ApplyFocalLaws();
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
	ApplyFocalLaws();
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
	ApplyFocalLaws();
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
		ApplyFocalLaws();
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
		ApplyFocalLaws();
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
		ApplyFocalLaws();
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
		ApplyFocalLaws();
	}

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
	ApplyFocalLaws();

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
	ApplyFocalLaws();
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
		ApplyFocalLaws();
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
		ApplyFocalLaws();
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
	ApplyFocalLaws();
	UpdateAllControls();
}


void CPhasedArrayFocalLawsDlg::OnBnClickedCheckReverseElements()
{
	m_checkReverseElements.GetCheck() == false ? theApp.m_PhasedArray[PORTSIDE].setReverseArray(false) : theApp.m_PhasedArray[PORTSIDE].setReverseArray(true);
	ApplyFocalLaws();
}
