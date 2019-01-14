#if !defined(AFX_WORKSPACEDLG_H__4EA0DEE9_889C_478B_9520_E5E76EC8674F__INCLUDED_)
#define AFX_WORKSPACEDLG_H__4EA0DEE9_889C_478B_9520_E5E76EC8674F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkSpaceDlg.h : header file
//
#include "StaticPosManage.h"
#include "EditSpinItem.h"
#include "ListCtrlCheck.h"
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceDlg dialog

struct	ItemProfileData {
	HTREEITEM hItem;
	HTREEITEM hFileName;
	HTREEITEM hLines;
	HTREEITEM hTaughtLines;
	HTREEITEM hScanLines;
	HTREEITEM hBraneLines;
	HTREEITEM hEnvelopeLines;
	HTREEITEM hUltrasonics;
};

class CWorkSpaceDlg : public CResizablePage
{
	DECLARE_DYNCREATE(CWorkSpaceDlg)

// Construction
public:
	void EraseAllFileNames();
	void SetAccessPrivelage();
	long ItemChanged(unsigned int nTemp,long nlTemp);
	int GetStartLadderIndex();
	void GetDirectory(CEdit *pEdit = NULL, CString *pInitDir = NULL);
	void SaveAsWorkSpace();
	void OpenWorkspace();
	void NewWorkspace();
	bool m_bTwoSided;
	int m_nMoveIndex;
	int m_nStrStandard;
	int m_nColumn;
	CString	m_strStatus[4];
	CString	m_strAction[3];
	CString	m_strStandard[2];
	void FillMoveLadderList();
	void CreateMoveLadderColumns();
	void FillPlayList();
	void CreatePlayListColumns();
	
	int	m_nOldScanLine;
	int m_nLastTimeDiff;
	time_t m_osLastCalibration;
	int	m_nSecondsInAStep;
	int m_nCutPasteL;
	int *m_pnCutArray;
	bool GetStartFinishLadderIndex(int *nStartIndex, int *nFinishIndex);
	int m_nLadderIndex;
	CStatic* m_pStatic;
	bool m_bBorderSelected;
	int m_nProfileIndex;
	bool m_bProfileSelected;
	void AdjustLayout();
	CRect m_rrVBorder;
	CPoint	m_ptLbDown;
	bool m_bLbDown;
	int	m_nCurrentID;
	CEdit* m_pEditCtrl;
	CImageList m_imageList;
	CBitmap m_bitmap[10];
	ItemProfileData* m_pProfileItem;
	void FillProfileList();
	void UpdateAllControls();
	CStaticPosManage m_StaticPosData;
	CStaticPosManage m_StaticPosProfile;
	CEditSpinItem* m_pEditSpinItem;
	
	CWnd** m_pDlg;
	CString m_strCopy;
	~CWorkSpaceDlg();   // standard constructor
	CWorkSpaceDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWorkSpaceDlg)
	enum { IDD = IDD_WORKSPACE_DLG };
	CButton	m_checkSwitchOffPumps;
	CSpinButtonCtrl	m_spinScanSpeed;
	CEdit	m_editScanSpeed;
	CEdit	m_editResumeLine;
	CEdit	m_editScanLine;
	CEdit	m_editLastCalTime;
	CEdit	m_editTimeDiff;
	CEdit	m_editRefImage;
	CEdit	m_editRefAttenuation;
	CListBox	m_listRefLevels;
	CStatic	m_staticView;
	CListBox	m_listProfiles;
	CEdit	m_editRefLevel;
	CComboBox	m_comboMaxScanTime;
	CSpinButtonCtrl	m_spinPositiveAttenuation;
	CSpinButtonCtrl	m_spinNegativeAttenuation;
	CEdit	m_editPositiveAttenuation;
	CEdit	m_editNegativeAttenuation;
	CEdit	m_editCorner1;
	CEdit	m_editCorner0;
	CStatic	m_staticMoveLadder;
	CListCtrl	m_listMoveLadder;
	CListCtrlCheck	m_listPlayOrder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorkSpaceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNewprofile();
	afx_msg void OnSaveAsProfile();
	afx_msg void OnDeleteProfile();
	afx_msg void OnRenameProfile();
	afx_msg void OnNewWorkspace();
	afx_msg void OnSaveAsWorkSpace();
	afx_msg void OnOpenWorkspace();
	afx_msg void OnCopyProfile();
	afx_msg void OnPasteProfile();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnProfileworkspaceAddprofile();
	afx_msg void OnClickListInspection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListInspection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListInspection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListInspection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProfileworkspaceSendtoladder();
	afx_msg void OnLadderClearcomponentvariables();
	afx_msg void OnLadderClearfilenames();
	afx_msg void OnLadderClearpathnames();
	afx_msg void OnLadderClearuniqueidentifiers();
	afx_msg void OnLadderDelete();
	afx_msg void OnLadderFilenamepathname();
	afx_msg void OnLadderCopyfilename();
	afx_msg void OnLadderCopyincfilename();
	afx_msg void OnLadderCopyincuniqueidentifier();
	afx_msg void OnLadderCopypathname();
	afx_msg void OnLadderCopyuniqueidentifier();
	afx_msg void OnLadderCut();
	afx_msg void OnLadderPaste();
	afx_msg void OnPathCopy();
	afx_msg void OnPathPaste();
	afx_msg void OnFilenameCopy();
	afx_msg void OnFilenamePaste();
	afx_msg void OnFilenamePastea1();
	afx_msg void OnDeltaposSpinNegativeAttenuation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPositiveAttenuation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboMaxScanTime();
	afx_msg void OnChangeEditNegativeAttenuation();
	afx_msg void OnChangeEditPositiveAttenuation();
	afx_msg void OnChangeEditCorner0();
	afx_msg void OnChangeEditCorner1();
	afx_msg void OnSelchangeListProfiles();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDblclkListProfiles();
	afx_msg void OnStatusReset();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonInspectionCriteria();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnCheckSwitchOffPumps();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEDLG_H__4EA0DEE9_889C_478B_9520_E5E76EC8674F__INCLUDED_)
