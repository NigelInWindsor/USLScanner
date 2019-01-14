// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__460B726A_3903_4C2D_B9EE_975FE5233C70__INCLUDED_)
#define AFX_DATA_H__460B726A_3903_4C2D_B9EE_975FE5233C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Coord.h"	// Added by ClassView
#include "Version1.h"
#include "HRTimer.h"
#include "PolyCoord.h"	// Added by ClassView
#include "VertexArray.h"
#include "CADElement.h"
#include "DataProperties.h"
#include "Inclusions.h"


#define	NUMBER_BUFFERS			100
#define	NUMBER_VOLUME_BUFFERS	256
#define	NUMBER_BSCAN_BUFFERS	2
#define	MAX_DETAILS				200

#define APPEND_BOTTOM	0
#define APPEND_TOP		1
#define	APPEND_LEFT		2
#define	APPEND_RIGHT	3


typedef struct _ANNOTATION {
	int nStructSize;
	int nXs;
	int	nYs;
	float fXp;
	float fYp;
	WCHAR Text[256];
	LOGFONT	lf;
	COLORREF	rgbTextColor;
	COLORREF	rgbBkColor;
	int	nApplyToAllImage;
	int	nImageType;
	int	nImageNumber;
	int	nBkMode;
	int	nSpare[19];
} ANNOTATION, * PANNOTATION;


/*struct _SIFData{
	CCoord Position;
	int AmpL;
	int AmpImage[100];
	int ThickL;
	int Thickimage[100];
}*/

// These defines and structures for printing Cones and cylinders
#define	CONE_TOP		0
#define	CONE_BOT		1
#define CONE_INNER		0
#define	CONE_OUTER		1

struct ConeData {
	int nStructSize;

	float fR[2];
	float fC[2];
	float fScanHeight;
	float fHeight;
	float fAngle;
	float fTriangleAngle;
};

struct DonutData {
	int nStructSize;

	float fR[2];
	float fC[2];
	float fRSqr[2];
	float fArcAngle;
	float fTriangleAngle;
	float fTanArcAngle;
	float fTanTriangleAngle;

	float fSector[2];
	int	nSample[2];

	int	nArcBufferWidth;
	int nArcBufferHeight;

	unsigned char* cArcAmpBuffer;
	int *nSrcPtrArray;

	float fPaperWidth;
	float fPaperHeight;


};

#define	D_CAST						0
#define	D_STANDARD					1
#define	D_BATCH						2		//Also filename
#define	D_TEST_DATE					3
#define	D_OPERATOR					4
#define	D_UTFILE					5
#define	D_SENTENCE					6
#define	D_NOTES						7
#define D_SETUP_FILE				8
#define	D_CUSTOMER					9
#define	D_CONTRACT					10
#define	D_SPECIFICATION				11
#define	D_ITEM						12
#define	D_TECHNIQUE					13
#define	D_LOT						14
#define	D_ALLOY						15
#define	D_HEAT_NO					16
#define	D_ITEM_CODE					17
#define	D_NUMBER_ITEMS_IN_LOT		18
#define	D_ULTRASONIC_SPEC			19
#define	D_CRITERIA					20
#define	D_DOCUMENT					21
#define	D_FORMULA					22
#define	D_PRODUCTION_FACILITY		23
#define	D_BATCH_NUMBER				24
#define D_UNIQUE_ID					25


extern UINT	ReadAllVolume(LPVOID pParam);
extern UINT CalculateLineAndSampleThread(LPVOID pParam);

class CData  
{

public:
	void InvalidateVolBytesPerSample();
	void ImportAllEnvelopesFromProfile();
	void SaveImageNumber(CUSLFile* pFile, int nImageNumber, int nSentence);
	void Rotate90Degrees(CProgressCtrl* pProgress);
	void CopyInclusions(CInclusions* pInclusions);
	bool BuildInclusionData(int nImageType,int nImageNumber,CRect rrSample);

	CInclusions m_Inclusions;
	void AutoFillDataProperties();
	bool SaveBladeCSV(CFile *pFile,int nSide, int nLine, bool  bEverything_TrafficLights);

	inline int MinMax(int pn,int nMin,int nMax);
	int		MinMax(int *pnV, int nMin, int nMax);
	long	MinMax(long *pnV, int nMin, int nMax);

	void ClearLinePriorToScan(int nWhichScanLineType,int nLine);
	void CollectBladeData();
	bool SaveBladeXML(CUSLFile *pFile, int nCad);
	bool SaveBladeXML(CUSLFile *pFile);
	void FindLineMaxXExtent(int nWhich, float *pfMin, float *pfMax);
	void CopyBladeLinesFromProfile();

	D3DXMATRIXA16 m_Matrix[4];

	void	CropLeftEdge(int nNumberSamples);
	void	CropRightEdge(int nNumberSamples);
	void	AppendDataSet(int nWhichSide, CData *pNewData);
	void	PutvLineSamplesBackInttoScanCoordinates();
	void	PutvLineSampleInCADCoordinates();
	bool	HaveAllLinesBeenScanned();
	bool	GetVerticesAndResultsForALine(int nLine, CStringArray  &Array);
	void	GetResultsForALine(int nLine, CStringArray &Array);
	bool	TransformScanVertexToCADVertexWithTwist(D3DXVECTOR3 *ptDest, D3DXVECTOR3 *normDest, CUSLVertex *vertSrc, int nLine);
	void	GetAmplitudeCrossSection(int nImageNumber,int nLine,int *pnAmplitudeArray);
	void	GetThicknessCrossSection(int nImageNumber,int nLine,float *pThicknessArray);
	void	MoveSideOfParallelogram(int nCorner, float fSample, float fLine);
	void	MoveCornerOfParallelogram(int nCorner, float fSample, float fLine);
	void	MoveSideOfCentreLine(int nSide, float fSample, float fLine);
	bool	SetCADElementFlags(int nFlags);
	float	GetHistogramDefectHeight(HistogramStruct *pHist,CString *pBuff,bool bVerbose);
	float	GetHistogramDefectWidth(HistogramStruct *pHist,CString *pBuff,bool bVerbose);
	float	GetHistogramDefectArea(HistogramStruct *pHist,CString *pBuff,bool bVerbose);
	float	GetHistogramDefectLevel(HistogramStruct *pHist,CString *pBuff,bool bVerbose);
	float	GetHistogramRefLevel(HistogramStruct *pHist,CString *pBuff = NULL,bool bVerbose = false);
	void	BuildHistogramString(int nFlags,HistogramStruct *pHist,bool bVerbose = false);
	void	SetWidthSameHeightCADElement(int nPtr);
	void	SetHeightSameWidthCADElement(int nPtr);
	float	CircumferenceEllipse(float fWidth,float fHeight);
	void	ComputeAllElementStats(int nPtr);
	void	SnapToNearestCADElement(int nSrcElement,int nFeature);
	bool	IsCADElement();
	void	ToggleCADElementFlag(int nFlag);
	bool	GetCADElementFlags(int *pnFlags);
	void	GetCADVerboseStats(int nPtr, CString *pBuff);
	float	GetCADElementLength(int nPtr, CString *pBuff = NULL,bool bVerbose = false);
	float	GetCADElementArea(int nPtr, CString *pBuff = NULL,bool bVerbose = false);
	float	GetCADElementHeight(int nPtr, CString *pBuff = NULL,bool bVerbose = false);
	float	GetCADElementWidth(int nPtr, CString *pBuff = NULL,bool bVerbose = false);
	void	GetCADElementHistogram(int nPtr,CString *pBuff = NULL,bool bVerbose = false);
	int GetCADElement();
	int	TempStoreNodesCADElement(float fSample,float fLine);
	int	OffsetCADElement(float fSample,float fLine);
	int	DeleteCADElement(int nPtr);
	int	ModifyLeftCADElement(float fSample,float fLine);
	int	ModifyRightCADElement(float fSample,float fLine);
	int	ModifyTopCADElement(float fSample,float fLine);
	int	ModifyBottomCADElement(float fSample,float fLine);
	int	ModifyNodeCADElement(int nNode,float fSample,float fLine);
	int	AddCADElement(CCADElement *pElement);
	int	AddCADElement(CADElementType Type,float fSample,float fLine,int nImageType, int nImageNumber, int nPaletteNumber);
	int AddCADElement();
	int AddCADNode(int nElement, CADElementType Type, CCoord Cp);
	int FindNearestCADNode(CCoord Cp);
	bool GetCoordFromLineSample(CPoint ptSample,CCoord *pCp);
	bool IsLineSample();
	void SetPloygonLengthAreaMode(int nWhich, int nMode);
	void GenerateSamplePositionLine(int nLine,CPolyCoord *pLine);
	void GenerateSamplePositionVertices();
	void GenerateRotarySamplePositionVertices();
	void ChangeFlattenMode(int nNewFlattenMode);
	CVertexArray	*m_vLineSample;
	void ClearAllPolygons();
	void ClearPolygon(int nIndex);
	void AddPolygon(int *nPolyPtr);
	void AddPolygonPt(int *nPolyPtr,CCoord Cp);
	CHRTimer m_HR;
	double GenerateArcAmpBuffer(int nImageNumber,ConeData *pCone,DonutData *pDonut,float fGratStartAngle);
	double GenerateArcAmpBufferNieve(int nImageNumber,ConeData *pCone,DonutData *pDonut,float fGratStartAngle);
	void MedianFilter3X3(int nSrcType,int nSrc, int nDest);
	void MeanFilter3X3(int nSrcType,int nSrc, int nDest);
	void SobelEdgeFilter(int nSrcType,int nSrc, int nDest);
	void DifferentiateAmpBuffer(int nSrcType,int nSrc,int nDest);
	void FilterAmpBufferWithConvolutionFilter(int nSrcType,int nSrc,int nDest);
	void XMLWriteMES(CUSLFile *pFile,CString Buff);
	void MESSaveOEM();
	bool MESRetrieveConsumable(CString FileName);
	void FillForLineIncrement(int nLine,int nIncrement);
	void FillForDPCError(int nLine);
	void DeGlitch(int nLine);
	bool SaveXMLAttempt1(CUSLFile *pFile,int nTS,int nImageType,int nImageNumber);
	bool SaveXMLAttempt0(CUSLFile *pFile,int nTS,int nImageType,int nImageNumber);
	bool SaveCSV(CUSLFile *pFile, int nTimeSlot, int nImageType, int nImageNumber);
	bool SaveVolumeXMLAttempt0(CUSLFile *pDestFile,CUSLFile *pSrcFile,int nVolumeNumber);
	void XLMPointId(int nPtr,CCoord Cp,CArchive *pArch);
	void XMLEntryLabel(CString strLabel,int nValue,CArchive *pArch);
	void XMLEntryLabel(CString strLabel,CString strField,CArchive *pArch);
	void XMLEntryLabel(CString strLabel,CString strFormat,float fValue,CArchive *pArch);
	void XMLValuePid(int nSample, int nValue, CArchive *pArch);
	void XMLValuePid(int nSample, float fValue, int nPrecision, CArchive *pArch);
	void XMLValuePid(int nSample, double dValue, int nPrecision, CArchive *pArch);

	//This stuff is for the thread to read all the A scans
	void ReadEveryAscan(CUSLFile* pFile,int nVolumeNumber);
	CUSLFile* m_pFile64;
	int m_nVolumneNumber;
	int	m_nVolBytesPerSample;

	
	void ClearDetails(int nActionFlag);
	void FillAutomaticDetails(int nActionFlag);
	void RetrieveCompatibleThumbNail(CUSLFile* pFile,CDC* pDC,CRect rr);
	void DisplayThumbNail(CString csFilePath, CDC *pDC, CRect rr);
	void ConvertVolumeFromThermometerToTwosComplement(CUSLFile* pFile);
	void SaveBmpTexture(CUSLFile *pFile, CRect rrArea);
	int GetNumberTextureFaces();
	HRESULT	GenerateTextureFacesCoordinates(CProgressCtrl* pProgress);
	HRESULT BuildARGBBuffer(int nImageNumber,int nImageType);
	bool RetrieveComplexGeometryData(CUSLFile* pFile);
	bool m_bModified;
	int AddAnnotation(float fXp, float fYp, CString Text, LOGFONT lf, COLORREF rgbTextColor, int nImageType, int nImageNumber, int nApplyToAllImages, int nBkMode, COLORREF rgbBkColor);
	void DeleteAnnotation(int nIndex);
	void AppendDataBase(bool bWhich = true);
	bool IsNameRequired();
	void GetBscanTrace(int nWhich,int nSample,int nLine,char *pTrace);
	void VolumeGenerateVerticalBscan(CUSLFile* pFile,CProgressCtrl *pProgress,int nVolumeNumber,int nTS,int nSample,GatesData* pGate);
	void VolumeGenerateHorizontalBscan(CUSLFile* pFile,CProgressCtrl *pProgress,int nVolumeNumber,int nTS,int nLine,GatesData* pGate);
	void RenderOverlayPhaseImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor);
	void GenerateOneTimeOnlyLineLength();
	float GetCircumference(int nLine);
	void GetFastScanSizeIncrement(float * pfSize, float * pfIncrement, int nLine);
	float GetAreaOfSample(int nLine);
	void RenderExtImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor);
	void RenderTimeImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer);
	void RenderFilterImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor,int nLineDirection = 0);
	void RenderFilterImageHorizontal(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor);
	void RenderAmpImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor,int nLineDirection);
	void RenderAmpImageHorizontal(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor);
	void RenderAmpImageVertical(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor);
	void RenderAmpImagePolar(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor);
	void RenderPhaseImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer);
	void CirclePoints(char *pScreenBuffer,CPoint pt, int x, int y, int nn,int nMax,int nLine,int nImageNumber,int nBytesPixel,char* cColor,int nArrayWidth);
	void SetPixel(char *pScreenBuffer,int nX,int nY,char* cColor,int nBytesPixel,int nArrayWidth);
	int VolumeGenerateTimeCscanByte(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding);
	int VolumeGenerateTimeCscanWord(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding);
	int VolumeGenerateTimeCscan(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding);
	int VolumeGenerateAmpCscanByte(CUSLFile * pFile, CProgressCtrl * pProgress, GatesData * pGate, int nVolumeNumber, int nTS, int nEncoding, CString Name);
	int VolumeGenerateAmpCscanWord(CUSLFile * pFile, CProgressCtrl * pProgress, GatesData * pGate, int nVolumeNumber, int nTS, int nEncoding, CString Name);
	int VolumeGenerateAmpCscan(CUSLFile* pFile,CProgressCtrl* pProgress,GatesData *pGate,int nVolumeNumber,int nTS,int nEncoding,CString Name);
	void SaveVolumeDuringScan(int nLine);
	void SaveDuringScan();
	void AllocateVolumeBuffersAndOffsets();
	void RetrieveVolumeHeader(CUSLFile* pFile,int nImageNumber,int* pnTimeSlot);
	void CalculateStandardFileLength();
	void AllocateToolPathAndOffsets();
	void SaveScanLines();
	int GetFilteredTimeThickness(int nSample,int nLine,int nImageNumber,int nTimeThickness);
	int GetFilteredAmp(int nSample,int nLine,int nImageNumber, int nImageType = 0);
	char GetFilteredAmpChar(int nSample, int nLine, int nImageNumber);
	bool GetStringValue(int nImageType,int nImageNumber,int nPaletteNumber, int nTS,CPoint pt,CString *pBuff);
	void CollectComplexData();
	void CollectComplexDataWithProbeOffsets();
	void CollectComplexDataSingleProbe();
	void CollectComplexDataPhasedArray();
	void CollectComplexDataMultiProbeScan();
	void AllocateImageBuffers();
	void CalculateNumberOfImages();
	bool RemoveWaterSplash(int nImageType,int nImageNumber,CRect rr);
	bool BuildExtHistogramData();
	bool BuildTimeHistogramData();
	bool BuildAmpHistogramData();
	bool ReBuildHistogramData();
	bool BuildHistogramData(int nImageType,int nImageNumber,CRect rr);
	bool RetrieveFokkerDosFile(CUSLFile *pFile);
	bool RetrieveGenericScannerFile(CUSLFile *pFile);
	bool RetrieveCompatibleFile(CUSLFile *pFile);
	bool Retrieve(CUSLFile *pFile);
	int Save(CUSLFile* pFile, bool bSaveJustHeader = false);
	bool Save();
	void Zero();
	bool RetrieveCIPFile(CUSLFile *pFile); //CIP - Combined Image Position
	bool SaveCIPFile(CUSLFile *pFile);

	CPolyCoord *m_LineBlade[NUMBER_BUFFERS];
	int			m_nLineBladeL[NUMBER_BUFFERS];

	CCoord m_CpCursor;
	CCoord m_CpNode[4];
	int	m_nCADNode;
	int	m_nCADPtr;
	int	m_nCADElementL;
	CCADElement* m_pCADElement;

	CPolyCoord* m_pEnvelopeLine;
	int			m_nEnvelopeLineL;
	CPolyCoord* m_pPolygon;
	int			m_nPolygonL;
	bool		m_bCScansSaved;
	bool		m_bVolScansSaved;
;

	D3DXVECTOR3	vTTCentre;
	CCoord m_CpTurnTableCentre;
	CCoord m_CpFinish;
	CCoord m_CpStart;
	CCoord m_CpTranslation[MAX_LINE_TYPES];
	HistogramStruct	m_Hist;

	TimeSlotData	m_TS[256];
	UtGlobalData	m_Global;

	int				*m_nLineScannedStatus;
	CCoord			*m_pCpPos;
	unsigned char	*m_cAmpBuffer[NUMBER_BUFFERS];
	unsigned int	*m_nTimeBuffer[NUMBER_BUFFERS];
	unsigned char	*m_cPhaseBuffer[NUMBER_BUFFERS];
	unsigned char	*m_cVolumeBuffer[NUMBER_VOLUME_BUFFERS][256];
	unsigned char	*m_cBscanBuffer[NUMBER_BSCAN_BUFFERS];
	unsigned char	*m_cFilterBuffer[NUMBER_BUFFERS];
	unsigned __int16 *m_16ExtBuffer[NUMBER_BUFFERS];
	COLORREF		*m_pARGBArray;

	TextureFace		*m_pTextureFace;
	int				m_nTextureFaceL;
	ANNOTATION *m_pAnnotation;
	int m_nAnnotationL;
	float *m_fLineLength;
	float *m_fSampleWidth;

	UINT64 m_n64FileLength;
	UINT64 m_n64VolumeOffset[8];
	UINT64 m_n64ToolPathOffset;
	UINT64 m_n64ScanLinesOffset;

	int	m_nCollectScanLines;
	int	m_nCollectSurface;
	int	m_nNumberProbesPerImage;
	int	m_nFirstProbe;
	int	m_osTestDateTime;
	int	m_nSentence;
	int m_nSamplesLine;
	int m_nNumberLines;
	int	m_nNumberTimeSlots;
	int	m_nNumberImages;
	int	m_nNumberAmpImages[256];
	int	m_nNumberTimeImages[256];
	int	m_nNumberPhaseImages[256];
	int	m_nNumberVolumeImages;
	int	m_n32VolumeOffset[256];
	int	m_nNumberExtImages;
	int	m_nAscanLength;
	int	m_nRealAscanLength;
	int	m_nDetailsL;
	int m_nScanStartLine;
	int m_nScanFinishLine;
	int	m_nLineIncrement;
	int	m_nLineIncrementMode;
	ComponentDescription	m_nComponentDescription;
	int	m_nScanType;
	int	m_nCollectAxis;
	int	m_nFastAxis;
	int	m_nSlowAxis;
	int m_nImageNameL;
	int m_nFlattenMode;
	int m_nLogRange;

	int m_nConvolutionImageType;
	int m_nConvolutionSrc;
	int m_nConvolutionDest;
	bool m_bThreadEnabled;
	bool m_bThreadFinished;
	CWinThread* m_pThread;

	bool	m_bHeaderForVolumeIsValid;

	float	m_fFastIncrement;
	float	m_fSlowIncrement;
	float	m_fFastScanSize;
	float	m_fSlowScanSize;
	float	m_fComponentDiameter[2];	//First Line, Last Line;

	CString m_MachineLocation;
	CString m_MachineName;
	CString m_UniqueIdentification;
	CString	m_FilePath;
	CString	m_FileName;
	CString m_TestDateTime;
	CString m_ProfileName;
	CString m_ProfileDescription;
	CString m_Operator;
	CString	m_Details[MAX_DETAILS];
	CString	m_Caption[MAX_DETAILS];
	CString m_ImageName[NUMBER_BUFFERS];
	CString m_CIPFileName;
	CString m_CIPFilePath;
	CDataProperties m_DataProperties;

	CWnd* m_pParent;
	CData(CWnd* pParent = NULL);
	virtual ~CData();

	int m_nPtr[256];			//Used in collect complex data
	int m_nLastPtr[256];		//Used in collect complex data
	int	m_nLastLine;
	int m_nMinViewLine;
	int m_nMaxViewLine;

	int m_nNumberVertexLines;
	D3DXMATRIXA16 m_matCadToScan;

};


UINT ConvolutionThread(LPVOID pParam);

#endif // !defined(AFX_DATA_H__460B726A_3903_4C2D_B9EE_975FE5233C70__INCLUDED_)
