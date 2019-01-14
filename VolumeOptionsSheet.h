#if !defined(AFX_VOLUMEOPTIONSSHEET_H__F167D0C2_F393_46F7_AAA4_630574C182A7__INCLUDED_)
#define AFX_VOLUMEOPTIONSSHEET_H__F167D0C2_F393_46F7_AAA4_630574C182A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeOptionsSheet.h : header file
//
#include "VolumeAscanPage.h"
#include "GraticulePage.h"
#include "ZoomPage.h"
#include "RulerPage.h"
#include "HistogramOptionsPage.h"
#include "ImageFilterPage.h"
#include "ViewValuePage.h"
#include "RotaryOptionsPage.h"
#include "CorrelationPage.h"
#include "VolumeLibraryPage.h"

/////////////////////////////////////////////////////////////////////////////
// CVolumeOptionsSheet

class CVolumeOptionsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CVolumeOptionsSheet)

// Construction
public:
	CCorrelationPage* m_pCorrelationPage;
	CRotaryOptionsPage* m_pRotaryOptionsPage;
	CVolumeAscanPage* m_pVolumeAscanPage;
	CViewValuePage* m_pValuePage;
	CImageFilterPage* m_pFilterPage;
	CHistogramOptionsPage* m_pHistogramOptionsPage;
	CRulerPage* m_pRulerPage;
	CZoomPage* m_pZoomPage;
	CGraticulePage*	m_pGraticulePage;
	CVolumeLibraryPage*	m_pLibraryPage;
	void Construct(CWnd* pParent,CWnd** pDlg,GatesData* pGate,CData* pData, CWnd* pViewSheet);
	GatesData* m_pGate;
	CWnd* m_pViewSheet;
	CWnd** m_pDlg;
	CData* m_pData;
	CWnd* m_pParent;
	CVolumeOptionsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,GatesData *pGate = NULL,CData* pData = NULL,CWnd* pViewSheet = NULL);
	CVolumeOptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,GatesData *pGate = NULL,CData* pData = NULL, CWnd* pViewSheet = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVolumeOptionsSheet)
	public:
	virtual BOOL OnInitDialog();
	afx_msg HRESULT UpdateAllPages(WPARAM, LPARAM);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVolumeOptionsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVolumeOptionsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEOPTIONSSHEET_H__F167D0C2_F393_46F7_AAA4_630574C182A7__INCLUDED_)
