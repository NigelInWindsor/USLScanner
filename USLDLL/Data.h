// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__BD8DE06A_AD74_4F2F_8F46_95137EA6C63F__INCLUDED_)
#define AFX_DATA_H__BD8DE06A_AD74_4F2F_8F46_95137EA6C63F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"


#define	NUMBER_BUFFERS			100
#define	NUMBER_VOLUME_BUFFERS	256
#define	NUMBER_BSCAN_BUFFERS	2
#define	MAX_DETAILS				200
#define	MAX_LINE_TYPES			20

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
	int	nSpare[20];
} ANNOTATION, * PANNOTATION;


/*struct _SIFData{
	CCoord Position;
	int AmpL;
	int AmpImage[100];
	int ThickL;
	int Thickimage[100];
}*/

// These defines and structures for printing Cones and cylinders
#define	TOP		0
#define	BOT		1
#define INNER	0
#define	OUTER	1

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


UINT	ReadAllVolume(LPVOID pParam);

class CData  
{
public:
	bool Open(CString *FileName);
	CUSLFile* m_pFile64;
	int m_nVolumneNumber;

	int	m_nCADNode;
	int	m_nCADPtr;
	int	m_nCADElementL;

	int			m_nEnvelopeLineL;
	int			m_nPolygonL;


	int				*m_nLineScannedStatus;
	unsigned char	*m_cAmpBuffer[NUMBER_BUFFERS];
	unsigned int	*m_nTimeBuffer[NUMBER_BUFFERS];
	unsigned char	*m_cPhaseBuffer[NUMBER_BUFFERS];
	unsigned char	*m_cVolumeBuffer[NUMBER_VOLUME_BUFFERS][256];
	unsigned char	*m_cBscanBuffer[NUMBER_BSCAN_BUFFERS];
	unsigned char	*m_cFilterBuffer[NUMBER_BUFFERS];
	unsigned __int16 *m_16ExtBuffer[NUMBER_BUFFERS];
	COLORREF		*m_pARGBArray;

	int				m_nTextureFaceL;

	int AddAnnotation(float fXp,float fYp,CString Text,LOGFONT lf,COLORREF rgbTextColor,int nImageType,int nImageNumber,int nApplyToAllImages);
	int m_nAnnotationL;

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
	int	m_nScanType;
	int	m_nCollectAxis;
	int	m_nFastAxis;
	int	m_nSlowAxis;
	int m_nImageNameL;
	int m_nFlattenMode;

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

	CWnd* m_pParent;
	CData();
	virtual ~CData();

	int m_nLastPtr[256];		//Used in collect complex data
	int	m_nLastLine;
	int m_nMinViewLine;
	int m_nMaxViewLine;

	int m_nNumberVertexLines;
	D3DXMATRIXA16 m_matCadToScan;

};


UINT ConvolutionThread(LPVOID pParam);

#endif // !defined(AFX_DATA_H__BD8DE06A_AD74_4F2F_8F46_95137EA6C63F__INCLUDED_)
