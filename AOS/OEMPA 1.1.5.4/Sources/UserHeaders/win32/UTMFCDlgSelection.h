
#pragma once
#include "afxwin.h"

// CUTMFCDlgSelection dialog

class _CUTMFCDlgSelection
{
public:
	_CUTMFCDlgSelection();

};

class CUTMFCDlgSelection : public CDialog,
	public CUTMFCSelection,//public CUTKernelFile,
	public _CUTMFCDlgSelection
{
friend class CUTMFCSelection;
friend class CUTMFCContainer;
	DECLARE_DYNAMIC(CUTMFCDlgSelection)

public:
	CUTMFCDlgSelection(bool bInitFromKernel,bool bRunningVersionOnly=false,CWnd* pParent=NULL);
	virtual ~CUTMFCDlgSelection();
	virtual INT_PTR DoModal();
	void UpdateDialog(CUTMFCSelection &old);
	void InitFromKernel();
	void InitRootList();
	void UpdateLanguage();


protected:
	DECLARE_MESSAGE_MAP()
public:
	//CButton m_CheckWizardOutputs;
};
