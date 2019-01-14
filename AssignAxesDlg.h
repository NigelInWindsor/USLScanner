#if !defined(AFX_ASSIGNAXESDLG_H__580FBF1C_FAAD_4958_B5D2_A847870DF2E1__INCLUDED_)
#define AFX_ASSIGNAXESDLG_H__580FBF1C_FAAD_4958_B5D2_A847870DF2E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssignAxesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAssignAxesDlg dialog

class CAssignAxesDlg : public CDialog
{
// Construction
public:
	void SetAccessPrivelages();
	int MinMax(int nV,int nMin,int nMax);
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CAssignAxesDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor
	~CAssignAxesDlg();

// Dialog Data
	//{{AFX_DATA(CAssignAxesDlg)
	enum { IDD = IDD_ASSIGN_AXES_DIALOG };
	CEdit	m_editSafetyOutput;
	CEdit	m_editXHoldingFixed;
	CComboBox	m_comboXHoldingFixed;
	CEdit	m_editPump1Bit;
	CEdit	m_editPump0Bit;
	CComboBox	m_comboAudioPort;
	CEdit	m_editAudioBit;
	CEdit	m_editSi10Output;
	CComboBox	m_comboAndonOutputPort;
	CEdit	m_editAndonStatus;
	CEdit	m_editOutput;
	CEdit	m_editAndonWarning;
	CEdit	m_editAndonRunning;
	CEdit	m_editAndonFault;
	CEdit	m_editAndonOkay;
	CEdit	m_editKill;
	CEdit	m_editInput;
	CEdit	m_editDocked;
	CEdit	m_editAutomatic;
	CEdit	m_editLiftDownIn;
	CEdit	m_editLiftUpIn;
	CEdit	m_editAndonLights;
	CEdit	m_editLiftDown;
	CEdit	m_editLiftUp;
	CEdit	m_editInputXOR;
	CButton	m_checkTurntable;
	CButton	m_checkBridge;
	CButton	m_checkHoldingFixture;
	CEdit	m_editXBridgeNumber;
	CComboBox	m_comboXBridgeType;
	CEdit	m_editEBit;
	CEdit	m_editHoldingBrake;
	CEdit	m_editLeftAir;
	CEdit	m_editRightAir;
	CEdit	m_editGateBit;
	CEdit	m_editDoorBit;
	CEdit	m_editBrakeBit;
	CButton	m_checkAvailable0;
	CButton	m_checkAvailable1;
	CButton	m_checkAvailable2;
	CButton	m_checkAvailable3;
	CButton	m_checkAvailable4;
	CButton	m_checkAvailable5;
	CButton	m_checkAvailable6;
	CButton	m_checkAvailable7;
	CButton	m_checkAvailable8;
	CButton	m_checkAvailable9;
	CButton	m_checkAvailable10;
	CButton	m_checkAvailable11;
	CButton	m_checkAvailable12;
	CButton	m_checkAvailable13;
	CButton	m_checkAvailable14;
	CButton	m_checkAvailable15;
	CButton	m_checkAvailable16;
	CButton	m_checkAvailable17;
	CButton	m_checkAvailable18;
	CButton	m_checkAvailable19;
	CButton	m_checkAvailable20;
	CButton	m_checkAvailable21;
	CButton	m_checkAvailable22;
	CButton	m_checkAvailable23;
	CEdit	m_editLeftPump;
	CEdit	m_editRightPump;
	CEdit	m_editXHoldingMove;
	CEdit	m_editYHoldingMove;
	CEdit	m_editZHoldingMove;
	CEdit	m_editYHoldingFixed;
	CEdit	m_editZHoldingFixed;
	CEdit	m_editXRightNumber;
	CEdit	m_editYRightNumber;
	CEdit	m_editZRightNumber;
	CEdit	m_editXTRightNumber;
	CEdit	m_editYTRightNumber;
	CEdit	m_editRRightNumber;
	CEdit	m_editXLeftNumber;
	CEdit	m_editYLeftNumber;
	CEdit	m_editZLeftNumber;
	CEdit	m_editXTLeftNumber;
	CEdit	m_editYTLeftNumber;
	CEdit	m_editRLeftNumber;
	CComboBox	m_comboLeftPump;
	CComboBox	m_comboRightPump;
	CComboBox	m_comboXHoldingMove;
	CComboBox	m_comboYHoldingMove;
	CComboBox	m_comboZHoldingMove;
	CComboBox	m_comboYHoldingFixed;
	CComboBox	m_comboZHoldingFixed;
	CComboBox	m_comboXRightType;
	CComboBox	m_comboYRightType;
	CComboBox	m_comboZRightType;
	CComboBox	m_comboXTRightType;
	CComboBox	m_comboYTRightType;
	CComboBox	m_comboRRightType;
	CComboBox	m_comboXLeftType;
	CComboBox	m_comboYLeftType;
	CComboBox	m_comboZLeftType;
	CComboBox	m_comboXTLeftType;
	CComboBox	m_comboYTLeftType;
	CComboBox	m_comboRLeftType;
	//}}AFX_DATA
	CCheckListBox	m_listAudio;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssignAxesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAssignAxesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckHoldingFixture();
	afx_msg void OnCheckBridge();
	afx_msg void OnCheckTurntable();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditAutomatic();
	afx_msg void OnChangeEditBrakeBit();
	afx_msg void OnChangeEditDocked();
	afx_msg void OnChangeEditDoorBit();
	afx_msg void OnChangeEditEBit();
	afx_msg void OnChangeEditGateBit();
	afx_msg void OnChangeEditInputXor();
	afx_msg void OnChangeEditHoldingBrake();
	afx_msg void OnChangeEditKill();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAccept();
	afx_msg void OnSelchangeComboAudioPort();
	afx_msg void OnCheckChangeListAudio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASSIGNAXESDLG_H__580FBF1C_FAAD_4958_B5D2_A847870DF2E1__INCLUDED_)
