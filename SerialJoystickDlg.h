#include "afxwin.h"
#if !defined(AFX_SERIALJOYSTICKDLG_H__C27EC03D_A3C2_4ADD_933D_CF39EF890DA3__INCLUDED_)
#define AFX_SERIALJOYSTICKDLG_H__C27EC03D_A3C2_4ADD_933D_CF39EF890DA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialJoystickDlg.h : header file
//
extern	UINT PortReadThreadNonOverLapped (LPVOID lpvoid);
extern	UINT PortOverlappedReadThread (LPVOID lpvoid);


/////////////////////////////////////////////////////////////////////////////
// CSerialJoystickDlg dialog

#define	BUFFER_LENGTH	1024


class CSerialJoystick
{

public:
	bool m_bThreadFinished;
	bool m_bThreadEnabled;

	bool IsErrorCodeGood();
	BOOL WriteABuffer(char * lpBuf, DWORD dwToWrite);
	void GetRxBuffer(CString *pBuff, int nStart, int nLength);
	void ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered);
	virtual BOOL Initialize(LPCWSTR lpszPortName);
	virtual	void WriteByte(char Byte);
	virtual	void WriteStr(char *string);
	virtual	int  Hit(void);
	virtual	bool ReadByte(char *Byte);
	virtual	int	 ReadStr(char *string,char Eot);
	int ReadStrLenght(char *string,int CaracterNumber);
	virtual void WriteStrLenght(char *string, int BytesNumber);
	virtual	BOOL Close(void);
	void Start();
	void ClearBuffer();
	void ExtractJoyValues();
	void InitializeMinMaxValues();

	int		m_nFaultyDataPkts;
	int		m_nSwitches;
	int		m_nTAxis;
	int		m_nXAxis[16];
	int		m_nYAxis[16];
	int		m_nZAxis[16];
	int		m_nSAxis[16];
	int		m_JoyMin[4];
	int		m_JoyMax[4];
	int		m_nFir;

	HANDLE	hPort;
	HANDLE	hReadThread;
	char	*rx_buffer;
	int		rx_ptr;
	int		last_read_ptr;
	CWinThread	*pThread;
	WCHAR m_PortName[10];
	unsigned char	JoystickArray[30];
	int		nJoyPtr;
};


class CSerialJoystickDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CSerialJoystickDlg)

public:
	HBRUSH	m_hBr;
	void SetAccessPrivelage();
	void UpdateAllControls();
// Construction
	CSerialJoystickDlg();

// Dialog Data
	//{{AFX_DATA(CSerialJoystickDlg)
	enum { IDD = IDD_SERIAL_JOYSTICK_DIALOG };
	CEdit	m_editSAxis;
	CEdit	m_editZAxis;
	CEdit	m_editYAxis;
	CEdit	m_editXAxis;
	CEdit	m_editFaultyDataPkts;
	CEdit	m_editDeadBandSize;
	CEdit	m_editDockedBit;
	CEdit	m_editLeftSide;
	CEdit	m_editRightSide;
	CComboBox	m_comboJoystickSource;
	CEdit	m_editMainManip;
	CEdit	m_editDeadMan1;
	CEdit	m_editDeadMan0;
	CEdit	m_editSwitches;
	CEdit	m_editSpeed;
	CEdit	m_editScannerFixture;
	CEdit	m_editRxArray;
	CComboBox	m_comboCommPort;
	int		m_nJoyPtr;
	CString	m_stringElapsedTime;
	CString	m_stringPosCounter;
	CString	m_editJoyArray;
	int		m_nTAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialJoystickDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSerialJoystickDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboCommPort();
	afx_msg void OnChangeEditScannerfixture();
	afx_msg void OnChangeEditSpeed();
	afx_msg void OnChangeEditDeadman0();
	afx_msg void OnChangeEditDeadman1();
	afx_msg void OnChangeEditMainManip();
	afx_msg void OnChangeEditSideSelect();
	afx_msg void OnSelchangeComboJoystickType();
	afx_msg void OnChangeEditLeftSide();
	afx_msg void OnChangeEditRightSide();
	afx_msg void OnChangeEditDockedBit();
	afx_msg void OnChangeEditDeadbandSize();
	afx_msg void OnButtonReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editFrameToolJoyBit;
	afx_msg void OnEnChangeEditFrameTool();
	afx_msg void OnEnChangeEditFir();
	CEdit m_editFIR;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALJOYSTICKDLG_H__C27EC03D_A3C2_4ADD_933D_CF39EF890DA3__INCLUDED_)
