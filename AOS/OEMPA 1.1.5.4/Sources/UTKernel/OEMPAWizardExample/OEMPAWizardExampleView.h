// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// OEMPAWizardExampleView.h : interface of the COEMPAWizardExampleView class
//

#pragma once

class COEMPAWizardExampleDoc;
class CHWDlg;

class COEMPAWizardExampleView : public CEditView
{
protected: // create from serialization only
	COEMPAWizardExampleView();
	DECLARE_DYNCREATE(COEMPAWizardExampleView)

// Attributes
public:
	COEMPAWizardExampleDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~COEMPAWizardExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	int OpenTemplate(CString strFile);
	bool SaveTemplate(CString strFile);
	bool EditTemplate();

	static CHWDlg *g_pDlg;

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonHw();
	afx_msg void OnUpdateButtonHw(CCmdUI *pCmdUI);
	afx_msg void OnButtonFlash();
	afx_msg void OnUpdateButtonFlash(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in OEMPAWizardExampleView.cpp
inline COEMPAWizardExampleDoc* COEMPAWizardExampleView::GetDocument() const
   { return reinterpret_cast<COEMPAWizardExampleDoc*>(m_pDocument); }
#endif

