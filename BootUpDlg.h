#if !defined(AFX_BOOTUPDLG_H__DD64F0CD_DF57_460B_A6BC_B15C90DA90F7__INCLUDED_)
#define AFX_BOOTUPDLG_H__DD64F0CD_DF57_460B_A6BC_B15C90DA90F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BootUpDlg.h : header file
//
#include <atlbase.h>

/////////////////////////////////////////////////////////////////////////////
// CBootUpDlg dialog

class CBootUpDlg : public CDialog
{
// Construction
public:
	void RenderTitle(CDC* pDC);
	BITMAPFILEHEADER m_BmpFileHeader;
	BITMAPINFOHEADER m_BmpHeader;
	COLORREF* m_pImage;

	operator IPicture*() {return m_spIPicture;}
		
	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const {
		cx = cy = 0;
		const_cast<CBootUpDlg*>(this)->m_hr = m_spIPicture->get_Width(&cx);
		ASSERT(SUCCEEDED(m_hr));
		const_cast<CBootUpDlg*>(this)->m_hr = m_spIPicture->get_Height(&cy);
		ASSERT(SUCCEEDED(m_hr));
	}

	void Free() {
		if (m_spIPicture) {
			m_spIPicture.Release();
		}
	}

	BOOL RenderJPG(CDC* pDC, CRect rc=CRect(0,0,0,0),LPCRECT prcMFBounds=NULL) const;
	void RenderBMP(CDC* pDC, CRect rc=CRect(0,0,0,0));
	CSize GetImageSize(CDC* pDC=NULL) const;
	BOOL RetrieveJPGImage(CString FileName);
	BOOL Load(CArchive& ar);
	BOOL Load(IStream* pstm);
	bool RetrieveBMPImage(CString FileName);
	bool FindFile(CString *pBuff);
	void UpdatePrompt(CString Prompt);
	CString m_FileName;
	CBootUpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBootUpDlg)
	enum { IDD = IDD_BOOTUP_DIALOG };
	CStatic	m_staticTitle;
	CStatic	m_staticView;
	CEdit	m_editPrompt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBootUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComQIPtr<IPicture>m_spIPicture;		 // ATL smart pointer to IPicture
	HRESULT m_hr;								 // last error code

	// Generated message map functions
	//{{AFX_MSG(CBootUpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOTUPDLG_H__DD64F0CD_DF57_460B_A6BC_B15C90DA90F7__INCLUDED_)
