#if !defined(AFX_IMPORTTOOLPATHSDLG_H__E85A290C_9700_43A1_897F_9586CC135695__INCLUDED_)
#define AFX_IMPORTTOOLPATHSDLG_H__E85A290C_9700_43A1_897F_9586CC135695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportToolPathsDlg.h : header file
//
#include "ColourListCtrl.h"
#include "StaticScroll.h"
#include "DoubleScrollBar.h"

/////////////////////////////////////////////////////////////////////////////
// CImportToolPathsDlg dialog

#define	HIGHLIGHTED		1
#define	LINE_COORDINATE	2
#define	START_TOOLPATH	4
#define	FINISH_TOOLPATH	8
#define	EOL_TOOLPATH	0x10
#define	START_REF_PTS	0x20
#define	REF_PTS			0x40
#define START_ANC_PTS	0x80
#define	START_ENVELOPE	0x100
#define	FINISH_ENVELOPE	0x200

class CImportToolPathsDlg : public CDialog
{
// Construction
public:
	void ImportEnvelopes();
	void ImportCloudSet();
	void ImportAscii();
	void ImportAptSource();
	void SetToolBarCheckedState();
	void FillListEOP();
	void CreateColumnsEOP();
	CStringArray m_str;
	CString m_FileNamePath;
	CSize m_size;
	CPoint m_ptClient;
	bool m_bLBDown;
	bool m_bRBDown;
	bool m_bShiftKey;
	bool m_bCtrlKey;
	unsigned int *m_pFlag;
	int	m_nShiftIndex;
	int m_nRowsL;
	int m_nTopRow;
	int m_nLength;
	int m_nIndex;
	int	m_nToolPathL;

	void UpdateMainFrameTools();
	void SetWindowTitle();
	void SetFlags();
	void SetFlagsAptSource();
	void SetFlagsAscii();
	bool RetrieveFile(CString FilePathName);
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndImportToolBar;
	CReBar     m_wndRebar;
	CWnd** m_pDlg;
	
	CImportToolPathsDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportToolPathsDlg)
	enum { IDD = IDD_IMPORT_TOOLPATHS_DLG };
	CComboBox	m_comboExtractionMethod;
	CButton	m_checkLineNumbers;
	CListCtrl	m_listEOP;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CEdit	m_editSlowIncrement;
	CComboBox	m_comboDirection;
	CStatic	m_staticFinishEnv;
	CStatic	m_staticStartEnv;
	CEdit	m_editStartEnvelope;
	CEdit	m_editFinishEnvelope;
	CEdit	m_editCoordinate;
	CStatic	m_staticStartTP;
	CStatic	m_staticREF;
	CStatic	m_staticFinishTP;
	CStatic	m_staticEOL;
	CStatic	m_staticAnc;
	CProgressCtrl	m_ProgressBar;
	CComboBox	m_comboUnits;
	CComboBox	m_comboDestination;
	CEdit	m_editAncillaryPts;
	CEdit	m_editRefPoints;
	CDoubleScrollBar	m_scrollVert;
	CStatic	m_staticView;
	CEdit	m_editContent;
	CColourListCtrl	m_listContent;
	CEdit	m_editLineSeperator;
	CEdit	m_editStartToolPath;
	CEdit	m_editFinishToolPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportToolPathsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportToolPathsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnImportToolpathstart();
	afx_msg void OnImportToolpatheol();
	afx_msg void OnImportToolpathfinish();
	afx_msg void OnImportRefpointsstart();
	afx_msg void OnImportAncillaryptsstart();
	afx_msg void OnChangeEditAncillaryPts();
	afx_msg void OnChangeEditFinishToolpaths();
	afx_msg void OnChangeEditLineSeperator();
	afx_msg void OnChangeEditRefPts();
	afx_msg void OnChangeEditStartToolpaths();
	afx_msg void OnSelchangeComboDestination();
	afx_msg void OnSelchangeComboUnits();
	afx_msg void OnImportCopycoordinate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	bool ExtractCoordinate(CString & Buff, CCoord * pCp);
	void OnButtonImport();
	afx_msg void OnImportGotoToolpathstart();
	afx_msg void OnImportGotoToolpathfinish();
	afx_msg void OnImportGotoRefpoints();
	afx_msg void OnImportGotoAncillarypoints();
	afx_msg void OnChangeEditCoordinate();
	afx_msg void OnImportCoordinate();
	afx_msg void OnChangeEditStartEnvelope();
	afx_msg void OnChangeEditFinishEnvelope();
	afx_msg void OnImportEnvelopestart();
	afx_msg void OnImportEnvelopefinish();
	afx_msg void OnSelchangeComboDirection();
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonImportAscii();
	afx_msg void OnGetdispinfoListEop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListEop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListEop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListEop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEopDelete();
	afx_msg void OnCheckLineNumbers();
	afx_msg void OnButtonImportAptsource();
	afx_msg void OnSelchangeComboExtractMethod();
	afx_msg void OnButtonImportCloudset();
	afx_msg void OnButtonImportEnvelope();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTTOOLPATHSDLG_H__E85A290C_9700_43A1_897F_9586CC135695__INCLUDED_)
