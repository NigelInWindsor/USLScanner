// PhasedArrayProbeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PhasedArrayProbeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPhasedArrayProbeDlg dialog

extern UINT PhasedArrayProbeDlgThread(LPVOID pParam);

CPhasedArrayProbeDlg::CPhasedArrayProbeDlg()
	: CResizablePage(CPhasedArrayProbeDlg::IDD)
{
	//{{AFX_DATA_INIT(CPhasedArrayProbeDlg)
	//}}AFX_DATA_INIT
	
	m_nSide = PORTSIDE;
	m_nIndex = 0;
	m_pEditSpinItem = NULL;
	m_nColumn = 0;
}



void CPhasedArrayProbeDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhasedArrayProbeDlg)

	DDX_Control(pDX, IDC_COMBO_NUMBER_ELEMENTS, m_comboElementCount);
	DDX_Control(pDX, IDC_EDIT_ELEMENT_PITCH, m_editElementPitch);
	DDX_Control(pDX, IDC_SPIN_ELEMENT_PITCH, m_spinElementPitch);

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_WEDGE_ANGLE, m_editWedgeAngle);
	DDX_Control(pDX, IDC_SPIN_WEDGE_ANGLE, m_spinWedgeAngle);
	DDX_Control(pDX, IDC_EDIT_WEDGE_VELOCITY, m_editWedgeVelocity);
	DDX_Control(pDX, IDC_SPIN_WEDGE_VELOCITY, m_spinWedgeVelocity);
	DDX_Control(pDX, IDC_EDIT_WEDGE_HEIGHT_ELEMENT_1, m_editWedgeHeightElementOne);
	DDX_Control(pDX, IDC_SPIN_HEIGHT_ELEMENT_1, m_spinWedgeHeightElementOne);
	DDX_Control(pDX, IDC_COMBO_PROBE_TYPE, m_comboProbeType);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	DDX_Control(pDX, IDC_EDIT1, m_editVecE0E1);
}


BEGIN_MESSAGE_MAP(CPhasedArrayProbeDlg, CResizablePage)
	//{{AFX_MSG_MAP(CPhasedArrayProbeDlg)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ELEMENTS, OnClickListElements)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ELEMENTS, OnDblclkListElements)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ELEMENTS, OnGetdispinfoListElements)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ELEMENTS, OnRclickListElements)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_128, OnPhasedarrayprobelistNumberelements128)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_16, OnPhasedarrayprobelistNumberelements16)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_24, OnPhasedarrayprobelistNumberelements24)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_256, OnPhasedarrayprobelistNumberelements256)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_32, OnPhasedarrayprobelistNumberelements32)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_64, OnPhasedarrayprobelistNumberelements64)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_NUMBERELEMENTS_8, OnPhasedarrayprobelistNumberelements8)
	ON_EN_CHANGE(IDC_EDIT_ELEMENT_PITCH, OnChangeEditElementPitch)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_ELEMENTS, OnChangeEditElements)
	ON_BN_CLICKED(IDC_CHECK_REVERSE_ARRAY, OnCheckReverseArray)
	ON_COMMAND(ID_PHASEDARRAYPROBELIST_AUTOFILL, OnPhasedarrayprobelistAutofill)
	ON_COMMAND(ID_BUTTON_ELEMENTS, OnButtonElements)
	ON_COMMAND(ID_BUTTON_FOCAL_LAWS, OnButtonFocalLaws)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER_ELEMENTS, OnSelchangeComboNumberElements)
	ON_EN_CHANGE(IDC_EDIT_FOCAL_LENGTH, OnChangeEditFocalLength)
	ON_CBN_SELCHANGE(IDC_COMBO_DELAY_ALOGORITHM, OnSelchangeComboDelayAlogorithm)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ELEMENT_PITCH, OnDeltaposSpinElementPitch)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_PROBE_ANGLE, &CPhasedArrayProbeDlg::OnEnChangeEditWedgeAngle)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PROBE_ANGLE, &CPhasedArrayProbeDlg::OnDeltaposSpinWedgeAngle)
	ON_COMMAND(ID_BUTTON_RX_EQUALS_TX, OnButtonRxEqualTx)

	ON_EN_CHANGE(IDC_EDIT_WEDGE_VELOCITY, &CPhasedArrayProbeDlg::OnEnChangeEditWedgeVelocity)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WEDGE_VELOCITY, &CPhasedArrayProbeDlg::OnDeltaposSpinWedgeVelocity)
	ON_EN_CHANGE(IDC_EDIT_WEDGE_HEIGHT_ELEMENT_1, &CPhasedArrayProbeDlg::OnEnChangeEditWedgeHeightElement1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HEIGHT_ELEMENT_1, &CPhasedArrayProbeDlg::OnDeltaposSpinHeightElement1)
	ON_CBN_SELCHANGE(IDC_COMBO_PROBE_TYPE, &CPhasedArrayProbeDlg::OnCbnSelchangeComboProbeType)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT1, &CPhasedArrayProbeDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhasedArrayProbeDlg message handlers

int CPhasedArrayProbeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_PA_PROBE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create porosity ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}


BOOL CPhasedArrayProbeDlg::OnInitDialog() 
{
	BOOL bResult = CResizablePage::OnInitDialog();
	CString Buff;

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

	m_comboElementCount.AddString(L"8");
	m_comboElementCount.AddString(L"16");
	m_comboElementCount.AddString(L"24");
	m_comboElementCount.AddString(L"32");
	m_comboElementCount.AddString(L"64");
	m_comboElementCount.AddString(L"128");
	m_comboElementCount.AddString(L"256");

	m_spinWedgeAngle.SetRange(0, 7200);
	m_spinWedgeHeightElementOne.SetRange(0, 100);
	m_spinWedgeVelocity.SetRange(0, 100);

	Buff.Format(IDS_Linear_flat_array);	 m_comboProbeType.AddString(Buff);
	Buff.Format(IDS_Linear_concave);	m_comboProbeType.AddString(Buff);
	Buff.Format(IDS_Linear_convex);		m_comboProbeType.AddString(Buff);

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPhasedArrayProbeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CResizablePage::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPhasedArrayProbeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd()) {
		Invalidate(FALSE);
	}

}

void CPhasedArrayProbeDlg::InvalidateHardware(bool bFlag)
{
	if(theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX) 	theApp.m_PhasedArray[PORTSIDE].setRxEqualTx();
	theApp.m_PhasedArray[PORTSIDE].CalculateTxFocalLaws();
	theApp.m_PhasedArray[PORTSIDE].CalculateRxFocalLaws();

	Invalidate(false);
}

void CPhasedArrayProbeDlg::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;
	D3DXVECTOR3 vect;

	m_comboProbeType.SetCurSel(theApp.m_PhasedArray[PORTSIDE].getProbeType());

	Buff.Format(L"%.01f%s", theApp.m_PhasedArray[PORTSIDE].getWedgeAngle(), DEGREES);
	m_editWedgeAngle.SetWindowTextW(Buff);
	m_spinWedgeAngle.SetPos((int)(theApp.m_PhasedArray[PORTSIDE].getWedgeAngle() * 10.0f));

	Buff.Format(L"%.02f mm",theApp.m_PhasedArray[m_nSide].getElementPitch());
	m_editElementPitch.SetWindowText(Buff);

	switch(theApp.m_PhasedArray[PORTSIDE].getNumberElements()) {
	case 8: m_comboElementCount.SetCurSel(0);
		break;
	case 16: m_comboElementCount.SetCurSel(1);
		break;
	case 24: m_comboElementCount.SetCurSel(2);
		break;
	case 32: m_comboElementCount.SetCurSel(3);
		break;
	case 64: m_comboElementCount.SetCurSel(4);
		break;
	case 128: m_comboElementCount.SetCurSel(5);
		break;
	case 256: m_comboElementCount.SetCurSel(6);
		break;
	}

	Buff.Format(L"%d m/s", theApp.m_PhasedArray[PORTSIDE].getWedgeVelocity());
	m_editWedgeVelocity.SetWindowTextW(Buff);
	Buff.Format(L"%.01f mm", theApp.m_PhasedArray[PORTSIDE].getWedgeHeightElementOne());
	m_editWedgeHeightElementOne.SetWindowTextW(Buff);

	vect = theApp.m_PhasedArray[0].getVectorEOE1();
	Buff.Format(L"i:%.03f j:%.03f k:%.03f", vect.x, vect.y, vect.z);
	m_editVecE0E1.SetWindowTextW(Buff);

	FillList();

	SetToolBarCheckedState();

	setWindowTitle();
}

void CPhasedArrayProbeDlg::FillList()
{
	CString Buff, Gain;

	m_listCoords.ResetContent();
	if (theApp.m_LastSettings.nPhasedArrayMask & PA_DISPLAY_ELEMENTS) {
		for (int ii = 0; ii < theApp.m_PhasedArray[PORTSIDE].getNumberElements(); ii++) {
			Buff = theApp.m_PhasedArray[PORTSIDE].getstrElementPos(ii);
			m_listCoords.AddString(Buff);
		}
	}
	else {
		for (int ii = 0; ii < theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws(); ii++) {
			Buff = theApp.m_PhasedArray[PORTSIDE].getstrFocalLawPos(TX_FL,ii);
			Gain = theApp.m_PhasedArray[PORTSIDE].getstrFocalLawGain(RX_FL, ii);
			m_listCoords.AddString(Buff + L" " + Gain);
		}
	}
}



void CPhasedArrayProbeDlg::OnGetdispinfoListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nIndex;
	D3DXVECTOR3	vec;

	str[0]=0;

	nIndex = pDispInfo->item.iItem;

	if(theApp.m_LastSettings.nPhasedArrayMask & PA_DISPLAY_ELEMENTS) {
		if(nIndex < theApp.m_PhasedArray[m_nSide].getNumberElements()) {

			switch(pDispInfo->item.iSubItem) {
			case 0:
				break;
			case 1:	swprintf_s(str,L"%.02f",theApp.m_PhasedArray[m_nSide].m_PAElement[nIndex].vecPt.x);
				break;
			case 2:	swprintf_s(str,L"%.02f",theApp.m_PhasedArray[m_nSide].m_PAElement[nIndex].vecPt.y);
				break;
			case 3:	swprintf_s(str,L"%.02f",theApp.m_PhasedArray[m_nSide].m_PAElement[nIndex].vecPt.z);
				break;
			case 4:	swprintf_s(str,L"%.03f",theApp.m_PhasedArray[m_nSide].m_PAElement[nIndex].vecNorm.x);
				break;
			case 5:	swprintf_s(str,L"%.03f",theApp.m_PhasedArray[m_nSide].m_PAElement[nIndex].vecNorm.y);
				break;
			case 6:	swprintf_s(str,L"%.03f",theApp.m_PhasedArray[m_nSide].m_PAElement[nIndex].vecNorm.z);
				break;
			}
		}
	} else {
		if(nIndex < theApp.m_PhasedArray[m_nSide].getNumberFocalLaws()) {

			switch(pDispInfo->item.iSubItem) {
			case 0:
				break;
			case 1:	swprintf_s(str, L"%.02f", theApp.m_PhasedArray[m_nSide].getFocalLawPos(RX_FL, nIndex).x);
				break;
			case 2:	swprintf_s(str,L"%.02f",theApp.m_PhasedArray[m_nSide].getFocalLawPos(RX_FL, nIndex).y);
				break;
			case 3:	swprintf_s(str,L"%.02f",theApp.m_PhasedArray[m_nSide].getFocalLawPos(RX_FL, nIndex).z);
				break;
			case 4:	swprintf_s(str,L"%.03f",theApp.m_PhasedArray[m_nSide].getFocalLawNorm(RX_FL, nIndex).x);
				break;
			case 5:	swprintf_s(str,L"%.03f",theApp.m_PhasedArray[m_nSide].getFocalLawNorm(RX_FL, nIndex).y);
				break;
			case 6:	swprintf_s(str,L"%.03f",theApp.m_PhasedArray[m_nSide].getFocalLawNorm(RX_FL, nIndex).z);
				break;
			case 7: swprintf_s(str,L"%.01fdB",theApp.m_LSA.GetFLGain(nIndex));
				break;
			}
		}
	};

	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}


void CPhasedArrayProbeDlg::OnClickListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	
	*pResult = 0;
}

void CPhasedArrayProbeDlg::OnDblclkListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	SAFE_DELETE( m_pEditSpinItem );

	m_nIndex=pDispInfo->item.mask;

	if((m_nIndex>=0) && (m_nIndex<theApp.m_PhasedArray[m_nSide].getNumberElements())) {
		m_listElements.GetWindowRect(CtrlRect);
		m_listElements.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:	m_pEditSpinItem->Initialize(&theApp.m_PhasedArray[m_nSide].m_PAElement[m_nIndex].vecPt.x,-2000.0f,200.0f,0.1f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 2:	m_pEditSpinItem->Initialize(&theApp.m_PhasedArray[m_nSide].m_PAElement[m_nIndex].vecPt.y,-2000.0f,200.0f,0.1f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 3:	m_pEditSpinItem->Initialize(&theApp.m_PhasedArray[m_nSide].m_PAElement[m_nIndex].vecPt.z,-2000.0f,200.0f,0.1f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 4:	m_pEditSpinItem->Initialize(&theApp.m_PhasedArray[m_nSide].m_PAElement[m_nIndex].vecNorm.x,-1.0f,1.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 5:	m_pEditSpinItem->Initialize(&theApp.m_PhasedArray[m_nSide].m_PAElement[m_nIndex].vecNorm.y,-1.0f,1.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 6:	m_pEditSpinItem->Initialize(&theApp.m_PhasedArray[m_nSide].m_PAElement[m_nIndex].vecNorm.z,-1.0f,1.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}

	}

	*pResult = 0;
}

//ID_PHASEDARRAYPROBELIST_AUTOFILL

void CPhasedArrayProbeDlg::OnRclickListElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	UINT id;
	CString Buff;

	POSITION pos = m_listElements.GetFirstSelectedItemPosition();
	m_nIndex=m_listElements.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_PHASEDARRAY_PROBE_LIST_MENU)) {

		pPopup = menu->GetSubMenu(0);

		id = pPopup->GetMenuItemID(1);
		switch(m_nColumn = pDispInfo->item.iItem) {
		case 1: Buff.Format(L"Auto fill X axis");
			break;
		case 2: Buff.Format(L"Auto fill Y axis");
			break;
		case 3: Buff.Format(L"Auto fill Z axis");
			break;
		case 4:
		case 5:
		case 6: Buff.Format(L"Auto fill ijk");
			break;
		}
		pPopup->ModifyMenu(id,MF_BYCOMMAND,id,Buff);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
	};
	delete menu;
	
	*pResult = 0;
}

void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements256() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(256);
	UpdateAllControls();
}

void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements128() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(128);
	UpdateAllControls();
}

void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements64() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(64);
	UpdateAllControls();
}

void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements32() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(32);
	UpdateAllControls();
}

void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements24() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(24);
	UpdateAllControls();
}

void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements16() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(16);
	UpdateAllControls();
}


void CPhasedArrayProbeDlg::OnPhasedarrayprobelistNumberelements8() 
{
	theApp.m_PhasedArray[m_nSide].setNumberElements(8);
	UpdateAllControls();
}

void CPhasedArrayProbeDlg::OnChangeEditElementPitch() 
{
	CString Buff;
	float fTemp;

	m_editElementPitch.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	theApp.m_PhasedArray[m_nSide].setElementPitch(fTemp);

}

void CPhasedArrayProbeDlg::setWindowTitle()
{
	SetWindowText(theApp.m_PhasedArray[m_nSide].getFileName());
}


void CPhasedArrayProbeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_TX_APERTURE:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxAperture()) {
			theApp.m_PhasedArray[m_nSide].setTxAperture(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxAperture());
			m_editTxAperture.SetWindowText(Buff);
			ApplyFocalLaws();
			FillList();
		};
		break;
	case IDC_SPIN_RX_APERTURE:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxAperture()) {
			theApp.m_PhasedArray[m_nSide].setRxAperture(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getRxAperture());
			m_editRxAperture.SetWindowText(Buff);
			ApplyFocalLaws();
			FillList();
		};
		break;
	case IDC_SPIN_FIRST_TX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxFirstElement()) {
			theApp.m_PhasedArray[m_nSide].setTxFirstElement(nTemp);
			Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getTxFirstElement() + 1);
			m_editTxFirstElement.SetWindowText(Buff);
			ApplyFocalLaws();
			FillList();
		}
		break;
	case IDC_SPIN_LAST_TX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getTxLastElement()) {
			theApp.m_PhasedArray[m_nSide].setTxLastElement(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getTxLastElement() + 1);
			m_editTxLastElement.SetWindowText(Buff);
			ApplyFocalLaws();
			FillList();
		};
		break;
	case IDC_SPIN_FIRST_RX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxFirstElement()) {
			theApp.m_PhasedArray[m_nSide].setRxFirstElement(nTemp);
			Buff.Format(L"%d", theApp.m_PhasedArray[m_nSide].getRxFirstElement() + 1);
			m_editRxFirstElement.SetWindowText(Buff);
			ApplyFocalLaws();
			FillList();
		}
		break;
	case IDC_SPIN_LAST_RX_ELEMENT:
		nTemp = pSpin->GetPos() & 0xffff;
		if (nTemp - theApp.m_PhasedArray[m_nSide].getRxLastElement()) {
			theApp.m_PhasedArray[m_nSide].setRxLastElement(nTemp);
			Buff.Format(_T("%d"), theApp.m_PhasedArray[m_nSide].getRxLastElement() + 1);
			m_editRxLastElement.SetWindowText(Buff);
			ApplyFocalLaws();
			FillList();
		};
		break;
	};
	Invalidate(false);

	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPhasedArrayProbeDlg::ApplyFocalLaws()
{
	int nScanPitch = theApp.m_PhasedArray[m_nSide].getIntTxFocalLawPitch();
	int nNumberFocalLaws = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws();
	theApp.m_LSA.ApplyFocalLaws(nNumberFocalLaws, theApp.m_PhasedArray[PORTSIDE].getTxAperture(), theApp.m_PhasedArray[PORTSIDE].getTxFirstElement(), false, 0, nScanPitch, theApp.m_PhasedArray[PORTSIDE].getElementPitch());
	theApp.m_PhasedArray[PORTSIDE].setNumberFocalLaws(nNumberFocalLaws);
//	theApp.m_PhasedArray[PORTSIDE].CalculateFocalLawVertices();
	if(CURRENT_FL>=theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws()) {
		CURRENT_FL = theApp.m_UtUser.m_Global.nTimeSlot = theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws() - 1;
	}

}


void CPhasedArrayProbeDlg::OnChangeEditElements() 
{
}


void CPhasedArrayProbeDlg::OnCheckReverseArray() 
{
	theApp.m_PhasedArray[m_nSide].setReverseArray(m_checkReverseArray.GetCheck() & 1);
	ApplyFocalLaws();
	FillList();
}


void CPhasedArrayProbeDlg::OnPhasedarrayprobelistAutofill() 
{
	switch(m_nColumn) {
	case 1: theApp.m_PhasedArray[m_nSide].AutoFillCoordinates('X');
		break;
	case 2: theApp.m_PhasedArray[m_nSide].AutoFillCoordinates('Y');
		break;
	case 3: theApp.m_PhasedArray[m_nSide].AutoFillCoordinates('Z');
		break;
	case 4:
	case 5:
	case 6: theApp.m_PhasedArray[m_nSide].AutoFillCoordinates('i');
		break;
	};
	FillList();
}

void CPhasedArrayProbeDlg::OnButtonElements() 
{
	theApp.m_LastSettings.nPhasedArrayMask &= ~PA_DISPLAY_FOCAL_LAWS;
	theApp.m_LastSettings.nPhasedArrayMask |= PA_DISPLAY_ELEMENTS;
	
	SetToolBarCheckedState();
	FillList();
}

void CPhasedArrayProbeDlg::OnButtonFocalLaws() 
{
	theApp.m_LastSettings.nPhasedArrayMask &= ~PA_DISPLAY_ELEMENTS;
	theApp.m_LastSettings.nPhasedArrayMask |= PA_DISPLAY_FOCAL_LAWS;

	SetToolBarCheckedState();
	FillList();
}

void CPhasedArrayProbeDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_ELEMENTS))>=0)
		theApp.m_LastSettings.nPhasedArrayMask & PA_DISPLAY_ELEMENTS ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if ((nIndex = m_wndToolBar.CommandToIndex(ID_BUTTON_FOCAL_LAWS)) >= 0)
		theApp.m_LastSettings.nPhasedArrayMask & PA_DISPLAY_FOCAL_LAWS ? m_wndToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex, 0);
	if ((nIndex = m_wndToolBar.CommandToIndex(ID_BUTTON_RX_EQUALS_TX)) >= 0)
		theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX ? m_wndToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex, 0);


}

BOOL CPhasedArrayProbeDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   switch(nID) {
   case ID_BUTTON_ELEMENTS: strTipText.LoadString(IDS_Display_elements);
	   break;
   case ID_BUTTON_FOCAL_LAWS: strTipText.LoadString(IDS_Display_focal_laws);
	   break;
   case ID_BUTTON_RX_EQUALS_TX: strTipText.LoadStringW(ID_BUTTON_RX_EQUALS_TX);
	   break;
   case ID_BUTTON_DOWNLOAD_TO_HARDWARE: strTipText.LoadStringW(ID_BUTTON_DOWNLOAD_TO_HARDWARE);
	   break;
   }
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CPhasedArrayProbeDlg::OnSelchangeComboNumberElements() 
{
	CString Buff;

	switch(m_comboElementCount.GetCurSel()) {
	case 0: theApp.m_PhasedArray[PORTSIDE].setNumberElements(8);
		break;
	case 1: theApp.m_PhasedArray[PORTSIDE].setNumberElements(16);
		break;
	case 2: theApp.m_PhasedArray[PORTSIDE].setNumberElements(24);
		break;
	case 3: theApp.m_PhasedArray[PORTSIDE].setNumberElements(32);
		break;
	case 4: theApp.m_PhasedArray[PORTSIDE].setNumberElements(64);
		break;
	case 5: theApp.m_PhasedArray[PORTSIDE].setNumberElements(128);
		break;
	case 6: theApp.m_PhasedArray[PORTSIDE].setNumberElements(256);
		break;
	}

	ApplyFocalLaws();
	UpdateAllControls();

	Invalidate(false);
	
}


void CPhasedArrayProbeDlg::OnChangeEditFocalLength() 
{
	CString Buff;
	float fDistance = theApp.m_PhasedArray[PORTSIDE].getTxFocalLength();
	m_editFocalLength.GetWindowText(Buff);
	_WTOF(Buff,fDistance);
	if(fDistance - theApp.m_PhasedArray[PORTSIDE].getTxFocalLength()) {
		theApp.m_PhasedArray[PORTSIDE].setTxFocalLength(fDistance);
		ApplyFocalLaws();
		InvalidateDelays();
		InvalidateHardware(false);
		m_nUpdateHardware |= U_TX_FL;
	}

}


void CPhasedArrayProbeDlg::OnSelchangeComboDelayAlogorithm() 
{
	theApp.m_PhasedArray[PORTSIDE].m_nFocalLawAlgorithm = m_comboDelayAlgorithm.GetCurSel();
	ApplyFocalLaws();
	InvalidateDelays();
}


void CPhasedArrayProbeDlg::InvalidateDelays()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	theApp.m_PhasedArray[PORTSIDE].getDelays(&Buff);
	if (m_editDelays.GetSafeHwnd())	m_editDelays.SetWindowText(Buff);
}

void CPhasedArrayProbeDlg::OnDeltaposSpinElementPitch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	float fElementPitch = theApp.m_PhasedArray[m_nSide].getElementPitch();
	float fTemp = fElementPitch;

	fTemp -= ((float)pNMUpDown->iDelta * 0.01f);
	if(fTemp - fElementPitch) {
		theApp.m_PhasedArray[m_nSide].setElementPitch(fTemp);

		InvalidateHardware(false);
		m_nUpdateHardware |= U_TX_FL;
		m_nUpdateHardware |= U_RX_FL;

		UpdateAllControls();
		Invalidate(false);
	}
	
	*pResult = 0;
}









void CPhasedArrayProbeDlg::OnPaint()
{
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CBitmap bmp;
	CRect rr;
	COLORREF* m_pArray;

	theApp.m_PhasedArray[PORTSIDE].CalculateElementCoordinates();

	m_staticView.GetWindowRect(&m_rrView);
	ScreenToClient(&m_rrView);
	m_rrView.DeflateRect(1, 1);

	m_pArray = new COLORREF[(m_rrView.Width() + 2) * (m_rrView.Height() + 2)];

	bmp.CreateCompatibleBitmap(&dc, m_rrView.Width(), m_rrView.Height());
	bmp.SetBitmapBits((DWORD)(m_rrView.Width()*m_rrView.Height() * 4), m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	rr = m_rrView;
	rr.OffsetRect(-m_rrView.left, -m_rrView.top);
	dcMem.FillRect(&rr, &CBrush(RGB(255, 255, 255)));

	//
	m_fViewPortWidth = theApp.m_PhasedArray[PORTSIDE].getProbeLength() * 1.3f;
	m_fViewPortHeight = m_fViewPortWidth * (float)rr.Height() / (float)rr.Width();
	m_vectO = D3DXVECTOR3(m_fViewPortWidth / 2.0f, m_fViewPortHeight / 2.0f, 0.0f);

	
	switch (theApp.m_PhasedArray[PORTSIDE].getProbeType()) {
	default:
		RenderLinearProbe(&dcMem, &rr);
		RenderFocalLaws(&dcMem, &rr);
		break;
	case LINEAR_CONCAVE:
		RenderConcaveProbe(&dcMem, &rr);
		break;
	}
	RenderDelays(&dcMem, &rr);

	dc.BitBlt(m_rrView.left, m_rrView.top, m_rrView.Width(), m_rrView.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	SAFE_DELETE(m_pArray);


}


void CPhasedArrayProbeDlg::OnEnChangeEditWedgeAngle()
{
	CString Buff;
	float fTemp;

	m_editWedgeAngle.GetWindowText(Buff);
	_WTOF(Buff, fTemp);

	theApp.m_PhasedArray[PORTSIDE].setWedgeAngle(fTemp);

	m_spinWedgeAngle.SetPos((int)(fTemp*10.0f));
}


void CPhasedArrayProbeDlg::OnDeltaposSpinWedgeAngle(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString Buff;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fAngle = theApp.m_PhasedArray[PORTSIDE].getWedgeAngle();

	fAngle += ((float)pNMUpDown->iDelta * 0.5f);
	theApp.m_PhasedArray[PORTSIDE].setWedgeAngle(fAngle);
	Buff.Format(L"%.01f%s", theApp.m_PhasedArray[PORTSIDE].getWedgeAngle(),DEGREES);
	m_editWedgeAngle.SetWindowText(Buff);

	Invalidate(false);

	*pResult = 0;
}



void CPhasedArrayProbeDlg::RenderFocalLaws(CDC* pDC, CRect* rr)
{
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penYellow(PS_SOLID, 1, RGB(255, 255, 0));
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen *pOldPen = pDC->SelectObject(&penBlack);
	CPoint* pt = new CPoint[512 + 1];
	BYTE* pByte = new BYTE[512 + 1];
	D3DXMATRIXA16 matWorld, matBeam;
	D3DXVECTOR3 vecP, vecN;

	D3DXMatrixRotationZ(&matWorld, theApp.m_PhasedArray[PORTSIDE].getWedgeAngle() * DEG_TO_RAD);
	D3DXMatrixRotationZ(&matBeam, theApp.m_PhasedArray[PORTSIDE].getTxBeamAngle(0) * DEG_TO_RAD);

	int nn = 0;
	for (int nFL = 0; nFL < theApp.m_PhasedArray->getNumberFocalLaws(); nFL++) {
		vecP = theApp.m_PhasedArray->getFocalLawCenterPos(TX_FL, nFL);
		D3DXVec3TransformCoord(&vecN, &(theApp.m_Kinematics.m_fDesiredWaterPath[0] * D3DXVECTOR3(0.0f, -1.0f, 0.0f)), &matBeam);
		vecN += vecP;
		WorldToClient(&vecP, rr, &pt[nn], &matWorld);		pByte[nn++] = PT_MOVETO;
		WorldToClient(&vecN, rr, &pt[nn], &matWorld);		pByte[nn++] = PT_LINETO;
	}

	pDC->PolyDraw(pt, pByte, nn);

	delete pByte;
	delete pt;
	pDC->SelectObject(pOldPen);
}

void CPhasedArrayProbeDlg::WorldToClient(float fX, float fY, CRect *rrClient, CPoint *pt)
{
	pt->x = rrClient->left + (int)(fX * (float)rrClient->Width() / m_fViewPortWidth);
	pt->y = rrClient->bottom - (int)(fY * (float)rrClient->Height() / m_fViewPortHeight);
}

void CPhasedArrayProbeDlg::WorldToClient(D3DXVECTOR3 *vec, CRect *rrClient, CPoint *pt, D3DXMATRIXA16 *pMat)
{
	if (pMat) {
		D3DXVec3TransformCoord(vec, vec, pMat);
	}
	pt->x = rrClient->left + (int)((vec->x + m_vectO .x)* (float)rrClient->Width() / m_fViewPortWidth);
	pt->y = rrClient->bottom - (int)((vec->y + m_vectO.y) * (float)rrClient->Height() / m_fViewPortHeight);
}






void CPhasedArrayProbeDlg::RenderDelays(CDC* pDC, CRect* rr )
{
	LOGFONT		lf;
	CString		Buff,Temp;
	int pnX, pnY;

	if (GetSafeHwnd() == NULL) return;

	ZeroMemory(&lf, sizeof lf);
	lf.lfHeight = 12;
	theApp.SetFontLanguage(&lf);
	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);

	int nCurrentFL = theApp.m_LSA.m_nScopeViewLaw;

	//////////////////////////////////////////////////////////
	Temp.LoadStringW(IDS_Transmit_delays_in_ns);
	Buff.Format(L"%s: 0x%llp", Temp, (__int64)theApp.m_PhasedArray[PORTSIDE].getElementEnableMask(nCurrentFL, 0));
	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOutW(rr->Width() / 4 - size.cx / 2, 2, Buff);
	
	for (int nElement = 0; nElement < theApp.m_PhasedArray[PORTSIDE].getTxAperture(); nElement++) {
		Buff.Format(L"%.0f", theApp.m_PhasedArray[PORTSIDE].getTxDelay(nCurrentFL, nElement) / 1e-9);

		pnX = MulDiv(nElement + 1, rr->Width() / 2, theApp.m_PhasedArray[PORTSIDE].getTxAperture() + 1);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOutW(pnX - size.cx / 2, size.cy, Buff);
	}

	//////////////////////////////////////////////////////
	Temp.LoadStringW(IDS_Receive_delays_in_ns);
	Buff.Format(L"%s: 0x%I64X", Temp, theApp.m_PhasedArray[PORTSIDE].getElementEnableMask(nCurrentFL, 1));
	size = pDC->GetTextExtent(Buff);
	int nLeftEdge = rr->Width() - rr->Width() / 4;
	pDC->TextOutW( nLeftEdge - size.cx / 2, pnY=2, Buff);
	pnY += size.cy;

	nLeftEdge = rr->Width() - rr->Width() / 2;
	for (int nTOF = 0; nTOF < 4; nTOF++) {
		if (nTOF == 0 || theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF) > 0.0f) {

			Buff.Format(L"%.01f mm %.0f ns", theApp.m_PhasedArray[PORTSIDE].getRxFocalLength(nTOF), theApp.m_PhasedArray[PORTSIDE].getRxFocalTime(nCurrentFL,nTOF) / 1e-9);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOutW(rr->Width() - rr->Width() / 4 - size.cx / 2, pnY, Buff);
			pnY += size.cy;

			for (int nElement = 0; nElement < theApp.m_PhasedArray[PORTSIDE].getRxAperture(); nElement++) {
				Buff.Format(L"%.0f", theApp.m_PhasedArray[PORTSIDE].getRxDelay(nCurrentFL, nTOF, nElement) / 1e-9);

				pnX = MulDiv(nElement + 1, rr->Width() / 2, theApp.m_PhasedArray[PORTSIDE].getRxAperture() + 1) + nLeftEdge;
				size = pDC->GetTextExtent(Buff);
				pDC->TextOutW(pnX - size.cx / 2, pnY, Buff);
			}
			pnY += size.cy;
		}
	}


	pDC->SelectObject(pOldFont);
}




void CPhasedArrayProbeDlg::OnButtonRxEqualTx()
{
	theApp.m_LastSettings.nPhasedArrayMask & PA_RX_EQUAL_TX ? theApp.m_LastSettings.nPhasedArrayMask &= ~PA_RX_EQUAL_TX : theApp.m_LastSettings.nPhasedArrayMask |= PA_RX_EQUAL_TX;
	InvalidateHardware(false);
	UpdateAllControls();
}



BOOL CPhasedArrayProbeDlg::OnSetActive()
{
	theApp.m_LastSettings.nLastPhasedArrayTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	UpdateAllControls();
	theApp.StartThread(L"Phased Array Probe Dlg", &PhasedArrayProbeDlgThread, this, 200, THREAD_PRIORITY_NORMAL);

	return CResizablePage::OnSetActive();
}


BOOL CPhasedArrayProbeDlg::OnKillActive()
{

	theApp.SuspendThread(this);

	return CPropertyPage::OnKillActive();
}

UINT PhasedArrayProbeDlgThread(LPVOID pParam)
{
	CPhasedArrayProbeDlg* pParent = (CPhasedArrayProbeDlg*)pParam;

	if (WaitForSingleObject(pParent->m_hSemaphore, 10) == WAIT_OBJECT_0) {

		if (pParent->GetSafeHwnd() && pParent->IsWindowVisible() == TRUE) {
			if (pParent->m_nUpdateHardware & U_TX_FL) {
				theApp.m_PhasedArray[PORTSIDE].DownloadTxFocalLawsToHardware();
				pParent->m_nUpdateHardware &= ~U_TX_FL;
			}

			if (pParent->m_nUpdateHardware & U_RX_FL) {
				theApp.m_PhasedArray[PORTSIDE].DownloadRxFocalLawsToHardware();
				pParent->m_nUpdateHardware &= ~U_RX_FL;
			}
		}
		ReleaseSemaphore(pParent->m_hSemaphore, 1, NULL);
	}

	return THREAD_CONTINUE;
}



void CPhasedArrayProbeDlg::OnEnChangeEditWedgeVelocity()
{
	CString Buff;
	int nVelocity;

	m_editWedgeVelocity.GetWindowTextW(Buff);
	nVelocity = _ttoi(Buff);
	theApp.m_PhasedArray[PORTSIDE].setWedgeVelocity(nVelocity);

}


void CPhasedArrayProbeDlg::OnDeltaposSpinWedgeVelocity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nVelocity = theApp.m_PhasedArray[PORTSIDE].getWedgeVelocity();
	nVelocity += pNMUpDown->iDelta * 10;
	theApp.m_PhasedArray[PORTSIDE].setWedgeVelocity(nVelocity);
	UpdateAllControls();

	*pResult = 0;
}


void CPhasedArrayProbeDlg::OnEnChangeEditWedgeHeightElement1()
{
	CString Buff;
	float fHeight;

	m_editWedgeHeightElementOne.GetWindowTextW(Buff);
	_WTOF(Buff, fHeight);
	theApp.m_PhasedArray[PORTSIDE].setWedgeHeightElementOne(fHeight);
}


void CPhasedArrayProbeDlg::OnDeltaposSpinHeightElement1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	float fHeight = theApp.m_PhasedArray[PORTSIDE].getWedgeHeightElementOne();
	fHeight += (float)pNMUpDown->iDelta * 0.1f;
	theApp.m_PhasedArray[PORTSIDE].setWedgeHeightElementOne(fHeight);
	UpdateAllControls();

	*pResult = 0;
}


void CPhasedArrayProbeDlg::OnCbnSelchangeComboProbeType()
{
	theApp.m_PhasedArray[PORTSIDE].setProbeType(m_comboProbeType.GetCurSel());
	Invalidate(false);
}



void CPhasedArrayProbeDlg::OnClose()
{
	theApp.SuspendThread(this);

	CResizablePage::OnClose();
}


void CPhasedArrayProbeDlg::OnDestroy()
{
	CResizablePage::OnDestroy();

	theApp.SuspendThread(this);
}

void CPhasedArrayProbeDlg::RenderLinearProbe(CDC* pDC, CRect* rr)
{
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penYellow(PS_SOLID, 1, RGB(255, 255, 0));
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen *pOldPen = pDC->SelectObject(&penBlack);
	CPoint* pt = new CPoint[512 + 1];
	BYTE* pByte = new BYTE[512 + 1];
	D3DXMATRIXA16 matWorld;
	D3DXVECTOR3 vecP;
	CBrush brushOrange(RGB(255, 128, 0));
	CBrush brushPaleBlue(RGB(200, 200, 255));

	D3DXMatrixRotationZ(&matWorld, theApp.m_PhasedArray[PORTSIDE].getWedgeAngle() * DEG_TO_RAD);

	float fElementPitch = theApp.m_PhasedArray[PORTSIDE].getElementPitch();
	int nElementCount = theApp.m_PhasedArray[PORTSIDE].getNumberElements();
	float fProbeLength = fElementPitch * (float)(nElementCount + 2);
	float fHalfCrystalWidth = theApp.m_PhasedArray[PORTSIDE].getElementPitch() / 2.0f;

	CRect rrCrystals;
	int nn = 0;
	int ii;
	//Draw bottom of crystals against wedge
	vecP.x = fProbeLength / 2.0f;	vecP.y = 0.0f;
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_MOVETO;
	vecP.x = fProbeLength / -2.0f;	vecP.y = 0.0f;
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;

	//Draw top of crystals inside probe //Move to top left
	vecP = theApp.m_PhasedArray[PORTSIDE].getElementPos(0);
	vecP.x -= fHalfCrystalWidth;	 vecP.y += theApp.m_PhasedArray[PORTSIDE].getElementPitch();
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_MOVETO;
	rrCrystals.TopLeft() = pt[nn - 1];

	//Draw top right
	vecP = theApp.m_PhasedArray[PORTSIDE].getElementPos(nElementCount - 1);
	vecP.x += fHalfCrystalWidth;	 vecP.y += theApp.m_PhasedArray[PORTSIDE].getElementPitch();
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;

	//Draw Right edge 
	vecP = theApp.m_PhasedArray[PORTSIDE].getElementPos(nElementCount - 1);
	vecP.x += fHalfCrystalWidth;	 vecP.y += 0.0f;
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;
	rrCrystals.BottomRight() = pt[nn - 1];

	//Draw Bottom edge
	vecP = theApp.m_PhasedArray[PORTSIDE].getElementPos(0);
	vecP.x -= fHalfCrystalWidth;	 vecP.y += 0.0f;
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;

	//Draw Left edge 
	pt[nn] = pt[2];	pByte[nn++] = PT_LINETO;

	pDC->SelectObject(&penBlack);
	pDC->PolyDraw(pt, pByte, nn);

	//Flood file all elements
	pDC->SelectObject(&brushOrange);
	pDC->FloodFill(rrCrystals.CenterPoint().x, rrCrystals.CenterPoint().y, RGB(0, 0, 0));

	///////////////////////////////////////
	//Render Aperture
	nn = 0;
	int nFL = theApp.m_UtUser.m_Global.nTimeSlot;
	D3DXVECTOR3 vecBL = theApp.m_PhasedArray[PORTSIDE].getFocalLawElementPos(TX_FL, nFL, 0);
	D3DXVECTOR3 vecBR = theApp.m_PhasedArray[PORTSIDE].getFocalLawElementPos(TX_FL, nFL, theApp.m_PhasedArray[PORTSIDE].getTxAperture() - 1);
	vecBL.x -= fHalfCrystalWidth;
	vecBR.x += fHalfCrystalWidth;
	vecP = vecBL;
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_MOVETO;

	vecP = vecBL;	vecP.y += theApp.m_PhasedArray[PORTSIDE].getElementPitch();
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;
	rrCrystals.TopLeft() = pt[nn - 1];

	vecP = vecBR;	vecP.y += theApp.m_PhasedArray[PORTSIDE].getElementPitch();
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;

	vecP = vecBR;
	WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;
	rrCrystals.BottomRight() = pt[nn - 1];
	pDC->PolyDraw(pt, pByte, nn);

	//Flood fill all Aperture
	pDC->SelectObject(&brushPaleBlue);
	pDC->FloodFill(rrCrystals.CenterPoint().x, rrCrystals.CenterPoint().y, RGB(0, 0, 0));

	//Draw element edges
	for (ii = 0, nn = 0; ii < nElementCount; ii++) {
		vecP = theApp.m_PhasedArray[PORTSIDE].getElementPos(ii);
		vecP.x += fHalfCrystalWidth;	 vecP.y += 0.0f;
		WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_MOVETO;

		vecP = theApp.m_PhasedArray[PORTSIDE].getElementPos(ii);
		vecP.x += fHalfCrystalWidth;	 vecP.y += theApp.m_PhasedArray[PORTSIDE].getElementPitch();
		WorldToClient(&vecP, rr, &pt[nn], &matWorld);	pByte[nn++] = PT_LINETO;
	}

	pDC->SelectObject(&penBlack);
	pDC->PolyDraw(pt, pByte, nn);


	delete pt;
	delete pByte;
	pDC->SelectObject(pOldPen);
}

void CPhasedArrayProbeDlg::RenderConcaveProbe(CDC* pDC, CRect* rr)
{
	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penYellow(PS_SOLID, 1, RGB(255, 255, 0));
	CPen penBlue(PS_SOLID, 1, RGB(0, 0, 255));
	CPen *pOldPen = pDC->SelectObject(&penBlack);
	CPoint* ptInner = new CPoint[512 + 1];
	CPoint* ptOuter = new CPoint[512 + 1];
	CPoint* ptVert = new CPoint[512 + 1];
	BYTE* pByte = new BYTE[512 + 1];
	BYTE* pByteVert = new BYTE[512 + 1];
	D3DXMATRIXA16 matWorld;
	D3DXVECTOR3 vecP;
	CBrush brushOrange(RGB(255, 128, 0));
	CBrush brushPaleBlue(RGB(200, 200, 255));
	CRect rrProbe;
	D3DXVECTOR3 vectIn,vectOut;
	D3DXVECTOR4 vPt;
	D3DXMATRIX mat;

	rrProbe.left = rr->CenterPoint().x - rr->Height() / 3;
	rrProbe.right = rr->CenterPoint().x + rr->Height() / 3;
	rrProbe.top = rr->CenterPoint().y - rr->Height() / 3;
	rrProbe.bottom = rr->CenterPoint().y + rr->Height() / 3;

	float fInnerRadius = (float)rr->Height() / 3.0f;
	float fAngle = ((PIf / 2.0f) / ((float)theApp.m_PhasedArray[PORTSIDE].getNumberElements())) / 2.0f;
	vectIn.x = fInnerRadius;
	vectIn.y = -fInnerRadius * tanf(fAngle);
	vectIn.z = 0.0f;
	float fOuterRadius = fInnerRadius - 2.0f * vectIn.y;
	vectOut.x = fOuterRadius;
	vectOut.y = -fOuterRadius * tanf(fAngle);
	vectOut.z = 0.0f;

	int nIndex = 0;
	int nn = 0;
	for (int nE = 0; nE <= theApp.m_PhasedArray[PORTSIDE].getNumberElements(); nE++) {
		fAngle = ((float)nE * (PIf / 2.0f)) / ((float)theApp.m_PhasedArray[PORTSIDE].getNumberElements() - 1);
		D3DXMatrixRotationZ(&mat, fAngle);
		mat._41 = (float)rrProbe.CenterPoint().x;
		mat._42 = (float)rrProbe.CenterPoint().y;

		D3DXVec3Transform(&vPt, &vectIn, &mat);
		ptInner[nIndex] = CPoint((int)vPt.x, (int)vPt.y);
		D3DXVec3Transform(&vPt, &vectOut, &mat);
		ptOuter[nIndex] = CPoint((int)vPt.x, (int)vPt.y);

		ptVert[nn] = ptInner[nIndex];	pByteVert[nn++] = PT_MOVETO;
		ptVert[nn] = ptOuter[nIndex];	pByteVert[nn++] = PT_LINETO;

		if (nIndex == 0) {
			pByte[nIndex++] = PT_MOVETO;
		}
		else {
			pByte[nIndex++] = PT_LINETO;
		}

	}
	pDC->PolyDraw(ptInner, pByte, nIndex);
	pDC->PolyDraw(ptOuter, pByte, nIndex);
	pDC->PolyDraw(ptVert, pByteVert, nn);

	for (int nE = 0; nE < theApp.m_PhasedArray[PORTSIDE].getNumberElements(); nE++) {
		pDC->SelectObject(&brushOrange);
		pDC->FloodFill((ptInner[nE].x + ptOuter[nE+1].x) / 2, (ptInner[nE].y + ptOuter[nE+1].y) / 2, RGB(0, 0, 0));
	}

	pDC->MoveTo(rrProbe.CenterPoint());
	pDC->LineTo(rrProbe.CenterPoint().x + (int)fInnerRadius, rrProbe.CenterPoint().y);
	pDC->MoveTo(rrProbe.CenterPoint());
	pDC->LineTo(rrProbe.CenterPoint().x, rrProbe.CenterPoint().y + (int)fInnerRadius);

	delete pByteVert;
	delete ptVert;
	delete ptInner;
	delete ptOuter;
	delete pByte;
	pDC->SelectObject(pOldPen);
}


void CPhasedArrayProbeDlg::OnEnChangeEdit1()
{
	CString Buff;
	D3DXVECTOR3 vec;

	m_editVecE0E1.GetWindowTextW(Buff);
	ExtractVector(Buff, NULL, &vec, NULL);

	theApp.m_PhasedArray[0].setVectorEOE1(vec);
}


BOOL CPhasedArrayProbeDlg::PreTranslateMessage(MSG* pMsg)
{
	HANDLE hWnd;
	CString	Buff;

	if (pMsg->message == 0x100) {

		switch (pMsg->wParam) {
		case 13:
			hWnd = ::GetFocus();
			if (hWnd == m_editVecE0E1.m_hWnd) {
				theApp.m_PhasedArray[PORTSIDE].CalculateElementCoordinates();
				theApp.m_PhasedArray[PORTSIDE].CalculateTxFocalLaws();
				theApp.m_PhasedArray[PORTSIDE].CalculateRxFocalLaws();

				FillList();

				::SetFocus(m_editVecE0E1.m_hWnd);
				return TRUE;
			}
			break;
		}
	}

	return CResizablePage::PreTranslateMessage(pMsg);
}
