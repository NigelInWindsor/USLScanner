
#pragma once
#include "afxwin.h"

// CUTMFCDlgNew dialog
#pragma warning( disable : 4251 )

class UTKERNEL_MFC_API_ALIGN _CUTMFCDlgNew
{
private:
	ULONGLONG m_ullPrivateMemory[109];
public:
	_CUTMFCDlgNew();

};

class UTKERNEL_MFC_API_ALIGN CUTMFCDlgNew : public _CUTMFCDlgNew, CDialog, CUTMFCSelection
{
	DECLARE_DYNAMIC(CUTMFCDlgNew)

public:
	CUTMFCDlgNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUTMFCDlgNew();
	virtual INT_PTR DoModal();
	int GetRootSize();
	int GetRootId(int iIndex,bool &bImage,bool &bWindow);
	bool GetTitle(int iIndex,CString &strTitle);

protected:
	DECLARE_MESSAGE_MAP()
	virtual int CUTMFCDlgNew::vf_GetReservedData();
};

#pragma warning( default : 4251 )
