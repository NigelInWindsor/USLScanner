// DlgTestFWMemory.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPATool.h"
#include "DlgTestFWMemory.h"
#include "afxdialogex.h"


// CDlgTestFWMemory dialog

IMPLEMENT_DYNAMIC(CDlgTestFWMemory, CDialog)

CDlgTestFWMemory::CDlgTestFWMemory(CHWDeviceOEMPA *pHWDeviceOEMPA,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestFWMemory::IDD, pParent)
	, m_iEditCycleCount(0)
	, m_strEditData(_T("0xCDCDCDCD"))
{
	m_pHWDeviceOEMPA = pHWDeviceOEMPA;
}

CDlgTestFWMemory::~CDlgTestFWMemory()
{
}

void CDlgTestFWMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CYCLE, m_iEditCycleCount);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strEditData);
}


BEGIN_MESSAGE_MAP(CDlgTestFWMemory, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CYCLE, &CDlgTestFWMemory::OnBnClickedButtonCycle)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, &CDlgTestFWMemory::OnBnClickedButtonFilter)
END_MESSAGE_MAP()


// CDlgTestFWMemory message handlers

bool HexaToInt(CString txt,DWORD &dwData)
{
	int sz = txt.GetLength();
	CString tmpStr;
	__int64 value;
	wchar_t c;
	__int64 powerFactor = 1;
	__int64 tmp;

	if(txt.Find(L"0x")!=0)
		return false;
	if(sz!=10)
		return false;
	value = 0;
	for (int i = sz - 1;i >= 2;i--)
	{
		c = txt[i];
		if ( (c >= '0') && (c <= '9'))
		{
			tmp = c - '0';
			value += powerFactor* tmp;
			powerFactor *= 16;
		}
		else if ( (c >= 'A') && (c <= 'F'))
		{
			tmp = c - 'A' + 10;
			value += powerFactor* tmp;
			powerFactor *= 16;
		}
		else if ( (c >= 'a') && (c <= 'f'))
		{
			tmp = c - 'a' + 10;
			value += powerFactor* tmp;
			powerFactor *= 16;
		}else
			return false;
	}
	dwData = (DWORD)value;
	return true;
}

void CDlgTestFWMemory::OnBnClickedButtonCycle()
{
	DWORD dwData;
	int iCycleCountMax;

	UpdateData();
	if(!HexaToInt(m_strEditData,dwData))
	{
		AfxMessageBox(L"Bad data!");
		return;
	}
	if(!m_pHWDeviceOEMPA || !m_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
	{
		AfxMessageBox(L"Not connected!");
		return;
	}
	iCycleCountMax = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetCycleCountMax();
	if(iCycleCountMax<=0)
	{
		AfxMessageBox(L"Bad cycle count!");
		return;
	}
	if(m_iEditCycleCount>iCycleCountMax)
	{
		m_iEditCycleCount = iCycleCountMax;
		UpdateData(FALSE);
		AfxMessageBox(L"CycleCount is too many!");
		return;
	}
	CWaitCursor wait;
	if(!m_pHWDeviceOEMPA->LockDevice())
	{
		AfxMessageBox(L"Error to lock the device!");
		return;
	}
	m_pHWDeviceOEMPA->TestFWMemoryCycle(m_iEditCycleCount,dwData);
	m_pHWDeviceOEMPA->UnlockDevice();
}

void CDlgTestFWMemory::OnBnClickedButtonFilter()
{
	DWORD dwData;

	UpdateData();
	if(!HexaToInt(m_strEditData,dwData))
	{
		AfxMessageBox(L"Bad data!");
		return;
	}
	if(!m_pHWDeviceOEMPA || !m_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
	{
		AfxMessageBox(L"Not connected!");
		return;
	}
	CWaitCursor wait;
	if(!m_pHWDeviceOEMPA->LockDevice())
	{
		AfxMessageBox(L"Error to lock the device!");
		return;
	}
	m_pHWDeviceOEMPA->TestFWMemoryFilter((int)eOEMPAFilter15,dwData);
	m_pHWDeviceOEMPA->UnlockDevice();
}
