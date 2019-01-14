#if !defined(AFX_QUATERNIONPAGE_H__15981159_4EEE_4EEB_BAF9_C53F7B801437__INCLUDED_)
#define AFX_QUATERNIONPAGE_H__15981159_4EEE_4EEB_BAF9_C53F7B801437__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuaternionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQuaternionPage dialog

class CQuaternionPage : public CDialog
{
	DECLARE_DYNCREATE(CQuaternionPage)

// Construction
public:
	void RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXVECTOR3 vec);
	void RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXVECTOR4 vec);
	void RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXQUATERNION q);
	void RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, D3DXMATRIX m);
	void RenderText(CDC* pDC, CPoint* pt, COLORREF rgb, int nLines, CString Buff);
	D3DXVECTOR4	m_vSurface;
	COLORREF	*m_pArray;
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	
	CWnd** m_pDlg;
	void UpdateAllControls();
	CQuaternionPage(CWnd* pParent = NULL,CWnd** pDlg = NULL);
	~CQuaternionPage();

// Dialog Data
	//{{AFX_DATA(CQuaternionPage)
	enum { IDD = IDD_QUATERNION_PAGE };
	CSliderCtrl	m_sliderV;
	CSliderCtrl	m_sliderZ;
	CSliderCtrl	m_sliderY;
	CSliderCtrl	m_sliderX;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CQuaternionPage)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CQuaternionPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUATERNIONPAGE_H__15981159_4EEE_4EEB_BAF9_C53F7B801437__INCLUDED_)
