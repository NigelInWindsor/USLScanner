#if !defined(AFX_AXESVIRTUALPAGE_H__2EA826F1_304A_4C64_8A80_3D29590A0935__INCLUDED_)
#define AFX_AXESVIRTUALPAGE_H__2EA826F1_304A_4C64_8A80_3D29590A0935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesVirtualPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxesVirtualPage dialog

class CAxesVirtualPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAxesVirtualPage)

// Construction
public:
	void InvalidateVariables(int nAxis,int nVAxis,CEdit *peditPos,CEdit *peditVPos,CButton *pcheckInPos,CButton *pcheckVInPos,CButton *pcheckAmp,CButton *pcheckVAmp);
	int m_nPos[32];
	int	m_nStatus[32];

	void UpdateAllControls();
	
	CAxesVirtualPage();
	~CAxesVirtualPage();

// Dialog Data
	//{{AFX_DATA(CAxesVirtualPage)
	enum { IDD = IDD_AXES_VIRTUAL_PAGE };
	CButton	m_checkVAmp9;
	CButton	m_checkVAmp8;
	CButton	m_checkVAmp7;
	CButton	m_checkVAmp6;
	CButton	m_checkVAmp5;
	CButton	m_checkVAmp4;
	CButton	m_checkVAmp3;
	CButton	m_checkVAmp2;
	CButton	m_checkVAmp10;
	CButton	m_checkVAmp1;
	CButton	m_checkVAmp0;
	CButton	m_checkAmp9;
	CButton	m_checkAmp8;
	CButton	m_checkAmp7;
	CButton	m_checkAmp6;
	CButton	m_checkAmp5;
	CButton	m_checkAmp4;
	CButton	m_checkAmp3;
	CButton	m_checkAmp2;
	CButton	m_checkAmp10;
	CButton	m_checkAmp1;
	CButton	m_checkAmp0;
	CEdit	m_editCoordinateStatus;
	CEdit	m_editProgramStatus;
	CEdit	m_editCoordSystem2;
	CEdit	m_editCoordSystem1;
	CStatic	m_static10;
	CStatic	m_static9;
	CStatic	m_static8;
	CStatic	m_static7;
	CStatic	m_static6;
	CStatic	m_static5;
	CStatic	m_static4;
	CStatic	m_static3;
	CStatic	m_static2;
	CStatic	m_static1;
	CStatic	m_static0;
	CEdit	m_editVPos10;
	CEdit	m_editVPos9;
	CEdit	m_editVPos8;
	CEdit	m_editVPos7;
	CEdit	m_editVPos6;
	CEdit	m_editVPos5;
	CEdit	m_editVPos4;
	CEdit	m_editVPos3;
	CEdit	m_editVPos2;
	CEdit	m_editVPos1;
	CEdit	m_editVPos0;
	CEdit	m_editPos10;
	CEdit	m_editPos9;
	CEdit	m_editPos8;
	CEdit	m_editPos7;
	CEdit	m_editPos6;
	CEdit	m_editPos5;
	CEdit	m_editPos4;
	CEdit	m_editPos3;
	CEdit	m_editPos2;
	CEdit	m_editPos1;
	CEdit	m_editPos0;
	CButton	m_checkVInPos10;
	CButton	m_checkVInPos9;
	CButton	m_checkVInPos8;
	CButton	m_checkVInPos7;
	CButton	m_checkVInPos6;
	CButton	m_checkVInPos5;
	CButton	m_checkVInPos4;
	CButton	m_checkVInPos3;
	CButton	m_checkVInPos2;
	CButton	m_checkVInPos1;
	CButton	m_checkVInPos0;
	CButton	m_checkInPos10;
	CButton	m_checkInPos9;
	CButton	m_checkInPos8;
	CButton	m_checkInPos7;
	CButton	m_checkInPos6;
	CButton	m_checkInPos5;
	CButton	m_checkInPos4;
	CButton	m_checkInPos3;
	CButton	m_checkInPos2;
	CButton	m_checkInPos1;
	CButton	m_checkInPos0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAxesVirtualPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAxesVirtualPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESVIRTUALPAGE_H__2EA826F1_304A_4C64_8A80_3D29590A0935__INCLUDED_)
