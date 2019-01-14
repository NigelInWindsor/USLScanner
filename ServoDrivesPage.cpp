// ServoDrivesPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ServoDrivesPage.h"
#include "SiUSBXp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServoDrivesPage property page

IMPLEMENT_DYNCREATE(CServoDrivesPage, CPropertyPage)

CServoDrivesPage::CServoDrivesPage() : CPropertyPage(CServoDrivesPage::IDD)
{
	//{{AFX_DATA_INIT(CServoDrivesPage)
	//}}AFX_DATA_INIT
	
	m_nShelf = 0;
	m_nWhichDevice = 0;
	m_hDevice = NULL;
	InitializeDiagnostics();
}

CServoDrivesPage::~CServoDrivesPage()
{
	if(m_hDevice) {
		SI_Close(m_hDevice);
		m_hDevice=0;
	}
}

void CServoDrivesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServoDrivesPage)
	DDX_Control(pDX, IDC_COMBO_AMP_TYPE_3, m_comboAmpType3);
	DDX_Control(pDX, IDC_COMBO_AMP_TYPE_2, m_comboAmpType2);
	DDX_Control(pDX, IDC_COMBO_AMP_TYPE_1, m_comboAmpType1);
	DDX_Control(pDX, IDC_EDIT_TEMPERATURE, m_editTemperature);
	DDX_Control(pDX, IDC_EDIT_SCANNER_5V, m_editScanner5V);
	DDX_Control(pDX, IDC_EDIT_SCANNER_12V, m_editScanner12V);
	DDX_Control(pDX, IDC_EDIT_CONTROLLER_LOGIC, m_editController5V);
	DDX_Control(pDX, IDC_EDIT_P_OP_3, m_editPOP3);
	DDX_Control(pDX, IDC_EDIT_P_OP_2, m_editPOP2);
	DDX_Control(pDX, IDC_EDIT_P_OP_1, m_editPOP1);
	DDX_Control(pDX, IDC_EDIT_N_OP_3, m_editNOP3);
	DDX_Control(pDX, IDC_EDIT_N_OP_2, m_editNOP2);
	DDX_Control(pDX, IDC_EDIT_N_OP_1, m_editNOP1);
	DDX_Control(pDX, IDC_EDIT_P_DRIVE_AMP_3, m_editPDriveAmp3);
	DDX_Control(pDX, IDC_EDIT_P_DRIVE_AMP_2, m_editPDriveAmp2);
	DDX_Control(pDX, IDC_EDIT_N_DRIVE_AMP_3, m_editNDriveAmp3);
	DDX_Control(pDX, IDC_EDIT_N_DRIVE_AMP_2, m_editNDriveAmp2);
	DDX_Control(pDX, IDC_EDIT_HT_AMP_3, m_editHTAmp3);
	DDX_Control(pDX, IDC_EDIT_HT_AMP_2, m_editHTAmp2);
	DDX_Control(pDX, IDC_EDIT_FAULT_3, m_editFaultAmp3);
	DDX_Control(pDX, IDC_EDIT_FAULT_2, m_editFaultAmp2);
	DDX_Control(pDX, IDC_EDIT_CURRENT_AMP_3, m_editCurrentAmp3);
	DDX_Control(pDX, IDC_EDIT_CURRENT_AMP_2, m_editCurrentAmp2);
	DDX_Control(pDX, IDC_EDIT_P_DRIVE_AMP_1, m_editPDriveAmp1);
	DDX_Control(pDX, IDC_EDIT_N_DRIVE_AMP_1, m_editNDriveAmp1);
	DDX_Control(pDX, IDC_EDIT_HT_AMP_1, m_editHTAmp1);
	DDX_Control(pDX, IDC_EDIT_FAULT_1, m_editFaultAmp1);
	DDX_Control(pDX, IDC_EDIT_CURRENT_AMP_1, m_editCurrentAmp1);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editAddress);
	DDX_Control(pDX, IDC_EDIT_DATE, m_editDate);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_comboDrive);
	DDX_Control(pDX, IDC_EDIT_MAINS, m_editMains);
	DDX_Control(pDX, IDC_EDIT_ENABLE_IN_3, m_editEnable3);
	DDX_Control(pDX, IDC_EDIT_ENABLE_IN_2, m_editEnable2);
	DDX_Control(pDX, IDC_EDIT_ENABLE_IN_1, m_editEnable1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServoDrivesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CServoDrivesPage)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE, OnSelchangeComboDrive)
	ON_CBN_SELCHANGE(IDC_COMBO_AMP_TYPE_1, OnSelchangeComboAmpType1)
	ON_CBN_SELCHANGE(IDC_COMBO_AMP_TYPE_2, OnSelchangeComboAmpType2)
	ON_CBN_SELCHANGE(IDC_COMBO_AMP_TYPE_3, OnSelchangeComboAmpType3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServoDrivesPage message handlers

BOOL CServoDrivesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	for(int ii=0;ii<m_nNumDevices;ii++) {
		m_comboDrive.AddString(m_DeviceNames.GetAt(ii));
	}
	m_comboDrive.SetCurSel(m_nWhichDevice);

	if(m_nNumDevices) {
		SI_STATUS status = SI_Open(m_nWhichDevice, &m_hDevice);
	}
	RequestIDData(&m_strName, &m_strAddress, &m_strDateTime);
	
	m_comboAmpType1.AddString(_T("TFMxx1-06-"));
	m_comboAmpType1.AddString(_T("TFMxx1-12-"));
	m_comboAmpType1.AddString(_T("TFMxx1-18-"));
	m_comboAmpType2.AddString(_T("TFMxx1-06-"));
	m_comboAmpType2.AddString(_T("TFMxx1-12-"));
	m_comboAmpType2.AddString(_T("TFMxx1-18-"));
	m_comboAmpType3.AddString(_T("TFMxx1-06-"));
	m_comboAmpType3.AddString(_T("TFMxx1-12-"));
	m_comboAmpType3.AddString(_T("TFMxx1-18-"));

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CServoDrivesPage::OnSetActive() 
{
	
	SetTimer(1,1000,NULL);
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CServoDrivesPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CServoDrivesPage::OnTimer(UINT nIDEvent) 
{
	
	CString Buff,strDisabled,strEnabled,strMains,strOk,strFault,strCurrent,strHT,strPDrive,strNDrive,strPOP,strNOP;
	int nHT,nCurrent,nPDrive,nNDrive,nPOP,nNOP;
	float fHT,fCurrent,fPDrive,fNDrive,fPOP,fNOP;
	COLORREF GREEN = RGB(0,100,0);
	COLORREF RED = RGB(200,0,0);
	COLORREF BLACK = RGB(0,0,0);
	COLORREF BLUE = RGB(0,0,200);
	float fCurrentMult[3] = { 1.54f,3.03f,4.55f };

	strDisabled.LoadString(IDS_Disabled);
	strEnabled.LoadString(IDS_Enabled);
	strMains.LoadString(IDS_Mains);
	strOk.LoadString(IDS_Ok);
	strFault.LoadString(IDS_Fault);
	strCurrent.LoadString(IDS_Current);
	strHT.LoadString(IDS_HT);
	strPDrive.LoadString(IDS_P_Drive);
	strNDrive.LoadString(IDS_N_Drive);
	strPOP.LoadString(IDS_POP);
	strNOP.LoadString(IDS_NOP);

	m_editName.SetWindowText(L"Name: " + m_strName);
	m_editAddress.SetWindowText(L"Address: " + m_strAddress);
	m_editDate.SetWindowText(L"Date: " + m_strDateTime);

	int nOldShelf = m_nShelf;
	m_nShelf = _ttoi(m_strAddress);
	m_nShelf &= 7;
	if(nOldShelf - m_nShelf) {
		UpdateAllControls();
	}

	RequestConditionData();

	//Globals
	float fScanner12V = (float)((m_cCondition[22] << 2) + ((m_cCondition[35] >> 0) & 0x03)) * 19.2e-3f;
	float fScanner5V = (float)((m_cCondition[23] << 2) + ((m_cCondition[35] >> 2) & 0x03)) * 7.5e-3f;
	float fController5V = (float)((m_cCondition[24] << 2) + ((m_cCondition[35] >> 4) & 0x03)) * 7.5e-3f;
	float fTemperature = (float)((m_cCondition[26] << 2) + ((m_cCondition[34] >> 4) & 0x03)) * 1e-1f;

	m_cCondition[3] & 0x80 ? m_editMains.SetTextColour(strMains,RED) : m_editMains.SetTextColour(strMains,GREEN);
	Buff.Format(_T("Scanner 12V = %.01f V"),fScanner12V);
	fabsf(fScanner12V - 12.0f) < 1.0f ? m_editScanner12V.SetTextColour(Buff,GREEN) : m_editScanner12V.SetTextColour(Buff,RED);
	Buff.Format(_T("Scanner 5V = %.01f V"),fScanner5V);
	fabsf(fScanner5V - 5.0f) < 0.30f ? m_editScanner5V.SetTextColour(Buff,GREEN) : m_editScanner5V.SetTextColour(Buff,RED);
	Buff.Format(_T("Controller 5V = %.01f V"),fController5V);
	fabsf(fController5V - 5.0f) < 0.25f ? m_editController5V.SetTextColour(Buff,GREEN) : m_editController5V.SetTextColour(Buff,RED);
	Buff.Format(_T("Temperature = %.01f C"),fTemperature);
	m_editTemperature.SetWindowText(Buff);

	//Amp 1
	nCurrent =	(m_cCondition[4] << 2) + ((m_cCondition[27] >> 0) & 0x03);
	nHT =		(m_cCondition[7] << 2) + ((m_cCondition[28] >> 0) & 0x03);
	nPDrive =	(m_cCondition[16] << 2) + ((m_cCondition[32] >> 0) & 0x03);
	nNDrive =	(m_cCondition[17] << 2) + ((m_cCondition[32] >> 2) & 0x03);
	nPOP =		(m_cCondition[10] << 2) + ((m_cCondition[29] >> 0) & 0x03);
	nNOP =		(m_cCondition[11] << 2) + ((m_cCondition[29] >> 2) & 0x03);
	fHT = (float)nHT * 255.0e-3f;
	fCurrent = (float)(nCurrent - 480) * 10.9e-3f * fCurrentMult[theApp.m_Tank.nAmpType[3 * m_nShelf + 0]];
	fPDrive = (float)(nPDrive - 480) * 30.3e-3f;
	fNDrive = (float)(nNDrive - 480) * 30.3e-3f;
	fPOP = (float)nPOP * 255.0e-3f;
	fNOP = (float)nNOP * 255.0e-3f;
//	Buff.Format(_T("%02x"),m_cCondition[3]);
//	m_editEnable1.SetTextColour(Buff,BLACK);
	m_cCondition[3] & 0x08  ? m_editEnable1.SetTextColour(strDisabled,BLACK) : m_editEnable1.SetTextColour(strEnabled,BLUE);
	m_cCondition[3] & 0x01  ? m_editFaultAmp1.SetTextColour(strOk,GREEN) : m_editFaultAmp1.SetTextColour(strFault,RED);
	Buff.Format(_T("%s %.02f A"),strCurrent,fCurrent);	m_editCurrentAmp1.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strHT, fHT);			m_editHTAmp1.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strPDrive, fPDrive);	m_editPDriveAmp1.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strNDrive, fNDrive);	m_editNDriveAmp1.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strPOP, fPOP);			m_editPOP1.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strNOP, fNOP);			m_editNOP1.SetTextColour(Buff,BLACK);

	//Amp 2
	nCurrent = (m_cCondition[5] << 2) + ((m_cCondition[27] >> 2) & 0x03);
	nHT =      (m_cCondition[8] << 2) + ((m_cCondition[28] >> 2) & 0x03);
	nPDrive =  (m_cCondition[18] << 2) + ((m_cCondition[33] >> 0) & 0x03);
	nNDrive =  (m_cCondition[19] << 2) + ((m_cCondition[33] >> 2) & 0x03);
	nPOP =		(m_cCondition[12] << 2) + ((m_cCondition[30] >> 0) & 0x03);
	nNOP =		(m_cCondition[13] << 2) + ((m_cCondition[30] >> 2) & 0x03);
	fHT = (float)nHT * 255.0e-3f;
	fCurrent = (float)(nCurrent - 480) * 10.9e-3f * fCurrentMult[theApp.m_Tank.nAmpType[3 * m_nShelf + 1]];
	fPDrive = (float)(nPDrive - 480) * 30.3e-3f;
	fNDrive = (float)(nNDrive - 480) * 30.3e-3f;
	fPOP = (float)nPOP * 255.0e-3f;
	fNOP = (float)nNOP * 255.0e-3f;
	m_cCondition[3] & 0x10  ? m_editEnable2.SetTextColour(strDisabled,BLACK) : m_editEnable2.SetTextColour(strEnabled,BLUE);
	m_cCondition[3] & 0x02  ? m_editFaultAmp2.SetTextColour(strOk,GREEN) : m_editFaultAmp2.SetTextColour(strFault,RED);
	Buff.Format(_T("%s %.02f A"),strCurrent,fCurrent);	m_editCurrentAmp2.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strHT, fHT);			m_editHTAmp2.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strPDrive, fPDrive);	m_editPDriveAmp2.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strNDrive, fNDrive);	m_editNDriveAmp2.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strPOP, fPOP);			m_editPOP2.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strNOP, fNOP);			m_editNOP2.SetTextColour(Buff,BLACK);

	//Amp 3
	nCurrent = (m_cCondition[6] << 2) + ((m_cCondition[27] >> 4) & 0x03);
	nHT =      (m_cCondition[9] << 2) + ((m_cCondition[28] >> 4) & 0x03);
	nPDrive =  (m_cCondition[20] << 2) + ((m_cCondition[34] >> 0) & 0x03);
	nNDrive =  (m_cCondition[21] << 2) + ((m_cCondition[34] >> 2) & 0x03);
	nPOP =		(m_cCondition[14] << 2) + ((m_cCondition[31] >> 0) & 0x03);
	nNOP =		(m_cCondition[15] << 2) + ((m_cCondition[31] >> 2) & 0x03);
	fHT = (float)nHT * 255.0e-3f;
	fCurrent = (float)(nCurrent - 480) * 10.9e-3f * fCurrentMult[theApp.m_Tank.nAmpType[3 * m_nShelf + 2]];
	fPDrive = (float)(nPDrive - 480) * 30.3e-3f;
	fNDrive = (float)(nNDrive - 480) * 30.3e-3f;
	fPOP = (float)nPOP * 255.0e-3f;
	fNOP = (float)nNOP * 255.0e-3f;
	m_cCondition[3] & 0x20  ? m_editEnable3.SetTextColour(strDisabled,BLACK) : m_editEnable3.SetTextColour(strEnabled,BLUE);
	m_cCondition[3] & 0x04  ? m_editFaultAmp3.SetTextColour(strOk,GREEN) : m_editFaultAmp3.SetTextColour(strFault,RED);
	Buff.Format(_T("%s %.02f A"),strCurrent,fCurrent);	m_editCurrentAmp3.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strHT, fHT);			m_editHTAmp3.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strPDrive, fPDrive);	m_editPDriveAmp3.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strNDrive, fNDrive);	m_editNDriveAmp3.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strPOP, fPOP);			m_editPOP3.SetTextColour(Buff,BLACK);
	Buff.Format(_T("%s %.01f V"),strNOP, fNOP);			m_editNOP3.SetTextColour(Buff,BLACK);

	CPropertyPage::OnTimer(nIDEvent);
}




void CServoDrivesPage::InitializeDiagnostics()
{
	SI_DEVICE_STRING	devStr;
	
	m_DeviceNames.RemoveAll();
	m_nNumDevices = 0;

	if(	m_hDevice ) {
		SI_Close(m_hDevice);
		m_hDevice=0;
	}

	SI_STATUS status = SI_GetNumDevices((DWORD*)&m_nNumDevices);

	if (status == SI_SUCCESS) {
		for (DWORD d = 0; d < (DWORD)m_nNumDevices; d++) {
			status = SI_GetProductString(d, devStr, SI_RETURN_SERIAL_NUMBER);

			if (status == SI_SUCCESS) {

				if(SI_Open(d, &m_hDevice) == SI_SUCCESS) {

					RequestIDData(&m_strName, &m_strAddress, &m_strDateTime);
					m_DeviceNames.Add(L"Shelf:" + m_strAddress);

					SI_Close(m_hDevice);
					m_hDevice=0;
				} else {
					m_DeviceNames.Add(L"Failed to get address");
				}

			} else {
				m_DeviceNames.Add(L"Failed to product name");
			}	
		}
	}

}

void CServoDrivesPage::OnSelchangeComboDrive() 
{
	int nDevice;
	nDevice=m_comboDrive.GetCurSel();

	if(m_nWhichDevice - nDevice) {
		m_nWhichDevice = nDevice;
		UpdateAllControls();

		if(m_hDevice) {
			SI_Close(m_hDevice);
			m_hDevice=0;
		}

		if(m_nNumDevices) {
			SI_STATUS status = SI_Open(m_nWhichDevice, &m_hDevice);
		}
		RequestIDData(&m_strName, &m_strAddress, &m_strDateTime);
	}	
}

bool CServoDrivesPage::RequestIDData(CString *Name, CString *Address, CString *DateTime)
{
	char Command[2] = {1,1};
	char ReadArray[256];
	DWORD dwNumWritten,dwNumRead;

	dwNumWritten = 0;
	dwNumRead = 0;

	Name->Empty();
	Address->Empty();
	DateTime->Empty();
	if(m_hDevice) {
		SI_STATUS status = SI_Write(m_hDevice, Command, 2, &dwNumWritten);

		if(status == SI_SUCCESS) {
			ZeroMemory(ReadArray, sizeof ReadArray);
			/*
			status = SI_Read(m_hDevice, ReadArray, sizeof ReadArray, &dwNumRead);
			if(status == SI_SUCCESS) {
				if((ReadArray[0]==1) && (ReadArray[1]==1)) {
					cNumChar = ReadArray[2];
					
					pUnicode = (_TCHAR*)&ReadArray[5];

					for(ii=0,jj=0;ii<cNumChar && pUnicode[ii]!= '&';ii++,jj++) {};
					Name->Format(_T("%*.*s"),jj,jj, pUnicode);
					pUnicode += (jj + 2);

					for(ii=0,jj=0;ii<cNumChar && pUnicode[ii]!= '&';ii++,jj++) {};
					Address->Format(_T("%*.*s"),jj,jj, pUnicode);
					pUnicode += (jj + 2);

					for(ii=0,jj=0;ii<cNumChar && pUnicode[ii]!= '&';ii++,jj++) {};
					DateTime->Format(_T("%*.*s"),jj,jj, pUnicode);
					pUnicode += (jj + 2);

					return true;
				}
			}
			*/
		}
	}
	return false;

}

bool CServoDrivesPage::RequestConditionData()
{
	char Command[2] = {2,1};
	unsigned char cNumChar;
	char ReadArray[256];
	DWORD dwNumWritten,dwNumRead;

	if(m_hDevice) {
		SI_STATUS status = SI_Write(m_hDevice, Command, 2, &dwNumWritten);

		if(status == SI_SUCCESS) {
			status = SI_Read(m_hDevice, ReadArray, sizeof ReadArray, &dwNumRead);

			if(status == SI_SUCCESS) {
				if((ReadArray[0]==2) && (ReadArray[1]==1)) {
					cNumChar = ReadArray[2];
					
					CopyMemory(m_cCondition,ReadArray,cNumChar-3);

					return true;
				}
			}
		}
	}
	return false;

}

void CServoDrivesPage::OnSelchangeComboAmpType1() 
{
	
	
	theApp.m_Tank.nAmpType[3 * m_nShelf + 0] = m_comboAmpType1.GetCurSel();
	theApp.SaveTank();
}

void CServoDrivesPage::OnSelchangeComboAmpType2() 
{
	
	
	theApp.m_Tank.nAmpType[3 * m_nShelf + 1] = m_comboAmpType2.GetCurSel();
	theApp.SaveTank();
}

void CServoDrivesPage::OnSelchangeComboAmpType3() 
{
	
	
	theApp.m_Tank.nAmpType[3 * m_nShelf + 2] = m_comboAmpType3.GetCurSel();
	theApp.SaveTank();
}

void CServoDrivesPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	

	m_comboAmpType1.SetCurSel(theApp.m_Tank.nAmpType[3 * m_nShelf + 0]);
	m_comboAmpType2.SetCurSel(theApp.m_Tank.nAmpType[3 * m_nShelf + 1]);
	m_comboAmpType3.SetCurSel(theApp.m_Tank.nAmpType[3 * m_nShelf + 2]);



}
