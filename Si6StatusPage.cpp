// Si6StatusPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "Si6StatusPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSi6StatusPage property page

IMPLEMENT_DYNCREATE(CSi6StatusPage, CPropertyPage)

CSi6StatusPage::CSi6StatusPage() : CPropertyPage(CSi6StatusPage::IDD)
{
	//{{AFX_DATA_INIT(CSi6StatusPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSi6StatusPage::~CSi6StatusPage()
{
}

void CSi6StatusPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSi6StatusPage)
	DDX_Control(pDX, IDC_EDIT_SWITCH_EOR, m_editSwitchEOR);
	DDX_Control(pDX, IDC_DSP_EDIT, m_DspEdit);
	DDX_Text(pDX, IDC_JOYSTICK_S_AXIS, m_nSAxis);
	DDX_Text(pDX, IDC_JOYSTICK_X_AXIS, m_nXAxis);
	DDX_Text(pDX, IDC_JOYSTICK_Y_AXIS, m_nYAxis);
	DDX_Text(pDX, IDC_JOYSTICK_Z_AXIS, m_nZAxis);
	DDX_Text(pDX, IDC_COUNT_R, m_nCountR);
	DDX_Text(pDX, IDC_COUNT_X, m_nCountX);
	DDX_Text(pDX, IDC_COUNT_XT, m_nCountXt);
	DDX_Text(pDX, IDC_COUNT_Y, m_nCountY);
	DDX_Text(pDX, IDC_COUNT_YT, m_nCountYt);
	DDX_Text(pDX, IDC_COUNT_Z, m_nCountZ);
	DDX_Text(pDX, IDC_POS_R, m_nPosR);
	DDX_Text(pDX, IDC_POS_X, m_nPosX);
	DDX_Text(pDX, IDC_POS_XT, m_nPosXt);
	DDX_Text(pDX, IDC_POS_Y, m_nPosY);
	DDX_Text(pDX, IDC_POS_YT, m_nPosYt);
	DDX_Text(pDX, IDC_POS_Z, m_nPosZ);
	DDX_Text(pDX, IDC_PROCESS_STATE, m_csProcessState);
	DDX_Text(pDX, IDC_DIR_REG, m_csDirReg);
	DDX_Text(pDX, IDC_SI6_IRQ, m_nSi6Irq);
	DDX_Text(pDX, IDC_DATA_PTR, m_nDataPtr);
	DDX_Text(pDX, IDC_EDIT_IRQ_MASK, m_stringIrqMask);
	DDX_Text(pDX, IDC_EDIT_LIMIT_SWITCHES, m_stringLimitSwitches);
	DDX_Text(pDX, IDC_EDIT_MSC_JOY_0, m_nMSCJoy0);
	DDX_Text(pDX, IDC_EDIT_MSC_JOY_1, m_nMSCJoy1);
	DDX_Text(pDX, IDC_EDIT_MSC_JOY_2, m_nMSCJoy2);
	DDX_Text(pDX, IDC_EDIT_MSC_JOY_3, m_nMSCJoy3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSi6StatusPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSi6StatusPage)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_SWITCH_EOR, OnChangeEditSwitchEor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSi6StatusPage message handlers

void CSi6StatusPage::OnTimer(UINT nIDEvent) 
{

	if(GetSafeHwnd() == NULL) return;

	

	m_nPosX=theApp.m_Axes[theApp.m_Tank.nXStep].nPos;
	m_nCountX=theApp.m_Axes[theApp.m_Tank.nXStep].nCount;

	m_nPosY=theApp.m_Axes[theApp.m_Tank.nYStep].nPos;
	m_nCountY=theApp.m_Axes[theApp.m_Tank.nYStep].nCount;

	m_nPosZ=theApp.m_Axes[theApp.m_Tank.nZStep].nPos;
	m_nCountZ=theApp.m_Axes[theApp.m_Tank.nZStep].nCount;

	m_nPosXt=theApp.m_Axes[theApp.m_Tank.nXtStep].nPos;
	m_nCountXt=theApp.m_Axes[theApp.m_Tank.nXtStep].nCount;

	m_nPosYt=theApp.m_Axes[theApp.m_Tank.nYtStep].nPos;
	m_nCountYt=theApp.m_Axes[theApp.m_Tank.nYtStep].nCount;

	m_nPosR=theApp.m_Axes[theApp.m_Tank.nRStep].nPos;
	m_nCountR=theApp.m_Axes[theApp.m_Tank.nRStep].nCount;


	m_nXAxis = theApp.m_Si6User.ReadJoystick(0);
	m_nYAxis = theApp.m_Si6User.ReadJoystick(1);
	m_nZAxis = theApp.m_Si6User.ReadJoystick(2);
	m_nSAxis = theApp.m_Si6User.ReadJoystick(3);

	m_nDataPtr = theApp.m_Si6User.m_Si6.nDataPtr;


	
	m_nMSCJoy0 = theApp.m_UtUser.MscAdcRead(0);
	m_nMSCJoy1 = theApp.m_UtUser.MscAdcRead(1);
	m_nMSCJoy2 = theApp.m_UtUser.MscAdcRead(2);
	m_nMSCJoy3 = theApp.m_UtUser.MscAdcRead(3);
	


	m_nSi6Irq = theApp.m_Si6User.m_Si6.nIrqCount;
//	m_nSi6Irq = theApp.m_Axes[theApp.m_Tank.nXStep].nIrqCount;

	m_csProcessState.Format(_T("%02xh"),theApp.m_Si6User.m_Si6.nProcessState);
	m_csDirReg.Format(_T("%02xh"),theApp.m_Si6User.m_Si6.nDirReg);
	m_stringIrqMask.Format(_T("%02xh"),theApp.m_Si6User.Inp(theApp.m_Si6User.m_Si6.nAddrs+SI6_PIC+1));
	m_stringLimitSwitches.Format(_T("%02x"),theApp.m_Si6User.ReadLimitSwitches());

/*
	pDsp=m_pCards->GetPtrToDsp();
	m_nDspIrq = pDsp->nIrqCount;

	str[0]=0;
	for(ii=0;ii<22;ii+=3 ) {
		sprintf(str,"%s%02x %02x %02x\r\n"),str,pDsp->Vv[ii],pDsp->Vv[ii+1],pDsp->Vv[ii+2]);
	};
	sprintf(str,"%sErrors=%d\r\n"),str,pDsp->ErrorCount);
	m_DspEdit.SetWindowText(str);
*/
	UpdateData(FALSE);
	

	CDialog::OnTimer(nIDEvent);
}

BOOL CSi6StatusPage::OnInitDialog() 
{
	CString Buff;
	
	CDialog::OnInitDialog();
/*
	DSP200Data* pDsp=m_pCards->GetPtrToDsp();
	pDsp->Vv[22]=0xff;
	pDsp->Vv[23]=0x00;
*/

	theApp.m_Si6User.m_Si6.nIrqCount = 0;

	Buff.Format(_T("%02x h"),theApp.m_Si6User.m_Si6.nSwitchEor);
	m_editSwitchEOR.SetWindowText(Buff);

	
	SetTimer(1,100,NULL);	
	return TRUE;
}

void CSi6StatusPage::OnOK() 
{

	CDialog::OnOK();
}

void CSi6StatusPage::OnChangeEditSwitchEor() 
{
	
	CString Buff;
	
	m_editSwitchEOR.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Si6User.m_Si6.nSwitchEor);
}
