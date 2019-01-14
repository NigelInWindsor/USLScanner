
#pragma once
#pragma warning( disable : 4251 )
/////////////////////////////////////////////////////////////////////////////
class UTKERNEL_MFC_API_ALIGN _CUTMFCTextView
{
private:
	ULONGLONG m_ullPrivateMemory[1];
public:
	_CUTMFCTextView(CRichEditCtrl* pRichEdit);

};

class UTKERNEL_MFC_API_ALIGN CUTMFCTextView : public CRichEditView, public _CUTMFCTextView
{
protected: // create from serialization only
	CUTMFCTextView();
	DECLARE_DYNCREATE(CUTMFCTextView)

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CUTMFCTextView();

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CUTMFCTreeCtrl;
class CTreeComboBox : public CComboBox
{
public:
	void Init(HTREEITEM hTreeComboBox, CUTMFCTreeCtrl *putTreeCtrl, CString strItemText);
	bool GetString(CString &string,HTREEITEM &hTreeItem);
private:
	CString			strCurrText;
	HTREEITEM m_hTreeComboBox;
	bool m_bOk;

	//{{AFX_MSG(CTreeComboBox)
	afx_msg void	OnCloseUp();
	afx_msg void	OnKillFocus();
	afx_msg void	OnEditUpdate();
	afx_msg void	OnSelChange();
	afx_msg void	OnSelEndOk();
	afx_msg void	OnSelEndCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class UTKERNEL_MFC_API_ALIGN _CUTMFCTreeView
{
private:
	ULONGLONG m_ullPrivateMemory[21];

};

class UTKERNEL_MFC_API_ALIGN CUTMFCTreeView : public _CUTMFCTreeView, public CTreeView
{
protected: // create from serialization only
	CUTMFCTreeView();
	DECLARE_DYNCREATE(CUTMFCTreeView)
public:

	void SetTitle(CString strTitle);
	void SetRootID(int iRootID);
	int GetRootID();
	void Close();

	bool IsKernelListItem();
	bool Command(wchar_t *pCmd);
	bool IsEditEnabled(CString &string,bool &bArray,bool &bEnum);

	void SetTextView(CUTMFCTextView *pTextView);
	bool ExpandAll();
	bool GetFullPath(HTREEITEM hTreeItem,CString &string,enumRootType &eRootClassID);
	bool IsPassword(HTREEITEM hTreeItem);
	bool IsEditEnabled(bool &bArray);
	bool GetStatus(int &iStatus,int &iLimitStatus);

protected:
	virtual int vf_GetReservedData();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#pragma warning( default : 4251 )
