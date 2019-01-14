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

#define	SELF_DESTRUCT	_T("7955868C-4127-5C64-1A79-A47918C3DC2A")

/////////////////////////////////////////////////////////////////////////////
// CEvaluationDlg dialog

CEvaluationDlg::CEvaluationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvaluationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvaluationDlg)
	m_stringText = _T("");
	//}}AFX_DATA_INIT
	m_nSelfDestructDate=0;
	strcpy_s(m_MachineID,"");

	if(FindSelfDestructDate(SELF_DESTRUCT,&m_nSelfDestructDate)==false) {//no date found, must be a new install
		SetSelfDestructDate(SELF_DESTRUCT);
		SetMachineName(SELF_DESTRUCT);
	}
	
	m_nHowManyDaysLeft = GetHowManyDaysLeft();

	GetCodeForADate();

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
}


BEGIN_MESSAGE_MAP(CEvaluationDlg, CDialog)
	//{{AFX_MSG_MAP(CEvaluationDlg)
	ON_EN_CHANGE(IDC_EDIT_REC_WORD1, OnChangeEditRecWord1)
	ON_EN_CHANGE(IDC_EDIT_REC_WORD2, OnChangeEditRecWord2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvaluationDlg message handlers

BOOL CEvaluationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strExpiryDate;
	strExpiryDate = *GetExpiryDate();

	if(GetHowManyDaysLeft()<=0) {
		m_stringText.Format(_T("Your software license expired on\n%s\nPlease renew license"), strExpiryDate);
	}else{
		m_stringText.Format(_T("Your licence will expire on %s"),strExpiryDate);
	}
	
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





bool CEvaluationDlg::FindSelfDestructDate(CString Name, time_t* pSelfDestructDate)
{
	CString	Key = L"SYSTEM\\CurrentControlSet\\Services\\" + Name;
	CRegKey	RegKey;
	CString	Buff;
	ULONGLONG	ullTemp;
	WCHAR str[100];
	unsigned long	nLength;

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,Key,KEY_READ)) {
	
		if (!RegKey.QueryQWORDValue(L"68658469", ullTemp)) { //68-D 65-A 84-T 69-E
			*pSelfDestructDate = ullTemp;
		} else {
			RegKey.Close();
			return false; //If we dont find a date then we dont have a machine ID
		};

		Buff = _T("ID");
		if (!RegKey.QueryStringValue(Buff, str, &(nLength = sizeof str))) {
			Buff.Format(_T("%s"), str);
			theApp.CStringToChar(Buff, m_MachineID);
		} else {
			RegKey.Close(); //If we cant find a machine Id then this is an error condition
			return false;
		};

		RegKey.Close();
	} else {
		return false;
	};

	return true;
}



void CEvaluationDlg::SetMachineName(CString Name)
{
	CString	Key = L"SYSTEM\\CurrentControlSet\\Services\\" + Name;
	CRegKey	RegKey;
	CRegKey SubKey;
	CString Buff;

	if (!RegKey.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services"), KEY_READ)) {

		if (!SubKey.Create(RegKey.m_hKey, SELF_DESTRUCT, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS)) {
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

int CEvaluationDlg::GetHowManyDaysLeft()
{
	CTime CurrentTime = CTime::GetCurrentTime();
	CTimeSpan TimeDiff(m_nSelfDestructDate - CurrentTime.GetTime());

	time_t nNumberDays = TimeDiff.GetDays();
	return (int)nNumberDays;
}


void CEvaluationDlg::CreateSendingCode(CString *Code) //create code that is sent to USL
{
	
	CString dd;
	int i, nLoopCount;

	for(i=0;i<12;i++) { m_SendByte[i]=0; }

	//redo with a while
	nLoopCount = strlen(m_MachineID);
	if(nLoopCount>6) nLoopCount=6;
	for(i=0;i<nLoopCount;i++) {
		memcpy(&m_SendByte[i],m_MachineID+i,sizeof(BYTE));
	}

	m_SendByte[9] = m_nSelfDestructDate&0xff;
	m_SendByte[8] = (m_nSelfDestructDate>>8)&0xff;
	m_SendByte[7] = (m_nSelfDestructDate>>16)&0xff;
	m_SendByte[6] = (m_nSelfDestructDate>>24)&0xff;


	m_SendByte[10] = GetVersionNumber();
	m_SendByte[11] = 0; //encoding


	Code->Format(_T("%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x"),m_SendByte[0],m_SendByte[1],m_SendByte[2],m_SendByte[3],m_SendByte[4],
		m_SendByte[5],m_SendByte[6],m_SendByte[7],m_SendByte[8],m_SendByte[9],m_SendByte[10],m_SendByte[11]);

}



//Check code received from USL
int CEvaluationDlg::OldCheckReceivingCode()
{

	int nRequiredDateCode;
	int nTemp;
	int Words[6];
	BYTE TempBytes[8];
	CString Buff;
	int i,b;
	int nMonthsIncrease;

	if(CheckifEvalCode()) {
		SetSelfDestructDate(SELF_DESTRUCT,1);
		return true;
	}


	m_nErrorCode=0;

	m_editWord1.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[0]);
	m_editWord2.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[1]);
	m_editWord3.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[2]);
	m_editWord4.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[3]);
	m_editWord5.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[4]);
	m_editWord6.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[5]);

	//check date first
	nRequiredDateCode = (int)(m_nSelfDestructDate + 1000000);
	nTemp = Words[3] & 0xffff;
	nTemp = nTemp<<16;
	nTemp += Words[4];
	if(nTemp!=nRequiredDateCode)
		m_nErrorCode |= 0x02;


	//Check Software version number
	if( ((Words[5]>>8) ^ GetVersionNumber()) & 0xff)
		m_nErrorCode |= 0x08;


	//check
	if((!strcmp(m_MachineID,""))&&(m_nErrorCode==0x00)){
		//This machine has no machineID and the previous checks were ok
		//Go ahead and give this machine the ID sent in the received code

		for(i=0,b=0;i<3;i++,b+=2) {
			m_MachineID[b] = (Words[i]>>8) & 0xff;
			m_MachineID[b+1] = Words[i] & 0xff;
		}
		m_MachineID[b] = '\0';
		SetMachineName(SELF_DESTRUCT);
		
	}else{
		//This Machine has an ID

		TempBytes[0] = ReverseBits(m_SendByte[5]);
		TempBytes[1] = ReverseBits(m_SendByte[4]);
		TempBytes[2] = ReverseBits(m_SendByte[3]);
		TempBytes[3] = ReverseBits(m_SendByte[2]);
		TempBytes[4] = ReverseBits(m_SendByte[1]);
		TempBytes[5] = ReverseBits(m_SendByte[0]);


		TempBytes[2] = TempBytes[2] ^ (nRequiredDateCode>>24)&0xFF;
		TempBytes[3] = TempBytes[3] ^ (nRequiredDateCode>>16)&0xFF;
		TempBytes[4] = TempBytes[4] ^ (nRequiredDateCode>>8)&0xFF;
		TempBytes[5] = TempBytes[5] ^ nRequiredDateCode&0xFF;


		for(i=0, b=0; i<3; i++,b+=2){
			if( ((Words[i]>>8)^TempBytes[b])&0xff )
				m_nErrorCode |= 0x01;
			if( ((Words[i])^TempBytes[b+1])&0xff )
				m_nErrorCode |= 0x01;
		}//for
	}



	if(m_nErrorCode!=0)
		return false;

	nMonthsIncrease = (Words[5]&0xff) - (Words[4]&0xff);

	if(nMonthsIncrease<0) 
		nMonthsIncrease = ((Words[5]&0xff)+0x100) - (Words[4]&0xff);


	SetSelfDestructDate(SELF_DESTRUCT,nMonthsIncrease);

	return true;

}

BYTE CEvaluationDlg::ReverseBits(BYTE input)
{

	BYTE Output=0x00;

	for(int i=0;i<7;i++){
		Output |= ((input>>i)&0x1); 
		Output = Output<<1;
	}

	return Output;
}



BYTE CEvaluationDlg::GetVersionNumber()
{
	if(theApp.m_fVersion==4.08f) {
		return 0x08;
	}else if(theApp.m_fVersion==4.09f){
		return 0x09;	
	}else if(theApp.m_fVersion==4.10f){
		return 0x0a;	
	}else if(theApp.m_fVersion==4.11f){
		return 0x0b;	
	}else if(theApp.m_fVersion==4.12f){
		return 0x0c;
	}else if(theApp.m_fVersion==4.13f){
		return 0x0d;
	}else if(theApp.m_fVersion==4.14f){
		return 0x0e;
	}else if(theApp.m_fVersion==4.15f){
		return 0x0f;
	}else if(theApp.m_fVersion==4.16f){
		return 0x10;
	}else if(theApp.m_fVersion==4.17f){
		return 0x11;
	}else if(theApp.m_fVersion==4.18f){
		return 0x12;
	}else if(theApp.m_fVersion==4.19f){
		return 0x13;
	}else if(theApp.m_fVersion==4.20f){
		return 0x14;
	}else if(theApp.m_fVersion==4.21f){
		return 0x15;
	}else if(theApp.m_fVersion==4.22f){
		return 0x16;
	}else if(theApp.m_fVersion==4.23f){
		return 0x17;
	}else if(theApp.m_fVersion==4.24f){
		return 0x18;
	}else if(theApp.m_fVersion==4.25f){
		return 0x19;
	}else if(theApp.m_fVersion==4.26f){
		return 0x1a;
	}else if(theApp.m_fVersion==4.27f){
		return 0x1b;
	}else if(theApp.m_fVersion==4.28f){
		return 0x1c;
	}else if(theApp.m_fVersion==4.29f){
		return 0x1d;
	}else if(theApp.m_fVersion==4.30f){
		return 0x1e;
	}else if(theApp.m_fVersion==4.31f){
		return 0x1f;
	}else if(theApp.m_fVersion==4.32f){
		return 0x20;
	}else if(theApp.m_fVersion==4.33f){
		return 0x21;
	}else if(theApp.m_fVersion==4.34f){
		return 0x22;
	}else if(theApp.m_fVersion==4.35f){
		return 0x23;
	}else if(theApp.m_fVersion==4.36f){
		return 0x24;
	}else if(theApp.m_fVersion==4.37f){
		return 0x25;
	}else if(theApp.m_fVersion==4.38f){
		return 0x26;
	}else if(theApp.m_fVersion==4.39f){
		return 0x27;
	}else if(theApp.m_fVersion==4.40f){
		return 0x28;
	}else if(theApp.m_fVersion==4.41f){
		return 0x29;
	}else if(theApp.m_fVersion==4.42f){
		return 0x2a;
	}else if(theApp.m_fVersion==4.43f){
		return 0x2b;
	}else if(theApp.m_fVersion==4.44f){
		return 0x2c;
	}else if(theApp.m_fVersion==4.45f){
		return 0x2d;
	}else if(theApp.m_fVersion==4.46f){
		return 0x2e;
	}else if(theApp.m_fVersion==4.47f){
		return 0x2f;
	}else if(theApp.m_fVersion==4.48f){
		return 0x30;
	}else if(theApp.m_fVersion==4.49f){
		return 0x31;
	}else if(theApp.m_fVersion==4.50f){
		return 0x32;
	}else if(theApp.m_fVersion==4.51f){
		return 0x33;
	}else if(theApp.m_fVersion==4.52f){
		return 0x34;
	}else if(theApp.m_fVersion==4.53f){
		return 0x35;
	}else if(theApp.m_fVersion==4.54f){
		return 0x36;
	}else if(theApp.m_fVersion==4.55f){
		return 0x37;
	}else if(theApp.m_fVersion==4.56f){
		return 0x38;
	}else if(theApp.m_fVersion==4.57f){
		return 0x39;
	}else if(theApp.m_fVersion==4.58f){
		return 0x3a;
	}else if(theApp.m_fVersion==4.59f){
		return 0x3b;
	}else if(theApp.m_fVersion==4.60f){
		return 0x3c;
	}else if(theApp.m_fVersion==4.61f){
		return 0x3d;
	}else if(theApp.m_fVersion==4.62f){
		return 0x3d;
	}else if(theApp.m_fVersion==4.63f){
		return 0x3e;
	}else if(theApp.m_fVersion==4.64f){
		return 0x3f;
	}else if(theApp.m_fVersion==4.65f){
		return 0x40;
	}else if(theApp.m_fVersion==4.66f){
		return 0x41;
	}else if(theApp.m_fVersion==4.67f){
		return 0x42;
	}else if(theApp.m_fVersion==4.68f){
		return 0x43;
	}else if(theApp.m_fVersion==4.70f){
		return 0x44;
	}else{
		return 0;
	}


	return 0;
}

void CEvaluationDlg::OnChangeEditRecWord1() 
{
	CString Buff;

	m_editWord1.GetWindowText(Buff);

	if(Buff.GetLength()>8) {
		Buff.Delete(8,Buff.GetLength()-8);
		m_editWord1.SetWindowText(Buff);
	}else if(Buff.GetLength()==8) {
		GetDlgItem(IDC_EDIT_REC_WORD2)->SetFocus();
	}

}

void CEvaluationDlg::OnChangeEditRecWord2() 
{

}

void CEvaluationDlg::OnChangeEditRecWord3() 
{
	CString Buff;

	m_editWord3.GetWindowText(Buff);

	if(Buff.GetLength()>4) {
		Buff.Delete(4,Buff.GetLength()-4);
		m_editWord3.SetWindowText(Buff);
	}else if(Buff.GetLength()==4) {
		GetDlgItem(IDC_EDIT_REC_WORD4)->SetFocus();
	}
	
}

void CEvaluationDlg::OnChangeEditRecWord4() 
{
	CString Buff;

	m_editWord4.GetWindowText(Buff);

	if(Buff.GetLength()>4) {
		Buff.Delete(4,Buff.GetLength()-4);
		m_editWord4.SetWindowText(Buff);
	}else if(Buff.GetLength()==4) {
		GetDlgItem(IDC_EDIT_REC_WORD5)->SetFocus();
	}
	
}

void CEvaluationDlg::OnChangeEditRecWord5() 
{
	CString Buff;

	m_editWord5.GetWindowText(Buff);

	if(Buff.GetLength()>4) {
		Buff.Delete(4,Buff.GetLength()-4);
		m_editWord5.SetWindowText(Buff);
	}else if(Buff.GetLength()==4) {
		GetDlgItem(IDC_EDIT_REC_WORD6)->SetFocus();
	}
	
}

void CEvaluationDlg::OnChangeEditRecWord6() 
{
	CString Buff;

	m_editWord6.GetWindowText(Buff);

	if(Buff.GetLength()>4) {
		Buff.Delete(4,Buff.GetLength()-4);
		m_editWord6.SetWindowText(Buff);
	}
	
}

void CEvaluationDlg::SetSelfDestructDate(CString Name, int nDateTime)
{
	CString	Key = L"SYSTEM\\CurrentControlSet\\Services\\" + Name;
	CRegKey	RegKey;
	CRegKey SubKey;
	CTime SelfDestructDate(nDateTime);

	if(!RegKey.Open(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Services"), KEY_READ | KEY_WRITE)) {

		if (!SubKey.Create(RegKey.m_hKey, SELF_DESTRUCT, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS)) {
			SubKey.SetQWORDValue(L"68658469", SelfDestructDate.GetTime()); //68-D 65-A 84-T 69-E
			m_nSelfDestructDate = SelfDestructDate.GetTime();

			SubKey.Close();
		}
		RegKey.Close();
	}
}

bool CEvaluationDlg::CheckifEvalCode()
{
	int Words[6];
	CString Buff;

	m_editWord1.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[0]);
	m_editWord2.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&Words[1]);

	if( (Words[0]==0xA0A0) && (Words[1]==0xB0B0))
		return true;

	return false;

}

int CEvaluationDlg::CheckReceivingCode()
{
	CString Buff;
	int Words[2];
	unsigned char *pBytes;

	if(CheckifEvalCode()) {
		SetSelfDestructDate(SELF_DESTRUCT,1);
		return true;
	}

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

	SetSelfDestructDate(SELF_DESTRUCT,Words[0]);
	return 1;
}

void CEvaluationDlg::GetCodeForADate()
{
	CString Buff, Date;
	CTime CurrentTime = CTime::GetCurrentTime();

	time_t osTestDateTime=CurrentTime.GetTime() + 2592000;
	CTime NewTime(osTestDateTime);

	unsigned char *pBytes = (unsigned char*)&osTestDateTime;
	int nSum = 0;
	for(int ii=0;ii<4;ii++) {
		nSum += pBytes[ii];
	}

	Date = NewTime.Format(_T("%d %b %Y"));

	Buff.Format(L"%s %x %d", Date , osTestDateTime, nSum);
}
