#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCustomSpinButtonCtrl window

class CMFCToolBarSpinButton;

class CCustomSpinButtonCtrl : public CSpinButtonCtrl
{
	// Construction
public:
	CCustomSpinButtonCtrl(CMFCToolBarSpinButton& btnSpinButton);

	// Attributes
public:
	CMFCToolBarSpinButton & m_btnSpinButton;

	// Operations
public:
	// Overrides

	// Implementation
public:
	virtual ~CCustomSpinButtonCtrl();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zdelta, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CMFCToolBarSpinButton : public CMFCToolBarButton
{
	DECLARE_SERIAL(CMFCToolBarSpinButton)

public:
	CMFCToolBarSpinButton();
	CMFCToolBarSpinButton(UINT uiId,
		int iImage = -1,
		DWORD dwStyle = 0,
		int iWidth = 0);

	virtual ~CMFCToolBarSpinButton();

	void SetRange(int iMin, int iMax);
	void SetValue(int iValue, BOOL bNotify = TRUE);

	static int SetRange(UINT uiCmd, int iMin, int iMax);
	static int SetPos(UINT uiCmd, int iValue);
	static int GetPos(UINT uiCmd);
	

	// Overrides:
	virtual void Serialize(CArchive& ar);
	virtual void CopyFrom(const CMFCToolBarButton& src);
	virtual SIZE OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnShow(BOOL bShow);
	virtual void OnChangeParentWnd(CWnd* pWndParent);
	virtual void OnMove();
	virtual void OnSize(int iSize);
	virtual HWND GetHwnd()
	{
		return m_wndSpinButton.GetSafeHwnd();
	}

	virtual BOOL CanBeStretched() const
	{
		return TRUE;
	}

	virtual BOOL HaveHotBorder() const
	{
		return FALSE;
	}

	virtual void OnDraw(CDC* /*pDC*/, const CRect& /*rect*/, CMFCToolBarImages* /*pImages*/,
		BOOL /*bHorz*/ = TRUE, BOOL /*bCustomizeMode*/ = FALSE,
		BOOL /*bHighlight*/ = FALSE,
		BOOL /*bDrawBorder*/ = TRUE,
		BOOL /*bGrayDisabledButtons*/ = TRUE) {}
	// Attributes:
public:
	CCustomSpinButtonCtrl	m_wndSpinButton;
	int				m_iWidth;
	DWORD			m_dwStyle;
	int				m_nMin;
	int				m_nMax;
	int				m_nValue;
};





