#if !defined(AFX_DATAVALUEPROPERTIESDLG_H__BF05EFCD_B700_4B9D_A226_1A08D74BE5EF__INCLUDED_)
#define AFX_DATAVALUEPROPERTIESDLG_H__BF05EFCD_B700_4B9D_A226_1A08D74BE5EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataValuePropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataValuePropertiesDlg dialog

class CDataValuePropertiesDlg : public CDialog
{
// Construction
public:
	void SetIndex(int nIndex);
	void UpdateAllControls();
	
	FeatureStruct* m_pFeature;
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CDataValuePropertiesDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL, FeatureStruct* pFeature=NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataValuePropertiesDlg)
	enum { IDD = IDD_DATA_VALUE_PROPERTIES_DLG };
	CComboBox	m_comboVariable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataValuePropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataValuePropertiesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboVariable();
	afx_msg void OnButtonFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAVALUEPROPERTIESDLG_H__BF05EFCD_B700_4B9D_A226_1A08D74BE5EF__INCLUDED_)
