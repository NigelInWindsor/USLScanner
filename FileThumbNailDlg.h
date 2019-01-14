#if !defined(AFX_FILETHUMBNAILDLG_H__78EEB2AD_C79E_4D19_AE9D_26CACD78257B__INCLUDED_)
#define AFX_FILETHUMBNAILDLG_H__78EEB2AD_C79E_4D19_AE9D_26CACD78257B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileThumbNailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileThumbNailDlg dialog

class CFileThumbNailDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CFileThumbNailDlg)

public:
	
	~CFileThumbNailDlg();
	CFileThumbNailDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

   	void UpdatePreview(CString csTemp);

protected:
// Dialog Data
	//{{AFX_DATA(C3DViewEditDlg)
	enum { IDD = IDD_FILETHUMBNAIL_DIALOG };
	CStatic m_staticPreview;
	CButton m_buttonCheck;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DViewEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL    OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFileThumbNailDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETHUMBNAILDLG_H__78EEB2AD_C79E_4D19_AE9D_26CACD78257B__INCLUDED_)
