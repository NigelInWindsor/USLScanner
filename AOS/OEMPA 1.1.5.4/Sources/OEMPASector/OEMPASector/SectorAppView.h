
// SectorAppView.h : interface of the CSectorAppView class
//

#pragma once


class CSectorAppView : public CView
{
protected: // create from serialization only
	CSectorAppView();
	DECLARE_DYNCREATE(CSectorAppView)

// Attributes
public:
	CSectorAppDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSectorAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SectorWeldView.cpp
inline CSectorAppDoc* CSectorAppView::GetDocument() const
   { return reinterpret_cast<CSectorAppDoc*>(m_pDocument); }
#endif

