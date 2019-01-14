// RobotEndEffectorPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "RobotEndEffectorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotEndEffectorPage property page

IMPLEMENT_DYNCREATE(CRobotEndEffectorPage, CPropertyPage)

CRobotEndEffectorPage::CRobotEndEffectorPage() : CPropertyPage(CRobotEndEffectorPage::IDD)
{
	//{{AFX_DATA_INIT(CRobotEndEffectorPage)
	//}}AFX_DATA_INIT
	
	m_nRobot = 0;

	wcscpy_s(m_wFileDescription,30, L"USL End Effector");
}

CRobotEndEffectorPage::~CRobotEndEffectorPage()
{
}

void CRobotEndEffectorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRobotEndEffectorPage)
	DDX_Control(pDX, IDC_EDIT_PROBE_VECTOR, m_editProbeVector);
	DDX_Control(pDX, IDC_EDIT_ARM_VECTOR, m_editArmVector);
	DDX_Control(pDX, IDC_COMBO_END_EFFECTOR_TYPE, m_comboEndEffectorType);
	DDX_Control(pDX, IDC_CHECK_ROBOT_1, m_checkRobot1);
	DDX_Control(pDX, IDC_CHECK_ROBOT_0, m_checkRobot0);
	DDX_Control(pDX, IDC_EDIT_PROBE_COORDINATE, m_editJ6toTT);
	DDX_Control(pDX, IDC_SPIN_X_ROTATION, m_spinXRotation);
	DDX_Control(pDX, IDC_EDIT_X_ROTATION, m_editXRotation);
	DDX_Control(pDX, IDC_SPIN_Y_ROTATION, m_spinYRotation);
	DDX_Control(pDX, IDC_EDIT_Y_ROTATION, m_editYRotation);
	DDX_Control(pDX, IDC_SPIN_Z_ROTATION, m_spinZRotation);
	DDX_Control(pDX, IDC_EDIT_Z_ROTATION, m_editZRotation);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_editFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRobotEndEffectorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRobotEndEffectorPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y_ROTATION, OnDeltaposSpinYRotation)
	ON_EN_CHANGE(IDC_EDIT_Y_ROTATION, OnChangeEditYRotation)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT_PROBE_COORDINATE, OnChangeEditProbeCoordinate)
	ON_BN_CLICKED(IDC_CHECK_ROBOT_0, OnCheckRobot0)
	ON_BN_CLICKED(IDC_CHECK_ROBOT_1, OnCheckRobot1)
	ON_CBN_SELCHANGE(IDC_COMBO_END_EFFECTOR_TYPE, OnSelchangeComboEndEffectorType)
	ON_EN_CHANGE(IDC_EDIT_ARM_VECTOR, OnChangeEditArmVector)
	ON_EN_CHANGE(IDC_EDIT_PROBE_VECTOR, OnChangeEditProbeVector)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotEndEffectorPage message handlers
int CRobotEndEffectorPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndFileToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndFileToolBar.LoadToolBar(IDR_END_EFFECTOR_TOOLBAR)) {
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

	m_wndRebar.AddBar(&m_wndFileToolBar);
	
	return 0;
}

BOOL CRobotEndEffectorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_spinXRotation.SetRange(0, 3600);
	m_spinYRotation.SetRange(0, 3600);
	m_spinZRotation.SetRange(0, 3600);

	theApp.m_ToolChangers.InitializeTypeStrings();
	for(int ii=0;ii<10;ii++) {
		m_comboEndEffectorType.AddString(theApp.m_ToolChangers.m_strType[ii]);
	};
	
	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRobotEndEffectorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

BOOL CRobotEndEffectorPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastRobotTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();
	
	return CPropertyPage::OnSetActive();
}

BOOL CRobotEndEffectorPage::OnKillActive() 
{
	return CPropertyPage::OnKillActive();
}

void CRobotEndEffectorPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	
	float fAngle;
	D3DXVECTOR3 vect;

	switch(m_nRobot) {
	case 0: m_checkRobot0.SetCheck(true);
		 m_checkRobot1.SetCheck(false);
		break;
	case 1: m_checkRobot0.SetCheck(false);
		 m_checkRobot1.SetCheck(true);
		break;
	}
	m_checkRobot0.SetWindowText(theApp.m_DlgSideName[0]);
	m_checkRobot1.SetWindowText(theApp.m_DlgSideName[1]);

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f\tI %.05f\tJ %.05f\tK %.05f",theApp.m_Robot[m_nRobot].m_vJ6ToolTip.x,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.y,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.z,
										theApp.m_Robot[m_nRobot].m_vAtToolTip.x, theApp.m_Robot[m_nRobot].m_vAtToolTip.y, theApp.m_Robot[m_nRobot].m_vAtToolTip.z);
	m_editJ6toTT.SetWindowText(Buff);

	fAngle = asinf(theApp.m_Robot[m_nRobot].m_vAtToolTip.x) * RAD_TO_DEG;
	m_spinXRotation.SetPos((int)(fAngle * 10.0f) + 1800);
	Buff.Format(L"%.01f%s", fAngle, theApp.m_Degrees);
	m_editXRotation.SetWindowText(Buff);

	fAngle = asinf(theApp.m_Robot[m_nRobot].m_vAtToolTip.y) * RAD_TO_DEG;
	m_spinYRotation.SetPos((int)(fAngle * 10.0f) + 1800);
	Buff.Format(L"%.01f%s", fAngle, theApp.m_Degrees);
	m_editYRotation.SetWindowText(Buff);

	fAngle = asinf(theApp.m_Robot[m_nRobot].m_vAtToolTip.z) * RAD_TO_DEG;
	m_spinZRotation.SetPos((int)(fAngle * 10.0f) + 1800);
	Buff.Format(L"%.01f%s", fAngle, theApp.m_Degrees);
	m_editZRotation.SetWindowText(Buff);


	m_editFilePath.SetWindowText(theApp.m_EndEffectorPathName[m_nRobot]);

	int nType = *theApp.m_ToolChangers.getPtrType(m_nRobot);
	m_comboEndEffectorType.SetCurSel(nType);

	theApp.m_ToolChangers.getArmVector(m_nRobot, &vect);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",vect.x, vect.y, vect.z);
	m_editArmVector.SetWindowText(Buff);

	theApp.m_ToolChangers.getProbeVector(m_nRobot, &vect);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f", vect.x, vect.y, vect.z);
	m_editProbeVector.SetWindowText(Buff);
}


void CRobotEndEffectorPage::OnDeltaposSpinYRotation(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	

	float fAngle0 = asinf(theApp.m_Robot[m_nRobot].m_vAtToolTip.x) * RAD_TO_DEG;
	float fAngle1 = fAngle0;

	fAngle1 += ((float)pNMUpDown->iDelta * 0.1f);
	if(fAngle1 - fAngle0) {
		theApp.m_Robot[m_nRobot].m_vAtToolTip.x = sinf(fAngle1 * DEG_TO_RAD);
		theApp.m_Robot[m_nRobot].m_vAtToolTip.y = 0.0f;
		theApp.m_Robot[m_nRobot].m_vAtToolTip.z = cosf(fAngle1 * DEG_TO_RAD);

		CalculateKinematics();
		UpdateAllControls();
		theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(m_nRobot,false);
	}
	*pResult = 0;
}

void CRobotEndEffectorPage::OnChangeEditYRotation() 
{
	
	CString Buff;

	float fAngle0 = asinf(theApp.m_Robot[m_nRobot].m_vAtToolTip.x) * RAD_TO_DEG;
	float fAngle1 = fAngle0;

	m_editYRotation.GetWindowText(Buff);
	_WTOF(Buff,fAngle1);

	if(fAngle1 - fAngle0) {
			theApp.m_Robot[m_nRobot].m_vAtToolTip.x = sinf(fAngle1 * DEG_TO_RAD);
		theApp.m_Robot[m_nRobot].m_vAtToolTip.y = 0.0f;
		theApp.m_Robot[m_nRobot].m_vAtToolTip.z = cosf(fAngle1 * DEG_TO_RAD);

		CalculateKinematics();
		theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(m_nRobot,false);
	}


}

BOOL CRobotEndEffectorPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_FILE_SAVEAS: strTipText.LoadString(IDS_Save_end_effector);
	   break;
   case ID_FILE_OPEN: strTipText.LoadString(IDS_Open_end_effector);
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

void CRobotEndEffectorPage::OnFileSaveas() 
{
	
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	int nOffsetArray[1024];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);

	CFileDialog FileDlg(FALSE,_T(".EndEffector"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("End Effector (*.EndEffector)|*.EndEffector|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			ULONGLONG nStartPosition = File.GetPosition();
			File.Write(nOffsetArray,sizeof nOffsetArray);
			File.Seek(nStartPosition,CFile::begin);

			File.Write(m_wFileDescription,wcslen(m_wFileDescription) * sizeof WCHAR);
			File.Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);
			File.Write(&theApp.m_Robot[m_nRobot].m_vJ6ToolTip,sizeof theApp.m_Robot[m_nRobot].m_vJ6ToolTip);
			File.Write(&theApp.m_Robot[m_nRobot].m_vAtToolTip, sizeof theApp.m_Robot[m_nRobot].m_vAtToolTip);
			File.Close();

			theApp.m_EndEffectorPathName[m_nRobot] = FilePath;
			UpdateAllControls();
		}
	}		
}

void CRobotEndEffectorPage::OnFileOpen() 
{
	
	CString FilePath,Buff;
	CUSLFile File;
	CFileException e;
	float fVersion;
	WCHAR	wDescription[30];
	ZeroMemory(wDescription,sizeof wDescription);

	CFileDialog FileDlg(TRUE,_T(".EndEffector"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("End Effector (*.EndEffector)|*.EndEffector|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			File.Read(wDescription,wcslen(m_wFileDescription) * sizeof WCHAR);

			if(wcscmp(wDescription, m_wFileDescription) == 0) {
				File.Read(&fVersion, sizeof fVersion);
				File.Read(&theApp.m_Robot[m_nRobot].m_vJ6ToolTip,sizeof theApp.m_Robot[m_nRobot].m_vJ6ToolTip);
				File.Read(&theApp.m_Robot[m_nRobot].m_vAtToolTip, sizeof theApp.m_Robot[m_nRobot].m_vAtToolTip);
				theApp.m_EndEffectorPathName[m_nRobot] = FilePath;
				UpdateAllControls();
				theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(m_nRobot,true);
			}
			File.Close();
		}
	}
}

void CRobotEndEffectorPage::OnChangeEditProbeCoordinate() 
{
	
	D3DXVECTOR3 vJ6 = theApp.m_Robot[m_nRobot].m_vJ6ToolTip;
	D3DXVECTOR3 vAtTool = theApp.m_Robot[m_nRobot].m_vAtToolTip;
	float fLength , fAngle;
	CString Buff;

	m_editJ6toTT.GetWindowTextW(Buff);

	ExtractVector(Buff,&theApp.m_Robot[m_nRobot].m_vJ6ToolTip,&theApp.m_Robot[m_nRobot].m_vAtToolTip);

	fLength = D3DXVec3Length(&(vJ6 - theApp.m_Robot[m_nRobot].m_vJ6ToolTip));
	fAngle = D3DXVec3Length(&(vAtTool - theApp.m_Robot[m_nRobot].m_vAtToolTip));
	if(fLength || fAngle) {
		theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(m_nRobot,true);
	}
	
}

void CRobotEndEffectorPage::OnCheckRobot0() 
{
	m_nRobot = 0;
	UpdateAllControls();
	
}

void CRobotEndEffectorPage::OnCheckRobot1() 
{
	m_nRobot = 1;
	UpdateAllControls();
}


void CRobotEndEffectorPage::OnSelchangeComboEndEffectorType() 
{
	theApp.m_ToolChangers.setType(0, m_comboEndEffectorType.GetCurSel());	
}


void CRobotEndEffectorPage::OnChangeEditArmVector() 
{
	
	CString Buff;
	D3DXVECTOR3 vArmLength;

	m_editArmVector.GetWindowText(Buff);
	ExtractVector(Buff, &vArmLength);

	theApp.m_ToolChangers.setArmVector(m_nRobot, &vArmLength);

//	CalculateKinematics();

}

void CRobotEndEffectorPage::OnChangeEditProbeVector() 
{
	CString Buff;
	D3DXVECTOR3 vProbeLength;

	m_editProbeVector.GetWindowText(Buff);
	ExtractVector(Buff, &vProbeLength);

	theApp.m_ToolChangers.setProbeVector(m_nRobot, &vProbeLength);

//	CalculateKinematics();

}


void CRobotEndEffectorPage::CalculateKinematics()
{
	if (GetSafeHwnd() == NULL) return;
	
	D3DXVECTOR3 vecArm,vecProbe;
	CString Buff;

	int nType = *theApp.m_ToolChangers.getPtrType(m_nRobot);
	float fAngle = asinf(theApp.m_Robot[m_nRobot].m_vAtToolTip.x) * RAD_TO_DEG;
	theApp.m_ToolChangers.getProbeVector(m_nRobot, &vecProbe);
	theApp.m_ToolChangers.getArmVector(m_nRobot, &vecArm);
	/*
	switch(nType) {
	case 1:
	case 2:
		theApp.m_Robot[m_nRobot].m_vJ6ToolTip.x = sinf(fAngle * DEG_TO_RAD) * vecProbe.z + vecArm.x;
		theApp.m_Robot[m_nRobot].m_vJ6ToolTip.y = vecArm.y;
		theApp.m_Robot[m_nRobot].m_vJ6ToolTip.z = cosf(fAngle * DEG_TO_RAD) * vecProbe.z + vecArm.z;
		break;
	}
	*/

	Buff.Format(L"X %.01f\tY %.01f\tZ %.01f\tI %.05f\tJ %.05f\tK %.05f",theApp.m_Robot[m_nRobot].m_vJ6ToolTip.x,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.y,theApp.m_Robot[m_nRobot].m_vJ6ToolTip.z,
										theApp.m_Robot[m_nRobot].m_vAtToolTip.x, theApp.m_Robot[m_nRobot].m_vAtToolTip.y, theApp.m_Robot[m_nRobot].m_vAtToolTip.z);
//	m_editJ6toTT.SetWindowText(Buff);

}