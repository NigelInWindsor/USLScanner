// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once
#pragma warning( disable : 4091)

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define POINTER_64 __ptr64

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#include <afxsock.h>            // MFC socket extensions

//#include <afxpriv.h>		//Manually added for private WM_KICKIDLE


#include "USLDriver\sys\USLIoctl.h"
#include "Si6Driver\sys\Si6IOctl.h"
#include "ServoDriver\sys\ServoIOctl.h"
#include "SI10Driver\Si10Ioctl.h"

//#define align(a)

#include "AOS\OEMPA 1.1.5.4\Sources\UserHeaders\win32\UTKernelDriver.h"
#include "AOS\OEMPA 1.1.5.4\Sources\UserHeaders\win32\UTKernelDriverOEMPA.h"
#include "AOS\OEMPA 1.1.5.4\Sources\UserHeaders\win32\UTKernelAPI.h"
#include "AOS\OEMPA 1.1.5.4\Sources\Common\CustomizedDriverAPI.h"
#include "AOS\OEMPA 1.1.5.4\Sources\Common\CustomizedWizardAPI.h"
#include "AOS\OEMPA 1.1.5.4\Sources\UserHeaders\win32\TimerProcess.h"


#include <malloc.h>
#include <math.h>
#include "USLFile.h"
#include "Coord.h"
#include "RectFloat.h"
#include "LeadScrew.h"
#include "Vertex.h"
#include "Constants.h"
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <DirectXMath.h>
#include <afxcontrolbars.h>

using namespace DirectX;

#define	USLDRIVER			0
#define	SI6DRIVER			1

#define	CURSOR_LEFT			37
#define	CURSOR_UP			38
#define	CURSOR_RIGHT		39
#define	CURSOR_DOWN			40
#define	PGUP				33
#define	PGDOWN				34

#define	LIMERICK_UNIVERSITY	484
#define	VTT_HELSINKY		485
#define	EXHIBITION			300
#define	TIMET				509
#define	CORUS				536
#define	TWI					531
#define	BAE_VERTICAL		320
#define	DOWTY				257
#define ACG					559
#define	CTG_COMPOSITES		560
#define	BAE_5METER			570
#define	BAE_FLAPERONS		565
#define	RAVENSCOURT			567
#define	AWE_ALDERMARSTON	574
#define	BAE_TROLLEY			590
#define	AIM_COMPOSITES		582
#define	QUEEN_MARYS_COLLEGE	586
#define	FOKKER				590
#define	HARBIN				592
#define	NLR					618
#define DOWTY_BSCAN			620
#define	GKN					645
#define	FOREPLANE			659
#define ICAM				662
#define RRFLATPACK			664
#define	GOODRICH_MACHINE_4	700
#define	BAE_FOREPLANE_2		701
#define GKN_ALABAMA_2		702
#define BAE_SUB				693 //This should be 702 as this number should match the job S number, but that has been used
#define HENGSHEN			791

#define	SI6				0
#define	SERVO			1
#define	PMAC_TURBO		1
#define	PMAC_NORMAL		2
#define SI10			3

#define	COLLECT_DATA	0
#define	REPLAY			1

#define	PORTSIDE				0
#define	STARBOARD				1
#define	WRISTFINGERPORTSIDE		2
#define	WRISTFINGERSTARBOARD	3

#define	MOVE_PORTSIDE		1
#define	MOVE_STARBOARD		2
#define	MOVE_BOTH			3

#define	OUTSIDE			0
#define	INSIDE			1

#define	TAUGHTLINES			0
#define	SCANLINES			1
#define	BRAINLINES			2
#define	ENVELOPELINES		3
#define	HOLES				4
#define	NORMALS				5
#define	TRANSFORM			6
#define	DETAILS				7
#define	HOLDINGFIXTURE		8
#define	NEWLINE				9
#define	SURFACELINES		10
#define	WAYLINES			11
#define	OUTSIDELINES		12
#define	INSIDELINES			13
#define	BRANEOUTSIDELINES	14
#define	BRANEINSIDELINES	15

#define	A_NOTHING			0
#define A_LONGDATE			1
#define A_LONGDATE_TIME		2
#define A_SHORTDATE			3
#define A_SHORTDATE_TIME	4
#define A_OPERATOR			5
#define	A_CLEAR				6

//messages to main frame
#define	INVALIDATE_VIEW	100

#define	HIST_ATTENUATION	0
#define	HIST_REFLECTION		1

#define UPDATE_TIMESLOTS 101

#define	MAX_LINE_TYPES			10


enum	VariablesUsed {
	H_RAXIS = 0,
	H_EOL,
	H_WP0,
	H_WP1,
	H_GAIN0,
	H_GAIN1,
	H_DELAY0,
	H_DELAY1,
	H_DELAY2,
	H_DELAY3,
	H_DELAY4,
	H_DELAY5,
	H_WIDTH0,
	H_WIDTH1,
	H_WIDTH2,
	H_WIDTH3,
	H_WIDTH4,
	H_WIDTH5,
};

enum ComponentDescription {
	RECTILINEAR = 0,
	OPEN_ROTARY,
	CLOSED_ROTARY,
	CIRCULAR,
};

enum FrequencyFilterType {
	FILTER_OFF = 0,
	LOWPASS,
	HIGHPASS,
	BANDPASS,
	BANDSTOP,
};


struct HistogramStruct {
	int	*pnData;
	int	nCoordL;
	int	nTotalNumberSamples;
	int	nTotalBelowThreshold[2];
	int	nTotalAboveThreshold[2];
	int	nMaxAmp;
	int	nPeakAmp;
	int	nAverageAmp;
	int	nAttenAmp;
	int	nMaxThickness;
	int	nAverageThickness;	//.01mm
	int	nPeakThickness;		//.01mm
	int	nAmpMode;
	int	nMaxValue;
	int	nMinValue;
	int	nAmplitudeScaleType;

	int nMaxExt;
	int nAverageExt;
	int	nPeakExt;
	float fLatchExt;
	float fPeakExt;
	float fAverageExt;

	float *pfArea;
	float fLatchAmp;
	float fPeakAmp;
	float fAverageAmp;
	float fRefAmp;
	float fLatchThickness;
	float fLatchTime;
	float fPeakTime;
	float fAverageTime;
	float fStd;
	float fCp;
	float fCpk;
	float fMaxValue;
	float fMinValue;
	float fAverageBetween;
	float fStdBetween;
	float fTotalAreaBelowThreshold[2];
	float fTotalAreaAboveThreshold[2];

	int	nSampleThreshold[2];
	int	nPaintThreshold;
	bool bMeanLatched;
	CRect rrThreshWidth;
	CRect rrThreshHeight;
	CRectFloat rrFloatThreshWidth;
	CRectFloat rrFloatThreshHeight;
	WCHAR	wStr[512];

	int nImageType;
	int nImageNumber;
	CRect rr;
};

struct HoldingFixtureCoord {
	int	nStructSize;

	float	fXFixed;
	float	fYFixed;
	float	fZFixed;
	float	fXMoving;
	float	fYMoving;
	float	fZMoving;

	void Zero() {
		ZeroMemory(this,sizeof this);
	}
};

struct	VectorStruct {
	int	nStructSize;
	float fI;
	float fJ;
	float fK;
};

struct	EntityStruct {
	int nStructSize;
	int nEntityType;
	int	nParameterData;
	int	nStructure;
	int	nLineFont;
	int	nLevel;
	int	nView;
	int	nMatrix;
	int	nLabel;
	int	nStatus;
	int	nLineWeight;
	int	nColor;
	int	nParameterLineCount;
	int	nFormNumber;
	int	nReserved0;
	int nReserved1;
	int	nEntityLabel;
	int	nEntitySubscript;

	float	*fData;
	int		nDataL;

	D3DXMATRIXA16 mat;
};


enum CADElementType {
	CAD_MOUSE = 0,
	CAD_LINE,
	CAD_ELLIPSE,
	CAD_MULTI_LINE,
	CAD_MOVE_NODE,
	CAD_RECTANGLE,
	CAD_PARALLELOGRAM,
	CAD_CENTRELINE,
};

enum VertexArrayType {
	VA_LINE = 1,
	VA_ARC,
	VA_CIRCLE,
	VA_ARRAY_LINE,
	VA_X_VECTOR,
	VA_Y_VECTOR,
	VA_Z_VECTOR,
	VA_HORIZ_PLANE,
	VA_VERT_PLANE,
	VA_ANGLE,
};

struct YPRROTATES
{
	float fYaw;
	float fPitch;
	float fRoll;
};

class CD3DArcBall_10
{
public:

    INT            m_iWidth;   // ArcBall's window width
    INT            m_iHeight;  // ArcBall's window height
    FLOAT          m_fRadius;  // ArcBall's radius in screen coords
    FLOAT          m_fRadiusTranslation; // ArcBall's radius for translating the target
    VOID SetTranslationRadius( FLOAT fRadiusTranslation ) { m_fRadiusTranslation = fRadiusTranslation; }


    D3DXQUATERNION m_qDown;               // Quaternion before button down
    D3DXQUATERNION m_qNow;                // Composite quaternion for current drag
    D3DXMATRIXA16  m_matRotation;         // Matrix for arcball's orientation
    D3DXMATRIXA16  m_matRotationDelta;    // Matrix for arcball's orientation
    D3DXMATRIXA16  m_matTranslation;      // Matrix for arcball's position
    D3DXMATRIXA16  m_matTranslationDelta; // Matrix for arcball's position
    BOOL           m_bDrag;               // Whether user is dragging arcball
    BOOL           m_bRightHanded;        // Whether to use RH coordinate system
	INT			   m_nDragAxes;			  // 0: All 1:X 2:Y 3:Z

    D3DXVECTOR3 ScreenToVector( int sx, int sy );

	int				m_iCurMouseX;      // Saved mouse position
    int				m_iCurMouseY;
    D3DXVECTOR3		m_vDown;         // Button down vector

	void SetTranslationMatrix( float fX,float fY, float fZ );
	void SetRotationMatrix( float fX,float fY, float fZ );
    D3DXMATRIX* GetRotationMatrix()         { return &m_matRotation; }
    D3DXMATRIX* GetRotationDeltaMatrix()    { return &m_matRotationDelta; }
    D3DXMATRIX* GetTranslationMatrix()      { return &m_matTranslation; }
    D3DXMATRIX* GetTranslationDeltaMatrix() { return &m_matTranslationDelta; }
    BOOL        IsBeingDragged()            { return m_bDrag; }

    VOID        SetRadius( FLOAT fRadius );
    VOID        SetWindow( INT w, INT h, FLOAT r=0.9 );
    VOID        SetRightHanded( BOOL bRightHanded ) { m_bRightHanded = bRightHanded; }

                CD3DArcBall_10();
    VOID        Init(bool bJustMatrices = FALSE);
};

struct WorldVertex
{
    D3DXVECTOR3 p; // position
    D3DXVECTOR3 n; // normal
    D3DXVECTOR2 t; // texture coordinate

    static const DWORD FVF;
};



struct PrimitiveData {
	LPDIRECT3DVERTEXBUFFER9 pVB;
	int	nPrimitiveType;
	int	nPrimitiveCount;
	int	nCullMode;
	int	nZMode;

	D3DXVECTOR3 Translate;
	D3DXVECTOR3 Rotate;

	bool	bComponent;
};

struct Vertex
{
    CCoord p; // position
	CPoint pt;	//Sample / Line
	bool bOnComponent;
};

struct TextureFace
{
	Vertex v[4];
};

#pragma pack(1)
struct CUSTOMVERTEX
{
    D3DXVECTOR3 vecPos;
    D3DXVECTOR3 vecNorm;
    D3DCOLOR    dwColor;
    D3DXVECTOR2 vecText;

    static const DWORD FVF;

};
//const DWORD CUSTOMVERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE 
 //      | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);

#pragma pack()


// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

//#endif // !defined(AFX_STDAFX_H__5A26A7F8_8F48_4902_A514_39C781418A32__INCLUDED_)

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
