// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DisplayImage.h"
#include "ChildView.h"
#include "DlgHW.h"
#include "DlgCscanDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WPARAM_EVENT_UPDATE_FROM_EXTERNAL	20101

long myround(double x);

// CChildView

bool g_bExit=false;

CChildView *g_pView=NULL;
CChildView::CChildView()
{
	g_pView = this;
	m_dwTimerID = 0;

	m_pImage = NULL;
	m_bTimerID = false;
	m_bSizeChanged = false;
	m_bSizeInit = false;

	for(int i=0;i<2;i++)
	{
		m_aiScaleAxisCount[i] = 0;
		m_aiScreenAxisCount[i] = 0;
		m_aiClientRect[i] = 0;
		m_bCompress[i] = false;
	}

	m_iAxisCount = 0;
	m_iDataBitSize = 0;
	m_iDataByteSize = 0;
	m_bDataSigned = false;
	m_eDisplayType = eProfileZ;
	InitializeCriticalSection(&m_CriticalSection);
	m_bDisplayImage = false;
	m_bDisplayInvert = false;
	m_bAscanSign = 0;
	ResetBuffer();
}

CChildView::~CChildView()
{
	g_bExit = true;
	Desalloc();
	DestroyImage();
	DeleteCriticalSection(&m_CriticalSection);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	//CString string=L"example";
	//dc.TextOutW(100,100,string);
	//CChildView::OnTimer(g_kTimerID);
	if(m_pImage)
	{
		m_pImage->Display(this,m_eDisplayType,m_bDisplayImage);
		::GdiFlush();
	}
}

void CChildView::DestroyImage()
{
	m_bTimerID = false;
	if(!m_pImage)
		return;
	m_pImage->Destroy();
	delete m_pImage;
	m_pImage = NULL;
}

bool CChildView::SetTimer(BOOL bEnable)
{
	if(bEnable && !m_dwTimerID)
	{
		m_dwTimerID = (DWORD)CWnd::SetTimer(g_kTimerID, g_kTimerDelay, NULL);
		if(m_dwTimerID)
			return true;
		return false;
	}
	if(!bEnable && m_dwTimerID)
	{
		KillTimer(m_dwTimerID);
		m_dwTimerID = 0;
		return true;
	}
	return false;
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	ASSERT(nIDEvent == g_kTimerID);
	m_bTimerID = false;
	bool bFlushGdi=false;
	bool bRefresh=false;
	static int iCount=0;

	if (m_pImage == NULL)
		return;
	EnterCriticalSection(&m_CriticalSection);
	if(m_ullDisplayCount!=m_ullUpdateCount)
	{
		m_ullDisplayCount = m_ullUpdateCount;
		bRefresh = true;
		if(m_iAcquisitionDataCount[m_iAcquisitionCycle]!=m_pImage->m_scaleWidth)
		{
			CreateImage();
			bRefresh = false;
		}
	}else
		bRefresh = false;
	if(bRefresh)
		InitImage(true);
	LeaveCriticalSection(&m_CriticalSection);

	Invalidate();
	UpdateWindow();
	iCount++;
	if(iCount*g_kTimerDelay>=1000)
	{//throughput display timer is 1000 ms (1 second). If the timer is too small the display is not good.
		iCount = 0;
		CDlgHW::DisplayEncoder();
	}
	return;
}

void CChildView::UpdateCycle(int iCycle)
{
	m_iDisplayCycle = iCycle;
	if(iCycle>=0)
		m_iAcquisitionCycle = iCycle;
}

void CChildView::UpdateCycleCountData(int iCycleCount)
{
	EnterCriticalSection(&m_CriticalSection);
	//1.1.5.2r if(m_iDisplayCycleCount != iCycleCount)
	{
		m_iDisplayCycleCount = iCycleCount;
		memset(m_sAcquisitionCscanAmp,0,sizeof(short)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_sAcquisitionCscanTof,0,sizeof(short)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_bAcquisitionCscanAmp,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_bAcquisitionCscanTof,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_bAcquisitionCscanAmpOverThreshold,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_bAcquisitionCscanTofValidity,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_bAcquisitionCscanIFOldReference,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
		memset(m_bAcquisitionCscanIFNotInitialized,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
	}
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::UpdateCycleCount(int iCycleCount)
{
	EnterCriticalSection(&m_CriticalSection);
	UpdateCycleCountData(iCycleCount);
	CreateImage();
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::UpdateImage(bool bEnable)
{
	EnterCriticalSection(&m_CriticalSection);
	m_bDisplayImage = bEnable;
	if(bEnable)
		m_eDisplayType = eXZ;
	else
		m_eDisplayType = eProfileZ;
	CreateImage();
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::UpdateInvert(bool bEnable)
{
	EnterCriticalSection(&m_CriticalSection);
	m_bDisplayInvert = bEnable;
	CreateImage();
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::UpdateColor()
{
	EnterCriticalSection(&m_CriticalSection);
	CreateImage();
	LeaveCriticalSection(&m_CriticalSection);
}

UINT WINAPI CChildView::ProcessAcquisitionAscan_0x00010103(int iAscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
	bool bRet=true;
	WORD wDataCount;
	static bool bStart=false;
	static bool bStop=false;
	enumBitSize eBitSize;
	BYTE *pbyAcquisitionDataSrc;
	char *pcAcquisitionDataSrc;
	WORD *pwAcquisitionDataSrc;
	short *psAcquisitionDataSrc;
	BYTE *pbyAcquisitionDataDst;
	char *pcAcquisitionDataDst;
	WORD *pwAcquisitionDataDst;
	short *psAcquisitionDataDst;

	if(g_bExit)
		return 0;
	if(!CDlgHW::m_bAcquisitionWholeStream)
		EnterCriticalSection(&m_CriticalSection);
	if(pAscanHeader->Check() && (pBufferMax || pBufferMin))
	{
		if(!CDlgHW::m_bAcquisitionWholeStream)
			CDlgCscanDisplay::ProcessAcquisitionAscan_0x00010103(iAscanCycle,acqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat);
		if((m_iDisplayCycle<0) || ((m_iDisplayCycle>=0) && (iAscanCycle==m_iDisplayCycle)))
		{
			m_iAcquisitionCycle = iAscanCycle;
			wDataCount = pAscanHeader->dataCount;
			if(!wDataCount)
				bRet = false;
			else{
				if(((int)wDataCount!=m_iAcquisitionDataCount[iAscanCycle]))
				{
					m_iAcquisitionDataCount[iAscanCycle] = (int)wDataCount;
					if(m_pAcquisitionDataMax[iAscanCycle])
						delete m_pAcquisitionDataMax[iAscanCycle];
					m_pAcquisitionDataMax[iAscanCycle] = new WORD[wDataCount];
#ifdef _MINMAX_BUFFER_
					if(m_pAcquisitionDataMin[iAscanCycle])
						delete m_pAcquisitionDataMin[iAscanCycle];
					m_pAcquisitionDataMin[iAscanCycle] = new WORD[wDataCount];
#endif _MINMAX_BUFFER_
				}
				m_iAcquisitionDataSize[iAscanCycle] = pAscanHeader->dataSize;
				eBitSize = (enumBitSize)pAscanHeader->bitSize;
				m_eAcquisitionDataBitSize[iAscanCycle] = eBitSize;
				if(pAscanHeader->sign)
					m_bAcquisitionDataSigned[iAscanCycle] = true;
				else
					m_bAcquisitionDataSigned[iAscanCycle] = false;
				memcpy(&m_AcqInfo[iAscanCycle],&acqInfo,sizeof(acqInfo));
				if(m_pAcquisitionDataMax[iAscanCycle])
				{
					if(pAscanHeader->sign)
						m_bAscanSign = true;
					else
						m_bAscanSign = false;
					if(eBitSize==eLog8Bits)
						m_bAscanSign = false;
					m_bAcquisitionDataMax[iAscanCycle] = false;
					m_bAcquisitionDataMin[iAscanCycle] = false;
					if(!CDlgHW::m_bCheckDisplayMax)
					{
						if(pBufferMax)
						{
							memcpy(m_pAcquisitionDataMax[iAscanCycle],pBufferMax,m_iAcquisitionDataCount[iAscanCycle]*pAscanHeader->dataSize);
							m_bAcquisitionDataMax[iAscanCycle] = true;
						}else{
							memset(m_pAcquisitionDataMax[iAscanCycle],0,m_iAcquisitionDataCount[iAscanCycle]*pAscanHeader->dataSize);
						}
#ifdef _MINMAX_BUFFER_
						if(m_pAcquisitionDataMin[iAscanCycle])
						{
							if(pBufferMin)
							{
								memcpy(m_pAcquisitionDataMin[iAscanCycle],pBufferMin,m_iAcquisitionDataCount[iAscanCycle]*pAscanHeader->dataSize);
								m_bAcquisitionDataMin[iAscanCycle] = true;
							}else{
								memset(m_pAcquisitionDataMin[iAscanCycle],0,m_iAcquisitionDataCount[iAscanCycle]*pAscanHeader->dataSize);
							}
						}
#endif _MINMAX_BUFFER_
					}else{
						if(pBufferMax)
						switch(eBitSize)
						{
						case eLog8Bits:
						case e8Bits:
							if(m_bAscanSign)
							{
								pcAcquisitionDataDst = (char*)m_pAcquisitionDataMax[m_iAcquisitionCycle];
								pcAcquisitionDataSrc = (char*)pBufferMax;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(pcAcquisitionDataDst[i]<pcAcquisitionDataSrc[i])
										pcAcquisitionDataDst[i] = pcAcquisitionDataSrc[i];
								}
							}else{
								pbyAcquisitionDataDst = (BYTE*)m_pAcquisitionDataMax[m_iAcquisitionCycle];
								pbyAcquisitionDataSrc = (BYTE*)pBufferMax;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(pbyAcquisitionDataDst[i]<pbyAcquisitionDataSrc[i])
										pbyAcquisitionDataDst[i] = pbyAcquisitionDataSrc[i];
								}
							}
							break;
						case e12Bits:
						case e16Bits:
							if(m_bAscanSign)
							{
								psAcquisitionDataDst = (short*)m_pAcquisitionDataMax[m_iAcquisitionCycle];
								psAcquisitionDataSrc = (short*)pBufferMax;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(psAcquisitionDataDst[i]<psAcquisitionDataSrc[i])
										psAcquisitionDataDst[i] = psAcquisitionDataSrc[i];
								}
							}else{
								pwAcquisitionDataDst = (WORD*)m_pAcquisitionDataMax[m_iAcquisitionCycle];
								pwAcquisitionDataSrc = (WORD*)pBufferMax;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(pwAcquisitionDataDst[i]<pwAcquisitionDataSrc[i])
										pwAcquisitionDataDst[i] = pwAcquisitionDataSrc[i];
								}
							}
							break;
						}
#ifdef _MINMAX_BUFFER_
						if(m_pAcquisitionDataMin[m_iAcquisitionCycle] && pBufferMin)
						switch(eBitSize)
						{
						case eLog8Bits:
						case e8Bits:
							if(m_bAscanSign)
							{
								pcAcquisitionDataDst = (char*)m_pAcquisitionDataMin[m_iAcquisitionCycle];
								pcAcquisitionDataSrc = (char*)pBufferMin;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(pcAcquisitionDataDst[i]<pcAcquisitionDataSrc[i])
										pcAcquisitionDataDst[i] = pcAcquisitionDataSrc[i];
								}
							}else{
								pbyAcquisitionDataDst = (BYTE*)m_pAcquisitionDataMin[m_iAcquisitionCycle];
								pbyAcquisitionDataSrc = (BYTE*)pBufferMin;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(pbyAcquisitionDataDst[i]<pbyAcquisitionDataSrc[i])
										pbyAcquisitionDataDst[i] = pbyAcquisitionDataSrc[i];
								}
							}
							break;
						case e12Bits:
						case e16Bits:
							if(m_bAscanSign)
							{
								psAcquisitionDataDst = (short*)m_pAcquisitionDataMin[m_iAcquisitionCycle];
								psAcquisitionDataSrc = (short*)pBufferMin;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(psAcquisitionDataDst[i]<psAcquisitionDataSrc[i])
										psAcquisitionDataDst[i] = psAcquisitionDataSrc[i];
								}
							}else{
								pwAcquisitionDataDst = (WORD*)m_pAcquisitionDataMin[m_iAcquisitionCycle];
								pwAcquisitionDataSrc = (WORD*)pBufferMin;
								for(int i=0;i<m_iAcquisitionDataCount[iAscanCycle];i++)
								{
									if(pwAcquisitionDataDst[i]<pwAcquisitionDataSrc[i])
										pwAcquisitionDataDst[i] = pwAcquisitionDataSrc[i];
								}
							}
							break;
						}
#endif _MINMAX_BUFFER_
					}
					bStart = false;
					bStop = false;
					m_ullUpdateCount++;
				}
			}
		}
	}else
		bRet = false;
	if(!CDlgHW::m_bAcquisitionWholeStream)
		LeaveCriticalSection(&m_CriticalSection);
	return 0;
}

typedef union unionAmp{
	char charData;
	BYTE byteData;
}unionAmp;
UINT WINAPI CChildView::ProcessAcquisitionCscan_0x00010X02(int iCscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
{
	unsigned int uiGateId,uiCycle,uiData;
	unionAmp *pAmp;
	const structCscanAmp_0x0102 *_pAmp;
	const structCscanAmpTof_0x0202 *_pAmpTof;

	EnterCriticalSection(&m_CriticalSection);
	if(pCscanHeader->Check() && pBufferAmp)
	{
		CDlgCscanDisplay::ProcessAcquisitionCscan_0x00010X02(iCscanCycle,acqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof);
		uiCycle = pCscanHeader->cycle;
		if(uiCycle<g_iOEMPACycleCountMax)
		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
		{
			_pAmp = &pBufferAmp[iIndex];
			uiGateId = _pAmp->gateId;
			if(uiGateId>=g_iOEMPAGateCountMax)
				continue;
			uiData = _pAmp->byAmp;
			pAmp = (unionAmp*)&uiData;
			if(pBufferAmp[iIndex].sign)
				m_sAcquisitionCscanAmp[uiGateId][uiCycle] = pAmp->charData;
			else
				m_sAcquisitionCscanAmp[uiGateId][uiCycle] = pAmp->byteData;
			m_bAcquisitionCscanAmp[uiGateId][uiCycle] = true;
			m_bAcquisitionCscanAmpOverThreshold[uiGateId][uiCycle] = (_pAmp->AmpOverThreshold?true:false);
			m_bAcquisitionCscanTofValidity[uiGateId][uiCycle] = false;
			m_bAcquisitionCscanIFOldReference[uiGateId][uiCycle] = (_pAmp->IFOldReference?true:false);
			m_bAcquisitionCscanIFNotInitialized[uiGateId][uiCycle] = (_pAmp->IFNotInitialized?true:false);
		}
	}
	if(pCscanHeader->Check() && pBufferAmpTof)
	{
		CDlgCscanDisplay::ProcessAcquisitionCscan_0x00010X02(iCscanCycle,acqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof);
		uiCycle = pCscanHeader->cycle;
		if(uiCycle<g_iOEMPACycleCountMax)
		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
		{
			_pAmpTof = &pBufferAmpTof[iIndex];
			uiGateId = _pAmpTof->gateId;
			if(uiGateId>=g_iOEMPAGateCountMax)
				continue;
			uiData = _pAmpTof->byAmp;
			pAmp = (unionAmp*)&uiData;
			if(pBufferAmpTof[iIndex].sign)
				m_sAcquisitionCscanAmp[uiGateId][uiCycle] = pAmp->charData;
			else
				m_sAcquisitionCscanAmp[uiGateId][uiCycle] = pAmp->byteData;
			if(CDlgHW::m_bMultiHWChannel)
			{
				m_sAcquisitionCscanTof[uiGateId][uiCycle] = _pAmpTof->wTof*CDlgHW::m_abyMultiChannelDecimation[uiCycle];
				m_sAcquisitionCscanTof[uiGateId][uiCycle] += (int)(CDlgHW::m_afMultiChannelStart[uiCycle]/CDlgHW::m_fMultiChannelPeriod);
			}else
				m_sAcquisitionCscanTof[uiGateId][uiCycle] = _pAmpTof->wTof;
			m_bAcquisitionCscanAmp[uiGateId][uiCycle] = true;
			m_bAcquisitionCscanTof[uiGateId][uiCycle] = true;
			m_bAcquisitionCscanAmpOverThreshold[uiGateId][uiCycle] = (_pAmpTof->AmpOverThreshold?true:false);
			m_bAcquisitionCscanTofValidity[uiGateId][uiCycle] = (_pAmpTof->TofValidity?true:false);
			m_bAcquisitionCscanIFOldReference[uiGateId][uiCycle] = (_pAmpTof->IFOldReference?true:false);
			m_bAcquisitionCscanIFNotInitialized[uiGateId][uiCycle] = (_pAmpTof->IFNotInitialized?true:false);
		}
	}
	LeaveCriticalSection(&m_CriticalSection);
	return 0;
}

void CChildView::ResetData()
{
	int iDataSize;
	enumBitSize eBitSize;

	EnterCriticalSection(&m_CriticalSection);
	for(int iCycle=0;iCycle<m_iDisplayCycleCount;iCycle++)
	{
		eBitSize = m_eAcquisitionDataBitSize[iCycle];
		switch(eBitSize)
		{
		case eLog8Bits:
		case e8Bits:	iDataSize = 1;break;
		case e12Bits:
		case e16Bits:	iDataSize = 2;break;
		}
		if(m_pAcquisitionDataMax[iCycle])
			memset(m_pAcquisitionDataMax[iCycle],0,m_iAcquisitionDataCount[iCycle]*iDataSize);
#ifdef _MINMAX_BUFFER_
		m_bAcquisitionDataMax[iCycle] = false;
		m_bAcquisitionDataMin[iCycle] = false;
		if(m_pAcquisitionDataMin[iCycle])
			memset(m_pAcquisitionDataMin[iCycle],0,m_iAcquisitionDataCount[iCycle]*iDataSize);
#endif _MINMAX_BUFFER_
	}
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::ResetBuffer()
{
	EnterCriticalSection(&m_CriticalSection);
	m_ullUpdateCount = 0;
	m_ullDisplayCount = 0;
	memset(m_pAcquisitionDataMax,0,sizeof(void*)*g_iOEMPACycleCountMax);
	m_iDisplayCycle = -1;
#ifdef _MINMAX_BUFFER_
	memset(m_bAcquisitionDataMax,0,sizeof(bool)*g_iOEMPACycleCountMax);
	memset(m_bAcquisitionDataMin,0,sizeof(bool)*g_iOEMPACycleCountMax);
	memset(m_pAcquisitionDataMin,0,sizeof(void*)*g_iOEMPACycleCountMax);
#endif _MINMAX_BUFFER_
	m_iDisplayCycleCount = 1;
	memset(m_iAcquisitionDataCount,0,sizeof(int)*g_iOEMPACycleCountMax);
	memset(m_iAcquisitionDataSize,0,sizeof(int)*g_iOEMPACycleCountMax);
	memset(m_eAcquisitionDataBitSize,0,sizeof(enumBitSize)*g_iOEMPACycleCountMax);
	memset(m_bAcquisitionDataSigned,0,sizeof(bool)*g_iOEMPACycleCountMax);
	m_iAcquisitionCycle = 0;
	memset(m_sAcquisitionCscanAmp,0,sizeof(short)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
	memset(m_sAcquisitionCscanTof,0,sizeof(short)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
	memset(m_bAcquisitionCscanAmp,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
	memset(m_bAcquisitionCscanTof,0,sizeof(bool)*g_iOEMPAGateCountMax*g_iOEMPACycleCountMax);
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::Desalloc()
{
	EnterCriticalSection(&m_CriticalSection);
	for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
	{
		if(m_pAcquisitionDataMax[iCycle])
			delete m_pAcquisitionDataMax[iCycle];
		m_pAcquisitionDataMax[iCycle] = NULL;
#ifdef _MINMAX_BUFFER_
		m_bAcquisitionDataMin[iCycle] = false;
		m_bAcquisitionDataMax[iCycle] = false;
		if(m_pAcquisitionDataMin[iCycle])
			delete m_pAcquisitionDataMin[iCycle];
		m_pAcquisitionDataMin[iCycle] = NULL;
#endif _MINMAX_BUFFER_
	}
	ResetBuffer();
	LeaveCriticalSection(&m_CriticalSection);
}

void CChildView::SetAcquisitionData(int iAcqRootID)
{
	CreateImage();
}

void CChildView::UpdateAxisCountAscan()
{
	int iDataCount;

	EnterCriticalSection(&m_CriticalSection);
	if(!m_ullUpdateCount)
		iDataCount = 128;//CDlgHW::m_iEditQty;
	else
		iDataCount = m_iAcquisitionDataCount[m_iAcquisitionCycle];
	LeaveCriticalSection(&m_CriticalSection);
	m_aiScaleAxisCount[0] = m_aiScreenAxisCount[0] = iDataCount;
	m_aiScreenAxisCount[1] = 1;
	if(!m_bDisplayImage)
		m_aiScaleAxisCount[1] = 256;
	else
		m_aiScaleAxisCount[1] = m_iDisplayCycleCount;
	m_aiScreenAxisCount[1] = m_aiScaleAxisCount[1];
	for(int iAxis=0;iAxis<2;iAxis++)
	{
		if(m_aiClientRect[iAxis]<m_aiScreenAxisCount[iAxis])
			m_bCompress[iAxis] = true;
		else
			m_bCompress[iAxis] = false;
	}
}

void CChildView::UpdateAxisCountCscan()
{
	int iDataCount;

	EnterCriticalSection(&m_CriticalSection);
	if(!m_ullUpdateCount)
		iDataCount = 128;//CDlgHW::m_iEditQty;
	else{
		if(CDlgCscanDisplay::IsHwGate())
			iDataCount = CDlgCscanDisplay::m_aiAcquisitionHWAmpCount[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
		else
			iDataCount = CDlgCscanDisplay::m_aiAcquisitionSWAmpCount[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
	}
	LeaveCriticalSection(&m_CriticalSection);
	m_aiScaleAxisCount[0] = m_aiScreenAxisCount[0] = iDataCount;
	m_aiScreenAxisCount[1] = 1;
	if(!m_bDisplayImage)
		m_aiScaleAxisCount[1] = 256;
	else
		m_aiScaleAxisCount[1] = m_iDisplayCycleCount;
	m_aiScreenAxisCount[1] = m_aiScaleAxisCount[1];
	for(int iAxis=0;iAxis<2;iAxis++)
	{
		if(m_aiClientRect[iAxis]<m_aiScreenAxisCount[iAxis])
			m_bCompress[iAxis] = true;
		else
			m_bCompress[iAxis] = false;
	}
}

void CChildView::CreateImage()
{
	CRect cRect;
	int iImageWidth,iImageHeight;
	int iScaleWidth,iScaleHeight;
	bool bSigned;

	if(m_pImage)
		delete m_pImage;
	m_pImage = NULL;
	int nBits = 0;

	bSigned = m_bAscanSign;
	GetClientRect(&cRect);
	m_aiClientRect[0] = cRect.Width();
	m_aiClientRect[1] = cRect.Height();

	if(!CDlgCscanDisplay::IsDisplayEnabled())
		UpdateAxisCountAscan();
	else
		UpdateAxisCountCscan();

	m_pImage = new CDisplayImage(this);
	ASSERT(m_pImage != NULL);
	if(m_bCompress[0])
		iImageWidth = m_aiClientRect[0];
	else
		iImageWidth = (int)m_aiScreenAxisCount[0];
	if(m_bCompress[1])
		iImageHeight = m_aiClientRect[1];
	else
		iImageHeight = (int)m_aiScreenAxisCount[1];
	iScaleWidth = (int)m_aiScaleAxisCount[0];
	iScaleHeight = (int)m_aiScaleAxisCount[1];
	if(!CDlgCscanDisplay::IsDisplayEnabled())
	{
		if(!m_bDisplayInvert)
			m_pImage->Create(this, iImageWidth, iImageHeight, iScaleWidth, iScaleHeight, bSigned, m_bDisplayImage, m_bDisplayInvert);
		else
			m_pImage->Create(this, iImageHeight, iImageWidth, iScaleHeight, iScaleWidth, bSigned, m_bDisplayImage, m_bDisplayInvert);
	}else{
		if(!m_bDisplayInvert)
			m_pImage->Create(this, iImageWidth, iImageHeight, iScaleWidth, iScaleHeight, false, m_bDisplayImage, m_bDisplayInvert);
		else
			m_pImage->Create(this, iImageHeight, iImageWidth, iScaleHeight, iScaleWidth, false, m_bDisplayImage, m_bDisplayInvert);
	}
	int numLeaders = max(2, (cRect.Width()*cRect.Height())>>16);
	int numLlamas = numLeaders*35;
	InitImage();
	m_bTimerID = true;
	//Invalidate();
	//UpdateWindow();
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    if (m_hWnd && cx && cy) {

		// Initialize screen format
		// First time init with an hwnd
		if (!m_bSizeInit) {
			CClientDC dc(this);
			int pixels;

			pixels = dc.GetDeviceCaps(BITSPIXEL) * dc.GetDeviceCaps(PLANES);
			m_bSizeInit =  true;

		}

		m_bSizeChanged = true;
		CreateImage();
		Invalidate();
		UpdateWindow();
		//OnSize();
    }
}

template <class type>
class CAutoDelete
{
public:
	CAutoDelete(type &a)
	{
		m_pData = &a;
	};
	~CAutoDelete()
	{
		if(!m_pData)
			return;
		if(*m_pData)
			delete *m_pData;
		*m_pData = NULL;
	};
private:
	type *m_pData;
};

#ifdef _MINMAX_BUFFER_
bool CChildView::InitImageAscanMin()
{
	unsigned char byValue;
	char cValue;
	unsigned short usValue,usOffset,usScale;
	short sValue;
	int xrect;
	int iAcquisitionDataCount;
	BYTE *pAcquisitionData;
	WORD *pwAcquisitionData;
	bool bAscanSign=m_bAscanSign;
	int iAcquisitionCycleMin=m_iAcquisitionCycle;

	if(iAcquisitionCycleMin<0)
		return false;
	if(iAcquisitionCycleMin>=m_iDisplayCycleCount)
		return false;
	iAcquisitionDataCount = m_iAcquisitionDataCount[iAcquisitionCycleMin];
	//if(m_iAcquisitionDataSize[iAcquisitionCycleMin]==sizeof(BYTE))
	if(	(m_eAcquisitionDataBitSize[iAcquisitionCycleMin]==e8Bits) || 
		(m_eAcquisitionDataBitSize[iAcquisitionCycleMin]==eLog8Bits)	)
	{
		if(m_eAcquisitionDataBitSize[iAcquisitionCycleMin]==eLog8Bits)
			bAscanSign = false;
		pAcquisitionData = (BYTE*)m_pAcquisitionDataMin[iAcquisitionCycleMin];
		if(!pAcquisitionData)
			return false;
		for(int z=0;z<m_aiScreenAxisCount[0];z++)
		{
			if(m_bCompress[0])
				xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
			else
				xrect = z;
			if(!bAscanSign)
				byValue = (BYTE)pAcquisitionData[z];
			else{
				cValue = (char)pAcquisitionData[z];
				byValue = cValue+128;
			}
			if(!m_bDisplayInvert)
				m_pImage->SetPointColorMin(xrect,byValue);
			else
				m_pImage->SetPointColorMin(xrect,byValue);
		}
	}
	//if(m_iAcquisitionDataSize[iAcquisitionCycleMin]==sizeof(WORD))
	if(	(m_eAcquisitionDataBitSize[iAcquisitionCycleMin]==e12Bits) ||
		(m_eAcquisitionDataBitSize[iAcquisitionCycleMin]==e16Bits)	)
	{
		if(m_eAcquisitionDataBitSize[iAcquisitionCycleMin]==e12Bits)
		{
			usOffset = 2048;//2^(12-1)
			usScale = 16;//2^(12-8)
		}else{
			usOffset = 32768;//2^(16-1)
			usScale = 256;//2^(16-8)
		}
		pwAcquisitionData = (WORD*)m_pAcquisitionDataMin[iAcquisitionCycleMin];
		if(!pAcquisitionData)
			return false;
		for(int z=0;z<m_aiScreenAxisCount[0];z++)
		{
			if(m_bCompress[0])
				xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
			else
				xrect = z;
			if(!m_bAscanSign)
				usValue = (WORD)pwAcquisitionData[z];
			else{
				sValue = (short)pwAcquisitionData[z];
				usValue = sValue+usOffset;
			}
			usValue /= usScale;//scale
			byValue = (BYTE)usValue;
			if(!m_bDisplayInvert)
				m_pImage->SetPointColorMin(xrect,byValue);
			else
				m_pImage->SetPointColorMin(xrect,byValue);
		}
	}
	return m_bAcquisitionDataMin[iAcquisitionCycleMin];
}
#endif _MINMAX_BUFFER_

void CChildView::DumpFileData(CStdioFile &file)
{
	CString string;
	char cValue;
	short sValue;
	int iAcquisitionDataCount;
	char *pAcquisitionData;
	short *psAcquisitionData;
	BYTE byValue;
	WORD wValue;

	for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
	{
		string.Format(L"\n\nGate %d (cycle Amp Tof AmpOverThreshold TofValidity IFOldReference IFNotInitialized)\n",iGate);
		file.WriteString(string);
		for(int iCycle=0;iCycle<m_iDisplayCycleCount;iCycle++)
		{
			if(m_bAcquisitionCscanAmp[iGate][iCycle])
			{
				if(m_bAcquisitionCscanTof[iGate][iCycle])
				{
					string.Format(L"\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",iCycle,m_sAcquisitionCscanAmp[iGate][iCycle],m_sAcquisitionCscanTof[iGate][iCycle],
						(m_bAcquisitionCscanAmpOverThreshold[iGate][iCycle]?1:0),
						(m_bAcquisitionCscanTofValidity[iGate][iCycle]?1:0),
						(m_bAcquisitionCscanIFOldReference[iGate][iCycle]?1:0),
						(m_bAcquisitionCscanIFNotInitialized[iGate][iCycle]?1:0)
						);
					file.WriteString(string);
				}else{
					string.Format(L"\t%d\t%d\t%d\t%d\t%d\t%d\n",iCycle,m_sAcquisitionCscanAmp[iGate][iCycle],
						(m_bAcquisitionCscanAmpOverThreshold[iGate][iCycle]?1:0),
						(m_bAcquisitionCscanTofValidity[iGate][iCycle]?1:0),
						(m_bAcquisitionCscanIFOldReference[iGate][iCycle]?1:0),
						(m_bAcquisitionCscanIFNotInitialized[iGate][iCycle]?1:0)
						);
					file.WriteString(string);
				}
			}
		}
	}
	for(int iCycle=0;iCycle<m_iDisplayCycleCount;iCycle++)
	{
		iAcquisitionDataCount = m_iAcquisitionDataCount[iCycle];
		string.Format(L"\n\nCycle %d Max (%d points)\n",iCycle,iAcquisitionDataCount);
		file.WriteString(string);
		pAcquisitionData = (char*)m_pAcquisitionDataMax[iCycle];
		psAcquisitionData = (short*)m_pAcquisitionDataMax[iCycle];
		for(int z=0;z<iAcquisitionDataCount;z++)
		{
			if(m_iAcquisitionDataSize[iCycle]==sizeof(BYTE))
			{
				cValue = (char)pAcquisitionData[z];
				if(m_bAcquisitionDataSigned[iCycle])
					string.Format(L"%d\n",(int)cValue);
				else{
					byValue = (BYTE)cValue;
					string.Format(L"%u\n",(unsigned int)byValue);
				}
			}else
			if(m_iAcquisitionDataSize[iCycle]==sizeof(WORD))
			{
				sValue = (short)psAcquisitionData[z];
				if(m_bAcquisitionDataSigned[iCycle])
					string.Format(L"%d\n",(int)sValue);
				else{
					wValue = (WORD)sValue;
					string.Format(L"%u\n",(unsigned int)wValue);
				}
			}
			file.WriteString(string);
		}
	}
#ifdef _MINMAX_BUFFER_
	for(int iCycle=0;iCycle<m_iDisplayCycleCount;iCycle++)
	{
		iAcquisitionDataCount = m_iAcquisitionDataCount[iCycle];
		string.Format(L"\n\nCycle %d Min (%d points)\n",iCycle,iAcquisitionDataCount);
		file.WriteString(string);
		pAcquisitionData = (char*)m_pAcquisitionDataMin[iCycle];
		psAcquisitionData = (short*)m_pAcquisitionDataMin[iCycle];
		for(int z=0;z<iAcquisitionDataCount;z++)
		{
			if(m_iAcquisitionDataSize[iCycle]==sizeof(BYTE))
			{
				cValue = (char)pAcquisitionData[z];
				if(m_bAcquisitionDataSigned[iCycle])
					string.Format(L"%d\n",(int)cValue);
				else{
					byValue = (BYTE)cValue;
					string.Format(L"%u\n",(unsigned int)byValue);
				}
			}else
			if(m_iAcquisitionDataSize[iCycle]==sizeof(WORD))
			{
				sValue = (short)psAcquisitionData[z];
				if(m_bAcquisitionDataSigned[iCycle])
					string.Format(L"%d\n",(int)sValue);
				else{
					wValue = (WORD)sValue;
					string.Format(L"%u\n",(unsigned int)wValue);
				}
			}
			file.WriteString(string);
		}
	}
#endif _MINMAX_BUFFER_
}

void CChildView::UpdateImage(int x,int y)
{
	if(!m_pImage->Check())
		return;
	Invalidate();
	UpdateWindow();
}

void CChildView::InitImage(bool bForceQuickAccess)
{
	if(!CDlgCscanDisplay::IsDisplayEnabled())
		InitImageAscan(bForceQuickAccess);
	else
		InitImageCscan(bForceQuickAccess);
}

void CChildView::InitImageAscan(bool bForceQuickAccess)
{
	unsigned char byValue;
	char cValue;
	unsigned short usValue,usOffset,usScale;
	short sValue;
	int xrect,yrect;
	DWORD dwValue=0;//could be an "eChar" or "UChar" or ... "ULong".
	BYTE *pcDataValue=NULL;
	BYTE *pcValue=NULL;
	int *piIndexX=NULL,*piIndexY=NULL;
	CAutoDelete<BYTE*> autoDeleteDataValue(pcDataValue);
	CAutoDelete<BYTE*> autoDeleteValue(pcValue);
	CAutoDelete<int*> autoDeleteX(piIndexX);
	CAutoDelete<int*> autoDeleteY(piIndexY);
	int iDataCursorX=0;
	int iDataCursorY=0;
	int iAcquisitionDataCount;
	double dValue;
	BYTE *pAcquisitionData;
	WORD *pwAcquisitionData;
	bool bAscanSign=m_bAscanSign;

	if(!m_pImage)
		return;
	if(m_iDataByteSize>sizeof(LONGLONG))
		return;
	if(!m_pImage->Check())
		return;
	m_pImage->ClearBits();
	iDataCursorX = 0;
	iDataCursorY = 0;
	if((iDataCursorX>=0) && (iDataCursorY>=0))
	{
		EnterCriticalSection(&m_CriticalSection);
		if(!m_ullUpdateCount)
		{
			if(!m_bDisplayImage)
			{
				for(int z=0;z<m_aiScreenAxisCount[0];z++)
				{
					if(m_bCompress[0])
						xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
					else
						xrect = z;
					dValue = (128+128.0*sin(2*z/128.0*3.1415))/256.0;
					dValue *= m_aiScreenAxisCount[1];
					byValue = (BYTE)dValue;
					if(!m_bDisplayInvert)
						m_pImage->SetPointColor(xrect,0,byValue);
					else
						m_pImage->SetPointColor(0,xrect,byValue);
				}
				m_pImage->SetBufferMin(false);
				m_pImage->SetBufferMax(true);
			}else{
				m_pImage->SetBufferMin(false);
				m_pImage->SetBufferMax(false);
				for(int z=0;z<m_aiScreenAxisCount[0];z++)
				{
					if(m_bCompress[0])
						xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
					else
						xrect = z;
					dValue = (128+128.0*sin(2*z/128.0*3.1415))/256.0;
					dValue *= 256;
					byValue = (BYTE)dValue;
					if(!m_bDisplayInvert)
						m_pImage->SetPointColor(xrect,0,byValue);
					else
						m_pImage->SetPointColor(0,xrect,byValue);
				}
			}
		}else{
			if(!m_bDisplayImage)
			{
				iAcquisitionDataCount = m_iAcquisitionDataCount[m_iAcquisitionCycle];
				if(	(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e8Bits) ||
					(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==eLog8Bits)	)
				{
					if(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==eLog8Bits)
						bAscanSign = false;
					pAcquisitionData = (BYTE*)m_pAcquisitionDataMax[m_iAcquisitionCycle];
					if(pAcquisitionData)
					for(int z=0;(z<m_aiScreenAxisCount[0]) && (z<m_iAcquisitionDataCount[m_iAcquisitionCycle]);z++)
					{
						if(m_bCompress[0])
							xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
						else
							xrect = z;
						if(!bAscanSign)
							byValue = (BYTE)pAcquisitionData[z];
						else{
							cValue = (char)pAcquisitionData[z];
							byValue = cValue+128;
						}
						if(!m_bDisplayInvert)
							m_pImage->SetPointColor(xrect,0,byValue);
						else
							m_pImage->SetPointColor(0,xrect,byValue);
					}
				}else
				if(	(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e12Bits) ||
					(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e16Bits)	)
				{
					if(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e12Bits)
					{
						usOffset = 2048;//2^(12-1)
						usScale = 16;//2^(12-8)
					}else{
						usOffset = 32768;//2^(16-1)
						usScale = 256;//2^(16-8)
					}
					pwAcquisitionData = (WORD*)m_pAcquisitionDataMax[m_iAcquisitionCycle];
					if(pwAcquisitionData)
					for(int z=0;(z<m_aiScreenAxisCount[0]) && (z<m_iAcquisitionDataCount[m_iAcquisitionCycle]);z++)
					{
						if(m_bCompress[0])
							xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
						else
							xrect = z;
						if(!m_bAscanSign)
							usValue = (WORD)pwAcquisitionData[z];
						else{
							sValue = (short)pwAcquisitionData[z];
							usValue = sValue+usOffset;
						}
						usValue /= usScale;
						byValue = (BYTE)usValue;
						if(!m_bDisplayInvert)
							m_pImage->SetPointColor(xrect,0,byValue);
						else
							m_pImage->SetPointColor(0,xrect,byValue);
					}
				}
				m_pImage->SetBufferMax(m_bAcquisitionDataMax[m_iAcquisitionCycle]);
#ifdef _MINMAX_BUFFER_
				if(InitImageAscanMin())
					m_pImage->SetBufferMin(true);
				else
					m_pImage->SetBufferMin(false);
#endif _MINMAX_BUFFER_
			}else{
				m_pImage->SetBufferMin(false);
				m_pImage->SetBufferMax(false);
				for(int iCycle=0;iCycle<m_iDisplayCycleCount;iCycle++)
				{
					if(m_bCompress[1])
						yrect = iCycle*m_aiClientRect[1]/m_aiScreenAxisCount[1];
					else
						yrect = iCycle;
					iAcquisitionDataCount = m_iAcquisitionDataCount[iCycle];
					if(	(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e8Bits) ||
						(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==eLog8Bits)	)
					{
						if(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==eLog8Bits)
							bAscanSign = false;
						pAcquisitionData = (BYTE*)m_pAcquisitionDataMax[iCycle];
						if(pAcquisitionData)
						for(int z=0;(z<m_aiScreenAxisCount[0]) && (z<m_iAcquisitionDataCount[iCycle]);z++)
						{
							if(m_bCompress[0])
								xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
							else
								xrect = z;
							if(!bAscanSign)
								byValue = (BYTE)pAcquisitionData[z];
							else{
								cValue = (char)pAcquisitionData[z];
								byValue = cValue+128;
							}
							if(!m_bDisplayInvert)
								m_pImage->SetPointColor(xrect,yrect,byValue);
							else
								m_pImage->SetPointColor(yrect,xrect,byValue);
						}
					}else
					if(	(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e12Bits) ||
						(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e16Bits)	)
					{
						if(m_eAcquisitionDataBitSize[m_iAcquisitionCycle]==e12Bits)
						{
							usOffset = 2048;//2^(12-1)
							usScale = 16;//2^(12-8)
						}else{
							usOffset = 32768;//2^(16-1)
							usScale = 256;//2^(16-8)
						}
						pwAcquisitionData = (WORD*)m_pAcquisitionDataMax[iCycle];
						if(pwAcquisitionData)
						for(int z=0;(z<m_aiScreenAxisCount[0]) && (z<m_iAcquisitionDataCount[iCycle]);z++)
						{
							if(m_bCompress[0])
								xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
							else
								xrect = z;
							if(!m_bAscanSign)
								usValue = (WORD)pwAcquisitionData[z];
							else{
								sValue = (short)pwAcquisitionData[z];
								usValue = sValue+usOffset;
							}
							usValue /= usScale;
							byValue = (BYTE)usValue;
							if(!m_bDisplayInvert)
								m_pImage->SetPointColor(xrect,yrect,byValue);
							else
								m_pImage->SetPointColor(yrect,xrect,byValue);
						}
					}
				}
			}
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
}

void CChildView::InitImageCscan(bool bForceQuickAccess)
{
	unsigned char byValue;
	char cValue;
	int xrect,yrect;
	DWORD dwValue=0;//could be an "eChar" or "UChar" or ... "ULong".
	BYTE *pcDataValue=NULL;
	BYTE *pcValue=NULL;
	int *piIndexX=NULL,*piIndexY=NULL;
	CAutoDelete<BYTE*> autoDeleteDataValue(pcDataValue);
	CAutoDelete<BYTE*> autoDeleteValue(pcValue);
	CAutoDelete<int*> autoDeleteX(piIndexX);
	CAutoDelete<int*> autoDeleteY(piIndexY);
	int iDataCursorX=0;
	int iDataCursorY=0;
	int iAcquisitionDataCount;
	double dValue;
	BYTE *pAcquisitionData;
	bool bAscanSign=false;//m_bAscanSign;
	LONGLONG ullCount;

	if(!m_pImage)
		return;
	if(m_iDataByteSize>sizeof(LONGLONG))
		return;
	if(!m_pImage->Check())
		return;
	m_pImage->ClearBits();
	iDataCursorX = 0;
	iDataCursorY = 0;
	if((iDataCursorX>=0) && (iDataCursorY>=0))
	{
		EnterCriticalSection(&m_CriticalSection);
		if(CDlgCscanDisplay::IsHwGate())
			ullCount = CDlgCscanDisplay::m_ullUpdateHWCount;
		else
			ullCount = CDlgCscanDisplay::m_ullUpdateSWCount;
		if(!ullCount)
		{
			if(!m_bDisplayImage)
			{
				for(int z=0;z<m_aiScreenAxisCount[0];z++)
				{
					if(m_bCompress[0])
						xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
					else
						xrect = z;
					dValue = (128+128.0*sin(2*z/128.0*3.1415))/256.0;
					dValue *= m_aiScreenAxisCount[1];
					byValue = (BYTE)dValue;
					if(!m_bDisplayInvert)
						m_pImage->SetPointColor(xrect,0,byValue);
					else
						m_pImage->SetPointColor(0,xrect,byValue);
				}
			}else{
				for(int z=0;z<m_aiScreenAxisCount[0];z++)
				{
					if(m_bCompress[0])
						xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
					else
						xrect = z;
					dValue = (128+128.0*sin(2*z/128.0*3.1415))/256.0;
					dValue *= 256;
					byValue = (BYTE)dValue;
					if(!m_bDisplayInvert)
						m_pImage->SetPointColor(xrect,0,byValue);
					else
						m_pImage->SetPointColor(0,xrect,byValue);
				}
			}
		}else{
			if(!m_bDisplayImage)
			{
				if(CDlgCscanDisplay::IsHwGate())
					iAcquisitionDataCount = CDlgCscanDisplay::m_aiAcquisitionHWAmpCount[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
				else
					iAcquisitionDataCount = CDlgCscanDisplay::m_aiAcquisitionSWAmpCount[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
				bAscanSign = CDlgCscanDisplay::m_abGateSign[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
				if(CDlgCscanDisplay::IsHwGate())
				{
					if(CDlgCscanDisplay::m_abGateEnableHW[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle])
						pAcquisitionData = (BYTE*)CDlgCscanDisplay::m_apAcquisitionHWAmp[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
					else
						pAcquisitionData = NULL;
				}else{
					if(CDlgCscanDisplay::m_abGateEnableSW[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle])
						pAcquisitionData = (BYTE*)CDlgCscanDisplay::m_apAcquisitionSWAmp[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
					else
						pAcquisitionData = NULL;
				}
				if(pAcquisitionData)
				for(int z=0;(z<m_aiScreenAxisCount[0]) && (z<iAcquisitionDataCount);z++)
				{
					if(m_bCompress[0])
						xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
					else
						xrect = z;
					if(!bAscanSign)
						byValue = (BYTE)pAcquisitionData[z];
					else{
						cValue = (char)pAcquisitionData[z];
						byValue = cValue+128;
					}
					if(!m_bDisplayInvert)
						m_pImage->SetPointColor(xrect,0,byValue);
					else
						m_pImage->SetPointColor(0,xrect,byValue);
				}
			}else{
				for(int iCycle=0;iCycle<m_iDisplayCycleCount;iCycle++)
				{
					if(m_bCompress[1])
						yrect = iCycle*m_aiClientRect[1]/m_aiScreenAxisCount[1];
					else
						yrect = iCycle;
					if(CDlgCscanDisplay::IsHwGate())
						iAcquisitionDataCount = CDlgCscanDisplay::m_aiAcquisitionHWAmpCount[CDlgCscanDisplay::m_iComboGate][iCycle];
					else
						iAcquisitionDataCount = CDlgCscanDisplay::m_aiAcquisitionSWAmpCount[CDlgCscanDisplay::m_iComboGate][iCycle];
					bAscanSign = CDlgCscanDisplay::m_abGateSign[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle];
					if(CDlgCscanDisplay::IsHwGate())
					{
						if(CDlgCscanDisplay::m_abGateEnableHW[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle])
							pAcquisitionData = (BYTE*)CDlgCscanDisplay::m_apAcquisitionHWAmp[CDlgCscanDisplay::m_iComboGate][iCycle];
						else
							pAcquisitionData = NULL;
					}else{
						if(CDlgCscanDisplay::m_abGateEnableSW[CDlgCscanDisplay::m_iComboGate][m_iAcquisitionCycle])
							pAcquisitionData = (BYTE*)CDlgCscanDisplay::m_apAcquisitionSWAmp[CDlgCscanDisplay::m_iComboGate][iCycle];
						else
							pAcquisitionData = NULL;
					}
					if(pAcquisitionData)
					for(int z=0;(z<m_aiScreenAxisCount[0]) && (z<iAcquisitionDataCount);z++)
					{
						if(m_bCompress[0])
							xrect = z*m_aiClientRect[0]/m_aiScreenAxisCount[0];
						else
							xrect = z;
						if(!bAscanSign)
							byValue = (BYTE)pAcquisitionData[z];
						else{
							cValue = (char)pAcquisitionData[z];
							byValue = cValue+128;
						}
						if(!m_bDisplayInvert)
							m_pImage->SetPointColor(xrect,yrect,byValue);
						else
							m_pImage->SetPointColor(yrect,xrect,byValue);
					}
				}
			}
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
}
