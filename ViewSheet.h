#if !defined(AFX_VIEWSHEET_H__338EA87F_0182_4021_B84E_730BE5E2FA45__INCLUDED_)
#define AFX_VIEWSHEET_H__338EA87F_0182_4021_B84E_730BE5E2FA45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSheet.h : header file
//
//#include "..\Resizable\ResizableLib\ResizableSheet.h"
#include "ResizableSheet.h"
#include "ViewPage.h"
#include "Data.h"	// Added by ClassView
#include "ViewOptionsSheet.h"
#include "HistogramDlg.h"
#include "ReportPrintDlg1.h"
#include "VolumeViewDlg.h"
#include "DetailsPage.h"
#include "ViewUltrasonicsDlg.h"
#include "ViewStatsDlg.h"
#include "XMLRulesDlg.h"
#include "SentenceDlg.h"
#include "ViewCommentsDlg.h"
#include "ToolPathPage.h"
#include "CADSheet.h"
#include "PrintConesCylindersDlg.h"
#include "CADTableDlg.h"
#include "ViewThicknessCrossSectionDlg.h"
#include "DataExportRulesDlg.h"
#include "DataPropertiesDlg.h"	// Added by ClassView
#include "MosaicDlg.h"
#include "InclusionsSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CViewSheet

class CViewSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CViewSheet)

// Construction
public:

	void One_to_One();
	void OnSave();
	CCADSheet* m_pCADSheet;

	void CreatePhaseOverlayPage(int nAction);
	void OpenWordPad(CString FileName);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CViewThicknessCrossSectionDlg* m_pViewThicknessCrossSectionDlg;
	CPrintConesCylindersDlg* m_pPrintConesCylindersDlg;
	CXMLRulesDlg* m_pXMLRulesDlg;
	CReportPrintDlg* m_pReportPrintDlg;
	CHistogramDlg* m_pHistogramDlg;
	CViewOptionsSheet* m_pViewOptionsSheet;
	CCADTableDlg* m_pCADTableDlg;
	CDataExportRulesDlg* m_pDataExportRulesDlg;
	CToolBar	m_wndPaletteToolBar;
	CToolBar	m_wndCADToolBar;
	CToolBar	m_wndOrientationToolBar;
	CToolBar    m_wndArchiveToolBar;
	CToolBar    m_wndToolsToolBar;
	CDialogBar	m_wndDialogBar;
	CReBar		m_wndRebar;
	CMenu		*m_pMenu;
	CDetailsPage* m_pDetailsPage;
	CString m_FilePath;
	CInclusionsSheet* m_pInclusionsSheet;
	CViewPage* m_pViewPage[256];
	CViewPage* m_pViewChildPage[256];
	CVolumeViewDlg* m_pVolumeView;
	CViewUltrasonicsDlg* m_pViewUltrasonicsDlg;
	CViewStatsDlg* m_pViewStatsDlg;
	CViewCommentsDlg* m_pViewCommentsDlg;
	CToolPathPage* m_pToolPathPage;
	CMosaicDlg* m_pMosaicDlg;
	CString	m_Name[256];
	
	CWnd** m_pDlg;
	CViewSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CViewSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,LPCTSTR FilePath = NULL);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void FileSaveas();
	void FileProperties();
	CDataPropertiesDlg *m_pDataPropertiesDlg;
	bool IsContinueScanAllowable();
	void InvalidateChildren();
	void OpenChildView();
	void RefreshCADTable();
	bool m_bRenderPolygons;
	void ChangePaletteOfActivePage(int nPaletteNumber);
	void PrintWusiWug(float fScale);

	void CreateImageProcessPage(int nAction,int nSrcType, int nSrc,int nDest);
	void RefreshViewOptionsSheet();
	void SetWindowTitle();
	bool InitializeFromUt(int nScanType);
	void InitializeFromProfile(int nScanType);
	void SetToolBarCheckedState();
	void EditTextSetWindowText(CString Buff);
	void EditTextGetWindowText(CString &pBuff);
	void InvalidateDueToHistogramChange();
	void InvalidateChild();
	void InvalidateChildCommandedFromScanThread();
	void SetTabNames();
	void AllocatePages(int nScanType);
	int	m_nZoomFactor;
	int	m_nToolOption;
	int m_nRectPolarDisplayMode;
	int	m_nScanLineDirection;
	bool m_bRenderAnnotation;
	bool m_bRenderProfile;
	bool m_bRenderWaypoints;
	bool m_bRenderEnvelope;
	bool m_bRenderBrane;
	bool m_bRenderDirChangeX;
	bool m_bRenderDirChangeY;
	bool m_bRenderDirChangeZ;
	bool m_bRenderDirChangeXtip;
	bool m_bRenderDirChangeZtip;
	bool m_bRenderZeroXtip;
	bool m_bRenderZeroZtip;
	bool m_bCtrlButton;
	bool m_bShiftButton;
	bool m_bCompatibleFile;
	CRect m_rrSamples;
	CData m_Data;
	virtual ~CViewSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnZoom100();
	afx_msg void OnZoom50();
	afx_msg void OnZoom25();
	afx_msg void OnZoom10();
	afx_msg void OnToolsOptions();
	afx_msg void OnToolsRuler();
	afx_msg void OnToolsHistogram();
	afx_msg void OnFileSaveas();
	afx_msg void OnToolsPointer();
	afx_msg void OnToolsValue();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnButtonZoomIn();
	afx_msg void OnButtonZoomOut();
	afx_msg void OnButtonAnnotation();
	afx_msg void OnFileSavepageinbmp();
	afx_msg void OnFileSavePageinCSV();
	afx_msg void OnFileDetails();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileSave();
	afx_msg void OnFileUltrasonics();
	afx_msg void OnFileStats();
	afx_msg void OnZoom1Pxto1Sample();
	afx_msg void OnFileXmlSavepage();
	afx_msg void OnFileXmlRules();
	afx_msg void OnChangeEditEditText();
	afx_msg void OnButtonOverlayProfile();
	afx_msg void OnButtonHorizontalScanLines();
	afx_msg void OnButtonVerticalScanlines();
	afx_msg void OnToolsPolygon();
	afx_msg void OnButtonPalette();
	afx_msg void OnButtonOverlayBrane();
	afx_msg void OnButtonSentence();
	afx_msg void OnButtonComments();
	afx_msg void OnUpdateVolumeView(CCmdUI* pCmdUI);
	afx_msg void OnToolsCad();
	afx_msg void OnWaypointsView();
	afx_msg void OnDiagDirChangeX();
	afx_msg void OnDiagDirChangeY();
	afx_msg void OnDiagDirChangeZ();
	afx_msg void OnDiagDirChangeXtip();
	afx_msg void OnDiagDirChangeZtip();
	afx_msg void OnDiagZeroXtip();
	afx_msg void OnDiagZeroZtip();
	afx_msg void OnPrint11();
	afx_msg void OnPrint12();
	afx_msg void OnPrint14();
	afx_msg void OnFileExportCIP();
	afx_msg void OnEnvelopeView();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonZoomVariable();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnFileExportClipboard();
	afx_msg void OnViewConesandcylinders();
	afx_msg void OnViewReport();
	afx_msg void OnViewVolume();
	afx_msg void OnButton1to1();
	afx_msg void OnButtonToolsRectangle();
	afx_msg void OnButtonToolsEllipse();
	afx_msg void OnButtonToolsLine();
	afx_msg void OnToolsCrosshair();
	afx_msg void OnToolsCursor();
	afx_msg void OnButtonToolsParallelogram();
	afx_msg void OnFileAppendtoamultiplescanfile();
	afx_msg void OnFileExportAptsource();
	afx_msg void OnDiagnosticsCreateProfile();
	afx_msg void OnButtonToolsCentreLine();
	afx_msg void OnToolsCrossSection();
	afx_msg void OnFileExportAptsourceRules();
	afx_msg void OnFileExportAptsourceSave();
	afx_msg void OnWindowNew();
	afx_msg void OnWindowAllnew();
	afx_msg void OnWindowTilehorizontally();
	afx_msg void OnWindowCloseall();
	afx_msg void OnWindowTilevertically();
	afx_msg void OnFileExportRules();
	afx_msg void OnButtonPal0();
	afx_msg void OnButtonPal1();
	afx_msg void OnButtonPal2();
	afx_msg void OnButtonPal3();
	afx_msg void OnButtonPal4();
	afx_msg void OnButtonPal6();
	afx_msg void OnButtonPal7();
	afx_msg void OnButtonPal5();
	afx_msg void OnFileProperties();
	afx_msg void OnFileMosaic();
	afx_msg void OnButtonSquash();
	afx_msg void OnViewInclusions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSHEET_H__338EA87F_0182_4021_B84E_730BE5E2FA45__INCLUDED_)
