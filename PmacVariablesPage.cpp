// PmacVariablesPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PmacVariablesPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacVariablesPage property page

IMPLEMENT_DYNCREATE(CPmacVariablesPage, CResizablePage)

CPmacVariablesPage::CPmacVariablesPage(int nVariableType) : CResizablePage(CPmacVariablesPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacVariablesPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pEditSpinItem=NULL;
	m_nVariableType = nVariableType;
	switch(m_nVariableType) {
	case 'I': m_nTop = 100;
		m_Variable = "I";
		break;
	case 'M': m_nTop=0;
		m_Variable = 'M';
		break;
	}
}

CPmacVariablesPage::~CPmacVariablesPage()
{
}

void CPmacVariablesPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacVariablesPage)
	DDX_Control(pDX, IDC_EDIT_SWAP, m_editSwap);
	DDX_Control(pDX, IDC_EDIT_JUMPTO, m_editJumpTo);
	DDX_Control(pDX, IDC_LIST_VARIABLES, m_listVariables);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacVariablesPage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacVariablesPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_VARIABLES, OnGetdispinfoListVariables)
	ON_BN_CLICKED(IDC_BUTTON_PLUS100, OnButtonPlus100)
	ON_BN_CLICKED(IDC_BUTTON_MINUS100, OnButtonMinus100)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VARIABLES, OnDblclkListVariables)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VARIABLES, OnClickListVariables)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SWAP, OnButtonSwap)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,UpdatePmacFromList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacVariablesPage message handlers

BOOL CPmacVariablesPage::OnInitDialog() 
{
	CString Buff;
	CResizablePage::OnInitDialog();
	
	m_StaticPosVariables.Initialize(this, m_listVariables, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosVariables.SetNewPos();

	Buff.Format(L"100 200");
	m_editSwap.SetWindowText(Buff);

	
	UpdateAllControls();
	CreateColumns();
	FillList();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPmacVariablesPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPmacVariablesPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		m_StaticPosVariables.SetNewPos();
	}
		
}

void CPmacVariablesPage::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),m_nTop);
	m_editJumpTo.SetWindowText(Buff);
}

void CPmacVariablesPage::CreateColumns()
{
	CSize ll;
	CString Buff;
	CString ColumnName[12] = { L"#",L"Description", L"Value"};
	int ColumnWidth[12] = { 30,200,100};
	CDC* pDC = GetDC();

	for (int ii=0;ii<3;ii++) {
		m_listVariables.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}
	
	ReleaseDC(pDC);

}

void CPmacVariablesPage::FillList()
{
	CString	Buff,rxBuff;
	char Temp[20];
	int	ii,jj,kk,nRxLength;

	InitializeStrings();
	theApp.m_PmacUser.RequestMultipleVariables(m_Variable,m_nTop,100+m_nTop,rxBuff);
	nRxLength = rxBuff.GetLength();

	m_listVariables.DeleteAllItems();

	for (ii=0,jj=0;ii<100;ii++) {
		Buff.Format(_T("%d"),ii+m_nTop);
		m_listVariables.InsertItem(ii, Buff.GetBuffer(255), ii);
		ZeroMemory(Temp,sizeof Temp);
		if(jj<nRxLength) {
			for(jj,kk=0;(rxBuff.GetAt(jj)!=13) && (kk<20); ) {
				if(isprint(rxBuff.GetAt(jj))) {
					Temp[kk++]=(char)rxBuff.GetAt(jj);
				}
				if(jj<nRxLength) {
					jj++;
					if(jj==nRxLength) break;
				} else {
					break;
				}
			}
		}
		m_strValue[ii] = Temp;
		jj++;
	};

	
	m_listVariables.EnsureVisible(0,FALSE);
	m_listVariables.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listVariables.SetFocus();
	m_listVariables.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


}


void CPmacVariablesPage::InitializeStrings()
{
	int	ii,jj,kk;
	char Temp[60];
	CString Buff;
	int	nTopI;
	int nRxStrLength;

	if(theApp.m_PmacUser.m_nPmacType==PMAC1) {
		nTopI = 900;
	} else {
		nTopI = 3300;
	}

	for(ii=0;ii<100;ii++) {
		m_strDescription[ii].Empty();
	}

	if(m_Variable=="I") {
		switch(theApp.m_PmacUser.m_nPmacType) {
		default:
			if(m_nTop<100) {
				m_strDescription[0]="Reserved";
				m_strDescription[1]="Serial Port Mode";
				m_strDescription[2]="Control Panel Disable";
				m_strDescription[3]="I/O Handshake Control";
				m_strDescription[4]="Communication Integrity Mode";
				m_strDescription[5]="Plc Programs On / Off";
				m_strDescription[6]="Error Reporting Mode";
				m_strDescription[7]="Reserved";
				m_strDescription[8]="Real Time Interrupt Period";
				m_strDescription[9]="Full/Abrevited Listing";
				m_strDescription[10]="Servo Interrupt Time";
				m_strDescription[11]="Program Move Calcultation Time";
				m_strDescription[12]="Jog to Pos Calc time";
				m_strDescription[13]="Program Move Segmentation Time";
				m_strDescription[14]="Position match on run enable";
				m_strDescription[15]="Degrees / Radians";
				m_strDescription[16]="Rotary Buffer request on point";
				m_strDescription[17]="Rotary Buffer request off point";
				m_strDescription[18]="Fixed buffer full warning point";
				m_strDescription[19]="Data gathering period";
				m_strDescription[20]="Data gathering selection mask";
				m_strDescription[21]="Data gathering source 1 Address";
				m_strDescription[22]="Data gathering source 2 Address";
				m_strDescription[23]="Data gathering source 3 Address";
				m_strDescription[24]="Data gathering source 4 Address";
				m_strDescription[25]="Data gathering source 5 Address";
				m_strDescription[26]="Data gathering source 6 Address";
				m_strDescription[27]="Data gathering source 7 Address";
				m_strDescription[28]="Data gathering source 8 Address";
				m_strDescription[29]="Data gathering source 9 Address";
				m_strDescription[30]="Data gathering source 10 Address";
				m_strDescription[31]="Data gathering source 11 Address";
				m_strDescription[32]="Data gathering source 12 Address";
				m_strDescription[33]="Data gathering source 13 Address";
				m_strDescription[34]="Data gathering source 14 Address";
				m_strDescription[35]="Data gathering source 15 Address";
				m_strDescription[36]="Data gathering source 16 Address";
				m_strDescription[37]="Data gathering source 17 Address";
				m_strDescription[38]="Data gathering source 18 Address";
				m_strDescription[39]="Data gathering source 19 Address";
				m_strDescription[40]="Data gathering source 20 Address";
				m_strDescription[41]="Data gathering source 21 Address";
				m_strDescription[42]="Data gathering source 22 Address";
				m_strDescription[43]="Data gathering source 23 Address";
				m_strDescription[44]="Data gathering source 24 Address";
				m_strDescription[45]="Data gathering Buffer location";
				m_strDescription[46]="Reserved";
				m_strDescription[47]="Address of pointer for CNTRL-W";
				m_strDescription[48]="Reservrd";
				m_strDescription[49]="Reserved";
				m_strDescription[50]="Rapid Mode Move";
				m_strDescription[51]="Leadscrew compensation enable";
			}
			if((m_nTop>=100) && (m_nTop<nTopI)) {
				m_strDescription[0]="Motor Activated";
				m_strDescription[1]="Commutation Enable";
				m_strDescription[2]="Command Output Address";
				m_strDescription[3]="Position Feedback Address";
				m_strDescription[4]="Velocity Feedback Address";
				m_strDescription[5]="Master Position Address";
				m_strDescription[6]="Following Enable";
				m_strDescription[7]="Scale Factor";
				m_strDescription[8]="Position Scale Factor";
				m_strDescription[9]="Velocity Scale Factor";
				m_strDescription[10]="Reserved";
				m_strDescription[11]="Fatal Following Error";
				m_strDescription[12]="Warning following error";
				m_strDescription[13]="Positive Software position limit";
				m_strDescription[14]="Negative Software position limit";
				m_strDescription[15]="Abort Deceleration";
				m_strDescription[16]="Motor Program Velocity";
				m_strDescription[17]="Motor Program Acceleration";
				m_strDescription[18]="";
				m_strDescription[19]="Jog/Home Acceleration";
				m_strDescription[20]="Jog/Home Acceleration Time";
				m_strDescription[21]="Jog/Home S-Curve Time";
				m_strDescription[22]="Jog Speed";
				m_strDescription[23]="Home Speed Direction";
				m_strDescription[24]="Flag mode control";
				m_strDescription[25]="Limit Home Flag";
				m_strDescription[26]="Home Offset";
				m_strDescription[27]="Position Rollover Range";
				m_strDescription[28]="In Position Band";
				m_strDescription[29]="DAC Bias";
				m_strDescription[30]="PID Proportional Gain";
				m_strDescription[31]="PID Derivitive Gain";
				m_strDescription[32]="PID Velocity FeedForward";
				m_strDescription[33]="PID Integral Gain";
				m_strDescription[34]="PID Integration Mode";
				m_strDescription[35]="PID Acceleration Feed Forward";
				m_strDescription[36]="PID Notch Filter N1";
				m_strDescription[37]="PID Notch Filter N2";
				m_strDescription[38]="PID Notch Filter D1";
				m_strDescription[39]="PID Notch Filter D2";
				m_strDescription[59]="User Written Servo Enable";
				m_strDescription[60]="Servo Cycle Period Extension";
				m_strDescription[63]="Integration Limit";
				m_strDescription[64]="DeadBand Gain";
				m_strDescription[65]="Dead Band Size";
				m_strDescription[67]="Linear Position Error";
				m_strDescription[68]="Friction Feed forward";
				m_strDescription[69]="Dac Output Command";
				m_strDescription[70]="Number Of Commutation cycles";
				m_strDescription[71]="Encoder counts per Commutation";
				m_strDescription[72]="Commutation Phase angle";
				m_strDescription[73]="Phase finding output Dac value";
				m_strDescription[74]="Phase finding time";
				m_strDescription[76]="Velocity phase advance gain";
				m_strDescription[77]="Induction motor magnetisation current";
				m_strDescription[78]="Induction motor slip gain";
				m_strDescription[79]="Second phase output bias";
				m_strDescription[80]="Power up mode";
				m_strDescription[85]="Backlash Take-up Rate";
				m_strDescription[86]="Backlash Size(1/16)";
				m_strDescription[87]="Backlash Hysteresis";
			}

			if(m_nTop>=nTopI) {
				m_strDescription[0]="Encoder Decode 1";
				m_strDescription[1]="Encoder Filter";
				m_strDescription[2]="Position Capture";
				m_strDescription[3]="Flag Select";
				m_strDescription[4]="Reserved";
				m_strDescription[5]="Encoder Decode 2";
				m_strDescription[6]="Encoder Filter";
				m_strDescription[7]="Position Capture";
				m_strDescription[8]="Flag Select";
				m_strDescription[9]="Reserved";
				m_strDescription[10]="Encoder Decode 3";
				m_strDescription[11]="Encoder Filter";
				m_strDescription[12]="Position Capture";
				m_strDescription[13]="Flag Select";
				m_strDescription[14]="Reserved";
				m_strDescription[15]="Encoder Decode 4";
				m_strDescription[16]="Encoder Filter";
				m_strDescription[17]="Position Capture";
				m_strDescription[18]="Flag Select";
				m_strDescription[19]="Reserved";
				m_strDescription[20]="Encoder Decode 5";
				m_strDescription[21]="Encoder Filter";
				m_strDescription[22]="Position Capture";
				m_strDescription[23]="Flag Select";
				m_strDescription[24]="Reserved";
				m_strDescription[25]="Encoder Decode 6";
				m_strDescription[26]="Encoder Filter";
				m_strDescription[27]="Position Capture";
				m_strDescription[28]="Flag Select";
				m_strDescription[29]="Reserved";
				m_strDescription[30]="Encoder Decode 7";
				m_strDescription[31]="Encoder Filter";
				m_strDescription[32]="Position Capture";
				m_strDescription[33]="Flag Select";
				m_strDescription[34]="Reserved";
			}
			break;
		case Turbo_PMAC_Ultralite:
		case Turbo_PMAC2_Ultralite:
			if(m_nTop<100) {
				m_strDescription[0]="Reserved";
				m_strDescription[1]="Serial Port Mode";
				m_strDescription[2]="Control Panel Disable";
				m_strDescription[3]="I/O Handshake Control";
				m_strDescription[4]="Communication Integrity Mode";
				m_strDescription[5]="Plc Programs On / Off";
				m_strDescription[6]="Error Reporting Mode";
				m_strDescription[7]="Reserved";
				m_strDescription[8]="Real Time Interrupt Period";
				m_strDescription[9]="Full/Abrevited Listing";
				m_strDescription[10]="Servo Interrupt Time";
				m_strDescription[11]="Program Move Calcultation Time";
				m_strDescription[12]="Jog to Pos Calc time";
				m_strDescription[13]="Program Move Segmentation Time";
				m_strDescription[14]="Position match on run enable";
				m_strDescription[15]="Degrees / Radians";
				m_strDescription[16]="Rotary Buffer request on point";
				m_strDescription[17]="Rotary Buffer request off point";
				m_strDescription[18]="Fixed buffer full warning point";
				m_strDescription[19]="Clock source I Variable number";
				m_strDescription[20]="MACRO IC 0 Base Address";
				m_strDescription[21]="MACRO IC 1 Base Address";
				m_strDescription[22]="MACRO IC 2 Base Address";
				m_strDescription[23]="MACRO IC 3 Base Address";
				m_strDescription[24]="Main DPRAM Base Address";
				m_strDescription[25]="UMAC Electrical MACRO enable";
				m_strDescription[26]="";
				m_strDescription[27]="";
				m_strDescription[28]="";
				m_strDescription[29]="";
				m_strDescription[30]="";
				m_strDescription[31]="";
				m_strDescription[32]="";
				m_strDescription[33]="";
				m_strDescription[34]="";
				m_strDescription[35]="";
				m_strDescription[36]="";
				m_strDescription[37]="";
				m_strDescription[38]="";
				m_strDescription[39]="";
				m_strDescription[40]="";
				m_strDescription[41]="";
				m_strDescription[42]="";
				m_strDescription[43]="";
				m_strDescription[44]="";
				m_strDescription[45]="";
				m_strDescription[46]="";
				m_strDescription[47]="";
				m_strDescription[48]="";
				m_strDescription[49]="";
				m_strDescription[70]="MACRO IC 0 Node Auxillary Register Enable";
				m_strDescription[71]="MACRO IC 0 Node Protocol Type Control";
				m_strDescription[72]="MACRO IC 1 Node Auxillary Register Enable";
				m_strDescription[73]="MACRO IC 1 Node Protocol Type Control";
				m_strDescription[74]="MACRO IC 2 Node Auxillary Register Enable";
				m_strDescription[75]="MACRO IC 2 Node Protocol Type Control";
				m_strDescription[76]="MACRO IC 3 Node Auxillary Register Enable";
				m_strDescription[77]="MACRO IC 3 Node Protocol Type Control";
				m_strDescription[78]="MACRO Type 1 Master/Slave Communications Timeout";
				m_strDescription[79]="MACRO Type 1 Master/Master Communications Timeout";
				m_strDescription[80]="MACRO Ring Check Period";
				m_strDescription[81]="MACRO Maximum Ring Error Count";
				m_strDescription[82]="MACRO Minimum Sync Packet Count";
				m_strDescription[83]="MACRO Parallel Ring Enable Mask";
				m_strDescription[84]="MACRO IC # for Master Communications";
				m_strDescription[85]="MACRO Ring Order Number";

			}
			if((m_nTop>=100) && (m_nTop<nTopI)) {
				m_strDescription[0]="Motor Activated";
				m_strDescription[1]="Commutation Enable";
				m_strDescription[2]="Command Output Address";
				m_strDescription[3]="Position Feedback Address";
				m_strDescription[4]="Velocity Feedback Address";
				m_strDescription[5]="Master Position Address";
				m_strDescription[6]="Following Enable";
				m_strDescription[7]="Scale Factor";
				m_strDescription[8]="Position Scale Factor";
				m_strDescription[9]="Velocity Scale Factor";
				m_strDescription[10]="Reserved";
				m_strDescription[11]="Fatal Following Error";
				m_strDescription[12]="Warning following error";
				m_strDescription[13]="Positive Software position limit";
				m_strDescription[14]="Negative Software position limit";
				m_strDescription[15]="Abort Deceleration";
				m_strDescription[16]="Motor Program Velocity";
				m_strDescription[17]="Motor Program Acceleration";
				m_strDescription[18]="";
				m_strDescription[19]="Jog/Home Acceleration";
				m_strDescription[20]="Jog/Home Acceleration Time";
				m_strDescription[21]="Jog/Home S-Curve Time";
				m_strDescription[22]="Jog Speed";
				m_strDescription[23]="Home Speed Direction";
				m_strDescription[24]="Flag mode control";
				m_strDescription[25]="Limit Home Flag";
				m_strDescription[26]="Home Offset";
				m_strDescription[27]="Position Rollover Range";
				m_strDescription[28]="In Position Band";
				m_strDescription[29]="DAC Bias";
				m_strDescription[30]="PID Proportional Gain";
				m_strDescription[31]="PID Derivitive Gain";
				m_strDescription[32]="PID Velocity FeedForward";
				m_strDescription[33]="PID Integral Gain";
				m_strDescription[34]="PID Integration Mode";
				m_strDescription[35]="PID Acceleration Feed Forward";
				m_strDescription[36]="PID Notch Filter N1";
				m_strDescription[37]="PID Notch Filter N2";
				m_strDescription[38]="PID Notch Filter D1";
				m_strDescription[39]="PID Notch Filter D2";
				m_strDescription[40]="Net Desired Position Filter Gain";
				m_strDescription[41]="Desired Position Limit Band";
				m_strDescription[42]="Amplifier Flag Address";
				m_strDescription[43]="Overtravel Limit Flag Address";
				m_strDescription[44]="Macro Slave Command Address";
				m_strDescription[59]="User Written Servo Enable";
				m_strDescription[60]="Servo Cycle Period Extension";
				m_strDescription[63]="Integration Limit";
				m_strDescription[64]="DeadBand Gain";
				m_strDescription[65]="Dead Band Size";
				m_strDescription[67]="Linear Position Error";
				m_strDescription[68]="Friction Feed forward";
				m_strDescription[69]="Dac Output Command";
				m_strDescription[70]="Number Of Commutation cycles";
				m_strDescription[71]="Encoder counts per Commutation";
				m_strDescription[72]="Commutation Phase angle";
				m_strDescription[73]="Phase finding output Dac value";
				m_strDescription[74]="Phase finding time";
				m_strDescription[76]="Velocity phase advance gain";
				m_strDescription[77]="Induction motor magnetisation current";
				m_strDescription[78]="Induction motor slip gain";
				m_strDescription[79]="Second phase output bias";
				m_strDescription[80]="Power up mode";
				m_strDescription[83]="Communtation Position address";
				m_strDescription[85]="Backlash Take-up Rate";
				m_strDescription[86]="Backlash Size(1/16)";
				m_strDescription[87]="Backlash Hysteresis";
			}

			if((m_nTop>=8000) && (m_nTop<8050)) {
				m_strDescription[0]="Conversion Table 1";
				m_strDescription[2]="Conversion Table 2";
				m_strDescription[4]="Conversion Table 3";
				m_strDescription[6]="Conversion Table 4";
				m_strDescription[8]="Conversion Table 5";
				m_strDescription[10]="Conversion Table 6";
				m_strDescription[12]="Conversion Table 7";
				m_strDescription[14]="Conversion Table 8";
				m_strDescription[16]="Conversion Table 9";
				m_strDescription[18]="Conversion Table 10";
				m_strDescription[20]="Conversion Table 11";
				m_strDescription[22]="Conversion Table 12";
				m_strDescription[24]="Conversion Table 13";
				m_strDescription[26]="Conversion Table 14";
				m_strDescription[28]="Conversion Table 15";
				m_strDescription[30]="Conversion Table 16";
			}
			
			break;
		}
	}	


	CString RxStr;
	if(m_Variable=="M") {
		theApp.m_PmacUser.RequestMVariableDefinitions(m_Variable,m_nTop,100+m_nTop,RxStr);
		if(nRxStrLength = RxStr.GetLength()) {
			for (ii=0,jj=0;ii<100;ii++) {
				ZeroMemory(Temp,sizeof Temp);
				for(jj,kk=0;(RxStr.GetAt(jj)!=13) && (kk<20); ) {
					if(isprint(RxStr.GetAt(jj))) {
						Temp[kk++]=(char)RxStr.GetAt(jj);
					}
					if(jj<nRxStrLength) {
						jj++;
						if(jj==nRxStrLength) return;
					} else {
						break;
					}
				}
				m_strDescription[ii]=Temp;
				if(jj<nRxStrLength) {
					jj++;
					if(jj==nRxStrLength) return;
				} else {
					break;
				}
			};
		};
	}


}

void CPmacVariablesPage::OnGetdispinfoListVariables(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,100,_T("%d"),pDispInfo->item.iItem+m_nTop);
		break;
	case 1:
		str[0]=0;
		swprintf_s(str,100,_T("%s"),(LPCWSTR)m_strDescription[pDispInfo->item.iItem]);
		break;
	case 2:
		swprintf_s(str,100,_T("%s"), (LPCWSTR)m_strValue[pDispInfo->item.iItem]);
		break;
	}
	
	pDispInfo->item.pszText=str;

	
	*pResult = 0;
}

void CPmacVariablesPage::OnButtonPlus100() 
{
	CString	Buff;
	m_editJumpTo.GetWindowText(Buff);
	m_nTop=_ttoi(Buff)+100;
	Buff.Format(_T("%d"),m_nTop);
	m_editJumpTo.SetWindowText(Buff);
	FillList();
	
}

void CPmacVariablesPage::OnButtonMinus100() 
{
	CString	Buff;
	m_editJumpTo.GetWindowText(Buff);
	m_nTop=_ttoi(Buff)-100;
	if(m_nTop<0) m_nTop=0;
	Buff.Format(_T("%d"),m_nTop);
	m_editJumpTo.SetWindowText(Buff);
	FillList();
	
}

void CPmacVariablesPage::OnDblclkListVariables(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listVariables.GetWindowRect(CtrlRect);
	m_listVariables.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	
		break;
	case 2:	m_pEditSpinItem->Initialize(&m_strValue[m_nIndex=pDispInfo->item.mask]);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	}
	*pResult = 0;
}

void CPmacVariablesPage::OnClickListVariables(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CPmacVariablesPage::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

long CPmacVariablesPage::UpdatePmacFromList(unsigned int nTemp,long nlTemp)
{
	CString Buff;

	Buff.Format(_T("%s%d=%s"),m_Variable,m_nIndex+m_nTop,m_strValue[m_nIndex]);
	theApp.m_PmacUser.SendStr(Buff);

	return 0;
}


void CPmacVariablesPage::OnButtonSwap() 
{
	CString	Buff,rxBuff;
	char Temp[20];
	int	ii,jj,kk,nRxLength;
	int nRange0,nRange1;
	

	m_editSwap.GetWindowText(Buff);

	Buff.MakeUpper();

	for(ii=0;ii<Buff.GetLength();ii++) {
		if(!(isdigit(Buff.GetAt(ii)) || (Buff.GetAt(ii) == ' '))) {
			Buff.Delete(ii);
			ii--;
		}
	}

	if(swscanf_s(Buff,L"%d %d",&nRange0,&nRange1) == 2) {

		Buff.Format(L"are you sure you want to swap %d-%d with %d-%d",nRange0,nRange0+99,nRange1,nRange1+99);
		if(MessageBox(Buff,L"Warning",MB_ICONWARNING|MB_YESNO)==IDYES) { 

			theApp.m_PmacUser.RequestMultipleVariables(m_Variable,nRange0,100+nRange0,rxBuff);
			nRxLength = rxBuff.GetLength();

			for (ii=0,jj=0;ii<100;ii++) {
				Buff.Format(_T("%d"),ii+m_nTop);
				m_listVariables.InsertItem(ii, Buff.GetBuffer(255), ii);
				ZeroMemory(Temp,sizeof Temp);
				if(jj<nRxLength) {
					for(jj,kk=0;(rxBuff.GetAt(jj)!=13) && (kk<20); ) {
						if(isprint(rxBuff.GetAt(jj))) {
							Temp[kk++]=(char)rxBuff.GetAt(jj);
						}
						if(jj<nRxLength) {
							jj++;
							if(jj==nRxLength) break;
						} else {
							break;
						}
					}
				}
				m_strRange0[ii] = Temp;
				jj++;
			};

			theApp.m_PmacUser.RequestMultipleVariables(m_Variable,nRange1,100+nRange1,rxBuff);
			nRxLength = rxBuff.GetLength();

			for (ii=0,jj=0;ii<100;ii++) {
				Buff.Format(_T("%d"),ii+m_nTop);
				m_listVariables.InsertItem(ii, Buff.GetBuffer(255), ii);
				ZeroMemory(Temp,sizeof Temp);
				if(jj<nRxLength) {
					for(jj,kk=0;(rxBuff.GetAt(jj)!=13) && (kk<20); ) {
						if(isprint(rxBuff.GetAt(jj))) {
							Temp[kk++]=(char)rxBuff.GetAt(jj);
						}
						if(jj<nRxLength) {
							jj++;
							if(jj==nRxLength) break;
						} else {
							break;
						}
					}
				}
				m_strRange1[ii] = Temp;
				jj++;
			};

			for(ii=0;ii<100;ii++) {
				Buff.Format(_T("%s%d=%s"),m_Variable,nRange0+ii,m_strRange1[ii]);
				theApp.m_PmacUser.SendStr(Buff);
				Buff.Format(_T("%s%d=%s"),m_Variable,nRange1+ii,m_strRange0[ii]);
				theApp.m_PmacUser.SendStr(Buff);
			}
		}
	}
}
