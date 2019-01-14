// DlgHwLink.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPAApplicationExample.h"
#include "DlgHwLink.h"
#include "afxdialogex.h"


// CDlgHwLink dialog

IMPLEMENT_DYNAMIC(CDlgHwLink, CDialog)

CDlgHwLink::CDlgHwLink(CHWDevice *pDevice,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHwLink::IDD, pParent)
	, m_strEditSystem(_T(""))
	, m_iSystemList(0)
	, m_iComboDefaultHwLink(0)
	, m_bCheckMatchedDevice(FALSE)
	, m_strEditMatchedDevice(_T(""))
{
	CHWDeviceOEMPA *pDeviceOEMPA=dynamic_cast<CHWDeviceOEMPA*>(pDevice);

	if(CHWDeviceOEMPA::IsMatchedDeviceHwLinkEnabled())
		m_bCheckMatchedDevice = TRUE;
	else
		m_bCheckMatchedDevice = FALSE;
	m_pDevice = pDevice;
	m_iDeviceId = m_pDevice->GetDeviceId();
}

CDlgHwLink::~CDlgHwLink()
{
}

void CDlgHwLink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOARD, m_SystemList);
	DDX_Text(pDX, IDC_EDIT_SYSTEM, m_strEditSystem);
	DDX_LBIndex(pDX, IDC_LIST_BOARD, m_iSystemList);
	DDX_Control(pDX, IDC_BUTTON_DESELECT, m_ButtonDeselect);
	DDX_Control(pDX, IDC_COMBO_DEFAULT_LINK, m_ComboDefaultHwLink);
	DDX_CBIndex(pDX, IDC_COMBO_DEFAULT_LINK, m_iComboDefaultHwLink);
	DDX_Control(pDX, IDC_EDIT_SYSTEM, m_EditSystem);
	DDX_Check(pDX, IDC_CHECK_MATCHED_DEVICE, m_bCheckMatchedDevice);
	DDX_Text(pDX, IDC_EDIT_MATCHED_DEVICE, m_strEditMatchedDevice);
}


BEGIN_MESSAGE_MAP(CDlgHwLink, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgHwLink::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST_BOARD, &CDlgHwLink::OnLbnSelchangeListBoard)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT, &CDlgHwLink::OnBnClickedButtonDeselect)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFAULT_LINK, &CDlgHwLink::OnCbnSelchangeComboDefaultHwLink)
	ON_BN_CLICKED(IDC_CHECK_MATCHED_DEVICE, &CDlgHwLink::OnBnClickedCheckMatchedDevice)
END_MESSAGE_MAP()


// CDlgHwLink message handlers

BOOL CDlgHwLink::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitSystemList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgHwLink::InitSystemList()
{
	wchar_t strName[MAX_PATH];
	wchar_t stringId[MAX_PATH];
	CString string,strAux;
	int iMasterDeviceId,iIndex,iDeviceId,iPort;
	DWORD dwData;
	enumHardwareLink eDefaultHwLink,eHardwareLink;
	enumHardwareLink eMatchedDeviceHwLink;
	int iMatchedDeviceId;
	CString strConnectionStatus;

	m_iComboDefaultHwLink = -1;
	if(CHWDevice::GetDefaultHwLink(m_iDeviceId,eDefaultHwLink))
		m_iComboDefaultHwLink = (int)eDefaultHwLink;
	if(m_pDevice->IsConnected(m_iDeviceId))
	{
		m_ComboDefaultHwLink.EnableWindow(FALSE);
		m_SystemList.EnableWindow(TRUE);
		m_ButtonDeselect.EnableWindow(TRUE);
		m_EditSystem.EnableWindow(TRUE);
	}else{
		m_ComboDefaultHwLink.EnableWindow(TRUE);
		m_SystemList.EnableWindow(FALSE);
		m_ButtonDeselect.EnableWindow(FALSE);
		m_EditSystem.EnableWindow(FALSE);
	}
	m_bIAmMaster = false;
	m_SystemList.ResetContent();
	m_iSystemList = -1;
	m_iMasterDeviceId = -1;
	for(int iDeviceId2=-1;iDeviceId2<CHWDevice::GetDeviceCountMax();iDeviceId2++)
	{
		if(iDeviceId2==m_iDeviceId)
			continue;
		if(iDeviceId2==-1)
			iDeviceId = m_iDeviceId;
		else
			iDeviceId = iDeviceId2;
		iPort = CHWDevice::GetMonitorPort(iDeviceId);
		if(CHWDevice::ReadBoardName(iDeviceId,strName,MAX_PATH) &&
			CHWDevice::ReadCommunicationName(iDeviceId,stringId,MAX_PATH))
		{
			if(CHWDevice::IsConnected(iDeviceId))//for MatchedDevices: master and salve are managed independently.
				strConnectionStatus = L"Connected";
			else
				strConnectionStatus = L"Disconnected";
			iMasterDeviceId = CHWDevice::GetMasterDeviceId(iDeviceId);
			eHardwareLink = CHWDevice::GetHardwareLink(iDeviceId);
			string = L"\"";
			string += strName;
			string += L"\" \"";
			string += stringId;
			string += L"\" (";
			switch(eHardwareLink)
			{
			case eUnlinked: strAux.Format(L"devId:%d, monitor=%d, Unlinked, %s)",iDeviceId,iPort,strConnectionStatus);break;
			case eMaster: strAux.Format(L"devId:%d, monitor=%d, Master, %s)",iDeviceId,iPort,strConnectionStatus);break;
			case eSlave: strAux.Format(L"devId:%d, monitor=%d, Slave, %s)",iDeviceId,iPort,strConnectionStatus);break;
			}
			string += strAux;
			if(m_iDeviceId==iDeviceId)
			{
				if(eHardwareLink==eMaster)
				{
					m_bIAmMaster = true;
					m_strEditSystem += string;
				}else{
					m_strEditSystem = string;
					m_iMasterDeviceId = CHWDevice::GetMasterDeviceId(iDeviceId);
				}
			}else{
				dwData = (DWORD)iDeviceId;
				if(eHardwareLink==eMaster)
					dwData += 0x10000;
if(CHWDevice::GetMatchedDeviceHwLink(iDeviceId,eMatchedDeviceHwLink,iMatchedDeviceId) &&
	(eMatchedDeviceHwLink==eSlave) && (iMatchedDeviceId==m_iDeviceId))
	m_strEditMatchedDevice = string;
else{
				iIndex = m_SystemList.AddString(string);
				if(m_iMasterDeviceId==iDeviceId)
					m_iSystemList = iIndex;
				m_SystemList.SetItemData(iIndex,dwData);
}
			}
		}
	}
	UpdateData(FALSE);
	m_ButtonDeselect.EnableWindow(m_iMasterDeviceId>=0?true:false);
}

void CDlgHwLink::OnBnClickedOk()
{
	CDialog::OnOK();
}

void CDlgHwLink::OnLbnSelchangeListBoard()
{
	DWORD dwData;

	if(m_bIAmMaster)
	{
		m_iSystemList = -1;
		UpdateData(FALSE);
		return;
	}
	UpdateData();
	//disconnect master
	if(m_iMasterDeviceId>=0)
	{
		if(m_pDevice->SlaveDisconnect())
			m_iMasterDeviceId = -1;
	}
	if(m_iSystemList<0)
		return;
	if(AfxMessageBox(L"Are you sure to select this device as a master ?",MB_YESNO)==IDNO)
		return;
	dwData = (DWORD)m_SystemList.GetItemData(m_iSystemList);
	if(m_pDevice->SlaveConnect((int)LOWORD(dwData)))
		m_iMasterDeviceId = m_iSystemList;
	InitSystemList();
}

void CDlgHwLink::OnBnClickedButtonDeselect()
{
	if(m_pDevice->SlaveDisconnect())
		m_iMasterDeviceId = -1;
	InitSystemList();
}

void CDlgHwLink::OnCbnSelchangeComboDefaultHwLink()
{
	enumHardwareLink eHardwareLink;
	bool bPreviousMasterUnregistered;

	UpdateData();
	if(m_iComboDefaultHwLink<0)
		return;
	eHardwareLink = (enumHardwareLink)m_iComboDefaultHwLink;
	if(!m_pDevice->SetDefaultHwLink(eHardwareLink,bPreviousMasterUnregistered))
		AfxMessageBox(L"Error: only one master can be used!");
	if(bPreviousMasterUnregistered)
		AfxMessageBox(L"Warning: previous master has been unregistered!");
	InitSystemList();
}

void CDlgHwLink::OnBnClickedCheckMatchedDevice()
{
	UpdateData();
	CHWDeviceOEMPA::EnableMatchedDeviceHwLink(m_bCheckMatchedDevice?true:false);
}
