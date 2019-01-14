#if !defined(AFX_PRINTERDIAGNOSTICSPAGE_H__7DC08352_E0DE_43F9_80FE_D5CBE10E4F02__INCLUDED_)
#define AFX_PRINTERDIAGNOSTICSPAGE_H__7DC08352_E0DE_43F9_80FE_D5CBE10E4F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrinterDiagnosticsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrinterDiagnosticsPage dialog

class CPrinterDiagnosticsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrinterDiagnosticsPage)

// Construction
public:
	
	void UpdateAllControls();
	CPrinterDiagnosticsPage();
	~CPrinterDiagnosticsPage();

// Dialog Data
	//{{AFX_DATA(CPrinterDiagnosticsPage)
	enum { IDD = IDD_PRINTER_DIAGNOSTICS_PAGE };
	CString	m_strBytesPerPixel;
	CString	m_strPaperSize;
	CString	m_strPrintableArea;
	CString	m_strResolution;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrinterDiagnosticsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPrinterDiagnosticsPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTERDIAGNOSTICSPAGE_H__7DC08352_E0DE_43F9_80FE_D5CBE10E4F02__INCLUDED_)
