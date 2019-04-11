// EvaluationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EvaluationDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SELF_DESTRUCT_GUID	_T("7955868C-4127-5C64-1A79-A47918C3DC2A")

/////////////////////////////////////////////////////////////////////////////
// CEvaluationDlg dialog

CEvaluationDlg::CEvaluationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvaluationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvaluationDlg)
	m_stringText = _T("");
	//}}AFX_DATA_INIT
	m_ullNeedsCode = -1;
	m_nSelfDestructDate=0;
	strcpy_s(m_MachineID,"");

	if(FindSelfDestructDate(SELF_DESTRUCT_GUID,&m_nSelfDestructDate)==false) {//no date found, must be a new install
		SetSelfDestructDate(0);
		SetMachineName(SELF_DESTRUCT_GUID);
	}

}


void CEvaluationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvaluationDlg)
	DDX_Control(pDX, IDC_EDIT_CODE, m_editCode);
	DDX_Control(pDX, IDC_EDIT_REC_WORD2, m_editWord2);
	DDX_Control(pDX, IDC_EDIT_REC_WORD1, m_editWord1);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_stringText);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DATETIMEPICKER_EVAL, m_datetimepickerEval);
}


BEGIN_MESSAGE_MAP(CEvaluationDlg, CDialog)
	//{{AFX_MSG_MAP(CEvaluationDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_EVAL, &CEvaluationDlg::OnDtnDatetimechangeDatetimepickerEval)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvaluationDlg message handlers

BOOL CEvaluationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strExpiryDate;
	strExpiryDate = *GetExpiryDate();

	if(IsCodeRequired() == true) {
		m_stringText.Format(_T("Your software license expired on\n%s\nPlease renew license"), strExpiryDate);
	}else{
		m_stringText.Format(_T("Your licence will expire on %s"),strExpiryDate);
	}
	
	m_datetimepickerEval.ShowWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEvaluationDlg::OnOK() 
{
	CString Buff;

	if(!CheckReceivingCode())
	{
		m_editWord1.SetWindowText(_T(""));
		m_editWord2.SetWindowText(_T(""));

		Buff.Format(_T("Incorrect Code Error: %04x"),m_nErrorCode);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);

	}

	CDialog::OnOK();

}





bool CEvaluationDlg::FindSelfDestructDate(CString GUID, time_t* pSelfDestructDate)
{
	CString	Key = L"SYSTEM\\CurrentControlSet\\Services\\" + GUID;
	CRegKey	RegKey;
	CString	Buff;
	ULONGLONG	ullTemp;
	WCHAR str[100];
	unsigned long	nLength;

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,Key,KEY_READ)) {
	
		if (!RegKey.QueryQWORDValue(L"68658469", ullTemp)) { //68-D 65-A 84-T 69-E
			*pSelfDestructDate = ullTemp;
		} else {
//			RegKey.Close();
//			return false; //If we dont find a date then we dont have a machine ID
		};

		if (!RegKey.QueryStringValue(L"ID", str, &(nLength = sizeof str))) {
			Buff.Format(_T("%s"), str);
			theApp.CStringToChar(Buff, m_MachineID);
		} else {
//			RegKey.Close(); //If we cant find a machine Id then this is an error condition
//			return false;
		};

		if (!RegKey.QueryQWORDValue(L"Needs code", m_ullNeedsCode)) { //68-D 65-A 84-T 69-E

		}
		else {
			RegKey.Close();
			return false; //If we dont find a date then we dont have a machine ID
		};

		RegKey.Close();
	} else {
		return false;
	};

	return true;
}



void CEvaluationDlg::SetMachineName(CString Name)
{
	CRegKey	RegKey;
	CRegKey SubKey;
	CString Buff;

	if (!RegKey.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services"), KEY_READ)) {

		if (!SubKey.Create(RegKey.m_hKey, SELF_DESTRUCT_GUID, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS)) {
			theApp.CharToCString(m_MachineID, Buff);
			SubKey.SetStringValue(Buff, _T("ID"));

			SubKey.Close();
		}
		RegKey.Close();
	}

}

CString *CEvaluationDlg::GetExpiryDate()
{
	static CString Buff;

	CTime ExpiryTime(m_nSelfDestructDate);
	Buff = ExpiryTime.Format(_T("%d %b %Y"));

	return &Buff;
}

bool CEvaluationDlg::IsCodeRequired()
{
	CRegKey	RegKey;
	CRegKey SubKey;
	CTime CurrentTime = CTime::GetCurrentTime();
	CTimeSpan TimeDiff(m_nSelfDestructDate - CurrentTime.GetTime());
	time_t nNumberDays = TimeDiff.GetDays();

	if (m_ullNeedsCode != 0) {
		return true;
	}

	if (nNumberDays <= 0) {
		if (!RegKey.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services"), KEY_READ | KEY_WRITE)) {

			if (!SubKey.Create(RegKey.m_hKey, SELF_DESTRUCT_GUID, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS)) {
				SubKey.SetQWORDValue(L"Needs code", 0x3445);
				SubKey.Close();
			}
			RegKey.Close();
		}
		return true;
	}
	return false;
}



void CEvaluationDlg::SetSelfDestructDate(time_t newdate)
{
	CRegKey	RegKey;
	CRegKey SubKey;
	CTime SelfDestructDate(newdate);

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services"), KEY_READ | KEY_WRITE)) {

		if (!SubKey.Create(RegKey.m_hKey, SELF_DESTRUCT_GUID, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS)) {
			SubKey.SetQWORDValue(L"68658469", SelfDestructDate.GetTime()); //68-D 65-A 84-T 69-E
			SubKey.SetQWORDValue(L"Needs code", 0); //68-D 65-A 84-T 69-E
			m_nSelfDestructDate = SelfDestructDate.GetTime();

			SubKey.Close();
		}
		RegKey.Close();
	}
}


int CEvaluationDlg::CheckReceivingCode()
{
	CString Buff;
	int Words[2];
	unsigned char *pBytes;

	m_nErrorCode=0;
	m_editWord1.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[0]);
	m_editWord2.GetWindowText(Buff);
	swscanf_s(Buff,_T("%d"),&Words[1]);

	pBytes = (unsigned char*)&Words;
	int nSum = 0;
	for(int ii=0;ii<4;ii++) {
		nSum += pBytes[ii];
	}
	if(nSum != Words[1]) {
		return m_nErrorCode = -1;
	}

	SetSelfDestructDate(Words[0]);
	return 1;
}

BOOL CEvaluationDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message) {
	case WM_KEYUP:
		m_bCtrlButton = false;
		m_bShiftButton = false;
		break;
	case WM_KEYDOWN:
		switch (pMsg->wParam) {
		case 16: m_bShiftButton = true;
			break;
		case 17: m_bCtrlButton = true;//Ctrl button down
			break;
		case 'd':
		case 'D':
			if ((m_bCtrlButton == true) && (m_bShiftButton == true)) {
				m_datetimepickerEval.ShowWindow(TRUE);
				return true;
			};
			break;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CEvaluationDlg::OnDtnDatetimechangeDatetimepickerEval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime timeSelected;
	CString Buff;

	m_datetimepickerEval.GetTime(timeSelected);
	SetSelfDestructDate(timeSelected.GetTime());

	unsigned char *pBytes = (unsigned char*)&timeSelected;
	int nSum = 0;
	for (int ii = 0; ii < 4; ii++) {
		nSum += pBytes[ii];
	}

	Buff.Format(L"%x", timeSelected.GetTime());
	m_editWord1.SetWindowTextW(Buff);
	Buff.Format(L"%d", nSum);
	m_editWord2.SetWindowTextW(Buff);


	*pResult = 0;
}
