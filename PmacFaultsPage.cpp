// PmacFaultsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacFaultsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacFaultsPage property page

IMPLEMENT_DYNCREATE(CPmacFaultsPage, CPropertyPage)

CPmacFaultsPage::CPmacFaultsPage() : CPropertyPage(CPmacFaultsPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacFaultsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nRepairAxis = 0;

}

CPmacFaultsPage::~CPmacFaultsPage()
{
}

void CPmacFaultsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacFaultsPage)
	DDX_Control(pDX, IDC_COMBO_REPAIR_AXIS, m_comboRepairAxis);
	DDX_Control(pDX, IDC_EDIT_FAULTS, m_editFaults);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacFaultsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPmacFaultsPage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_PMAC_RESET, OnButtonPmacReset)
	ON_BN_CLICKED(IDC_BUTTON_ATTEMPT, OnButtonAttempt)
	ON_CBN_SELCHANGE(IDC_COMBO_REPAIR_AXIS, OnSelchangeComboRepairAxis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacFaultsPage message handlers

BOOL CPmacFaultsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_ErrorCodes.RemoveAll();
	m_ErrorCodes.InsertAt(5,L"Check mains supply and fuses");
	m_ErrorCodes.InsertAt(27,L"Check mains supply and fuses");
	m_ErrorCodes.InsertAt(32,L"Over speed; Trip the circuit breaker");
	m_ErrorCodes.InsertAt(37,L"Memory error; Cycle power");

	for(int nAxis = 0;nAxis<10;nAxis++) {
		m_comboRepairAxis.AddString(theApp.m_Axes[nAxis].cName);
	}
	m_comboRepairAxis.SetCurSel(m_nRepairAxis);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPmacFaultsPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,300,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CPmacFaultsPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CPmacFaultsPage::OnTimer(UINT nIDEvent) 
{
	CString Buff,strWindow,strTemp;
	int nIndex;

	strWindow.Empty();
	Buff.Empty();
	
	for(int nAxis = 0;nAxis<16;nAxis++) {

		if(theApp.m_Axes[nAxis].bAvailable == TRUE) {

			if(theApp.m_Axes[nAxis].nStatus & PMAC_AMPLIFIER_FAULT) {

				if(theApp.m_Axes[nAxis].nMacroNode >=0) {

					Buff.Format(L"MS%d,MI30=$0801",theApp.m_Axes[nAxis].nMacroNode);
					theApp.m_PmacUser.SendStr(Buff);
					theApp.m_PmacUser.RequestSlaveVariable(L"I",theApp.m_Axes[nAxis].nMacroNode,31,&Buff);

					ConvertToDecimal(Buff);

					nIndex = _ttoi(Buff);
					if (nIndex<0) nIndex = 0;
					if(nIndex>=m_ErrorCodes.GetSize()) nIndex = 0;
					strTemp.Format(L"%s: \tError code %s: \t%s\r\n",theApp.m_Axes[nAxis].cName, Buff, m_ErrorCodes.GetAt(nIndex));

					strWindow += strTemp;
				}
			}
		}
	}

	m_editFaults.SetWindowText(strWindow);

	CPropertyPage::OnTimer(nIDEvent);
}

void CPmacFaultsPage::ConvertToDecimal(CString &Buff)
{
	int nn;

	if(Buff.Find(L"$") == 0) {
		Buff.Replace(L"$",L"0x");
		swscanf_s(Buff,_T("%x"),&nn);
		Buff.Format(L"%d",nn);
	}

}

void CPmacFaultsPage::OnButtonReset() 
{
	CString Buff;

	KillTimer(1);
	Sleep(100);
	Buff.Format(L"MS0,MI30=$0A08");				//Select P.10.08	Fault Reset Select
	theApp.m_PmacUser.SendStr(Buff);
	Buff.Format(L"MS0,MI31=$1");				//Make it High
	theApp.m_PmacUser.SendStr(Buff);
	Sleep(500);
	Buff.Format(L"MS0,MI30=$0A08");				//Select P.10.08	Fault Reset Select
	theApp.m_PmacUser.SendStr(Buff);
	Buff.Format(L"MS0,MI31=$000040020201");		//Set it to look at External reset on DI2
	theApp.m_PmacUser.SendStr(Buff);

	SetTimer(1,500,NULL);
}



void CPmacFaultsPage::SetABBOutput(bool bFlag)
{
	CString Buff;

	KillTimer(1);
	Sleep(100);
	Buff.Format(L"MS0,MI30=$0C01");		//Select P.12.01	DIO 1 Configuration
	theApp.m_PmacUser.SendStr(Buff);
	Buff.Format(L"MS0,MI31=$0");		//Make it an 0:output 1:Input
	theApp.m_PmacUser.SendStr(Buff);

	Buff.Format(L"MS0,MI30=$0C04");		//Select P.12.04	DIO 1 Ptr
	theApp.m_PmacUser.SendStr(Buff);
	if(bFlag == false) {
		Buff.Format(L"MS0,MI31=$0");
	} else {
		Buff.Format(L"MS0,MI31=$1");
	}
	theApp.m_PmacUser.SendStr(Buff);

	SetTimer(1,500,NULL);
}

void CPmacFaultsPage::OnButtonPmacReset() 
{
	KillTimer(1);
	Sleep(100);
	theApp.m_PmacUser.SendStr(L"$$$");

	Sleep(1000);
	SetTimer(1,500,NULL);
}

void CPmacFaultsPage::OnButtonAttempt() 
{
	CString Buff;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nRepairAxis = m_comboRepairAxis.GetCurSel();
	theApp.m_Thread.m_nThreadAction = REPAIR_AXIS;
}

void CPmacFaultsPage::OnSelchangeComboRepairAxis() 
{
	m_nRepairAxis = m_comboRepairAxis.GetCurSel();
}

BOOL CPmacFaultsPage::PreTranslateMessage(MSG* pMsg) 
{
	CButton* pButton;
	CRect rr;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	switch(pMsg->message) {
	case 0x201://LBUTTONDOWN
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_UP); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_PmacUser.SendStr(L"M1150=$5000");
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_DOWN); pButton->GetWindowRect(rr);	ScreenToClient(rr);
		if(rr.PtInRect(point)) {
			theApp.m_PmacUser.SendStr(L"M1150=$6000");
		}
		break;

	case 0x202:	//LBUTTONUP
		theApp.m_PmacUser.SendStr(L"M1150=0");
		break;
	}
	
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}
