#if !defined(AFX_PROFILEWORKSHEETDLG_H__F9128C93_5045_48A7_9C49_6665531B3BDD__INCLUDED_)
#define AFX_PROFILEWORKSHEETDLG_H__F9128C93_5045_48A7_9C49_6665531B3BDD__INCLUDED_

#if _MSC_VER > 1000
#include <atlbase.h>
#pragma once
#endif // _MSC_VER > 1000
// ProfileWorksheetDlg.h : header file
//

#include "WorksheetData.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CProfileWorksheetDlg dialog

class CProfileWorksheetDlg : public CDialog
{
// Construction
public:
	CProfileWorksheetDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor
	CWnd** m_pDlg;

// Dialog Data
	//{{AFX_DATA(CProfileWorksheetDlg)
	enum { IDD = IDD_PROFILEWORKSHEET_DIALOG };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileWorksheetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	//JPG Image Stuff
	operator IPicture*() {return m_spIPicture;}
		
	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const {
		cx = cy = 0;
		const_cast<CProfileWorksheetDlg*>(this)->m_hr = m_spIPicture->get_Width(&cx);
		ASSERT(SUCCEEDED(m_hr));
		const_cast<CProfileWorksheetDlg*>(this)->m_hr = m_spIPicture->get_Height(&cy);
		ASSERT(SUCCEEDED(m_hr));
	}

	void Free() {
		if (m_spIPicture) {
			m_spIPicture.Release();
		}
	}

	BOOL Render(CDC* pDC, CRect rc=CRect(0,0,0,0),LPCRECT prcMFBounds=NULL) const;
	BOOL Load(CFile& file);
	BOOL Load(CArchive& ar);
	BOOL Load(IStream* pstm);
	CSize GetImageSize(CDC* pDC=NULL) const;

	CWorksheetData *m_pDataDlg;

	void DrawImage(CDC *pDC, CRect rr);
	void DrawText(CDC *pDC, CRect rr);

	CStaticPosManage m_StaticPosView;

	/* Data Items To be moved more globally */
public:
	CString m_Project;
	CString m_ProductNum;
	CString m_Product;

	CString m_HoldingFixture;
	CString m_Carriage1[3];
	CString m_Carriage2[3];
	CString m_FixtureNote;

	CString m_ProbeFrequency;
	CString m_SqrtLenLeft;
	CString m_SqrtOpenLeft;
	CString m_SqrtLenRight;
	CString m_SqrtOpenRight;

	CString m_Note;
	CString ImageFilename;

protected:
	CComQIPtr<IPicture>m_spIPicture;		 // ATL smart pointer to IPicture
	HRESULT m_hr;								 // last error code
	// Generated message map functions
	//{{AFX_MSG(CProfileWorksheetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditData();
	afx_msg void OnFilePrint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEWORKSHEETDLG_H__F9128C93_5045_48A7_9C49_6665531B3BDD__INCLUDED_)
