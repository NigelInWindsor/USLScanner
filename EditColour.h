#if !defined(AFX_EDITCOLOUR_H__B9369A2D_18C8_4297_95F8_C081200AD232__INCLUDED_)
#define AFX_EDITCOLOUR_H__B9369A2D_18C8_4297_95F8_C081200AD232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditColour.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditColour window

class CEditColour : public CEdit
{
// Construction
public:
	CEditColour();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditColour)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextColour(CString Buff,COLORREF rgbColour);
	BOOL SetReadOnly(BOOL flag);
	void EnableSpinCtrl();
	void SetBkColor(COLORREF rgbColor);
	void SetTextColour(COLORREF rgbColour);
	virtual ~CEditColour();

	// Generated message map functions
protected:
	CSpinButtonCtrl m_spinEdit;
	CBrush	m_brushBkgnd; // Holds Brush Color for the Edit Box
	COLORREF m_rgbBkColour; // Holds the Background Color for the Text
	COLORREF m_rgbTextColour; // Holds the Color for the Text
	//{{AFX_MSG(CColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor); // This Function Gets Called Every Time Your Window Gets Redrawn.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCOLOUR_H__B9369A2D_18C8_4297_95F8_C081200AD232__INCLUDED_)
