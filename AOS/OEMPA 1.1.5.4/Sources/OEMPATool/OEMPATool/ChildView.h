// ChildView.h : interface of the CChildView class
//


#pragma once

class CDisplayImage;

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();
	CDisplayImage *GetImage() const	{ return(m_pImage); }
	void SetAcquisitionData(int iAcqRootID);
	bool SetTimer(BOOL bEnable);
	void UpdateCycle(int iCycle);
	void DumpFileData(CStdioFile &file);
	void UpdateCycleCountData(int iCycleCount);
	void UpdateCycleCount(int iCycleCount);
	void UpdateImage(bool bEnable);
	void UpdateInvert(bool bEnable);
	void UpdateColor();
	void ResetData();
	void ResetBuffer();
	void Desalloc();
	// Generated message map functions
	UINT WINAPI ProcessAcquisitionAscan_0x00010103(int iCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	UINT WINAPI ProcessAcquisitionCscan_0x00010X02(int iCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

	void OnTimer(UINT_PTR nIDEvent);
	void CreateImage();
	void UpdateAxisCountAscan();
	void UpdateAxisCountCscan();
	void DestroyImage();
	void InitImage(bool bForceQuickAccess=false);
	void InitImageAscan(bool bForceQuickAccess=false);
	void InitImageCscan(bool bForceQuickAccess=false);
#ifdef _MINMAX_BUFFER_
	bool InitImageAscanMin();
#endif _MINMAX_BUFFER_
	void UpdateImage(int x,int y);

	static const UINT_PTR g_kTimerID = 31;
	static const UINT g_kTimerDelay = 50;//20 Hz
	DWORD m_dwTimerID;

	CRITICAL_SECTION m_CriticalSection;
	ULONGLONG m_ullUpdateCount;
	ULONGLONG m_ullDisplayCount;
	int m_iDisplayCycle;
	int m_iDisplayCycleCount;
	bool m_bDisplayImage;
	bool m_bDisplayInvert;
	bool m_bAscanSign;
	int m_iAcquisitionDataCount[g_iOEMPACycleCountMax];
	int m_iAcquisitionDataSize[g_iOEMPACycleCountMax];
	enumBitSize m_eAcquisitionDataBitSize[g_iOEMPACycleCountMax];
	bool m_bAcquisitionDataSigned[g_iOEMPACycleCountMax];
	structAcqInfoEx m_AcqInfo[g_iOEMPACycleCountMax];
	int m_iAcquisitionCycle;
#ifdef _MINMAX_BUFFER_
	WORD *m_pAcquisitionDataMin[g_iOEMPACycleCountMax];
	bool m_bAcquisitionDataMin[g_iOEMPACycleCountMax];
	bool m_bAcquisitionDataMax[g_iOEMPACycleCountMax];
#endif _MINMAX_BUFFER_
	WORD *m_pAcquisitionDataMax[g_iOEMPACycleCountMax];
	short m_sAcquisitionCscanAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];//short because could be signed or unsigned.
	short m_sAcquisitionCscanTof[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	bool m_bAcquisitionCscanAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	bool m_bAcquisitionCscanTof[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	bool m_bAcquisitionCscanAmpOverThreshold[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	bool m_bAcquisitionCscanTofValidity[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	bool m_bAcquisitionCscanIFOldReference[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
	bool m_bAcquisitionCscanIFNotInitialized[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];

	CDisplayImage *m_pImage;
	bool m_bTimerID;
	bool m_bSizeInit;
	bool m_bSizeChanged;

	int m_aiScaleAxisCount[2];
	int m_aiScreenAxisCount[2];
	int m_aiClientRect[2];
	bool m_bCompress[2];

	int m_iAxisCount;
	int m_iDataBitSize;
	int m_iDataByteSize;
	bool m_bDataSigned;
	enumDisplayType m_eDisplayType;
public:
};

