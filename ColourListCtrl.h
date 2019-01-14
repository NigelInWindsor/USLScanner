#if !defined(AFX_COLOURLISTCTRL_H__770A2B29_0233_410C_B592_F0FCB9B85C76__INCLUDED_)
#define AFX_COLOURLISTCTRL_H__770A2B29_0233_410C_B592_F0FCB9B85C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColourListCtrl.h : header file
//


struct	RowFormat {
	int			*nFlag;
	COLORREF	*rgbText;
	COLORREF	*rgbBk;
};

/////////////////////////////////////////////////////////////////////////////
// CColourListCtrl window

class CColourListCtrl : public CListCtrl
{
// Construction
public:
	int	m_nRows;
	int	m_nColumns;
	RowFormat* m_pRow;

	CColourListCtrl();
	DECLARE_DYNCREATE(CColourListCtrl)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColourListCtrl)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Zero();
	void SetTextRowColor(int nRow, COLORREF rgb);
	void SetBkRowColor(int nRow, COLORREF rgb);
	void ResetColors();
	virtual ~CColourListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColourListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void CustomDrawList( NMHDR * pNotifyStruct, LRESULT * result );

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURLISTCTRL_H__770A2B29_0233_410C_B592_F0FCB9B85C76__INCLUDED_)
