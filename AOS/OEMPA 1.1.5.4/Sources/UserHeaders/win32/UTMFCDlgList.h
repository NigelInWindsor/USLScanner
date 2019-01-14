
#pragma once
#include "afxwin.h"

// CUTMFCDlgList dialog

class UTKERNEL_MFC_API_ALIGN _CUTMFCDlgList
{
private:
	ULONGLONG m_ullPrivateMemory[1];
public:
	_CUTMFCDlgList();

};

class UTKERNEL_MFC_API_ALIGN CUTMFCDlgList : public CDialog, public CUTMFCSelection, public _CUTMFCDlgList
{
	DECLARE_DYNAMIC(CUTMFCDlgList)

public:
	CUTMFCDlgList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUTMFCDlgList();
	virtual INT_PTR DoModal();
	void UpdateDialog();
	void DisplayThisProcessOnly(bool bEnable);
	CUTMFCSelection* GetObjectSelect(){return (dynamic_cast<CUTMFCSelection*>(this));}


protected:
	DECLARE_MESSAGE_MAP()
};

