#pragma once
#include "ListItemEdit.h"
#include "USLMFCListCtrl.h"

// CCoordArrayPane

class CCoordArrayPane : public CDockablePane
{
	DECLARE_DYNAMIC(CCoordArrayPane)
	DECLARE_MESSAGE_MAP()
	enum ColumnAxis {
		_X1 = 0,
		_Y1,
		_Z1,
		_Xt1,
		_Yt1,
		_r1,
		_i1,
		_j1,
		_k1,
		_w1,
		_X2,
		_Y2,
		_Z2,
		_Xt2,
		_Yt2,
		_r2,
		_i2,
		_j2,
		_k2,
		_w2,
		_eol,
		_ref_text,
		_ref_type,
		_ref_vector_length,
	};

public:
	int	m_nID;
	CString	m_strRefName[6];
	CString m_strRefType[2];
	CString m_LineStyles[4];
	CListItemEdit* m_pListItemEdit;
	void FillLineList();
	void EnsureLineListVisible();
	void FillList();
	void CreateColumns();
	void AdjustLayout();
	void CreateFont();
	void ChangeFontSize();
	CCoordArrayPane();
//	CCoordArrayPane(int nIDHighBitMap, bool bCombo, bool bList, int nIDDlg);
	virtual ~CCoordArrayPane();

	void SetVSDotNetLook(BOOL bSet)
	{
//		m_wndPropList.SetVSDotNetLook(bSet);
//		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont		m_Font;
	CUSLMFCListCtrl	m_listCoords;
	CUSLMFCListCtrl	m_listLines;
	int	m_nNumberColumns;
	CString m_ColumnName[30];
	int	m_nColumnAxis[30];
	int	m_nIndex;
	int	m_nColumn;
	int	m_nLine;
	int m_nLineSize;
	CPolyCoord* m_pCpArray;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnGetdispinfoListCoords(NMHDR *pNMHDR, LRESULT *pResult);
	void GetdispinfoListRef(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnDblclkListCoords(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListRef(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnColumnclickListCoords(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListLines(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnClickListCoords(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListLines(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetdispinfoListLines(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnKeydownListCoords(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnKeydownListLines(NMHDR * pNMHDR, LRESULT * pResult);
	void SetWindowTitle();
	afx_msg void OnRclickListLines(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnRclickListCoords(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnLinesCopy();
	afx_msg void OnLinesPaste();
	afx_msg void OnLinesCut();
	afx_msg void OnCoordsCut();
	afx_msg void OnCoordsCopy();
	afx_msg void OnCoordsPaste();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLinesAllSpline();
	afx_msg void OnLinesAllLinear();
	afx_msg void OnBraneCopy();
	afx_msg void OnBranePaste();
};


