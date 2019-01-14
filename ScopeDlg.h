//{{AFX_INCLUDES()
#include "uslbar.h"
//}}AFX_INCLUDES
#if !defined(AFX_SCOPEDLG_H__A4D0FD87_AAEC_4314_9F28_E9CA93477FB1__INCLUDED_)
#define AFX_SCOPEDLG_H__A4D0FD87_AAEC_4314_9F28_E9CA93477FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScopeDlg1.h : header file
//

#include "StaticPosManage.h"
#include "USLFile.h"

/////////////////////////////////////////////////////////////////////////////
// CScopeDlg dialog

#define MAXIMUMDELAYOSCILLOSCOPENS	450000

class CScopeDlg : public CDialog
{
// Construction
public:
	CString m_ThreadName;
	void CopyDAC();
	void CopyGates();
	void CopyScopeDelayWidth();
	int		MinMax(int* pnV,int nMin,int nMax);
	CString m_WindowName;
	int m_nMaxSample;
	float m_fNominalGain;
	int m_nNominalAmp;
	void SetWindowName();
	bool m_bTrackCurrentTimeSlot;
	int	m_nDlgNumber;
	int m_nTimeSlot;
	int m_nOldConversions;
	clock_t c_f;
	clock_t c_i;
	CRect m_rectPlayBar;
	bool IsPlayBack(CPoint pt,CRect rr);
	CUSLFile m_FileRecord;
	ADC200Data m_AdcRecording;
	int	m_nOffsetRecording;
	int	m_nRecordingL;
	int	m_nPtrRecording;
	bool m_bRecording;
	void UpdateHardware();
	int m_nTraceWidth;
	int m_nTraceDelay;
	HANDLE m_hSemaphore;
	
	void UpdateFromMainFrame();
	bool m_bModifyDacPosition;
	int m_nWhichDacPt;
	CPoint m_ptDac[64];
	bool FindNearestDacPt(CPoint point,CRect rr, int *nPt);
	void DacTableChanged();
	bool FindDACStartPt(int *nStart);
	void RefreshLockVariables();
	void CheckLockDelayFromZHeight();
	double m_fInitialZPosLock;
	int m_nInterfaceDelayLock;
	int m_nScopeDelayLock;
	bool m_bDelayLockedToZ;

	CPoint m_ptClient;
	int m_nGain;
	void DrawPlayBackControls(CDC *pDC,CRect rr);
	void DrawRecordingControls(CDC *pDC,CRect rr);
	void DrawDacCurve(CDC *pDC, CRect rr);
	int m_nWidthNanosec;
	int m_nOffsetNanosec;
	int m_nMaxScrollBarWidth;
	bool CalculateDelayWidthFromScrollBar();
	bool CalculateScrollBarFromDelayWidth();
	int m_nScrollBar[2];
	bool m_bModifyDelayWidth;
	CString	m_Units[4];
	void AllocateScreenArray();
	COLORREF	*m_pArray;
	CPoint		*m_pPtTrace;
	int m_nArrayWidth;
	int m_nArrayHeight;

	bool bModifyGatePosition;
	bool FindNearestGate(CPoint point,CRect rr,int *nGate,int *nEnd);
	HRESULT RefreshScopeDisplay(WPARAM, LPARAM);
	void SetRfInputType(int nType);
	void DrawGraticule(CDC* pDC,CRect rr);
	void DrawTimsGates(CDC* pDC,CRect rr);
	void DrawGates(CDC* pDC,CRect rr);
	void DrawDacPts(CDC *pDC,CRect rr);
	struct GatesData* pGate;
	int m_nGate;
	int	m_nEnd;
	void UpdateAllControls();
	CWnd	**m_pDlg;
	int		nGArray[2000];
	int		nGArray2[2000];
	char	Trace2[4096];

	void CalculateGArray();
	void DrawGTrace(int nDisplayDevice = SCREEN,CUSLFile* pFile = NULL);
	void RenderStatistics(CDC * pDC, CRect rr, ADC200Data * pAdc);
	void DrawRuler(CDC * pDC, CRect rr);

	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosGateMode;
	CStaticPosManage m_StaticPosGateNumber;
	CStaticPosManage m_StaticPosGateDelay;
	CStaticPosManage m_StaticPosGateWidth;
	CStaticPosManage m_StaticPosDelay;
	CStaticPosManage m_StaticPosWidth;
	CStaticPosManage m_StaticPosSpinDelay;
	CStaticPosManage m_StaticPosSpinWidth;
	CStaticPosManage m_StaticPosSpinThreshold;
	CStaticPosManage m_StaticPosEditThreshold;
	CStaticPosManage m_StaticPosThreshold;
	CStaticPosManage m_StaticPosDelayWidth;
	CStaticPosManage m_StaticPosAlarmText;
	CStaticPosManage m_StaticPosAlarm;

	CScopeDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL,int nTimeSlot = 0,int nDlgNumber=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScopeDlg)
	enum { IDD = IDD_SCOPE_DIALOG1 };
	CStatic	m_staticView;
	CUSLBar	m_scrollbarDelayWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScopeDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScopeDlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnScrollHasMovedUslbarctrl(long LowPosition, long HighPosition, long Width);
	afx_msg void OnScopeLockdelaytoz();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnScopeDacrefamplitude();
	afx_msg void OnScopeStoredacpoint();
	afx_msg void OnScopeDisplaydacpoints();
	afx_msg void OnScopeDeletedacpoint();
	afx_msg void OnScopeSaveasbmp();
	afx_msg void OnScopeStartrecording();
	afx_msg void OnScopeStoprecording();
	afx_msg void OnScopePlayrecording();
	afx_msg void OnScopeScaleBottom();
	afx_msg void OnScopeScaleLeft();
	afx_msg void OnScopeScaleRight();
	afx_msg void OnScopeScaleTop();
	afx_msg void OnScopeDrawmodeVector();
	afx_msg void OnScopeDrawmodeDots();
	afx_msg void OnScopeStatistics();
	afx_msg void OnScopeCurrenttimeslot();
	afx_msg void OnScopeSlot1();
	afx_msg void OnScopeSlot2();
	afx_msg void OnScopeSlot3();
	afx_msg void OnScopeSlot4();
	afx_msg void OnScopeSlot5();
	afx_msg void OnScopeSlot6();
	afx_msg void OnScopeSlot7();
	afx_msg void OnScopeSlot8();
	afx_msg void OnScopeAutointerfacegate();
	afx_msg void OnScopeMainbangdelayZero();
	afx_msg void OnScopeMainbangdelayMousepos();
	afx_msg void OnScopeMainbangdelayInterface();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnScopeRuler();
	afx_msg void OnRulerOff();
	afx_msg void OnRulerOffMaterial();
	afx_msg void OnRulerWaterMaterial();
	afx_msg void OnRulerMaterialMaterial();
	afx_msg void OnScopeProperties();
};

	UINT	OscilloscopeThread(LPVOID pParam);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOPEDLG_H__A4D0FD87_AAEC_4314_9F28_E9CA93477FB1__INCLUDED_)
