#include "stdafx.h"
#include "USLScanner.h"
#include "AxisPane.h"
#include "USLPropertyGridProperty.h"


CAxisPane::CAxisPane() : CDockablePane()
{
	m_nAxis = 0;
}


CAxisPane::~CAxisPane()
{
}
BEGIN_MESSAGE_MAP(CAxisPane, CDockablePane)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_CBN_SELENDOK(ID_COMBO_PROPERTY, OnSelendokProperty)
	ON_WM_SIZE()
	ON_MESSAGE(UI_SET_ACCESS_PRIVELAGES, SetAccessPrivelages)
END_MESSAGE_MAP()


int CAxisPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_COMBO_PROPERTY))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	CString Buff;
	for (int nAxis = 0; nAxis < 20; nAxis++) {
		Buff.Format(L"Axis %d : %s", nAxis + 1, theApp.m_Axes[nAxis].cName);
		m_wndObjectCombo.AddString(Buff);
	}
	m_wndObjectCombo.SetCurSel(m_nAxis);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_PROPERTY_LIST))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	CreateFont();
	AdjustLayout();

	return 0;
}

void CAxisPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CAxisPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect(&rectCombo);
	int nComboHeight = rectCombo.Height();

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + nComboHeight, rectClient.Width(), rectClient.Height() - nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CAxisPane::CreateFont()
{
	LOGFONT lf;
	::DeleteObject(m_Font.Detach());

	if (theApp.m_LastSettings.bUseAlernativeToSystemFont == false) {
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;
	}
	else {
		lf = theApp.m_LastSettings.lfMenus;
	}
	lf.lfHeight = MulDiv(lf.lfHeight, theApp.nPercentageTextSize[theApp.m_LastSettings.nTextPercentageSize = 3], 100);
	m_Font.CreateFontIndirect(&lf);
	m_wndPropList.SetFont(&m_Font);
	m_wndObjectCombo.SetFont(&m_Font);

	return 0;
}

HRESULT CAxisPane::SetAccessPrivelages(WPARAM wp, LPARAM lp)
{
	bool bFlag;
	theApp.m_nLogonLevelMask&theApp.m_cAccess[m_nID] ? bFlag = true : bFlag = false;

	int nGroupCount = m_wndPropList.GetPropertyCount();
	for(int ii = 0; ii < nGroupCount; ii++) {

		CMFCPropertyGridProperty* pGroup = m_wndPropList.GetProperty(ii);
		int nItemCount = pGroup->GetSubItemsCount();
		for (int nItem = 0; nItem < nItemCount; nItem++) {
			CMFCPropertyGridProperty* pProperty = pGroup->GetSubItem(nItem);
			pProperty->Enable(bFlag);
		}
	}
	return 0;
}

#define	ID_BASE					0
#define	ID_NAME					ID_BASE + 1
#define	ID_STEPSIZE				ID_BASE + 2
#define	ID_MOTOR_TYPE			ID_BASE + 3
#define	ID_AXIS_TYPE			ID_BASE + 4
#define	ID_AMPLIFIER_TYPE		ID_BASE + 5
#define	ID_UNITS_TYPE			ID_BASE + 6
#define	ID_AVAILABLE			ID_BASE + 7
#define	ID_PRECISION			ID_BASE + 8
#define	ID_PHYSICAL_AXIS		ID_BASE + 9
#define ID_SPEED				ID_BASE + 10
#define	ID_MOVE_SPEED			ID_BASE + 11
#define	ID_JOG_SPEED			ID_BASE + 12
#define	ID_JOG_ACCELERATION		ID_BASE + 13
#define	ID_HOME_SPEED			ID_BASE + 14
#define	ID_ACCELERATION			ID_BASE + 15
#define ID_STEPS_PER_REV		ID_BASE + 16
#define ID_MODULO_ABSOLUTE_MODE	ID_BASE + 17
#define ID_KINEMATIC_NEGATIVE	ID_BASE + 18
#define ID_KINEMATIC_POSITIVE	ID_BASE + 19
#define ID_LIMIT_NEGATIVE		ID_BASE + 20
#define ID_LIMIT_POSITIVE		ID_BASE + 21

int CAxisPane::InitPropList()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CUSLPropertyGridProperty* pGroup;
	CUSLPropertyGridProperty* pProp;
	CUSLPropertyGridProperty* pPropCombo;
	CUSLPropertyGridProperty* pPropSpin;
	CString MotorType[] = { L"Stepper", L"Servo",  L"Robot" };
	CString AxisType[] = { L"Linear / Arc", L"Rotary",  L"Roller" };
	CString Units[] = { L"mm", L"\x022", L"\x0b0" };
	CString ModuloMode[] = { L"In range", L"Shortest", L"Positive", L"Negative" };
	CString AmplifierType[] = { L"", L"Kollmorgen", L"Copley", L"Staubli", L"GeoBrick" };

	CString Buff;
	CString Format;
	int nPrecision[] = { 0, 2, 2, 2, 3, 3 };
	bool bFlag;


	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	Buff.Format(L"Axis %d", m_nAxis + 1);
	pGroup = new CUSLPropertyGridProperty(Buff, ID_BASE, FALSE);
	pProp = new CUSLPropertyGridProperty(_T("Name"), (_variant_t)theApp.m_Axes[m_nAxis].cName, _T("Enter the name of the axis"), ID_NAME);
	pGroup->AddSubItem(pProp);

	pPropCombo = new CUSLPropertyGridProperty(_T("Motor Type"), (_variant_t)MotorType[theApp.m_Axes[m_nAxis].nMotorType], _T("Set the motor type"), ID_MOTOR_TYPE);
	for (int nType = 0; nType < 3; nType++)
		pPropCombo->AddOption(MotorType[nType]);
	pGroup->AddSubItem(pPropCombo);

	pPropCombo = new CUSLPropertyGridProperty(_T("Axis Type"), (_variant_t)AxisType[theApp.m_Axes[m_nAxis].nAxisType], _T("Set the axis type"), ID_AXIS_TYPE);
	for (int nType = 0; nType < 3; nType++)
		pPropCombo->AddOption(AxisType[nType]);
	pGroup->AddSubItem(pPropCombo);

	pPropCombo = new CUSLPropertyGridProperty(_T("Amplifier Type"), (_variant_t)AmplifierType[theApp.m_Axes[m_nAxis].eAmplifierType], _T("Set the amplifier type"), ID_AMPLIFIER_TYPE);
	for (int nType = 0; nType < 3; nType++)
		pPropCombo->AddOption(AmplifierType[nType]);
	pGroup->AddSubItem(pPropCombo);

	Format.Format(L"%s %s", L"%.07f ", Units[MinMax(&theApp.m_Axes[m_nAxis].nUnitSelected, 0, 2)]);
	pPropSpin = new CUSLPropertyGridProperty(_T("Step size"), (_variant_t)(int)(0), _T("Specifies step size"), ID_STEPSIZE, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fStepSize, -1.0f, 1.0f, 7);
	pGroup->AddSubItem(pPropSpin);

	pPropCombo = new CUSLPropertyGridProperty(_T("Units"), (_variant_t)Units[theApp.m_Axes[m_nAxis].nUnitSelected], _T("Set the type of units"), ID_UNITS_TYPE);
	for (int nUnits = 0; nUnits < 3; nUnits++)
		pPropCombo->AddOption(Units[nUnits]);
	pGroup->AddSubItem(pPropCombo);


	theApp.m_Axes[m_nAxis].bAvailable == TRUE ? bFlag = true : bFlag = false;
	pGroup->AddSubItem(new CUSLPropertyGridProperty(_T("Available"), (_variant_t)bFlag, _T("Is this axis available"), ID_AVAILABLE));

	Format.Format(L"%s", L"%.0f");
	pPropSpin = new CUSLPropertyGridProperty(_T("Display precision"), (_variant_t)(int)(0), _T("How many decimal places will the position be rounded to"), ID_PRECISION, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_Axes[m_nAxis].nPrecision, 0.0f, 7.0f, 0);
	pGroup->AddSubItem(pPropSpin);

	pPropSpin = new CUSLPropertyGridProperty(L"Physical axis", (_variant_t)(int)(0), _T("Physical axis in the motion controller"), ID_PHYSICAL_AXIS, L"%.0f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_Axes[m_nAxis].nPhysicalAxis, 1.0f, 32.0f, 0);
	pGroup->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup);


	Buff.Format(L"Speeds, Accel / Decel");
	pGroup = new CUSLPropertyGridProperty(Buff, ID_SPEED, FALSE);
	Format.Format(L"%s %ss\x02c9\x0b9", L"%.01f ", Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	pPropSpin = new CUSLPropertyGridProperty(_T("Move Speed"), (_variant_t)(int)(0), _T("Specifies step size"), ID_MOVE_SPEED, Format, NULL, NULL, (CWnd*)this);
	float fMoveSpeed = theApp.m_Axes[m_nAxis].fMaxMoveSpeed * 1000.0f * theApp.m_Axes[m_nAxis].fStepSize;
	pPropSpin->EnableSpinControl(TRUE, fMoveSpeed, 0.1f, 1000.0f, 1);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Home Speed"), (_variant_t)(int)(0), _T("Specifies step size"), ID_HOME_SPEED, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fHomeSpeed, -1000.0f, 1000.0f, 1);
	pGroup->AddSubItem(pPropSpin);
	Format.Format(L"%s %ss\x02c9\x0b2", L"%.02f ", Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	pPropSpin = new CUSLPropertyGridProperty(_T("Acceleration"), (_variant_t)(int)(0), _T("Specifies step size"), ID_ACCELERATION, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fAcceleration, -100.0f, 3000.0f, 2);
	pGroup->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup);

	Buff.Format(L"Joystick");
	pGroup = new CUSLPropertyGridProperty(Buff, ID_SPEED, FALSE);
	Format.Format(L"%s %ss\x02c9\x0b9", L"%.01f ", Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	pPropSpin = new CUSLPropertyGridProperty(_T("Jog Speed"), (_variant_t)(int)(0), _T("Max speed of a J+ move"), ID_JOG_SPEED, Format, NULL, NULL, (CWnd*)this);
	float fJogSpeed = theApp.m_Axes[m_nAxis].fMaxMoveSpeed * 1000.0f * theApp.m_Axes[m_nAxis].fStepSize;
	pPropSpin->EnableSpinControl(TRUE, fJogSpeed, 0.1f, 1000.0f, 1);
	pGroup->AddSubItem(pPropSpin);
	Format.Format(L"%s %ss\x02c9\x0b2", L"%.02f ", Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	pPropSpin = new CUSLPropertyGridProperty(_T("Jog Acceleration"), (_variant_t)(int)(0), _T("Acceleration of a J+ move"), ID_JOG_ACCELERATION, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fJoystickAcceleration, -100.0f, 1000.0f, 2);
	pGroup->AddSubItem(pPropSpin);

	m_wndPropList.AddProperty(pGroup);

	pGroup = new CUSLPropertyGridProperty(_T("Modulo"));
	pPropSpin = new CUSLPropertyGridProperty(_T("Encoder lines per rev"), (_variant_t)(int)(0), _T("Specifies step size"), ID_STEPS_PER_REV, L"%.01f", NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, (float)theApp.m_Axes[m_nAxis].dStepsPerRev, 0.0f, 1000000.0f, 1);
	pGroup->AddSubItem(pPropSpin);

	pPropCombo = new CUSLPropertyGridProperty(_T("Absolute direction"), (_variant_t)ModuloMode[theApp.m_Axes[m_nAxis].nModuloMode], L"Direction for absolute motion task", ID_MODULO_ABSOLUTE_MODE);
	for (int ii = 0; ii<4; ii++)
		pPropCombo->AddOption(ModuloMode[ii]);
	pPropCombo->AllowEdit(FALSE);
	pGroup->AddSubItem(pPropCombo);

	m_wndPropList.AddProperty(pGroup);

#define ID_KINEMATIC_NEGATIVE	ID_BASE + 18
#define ID_KINEMATIC_POSITIVE	ID_BASE + 19
#define ID_LIMIT_NEGATIVE		ID_BASE + 20
#define ID_LIMIT_POSITIVE		ID_BASE + 21

	pGroup = new CUSLPropertyGridProperty(_T("Limits"));
	Format.Format(L"%s%s", L"%.0f ", Units[theApp.m_Axes[m_nAxis].nUnitSelected]);
	pPropSpin = new CUSLPropertyGridProperty(_T("Kinematic negative"), (_variant_t)(int)(0), _T("Minimum position in a kinematic coordinated move"), ID_KINEMATIC_NEGATIVE, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fMinKin, -16000.0f, 16000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Kinematic positive"), (_variant_t)(int)(0), _T("Maximum position in a kinematic coordinated move"), ID_KINEMATIC_POSITIVE, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fMaxKin, -16000.0f, 16000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Travel negative"), (_variant_t)(int)(0), _T("Minimum position in any type of move"), ID_LIMIT_NEGATIVE, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fMinTravel, -16000.0f, 16000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	pPropSpin = new CUSLPropertyGridProperty(_T("Travel positive"), (_variant_t)(int)(0), _T("Maximum position in any type of move"), ID_LIMIT_POSITIVE, Format, NULL, NULL, (CWnd*)this);
	pPropSpin->EnableSpinControl(TRUE, theApp.m_Axes[m_nAxis].fMaxTravel, -16000.0f, 16000.0f, 0);
	pGroup->AddSubItem(pPropSpin);
	m_wndPropList.AddProperty(pGroup);

	SetAccessPrivelages();

	return 0;
}

LRESULT CAxisPane::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CUSLPropertyGridProperty * pProperty = (CUSLPropertyGridProperty *)lParam;
	CMFCPropertyGridColorProperty* pPropertyColour = (CMFCPropertyGridColorProperty *)lParam;
	CUSLPropertyGridProperty * pUSLProperty = (CUSLPropertyGridProperty *)lParam;
	CMFCPropertyGridFontProperty* pFontProperty = (CMFCPropertyGridFontProperty *)lParam;
	CUSLPropertyGridProperty* pPropCombo = (CUSLPropertyGridProperty *)lParam;
	CUSLPropertyGridProperty* pPropSpin = (CUSLPropertyGridProperty *)lParam;
	_variant_t value;
	CString Buff;
	int nID;

	nID = pProperty->GetData();

	switch (nID) {
	case ID_NAME:
		swprintf(theApp.m_Axes[m_nAxis].cName, 10, L"%s", pProperty->GetValue().bstrVal);
		Buff.Format(L"Axis %d: %s", m_nAxis + 1, pProperty->GetValue().bstrVal);
		m_wndObjectCombo.DeleteString(m_nAxis);
		m_wndObjectCombo.InsertString(m_nAxis, Buff);
		m_wndObjectCombo.SetCurSel(m_nAxis);
		break;
	case ID_MOTOR_TYPE: theApp.m_Axes[m_nAxis].nMotorType = pPropCombo->GetCurSel();
		break;
	case ID_AXIS_TYPE: theApp.m_Axes[m_nAxis].nAxisType = pPropCombo->GetCurSel();
		break;
	case ID_AMPLIFIER_TYPE: theApp.m_Axes[m_nAxis].eAmplifierType = (AmplifierType)pPropCombo->GetCurSel();
		break;
	case ID_STEPSIZE: theApp.m_Axes[m_nAxis].fStepSize = pPropSpin->GetFloatSpinValue();
		break;
	case ID_UNITS_TYPE: theApp.m_Axes[m_nAxis].nUnitSelected = pPropCombo->GetCurSel();
		InitPropList();
		break;
	case ID_AVAILABLE: theApp.m_Axes[m_nAxis].bAvailable = ((bool)pProperty->GetValue().boolVal);
		break;
	case ID_PRECISION: theApp.m_Axes[m_nAxis].nPrecision = ((int)pUSLProperty->GetFloatSpinValue());
		break;
	case ID_PHYSICAL_AXIS:theApp.m_Axes[m_nAxis].nPhysicalAxis = ((int)pUSLProperty->GetFloatSpinValue());
		break;
	case ID_STEPS_PER_REV: theApp.m_Axes[m_nAxis].dStepsPerRev = (double)pPropSpin->GetFloatSpinValue();
		break;
	case ID_MODULO_ABSOLUTE_MODE: theApp.m_Axes[m_nAxis].nModuloMode = pPropCombo->GetCurSel();
		break;
	case ID_KINEMATIC_NEGATIVE: theApp.m_Axes[m_nAxis].fMinKin = pPropSpin->GetFloatSpinValue();
		break;
	case ID_KINEMATIC_POSITIVE: theApp.m_Axes[m_nAxis].fMaxKin = pPropSpin->GetFloatSpinValue();
		break;
	case ID_LIMIT_NEGATIVE: theApp.m_Axes[m_nAxis].fMinTravel = pPropSpin->GetFloatSpinValue();
		break;
	case ID_LIMIT_POSITIVE: theApp.m_Axes[m_nAxis].fMaxTravel = pPropSpin->GetFloatSpinValue();
		break;
	case ID_ACCELERATION: theApp.m_Axes[m_nAxis].fAcceleration = pUSLProperty->GetFloatSpinValue();
		break;
	case ID_JOG_ACCELERATION: theApp.m_Axes[m_nAxis].fJoystickAcceleration = pUSLProperty->GetFloatSpinValue();
		break;


/*
	case ID_MOVE_SPEED: theApp.m_Axes[m_nAxis].setMoveSpeed(pUSLProperty->GetFloatSpinValue());
		break;
	case ID_JOG_SPEED: theApp.m_Axes[m_nAxis].setJogSpeed(pUSLProperty->GetFloatSpinValue());
		break;
	case ID_HOME_SPEED: theApp.m_Axes[m_nAxis].setHomeSpeed(pUSLProperty->GetFloatSpinValue());
		break;
		*/
	}

//	if (lParam == 0x1038bec8) {
		theApp.m_FBCtrl.DownloadAxisParameters(m_nAxis);
//	}

	return 0;

}

void CAxisPane::OnSelendokProperty()
{
	m_nAxis = m_wndObjectCombo.GetCurSel();
	InitPropList();
}

