#if !defined(AFX_VIEWOPTIONSSHEET_H__AF5AA065_47F4_4A92_A37B_D21A6CE3A763__INCLUDED_)
#define AFX_VIEWOPTIONSSHEET_H__AF5AA065_47F4_4A92_A37B_D21A6CE3A763__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewOptionsSheet.h : header file
//

#include "GraticulePage.h"
#include "ZoomPage.h"
#include "RulerPage.h"
#include "HistogramOptionsPage.h"
#include "ImageFilterPage.h"
#include "ViewValuePage.h"
#include "EdgeEqualizerPage.h"
#include "RotaryOptionsPage.h"
#include "PAEdgeEqualizerPage.h"
#include "ImageProcessorPage.h"
#include "OverlayPage.h"
#include "EnvelopePage.h"
#include "LEAlignPage.h"
#include "BladeTolerancePage.h"
#include "BladeMasterPage.h"
#include "BladeCSVPage.h"
#include "PhasePage.h"


/////////////////////////////////////////////////////////////////////////////
// CViewOptionsSheet

class CViewOptionsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CViewOptionsSheet)

// Construction
public:
	CPhasePage* m_pPhasePage;
	CBladeCSVPage* m_pBladeCSVPage;
	CBladeMasterPage* m_pBladeMasterPage;
	CLeAlignPage* m_pLeAlignPage;
	CBladeTolerancePage* m_pBladeTolerancePage;
	CEnvelopePage* m_pEnvelopePage;
	COverlayPage* m_pOverlayPage;
	CImageProcessorPage* m_pImageProcessorPage;
	CPAEdgeEqualizerPage* m_pPAEdgeEqualizerPage;
	CRotaryOptionsPage* m_pRotaryOptionsPage;
	CEdgeEqualizerPage* m_pEdgePage;
	CViewValuePage* m_pValuePage;
	CImageFilterPage* m_pFilterPage;
	CHistogramOptionsPage* m_pHistogramOptionsPage;
	CRulerPage* m_pRulerPage;
	CZoomPage* m_pZoomPage;
	CGraticulePage*	m_pGraticulePage;
	void Construct(CWnd* pParent,CWnd** pDlg,CData* pData,int nOptions);
	CWnd* m_pViewSheet;
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CViewOptionsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg=NULL,CData* pData=NULL,int nOptions = 0);
	CViewOptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg=NULL,CData* pData=NULL, int nOptions = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewOptionsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	virtual ~CViewOptionsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewOptionsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWOPTIONSSHEET_H__AF5AA065_47F4_4A92_A37B_D21A6CE3A763__INCLUDED_)
