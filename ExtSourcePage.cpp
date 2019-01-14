// ExtSourcePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ExtSourcePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtSourcePage property page

IMPLEMENT_DYNCREATE(CExtSourcePage, CPropertyPage)

CExtSourcePage::CExtSourcePage() : CPropertyPage(CExtSourcePage::IDD)
{
	//{{AFX_DATA_INIT(CExtSourcePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CExtSourcePage::~CExtSourcePage()
{
}

void CExtSourcePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtSourcePage)
	DDX_Control(pDX, IDC_EDIT_CURRENT_ADC_6, m_editADC6);
	DDX_Control(pDX, IDC_EDIT_CURRENT_ADC_5, m_editADC5);
	DDX_Control(pDX, IDC_COMBO_OUTPUT_MODE, m_comboOutputMode);
	DDX_Control(pDX, IDC_EDIT_CURRENT_ADC_4, m_editADC4);
	DDX_Control(pDX, IDC_EDIT_CURRENT_ADC_3, m_editADC3);
	DDX_Control(pDX, IDC_EDIT_CURRENT_ADC_2, m_editADC2);
	DDX_Control(pDX, IDC_EDIT_CURRENT_ADC_1, m_editADC1);
	DDX_Control(pDX, IDC_COMBO_ADC_4, m_comboInputRange4);
	DDX_Control(pDX, IDC_COMBO_ADC_3, m_comboInputRange3);
	DDX_Control(pDX, IDC_COMBO_ADC_2, m_comboInputRange2);
	DDX_Control(pDX, IDC_COMBO_ADC_1, m_comboInputRange1);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ADC_6, m_checkCollectADC6);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ADC_5, m_checkCollectADC5);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ADC_4, m_checkCollectADC4);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ADC_3, m_checkCollectADC3);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ADC_2, m_checkCollectADC2);
	DDX_Control(pDX, IDC_CHECK_COLLECT_ADC_1, m_checkCollectADC1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtSourcePage, CPropertyPage)
	//{{AFX_MSG_MAP(CExtSourcePage)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_1, OnSelchangeComboAdc1)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_MODE, OnSelchangeComboOutputMode)
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_2, OnSelchangeComboAdc2)
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_3, OnSelchangeComboAdc3)
	ON_CBN_SELCHANGE(IDC_COMBO_ADC_4, OnSelchangeComboAdc4)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ADC_1, OnCheckCollectAdc1)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ADC_2, OnCheckCollectAdc2)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ADC_3, OnCheckCollectAdc3)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ADC_4, OnCheckCollectAdc4)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ADC_5, OnCheckCollectAdc5)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_ADC_6, OnCheckCollectAdc6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtSourcePage message handlers

BOOL CExtSourcePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.Format(L"-10V - +10V");
	m_comboInputRange1.AddString(Buff);	m_comboInputRange2.AddString(Buff);	m_comboInputRange3.AddString(Buff);	m_comboInputRange4.AddString(Buff);
	Buff.Format(L"-5V - +5V");
	m_comboInputRange1.AddString(Buff);	m_comboInputRange2.AddString(Buff);	m_comboInputRange3.AddString(Buff);	m_comboInputRange4.AddString(Buff);
	Buff.Format(L"-2V5 - +2V5");
	m_comboInputRange1.AddString(Buff);	m_comboInputRange2.AddString(Buff);	m_comboInputRange3.AddString(Buff);	m_comboInputRange4.AddString(Buff);
	Buff.Format(L"-1V25 - +1V25");
	m_comboInputRange1.AddString(Buff);	m_comboInputRange2.AddString(Buff);	m_comboInputRange3.AddString(Buff);	m_comboInputRange4.AddString(Buff);

	m_comboOutputMode.AddString(L"Unsigned decimal");
	m_comboOutputMode.AddString(L"Signed decimal");
	m_comboOutputMode.AddString(L"Volts");
	m_comboOutputMode.AddString(L"%");


	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CExtSourcePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboInputRange1.SetCurSel(theApp.m_UtUser.m_Global.Ext.cGain[0]);
	m_comboInputRange2.SetCurSel(theApp.m_UtUser.m_Global.Ext.cGain[1]);
	m_comboInputRange3.SetCurSel(theApp.m_UtUser.m_Global.Ext.cGain[2]);
	m_comboInputRange4.SetCurSel(theApp.m_UtUser.m_Global.Ext.cGain[3]);

	m_checkCollectADC1.SetCheck(theApp.m_UtUser.m_Global.Ext.bCollect[0]);
	m_checkCollectADC2.SetCheck(theApp.m_UtUser.m_Global.Ext.bCollect[1]);
	m_checkCollectADC3.SetCheck(theApp.m_UtUser.m_Global.Ext.bCollect[2]);
	m_checkCollectADC4.SetCheck(theApp.m_UtUser.m_Global.Ext.bCollect[3]);
	m_checkCollectADC5.SetCheck(theApp.m_UtUser.m_Global.Ext.bCollect[4]);
	m_checkCollectADC6.SetCheck(theApp.m_UtUser.m_Global.Ext.bCollect[5]);

	m_comboOutputMode.SetCurSel(theApp.m_LastSettings.nExtSourceDisplayMode);

}

BOOL CExtSourcePage::OnSetActive() 
{
	theApp.m_DataTranslationHW.SetADCSingleValueMode();

	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CExtSourcePage::OnKillActive() 
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CExtSourcePage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	int nArray[8];
	int ii;
	double dRange[4] = {10.0, 5.0, 2.5, 1.25};
	double dValue;

	int nChannel = 0;
	for(ii=0;ii<6;ii++) {
		if(theApp.m_UtUser.m_Global.Ext.bCollect[ii] == (unsigned char)true) {
			
			if(ii<4) {
				nArray[ii] = theApp.m_DataTranslationHW.m_nExtADCArray[ii];
			} else {
				nArray[ii] = theApp.m_Si10User.ReadJoystick(ii-4) << 7;
			}

			switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
			case 0:	//Unsigned decimal
				Buff.Format(L"%d",nArray[ii]);
				break;
			case 1:	//Signed Decimal
				Buff.Format(L"%d",nArray[ii]-32768);
				break;
			case 2: //Volts;
				if(ii<4) {
					dValue = (double)(nArray[ii]-32768) * dRange[ theApp.m_UtUser.m_Global.Ext.cGain[ii] & 3 ] / 32768.0;
				} else {
					dValue = (double)(nArray[ii]) * 5.0 / 32768.0;
				}
				Buff.Format(L"%.02fV",dValue);
				break;
			case 3: //%
				dValue = (double)(nArray[ii]-32768) * 100.0 / 32768.0;
				Buff.Format(L"%%%.02f",dValue);
				break;
			}
			nChannel++;
		} else {
			Buff.Empty();
		}
		switch(ii) {
		case 0: m_editADC1.SetWindowText(Buff);
			break;
		case 1: m_editADC2.SetWindowText(Buff);
			break;
		case 2: m_editADC3.SetWindowText(Buff);
			break;
		case 3: m_editADC4.SetWindowText(Buff);
			break;
		case 4: m_editADC5.SetWindowText(Buff);
			break;
		case 5: m_editADC6.SetWindowText(Buff);
			break;
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}

void CExtSourcePage::OnSelchangeComboOutputMode() 
{
	theApp.m_LastSettings.nExtSourceDisplayMode = m_comboOutputMode.GetCurSel();
}

void CExtSourcePage::OnSelchangeComboAdc1() 
{
	theApp.m_UtUser.m_Global.Ext.cGain[0] = m_comboInputRange1.GetCurSel();
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnSelchangeComboAdc2() 
{
	theApp.m_UtUser.m_Global.Ext.cGain[1] = m_comboInputRange2.GetCurSel();
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnSelchangeComboAdc3() 
{
	theApp.m_UtUser.m_Global.Ext.cGain[2] = m_comboInputRange3.GetCurSel();
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnSelchangeComboAdc4() 
{
	theApp.m_UtUser.m_Global.Ext.cGain[3] = m_comboInputRange4.GetCurSel();
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::UpdateDataTranslationHardWare()
{
	KillTimer(1);
//	theApp.m_DataTranslationHW.StopADCConversion();
//	theApp.m_DataTranslationHW.StartADCContinuousMode();

	theApp.m_DataTranslationHW.StopADCConversion();
	theApp.m_DataTranslationHW.SetADCSingleValueMode();

	SetTimer(1,200,NULL);

}

void CExtSourcePage::OnCheckCollectAdc1() 
{
	m_checkCollectADC1.GetCheck() == 1 ? theApp.m_UtUser.m_Global.Ext.bCollect[0] = true :  theApp.m_UtUser.m_Global.Ext.bCollect[0] = false;
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnCheckCollectAdc2() 
{
	m_checkCollectADC2.GetCheck() == 1 ? theApp.m_UtUser.m_Global.Ext.bCollect[1] = true :  theApp.m_UtUser.m_Global.Ext.bCollect[1] = false;
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnCheckCollectAdc3() 
{
	m_checkCollectADC3.GetCheck() == 1 ? theApp.m_UtUser.m_Global.Ext.bCollect[2] = true :  theApp.m_UtUser.m_Global.Ext.bCollect[2] = false;
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnCheckCollectAdc4() 
{
	m_checkCollectADC4.GetCheck() == 1 ? theApp.m_UtUser.m_Global.Ext.bCollect[3] = true :  theApp.m_UtUser.m_Global.Ext.bCollect[3] = false;
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnCheckCollectAdc5() 
{
	m_checkCollectADC5.GetCheck() == 1 ? theApp.m_UtUser.m_Global.Ext.bCollect[4] = true :  theApp.m_UtUser.m_Global.Ext.bCollect[4] = false;
	UpdateDataTranslationHardWare();
}

void CExtSourcePage::OnCheckCollectAdc6() 
{
	m_checkCollectADC6.GetCheck() == 1 ? theApp.m_UtUser.m_Global.Ext.bCollect[5] = true :  theApp.m_UtUser.m_Global.Ext.bCollect[5] = false;
	UpdateDataTranslationHardWare();
}
