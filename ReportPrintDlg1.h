#if !defined(AFX_REPORTPRINTDLG_H__27B17FEC_DC0B_472F_89F4_6A4E58F45C22__INCLUDED_)
#define AFX_REPORTPRINTDLG_H__27B17FEC_DC0B_472F_89F4_6A4E58F45C22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportPrintDlg1.h : header file
//
#include "StaticPosManage.h"
#include "TextPropertiesDlg.h"
#include "LinePropertiesDlg.h"
#include "RectanglePropertiesDlg.h"
#include "ImagePropertiesDlg.h"
#include "DatePropertiesDlg.h"
#include "ListPropertiesDlg.h"
#include "BarGraphPropertiesDlg1.h"
#include "FeatureEditDlg.h"
#include "DataValuePropertiesDlg.h"
#include "ViewPage.h"
#include "PrinterUtils.h"
#include "ReportTablePropertiesDlg.h"
#include "ReportUltrasonicsPropertiesDlg.h"
#include "ReportHeaderFooterDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CReportPrintDlg dialog

#define	VIEWDLG				0
#define	MULTIPLERESULTSDLG	1

#define	PORTRAIT	0
#define	LANDSCAPE	1

class CReportPrintDlg : public CDialog
{
// Construction
public:
	void NextLine(float *pfTop, int *pnPageNumber, float fLineSep);
	void OnChangeEditPageNumber();
	void OnDeltaposSpinPageNumber(NMHDR* pNMHDR, LRESULT* pResult);
	void CheckPageNumber(int *pnPageNumber,float *pfTop);
	float RRBladeHeader(float fTop, float fLineSep, int nPageNumber);
	void FormatBladeSectionReport();
	void FormatBladePlanReport();
	void DrawModernTable(CPaintDC* pDC,FeatureStruct* pFeature,int nDevice);
	void CloseAllPropertiesDlgs();
	void OnReportScale(float fScale);
	void DrawCAD(CPaintDC *pDC, CRect rr, FeatureStruct *pFeature, int nDevice);
	void SetOrientation(int nOrientation );
	void AlicePage0();
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void CloseAllChildren();
	void UpdateWindowName();
	void RectToMM(CRect rr, float *fTop, float *fLeft, float *fBottom, float *fRight);
	void AddDataValue(int nDataValueType, float fTop, float fLeft, int nSize, COLORREF rgb,int nWeight);
	void AddText(CString Buff,float fTop,float fLeft,int nSize,COLORREF rgb,int nWeight=0,WCHAR *pFaceName = NULL, int nPageNumber = 0);
	void AddLine(float fTop,float fLeft,float fBottom,float fRight,int nWidth,COLORREF rgb);
	void MMtoCoords(float fTop,float fLeft,float fBottom,float fRight,CRect *prr);
	void VSMPOPageTwo();
	void VSMPOPageOne();
	void VSMPOPageZero();
	void DrawCTGAnalysisList(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice);
	void DrawMultipleResultsList(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice);
	void DrawDefectTableList(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawComponentStatsList(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawPl15List(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawPr15List(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawCADElementList(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawPr30List(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawReportList(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice);
	void DrawComponentDetailsList(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice);

	void DrawImageScale(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice);
	void DrawImageFileName(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice);
	void DrawGraticule(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice);
	void DrawBorder(CPaintDC *pDC,FeatureStruct* pFeature,int nDevice);
	void DeleteFeature(int nPtr);
	void AddFeature(int nType,CPoint pt,int nVerbose = TRUE);

	CBarGraphPropertiesDlg* m_pBarGraphPropertiesDlg;
	CTextPropertiesDlg*			m_pTextPropertiesDlg;
	CLinePropertiesDlg*			m_pLinePropertiesDlg;
	CRectanglePropertiesDlg*	m_pRectanglePropertiesDlg;
	CImagePropertiesDlg*		m_pImagePropertiesDlg;
	CDatePropertiesDlg*			m_pDatePropertiesDlg;
	CListPropertiesDlg*			m_pListPropertiesDlg;
	CFeatureEditDlg*			m_pFeatureEditDlg;
	CDataValuePropertiesDlg*	m_pDataValuePropertiesDlg;
	CReportTablePropertiesDlg*	m_pReportTablePropertiesDlg;
	CReportUltrasonicsPropertiesDlg* m_pReportUltrasonicsPropertiesDlg;
	CReportHeaderFooterDlg*		m_pReportHeaderFooterDlg;
	
	int m_nMoveAction;
	CString m_FileName;
	void MoveFeatures();
	int m_nNumberToBeModified;
	int m_nFeaturePtr;
	bool FindFeature(CPoint pt);
	bool FindFeature(CRect rr);
	CRect m_rectDefine;
	int m_nLBOption;
	void Zero();
	bool Retrieve(LPCTSTR lpszFileName);
	bool Save(LPCTSTR lpszFileName);

	void DrawDefineRectangle(int nStyle=PS_SOLID,int nWidth=1);
	void DrawDefineLine();

	void PaperToPrinterMM(CRect *prr,float *fTop,float *fBottom,float *fLeft,float *fRight);
	void PaperToPrinterMM(CPoint *pt,float *fX,float *fY);
	void PaperToDevice(CPoint *pPt,int nDevice);
	void PaperToDevice(CRect *prr,int nDevice);
	void PaperToClient(CPoint *pPt);
	void PaperToClient(CRect *prr);
	void ClientToPaper(CPoint* pPt);
	void ClientToPaper(CRect *pRect);
	void PaperToPrinter(CPoint *pPt);
	void PaperToPrinter(CRect *prr);

	void WorldToClient(CRect rr,float fX, float fY,CPoint *pt);
	void DrawAnnotation(CPaintDC *pDC,CRect rr, FeatureStruct *pFeature, int nDevice);
	void DrawDate(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawImage(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawText(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawDataValues(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawRectangle(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawLine(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawList(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawBarGraph(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawTable(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawUltrasonics(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawHeaderFooter(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawBladePlan(CPaintDC *pDC, CRect rr, int nDevice);

	void TextChanged(CString Caption,LOGFONT *plf,COLORREF rgbText);
	FeatureStruct *pFeature;
	int m_nFeatureL;
	int m_nContextOption;
	CPoint m_ptClient;
	COLORREF m_rgbText;
	void DrawBorders(CPaintDC* pDC,CRect rr,int nDevice);
	void SetAspectRatio();
	int		m_nOrientation;
	CRect m_rectPrintArea;
	CRect m_rectPaper;
	void GetPaperStatistics();

	HDC m_hdcPrinter;
	CDC m_dcPrinter;
//	DEVMODE FAR *m_lpDevMode;
	CPrintDialog* m_pPrintDlg;
	CStaticPosManage m_StaticPosView;

	float m_fPrintingAreaWidthMM;
	float m_fPrintingAreaHeightMM;

	HCURSOR m_hcursorVMove;
	HCURSOR m_hcursorHMove;
	HCURSOR m_hcursorAllMove;
	HCURSOR	m_hcursorCross;

	CDialogBar	m_wndPageNumberDlgBar;
	CToolBar   m_wndPrintToolBar;
	CReBar     m_wndRebar;

	int	m_nPageNumber;
	int	m_nMaxPageNumber;
	bool StopPage(CDC* pDC);
	int	m_nEditOption;
	int m_nParentType;
	CData* m_pData;
	
	CViewPage* m_pViewPage;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	void Construct(CWnd* pParent,CWnd** pDlg,CData* pData);
	int	nParentType;
	CReportPrintDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,CViewPage* pPage = NULL,CData* pData=NULL);   // standard constructor
	virtual ~CReportPrintDlg();
// Dialog Data
	//{{AFX_DATA(CReportPrintDlg)
	enum { IDD = IDD_REPORT_PRINT_DIALOG1 };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportPrintDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportPrintDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTemplatePagesetup();
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTemplateSaveas();
	afx_msg void OnTemplateOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnReportLeftalign();
	afx_msg void OnReportRightalign();
	afx_msg void OnReportCenter();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTemplatePrint();
	afx_msg void OnTemplateNew();
	afx_msg void OnExamplesPlateReport();
	afx_msg void OnExamplesSetupreport();
	afx_msg void OnTemplateEdit();
	afx_msg void OnSelectDelete();
	afx_msg void OnSelectMove();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonBargraph();
	afx_msg void OnButtonDate();
	afx_msg void OnButtonErase();
	afx_msg void OnButtonImage();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonList();
	afx_msg void OnButtonMove();
	afx_msg void OnButtonPointer();
	afx_msg void OnButtonRectangle();
	afx_msg void OnButtonRectangleSelection();
	afx_msg void OnButtonText();
	afx_msg void OnButtonVariable();
	afx_msg void OnButtonLandscape();
	afx_msg void OnButtonPortrait();
	afx_msg void OnReport11();
	afx_msg void OnTemplateExporttoword();
	afx_msg void OnReportScalex1();
	afx_msg void OnReportScale12();
	afx_msg void OnReportScale14();
	afx_msg void OnReportCenterpoint();
	afx_msg void OnReportProperties();
	afx_msg void OnReportOrigionalimage();
	afx_msg void OnButtonTable();
	afx_msg void OnButtonUltrasonics();
	afx_msg void OnButtonHeaderFooter();
	afx_msg void OnAnnotationX1();
	afx_msg void OnAnnotationX2();
	afx_msg void OnAnnotationX4();
	afx_msg void OnAnnotationX6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTPRINTDLG_H__27B17FEC_DC0B_472F_89F4_6A4E58F45C22__INCLUDED_)
