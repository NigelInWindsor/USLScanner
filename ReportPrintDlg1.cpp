// ReportPrintDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ReportPrintDlg1.h"
#include "ViewPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FONT_SIZE	12
/////////////////////////////////////////////////////////////////////////////
// CReportPrintDlg dialog

inline void CReportPrintDlg::Construct(CWnd* pParent,CWnd** pDlg,CData* pData)
{
	
	m_pDlg = pDlg;
	
	m_pParent = pParent;
	m_pData = pData;
	if(m_pData==NULL) m_pData = &theApp.m_Data;
	pFeature = NULL;
	m_nParentType=0;
	m_nPageNumber = 0;
	m_nMaxPageNumber = 0;

	m_pPrintDlg = new CPrintDialog(FALSE);
	m_pPrintDlg->GetDefaults();
	m_hdcPrinter = m_pPrintDlg->GetPrinterDC();
	if(m_hdcPrinter == NULL) {
		MessageBox(_T("No printer Drivers installed"));
	} else {
		m_dcPrinter.Attach(m_hdcPrinter);
	}


//	m_lf;
//	ZeroMemory(&m_lf, sizeof(LOGFONT));

//	m_lf.lfHeight = -16;
//	wcscmp(m_lf.lfFaceName,_T("Times New Roman") );

	m_nLBOption = 0;
	m_rectDefine.SetRectEmpty();

	m_nFeatureL = 0;
	m_nFeaturePtr = -1;
	m_nNumberToBeModified = 0;
	m_nMoveAction = 0;

	m_pTextPropertiesDlg = NULL;
	m_pLinePropertiesDlg = NULL;
	m_pRectanglePropertiesDlg = NULL;
	m_pImagePropertiesDlg = NULL;
	m_pDatePropertiesDlg = NULL;
	m_pListPropertiesDlg = NULL;
	m_pBarGraphPropertiesDlg = NULL;
	m_pFeatureEditDlg = NULL;
	m_pDataValuePropertiesDlg = NULL;
	m_pReportTablePropertiesDlg = NULL;
	m_pReportUltrasonicsPropertiesDlg = NULL;
	m_pReportHeaderFooterDlg = NULL;

	m_hcursorCross = AfxGetApp()->LoadCursor(IDC_CURSOR_CROSS);

}


CReportPrintDlg::CReportPrintDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CViewPage* pViewPage,CData* pData)
	: CDialog(CReportPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportPrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nParentType=0;
	m_pViewPage = pViewPage;
	Construct(pParent,pDlg,pData);


}

CReportPrintDlg::~CReportPrintDlg()
{
}


void CReportPrintDlg::Zero()
{
	int	ii;

	if(pFeature) {
		for(ii=0;ii<m_nFeatureL;ii++) {
			SAFE_DELETE(pFeature[ii].pCaption);
			SAFE_DELETE(pFeature[ii].pnIDArray);
		}
		delete pFeature;
	}
	pFeature = NULL;
	m_nFeatureL = 0;

}


void CReportPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportPrintDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CReportPrintDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_TEMPLATE_PAGESETUP, OnTemplatePagesetup)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TEMPLATE_SAVEAS, OnTemplateSaveas)
	ON_COMMAND(ID_TEMPLATE_OPEN, OnTemplateOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_REPORT_LEFTALIGN, OnReportLeftalign)
	ON_COMMAND(ID_REPORT_RIGHTALIGN, OnReportRightalign)
	ON_COMMAND(ID_REPORT_CENTRE, OnReportCenter)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_TEMPLATE_PRINT, OnTemplatePrint)
	ON_COMMAND(ID_TEMPLATE_NEW, OnTemplateNew)
	ON_COMMAND(ID_EXAMPLES_PLATEREPORT, OnExamplesPlateReport)
	ON_COMMAND(ID_EXAMPLES_SETUPREPORT, OnExamplesSetupreport)
	ON_COMMAND(ID_TEMPLATE_EDIT, OnTemplateEdit)
	ON_COMMAND(ID_SELECT_DELETE, OnSelectDelete)
	ON_COMMAND(ID_SELECT_MOVE, OnSelectMove)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_BARGRAPH, OnButtonBargraph)
	ON_COMMAND(ID_BUTTON_DATE, OnButtonDate)
	ON_COMMAND(ID_BUTTON_ERASE, OnButtonErase)
	ON_COMMAND(ID_BUTTON_IMAGE, OnButtonImage)
	ON_COMMAND(ID_BUTTON_LINE, OnButtonLine)
	ON_COMMAND(ID_BUTTON_LIST, OnButtonList)
	ON_COMMAND(ID_BUTTON_MOVE, OnButtonMove)
	ON_COMMAND(ID_BUTTON_POINTER, OnButtonPointer)
	ON_COMMAND(ID_BUTTON_RECTANGLE, OnButtonRectangle)
	ON_COMMAND(ID_BUTTON_RECTANGLE_SELECTION, OnButtonRectangleSelection)
	ON_COMMAND(ID_BUTTON_TEXT, OnButtonText)
	ON_COMMAND(ID_BUTTON_VARIABLE, OnButtonVariable)
	ON_COMMAND(ID_BUTTON_LANDSCAPE, OnButtonLandscape)
	ON_COMMAND(ID_BUTTON_PORTRAIT, OnButtonPortrait)
	ON_COMMAND(ID_REPORT_11, OnReport11)
	ON_COMMAND(ID_TEMPLATE_EXPORTTOWORD, OnTemplateExporttoword)
	ON_COMMAND(ID_REPORT_SCALEX1, OnReportScalex1)
	ON_COMMAND(ID_REPORT_SCALE12, OnReportScale12)
	ON_COMMAND(ID_REPORT_SCALE14, OnReportScale14)
	ON_COMMAND(ID_REPORT_CENTERPOINT, OnReportCenterpoint)
	ON_COMMAND(ID_REPORT_PROPERTIES, OnReportProperties)
	ON_COMMAND(ID_REPORT_ORIGIONALIMAGE, OnReportOrigionalimage)
	ON_COMMAND(ID_BUTTON_TABLE, OnButtonTable)
	ON_COMMAND(ID_BUTTON_ULTRASONICS, OnButtonUltrasonics)
	ON_COMMAND(ID_BUTTON_HEADER_FOOTER, OnButtonHeaderFooter)
	ON_COMMAND(ID_ANNOTATION_X1, OnAnnotationX1)
	ON_COMMAND(ID_ANNOTATION_X2, OnAnnotationX2)
	ON_COMMAND(ID_ANNOTATION_X4, OnAnnotationX4)
	ON_COMMAND(ID_ANNOTATION_X6, OnAnnotationX6)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PAGE_NUMBER, OnDeltaposSpinPageNumber)
	ON_EN_CHANGE(IDC_EDIT_PAGE_NUMBER, OnChangeEditPageNumber)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportPrintDlg message handlers
int CReportPrintDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndPrintToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndPrintToolBar.LoadToolBar(IDR_TOOLBAR_PRINT)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	
	m_wndPageNumberDlgBar.Create(this, IDD_PRINT_PAGE_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_PRINT_PAGE_DIALOGBAR);


	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}
	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+360,30,SWP_SHOWWINDOW);


	m_wndRebar.AddBar(&m_wndPrintToolBar);
	m_wndRebar.AddBar(&m_wndPageNumberDlgBar);
	
	return 0;
}

void CReportPrintDlg::PostNcDestroy() 
{
	if(m_pPrintDlg) delete m_pPrintDlg;
	m_pPrintDlg = NULL;

	Zero();	
	CDialog::PostNcDestroy();
	delete this;
}

void CReportPrintDlg::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CReportPrintDlg::IDD]=FALSE;

	CloseAllChildren();


 	delete m_pPrintDlg;
	m_pPrintDlg=NULL;

	switch(m_nParentType) {
	default:	theApp.m_ViewReportTemplateName = m_FileName;
		break;
	case 999: // just to shut up compiler
		break;
	}
	theApp.SaveRegistryVariables();
	
	CDialog::OnClose();
	DestroyWindow();
}

void CReportPrintDlg::OnDestroy() 
{
	

	if (CReportPrintDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CReportPrintDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CReportPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;
	
	CString FilePath ;
	switch(m_nParentType) {
	default:	FilePath = theApp.m_LastSettingsPath + "\\" + theApp.m_ViewReportTemplateName;
		break;
	case 999: // just to shut up compiler
		break;
	}
	Retrieve(FilePath);

//	AlicePage0();
//	VSMPOPageZero();
//	VSMPOPageOne();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	GetPaperStatistics();

	if (CReportPrintDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CReportPrintDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}
	
	m_StaticPosView.SetNewPos();   
	SetAspectRatio();


	CEdit* pEdit = (CEdit* ) m_wndPageNumberDlgBar.GetDlgItem(IDC_EDIT_PAGE_NUMBER);
	Buff.Format(L"%d",m_nPageNumber + 1);
	pEdit->SetWindowText(Buff);
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndPageNumberDlgBar.GetDlgItem(IDC_SPIN_PAGE_NUMBER);
	pSpin->SetRange(0,19);
	pSpin->SetPos(m_nPageNumber);


	
	SetIcon( AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportPrintDlg::OnSize(UINT nType, int cx, int cy) 
{
	
	CRect rr;

	CDialog::OnSize(nType, cx, cy);
	GetWindowRect(rr);

	if(GetSafeHwnd()) {
		rr.bottom = MulDiv(rr.Width(),m_rectPaper.Height(),m_rectPaper.Width()) + rr.top;
		SetWindowPos( NULL , rr.left, rr.top, rr.Width(), rr.Height(), SWP_SHOWWINDOW);

		m_StaticPosView.SetNewPos();  
		Invalidate(FALSE);

	}
	

}

void CReportPrintDlg::OnTemplatePagesetup() 
{
	

//	m_pPrintDlg->Create(AFX_IDD_PRINTSETUP,this);
	CPageSetupDialog dlg;
	dlg.DoModal();

	GetPaperStatistics();
}

void CReportPrintDlg::GetPaperStatistics()
{
 
	if(m_dcPrinter) {
		m_rectPaper.SetRectEmpty();
		m_rectPaper.right = m_dcPrinter.GetDeviceCaps(PHYSICALWIDTH);
		m_rectPaper.bottom = m_dcPrinter.GetDeviceCaps(PHYSICALHEIGHT);

		m_rectPrintArea.SetRectEmpty();
		m_rectPrintArea.left = 0;//m_dcPrinter.GetDeviceCaps(PHYSICALOFFSETX);
		m_rectPrintArea.top = 0;//m_dcPrinter.GetDeviceCaps(PHYSICALOFFSETY);
		m_rectPrintArea.right = m_rectPrintArea.left + m_dcPrinter.GetDeviceCaps(HORZRES);
		m_rectPrintArea.bottom = m_rectPrintArea.top + m_dcPrinter.GetDeviceCaps(VERTRES);


		int	nPrintingAreaWidth = m_dcPrinter.GetDeviceCaps(HORZSIZE);	//908			//287mm
		int	nPrintingAreaHeight = m_dcPrinter.GetDeviceCaps(VERTSIZE);	//1213			//405mm
		int nXRes = m_dcPrinter.GetDeviceCaps(LOGPIXELSX);					//600			//300 pixels / inch
		int nYRes = m_dcPrinter.GetDeviceCaps(LOGPIXELSY);					//600			//300 pixels / inch
		float fXResMM = (float)nXRes / 25.4f;
		float fYResMM = (float)nYRes / 25.4f;
		m_fPrintingAreaWidthMM   = (float)m_dcPrinter.GetDeviceCaps(HORZRES) / fXResMM;
		m_fPrintingAreaHeightMM  = (float)m_dcPrinter.GetDeviceCaps(VERTRES) / fYResMM;

		if(m_rectPaper.Width() > m_rectPaper.Height()) {
			m_nOrientation = DMORIENT_LANDSCAPE;
		} else {
			m_nOrientation = DMORIENT_PORTRAIT;
		}
	} else {
		m_rectPaper.SetRect(0,0,1800,2200);
		m_rectPrintArea.SetRect(40,40,1760,2160);

		m_fPrintingAreaWidthMM = 203.2f;
		m_fPrintingAreaHeightMM = 292.142f;

		m_nOrientation = DMORIENT_PORTRAIT;
	}

  
}

void CReportPrintDlg::SetAspectRatio()
{
	CRect rr;

	GetWindowRect(rr);
	rr.bottom = MulDiv(rr.Width(),m_rectPaper.Height(),m_rectPaper.Width()) + rr.top;
	SetWindowPos( NULL , rr.left, rr.top, rr.Width(), rr.Height(), SWP_SHOWWINDOW);

}

void CReportPrintDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	CRect rr;
	int nBitsPixel = dc.GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;

	m_staticView.GetWindowRect(&rr);
	rr.OffsetRect(-rr.left,-rr.top);

	CDC  dcMem;
	CBitmap bmp;
	char	*pArray;
	pArray = new char[(rr.Width()+2) * (rr.Height()+2) * 4];
	memset(pArray,0xff,(rr.Width()+2) * (rr.Height()+2) * 4);

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	DrawHeaderFooter((CPaintDC*)&dcMem,rr,SCREEN);
	DrawRectangle((CPaintDC*)&dcMem,rr,SCREEN);
	DrawText((CPaintDC*)&dcMem,rr,SCREEN);
	DrawImage((CPaintDC*)&dcMem,rr,SCREEN);
	DrawBladePlan((CPaintDC*)&dcMem,rr,SCREEN);
	DrawLine((CPaintDC*)&dcMem,rr,SCREEN);
	DrawDate((CPaintDC*)&dcMem,rr,SCREEN);
	DrawList((CPaintDC*)&dcMem,rr,SCREEN);
	DrawBarGraph((CPaintDC*)&dcMem,rr,SCREEN);
	DrawDataValues((CPaintDC*)&dcMem,rr,SCREEN);
	DrawTable((CPaintDC*)&dcMem,rr,SCREEN);
	DrawUltrasonics((CPaintDC*)&dcMem,rr,SCREEN);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;




}

void CReportPrintDlg::DrawBorders(CPaintDC *pDC, CRect rr, int nDevice)
{
	CRect rectBorder;
	CPen penGrey(PS_DOT,1,RGB(100,100,100));
	CPen* pOldPen = pDC->SelectObject(&penGrey);

	rectBorder = rr;
	rectBorder.left = MulDiv(m_rectPrintArea.left,rr.Width(),m_rectPaper.Width()) + rr.left;
	rectBorder.right = MulDiv(m_rectPrintArea.right,rr.Width(),m_rectPaper.Width()) + rr.left;
	rectBorder.top = MulDiv(m_rectPrintArea.top,rr.Height(),m_rectPaper.Height()) + rr.top;
	rectBorder.bottom = MulDiv(m_rectPrintArea.bottom,rr.Height(),m_rectPaper.Height()) + rr.top;

	pDC->MoveTo(rectBorder.TopLeft());
	pDC->LineTo(rectBorder.right,rectBorder.top);
	pDC->LineTo(rectBorder.BottomRight());
	pDC->LineTo(rectBorder.left,rectBorder.bottom);
	pDC->LineTo(rectBorder.TopLeft());

	pDC->SelectObject(pOldPen);
}

/*
void CReportPrintDlg::OnEditFont() 
{

	CClientDC dc(this);
//	lf.lfHeight = -MulDiv(m_nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
//	strcpy(lf.lfFaceName, m_FaceName);

	CFontDialog dlg(&m_lf);
	dlg.m_cf.rgbColors = m_rgbText;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&m_lf);
		m_rgbText = dlg.GetColor();
	}

}
*/
void CReportPrintDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup = NULL;
	m_ptClient = point;


	switch(m_nEditOption) {
	default:
		menu->LoadMenu(IDR_RB_REPORT_MENU);
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
		break;
	case ID_BUTTON_RECTANGLE_SELECTION:
		menu->LoadMenu(IDR_RB_REPORT_SELECT_MENU);
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
		break;
	}

	delete menu;
	
}



void CReportPrintDlg::DrawText(CPaintDC *pDC, CRect rr, int nDevice)
{

	int	ii;
	CString	Buff;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_TEXT) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);
			pDC->SetTextColor(pFeature[ii].rgbText);
			if(pFeature[ii].bModify==TRUE) {
				pDC->SetBkColor(RGB(238,240,155));
			} else {
				pDC->SetBkColor(RGB(255,255,255));
			}
			pDC->SetBkMode(TRANSPARENT);
			Buff.Format(_T("%s"),*pFeature[ii].pCaption);
			pt = pFeature[ii].rr.TopLeft();
			PaperToDevice(&pt,nDevice);
			pDC->TextOut(pt.x,pt.y,Buff);
			size = pDC->GetTextExtent(Buff);
			pFeature[ii].rectClient.TopLeft() = pt;
			pFeature[ii].rectClient.BottomRight() = pt + size;

			pDC->SelectObject(pOldFont);
			delete pFont;
		}
	}
	pDC->SetTextColor(rgbTextColor);

}

void CReportPrintDlg::DrawDataValues(CPaintDC *pDC, CRect rr, int nDevice)
{

	int	ii,nn;
	CString	Buff;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;

	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_DATA_VALUE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);
			pDC->SetTextColor(pFeature[ii].rgbText);
			if(pFeature[ii].bModify==TRUE) {
				pDC->SetBkColor(RGB(238,240,155));
				pDC->SetBkMode(OPAQUE);
			} else {
				pDC->SetBkColor(RGB(255,255,255));
				nOldBkMode = pDC->SetBkMode(TRANSPARENT);
			}
			Buff.Format(_T("%s ______"),*pFeature[ii].pCaption);
			nn=pFeature[ii].nDataValueType;
			if(m_pData->m_Details[nn].GetLength()) {
				Buff.Format(_T("%s %s"),*pFeature[ii].pCaption,m_pData->m_Details[nn]);
			}

			pt = pFeature[ii].rr.TopLeft();
			PaperToDevice(&pt,nDevice);
			pDC->TextOut(pt.x,pt.y,Buff);
			size = pDC->GetTextExtent(Buff);
			pFeature[ii].rectClient.TopLeft() = pt;
			pFeature[ii].rectClient.BottomRight() = pt + size;

			pDC->SelectObject(pOldFont);
			delete pFont;
		}
	}
	pDC->SetTextColor(rgbTextColor);
	pDC->SetBkMode(nOldBkMode);


}

void CReportPrintDlg::DrawDate(CPaintDC *pDC, CRect rr, int nDevice)
{

	int	ii;
	CString	Buff;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;
	CTime time(m_pData->m_osTestDateTime);
	CString Str;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_DATE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);
			pDC->SetTextColor(pFeature[ii].rgbText);
			pDC->SetBkColor(RGB(255,255,255));


			pt = pFeature[ii].rr.TopLeft();
			PaperToDevice(&pt,nDevice);
			switch(pFeature[ii].nDateFormat) {
			case 0: Buff = time.Format(_T("%d/%m/%Y"));
				break;
			case 1: Buff = time.Format(_T("%m/%d/%Y"));
				break;
			case 2: Buff = time.Format(_T("%d/%m/%y"));
				break;
			case 3: Buff = time.Format(_T("%m/%d/%y"));
				break;
			case 4: Buff = time.Format(_T("%d/%b/%Y"));
				break;
			case 5: Buff = time.Format(_T("%b/%d/%Y"));
				break;

			case 6: Buff = time.Format(_T("%d/%m/%Y %Hh%M"));
				break;
			case 7: Buff = time.Format(_T("%m/%d/%Y %Hh%M"));
				break;
			case 8: Buff = time.Format(_T("%d/%m/%y %Hh%M"));
				break;
			case 9: Buff = time.Format(_T("%m/%d/%y %Hh%M"));
				break;
			case 10: Buff = time.Format(_T("%d/%b/%Y %Hh%M"));
				break;
			case 11: Buff = time.Format(_T("%b/%d/%Y %Hh%M"));
				break;
			}

			if(pFeature[ii].pCaption->IsEmpty()==FALSE) {
				if(pFeature[ii].nCaptionPosition==0) {
					Str.Format(_T("%s %s"),*pFeature[ii].pCaption,Buff);
				} else {
					Str.Format(_T("%s %s"),Buff,*pFeature[ii].pCaption);
				}
			} else {
				Str = Buff;
			}
			pDC->TextOut(pt.x,pt.y,Str);
			size = pDC->GetTextExtent(Str);
			pFeature[ii].rectClient.TopLeft() = pt;
			pFeature[ii].rectClient.BottomRight() = pt + size;

			pDC->SelectObject(pOldFont);
			delete pFont;
		}
	}
	pDC->SetTextColor(rgbTextColor);

}

void CReportPrintDlg::ClientToPaper(CPoint *pPt)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	pPt->x = MulDiv(pPt->x-rr.left,10000,rr.Width());
	pPt->y = MulDiv(pPt->y-rr.top,10000,rr.Height());

}

void CReportPrintDlg::ClientToPaper(CRect *pRect)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	pRect->left = MulDiv(pRect->left-rr.left,10000,rr.Width());
	pRect->top = MulDiv(pRect->top-rr.top,10000,rr.Height());
	pRect->right = MulDiv(pRect->right-rr.left,10000,rr.Width());
	pRect->bottom = MulDiv(pRect->bottom-rr.top,10000,rr.Height());

}

void CReportPrintDlg::PaperToDevice(CPoint *pPt,int nDevice)
{

	switch(nDevice) {
	case 0: PaperToClient(pPt);
		break;
	case 1: PaperToPrinter(pPt);
		break;
	}
}

void CReportPrintDlg::PaperToDevice(CRect *prr,int nDevice)
{
	switch(nDevice) {
	case 0: PaperToClient(prr);
		break;
	case 1: PaperToPrinter(prr);
		break;
	}
}

void CReportPrintDlg::PaperToClient(CPoint *pPt)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

//	pPt->x = MulDiv(pPt->x,rr.Width(),10000)+rr.left;
//	pPt->y = MulDiv(pPt->y,rr.Height(),10000)+rr.top;
	pPt->x = MulDiv(pPt->x,rr.Width(),10000);
	pPt->y = MulDiv(pPt->y,rr.Height(),10000);

}

void CReportPrintDlg::PaperToClient(CRect *prr)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

//	prr->left = MulDiv(prr->left,rr.Width(),10000)+rr.left;
//	prr->top = MulDiv(prr->top,rr.Height(),10000)+rr.top;
//	prr->right = MulDiv(prr->right,rr.Width(),10000)+rr.left;
//	prr->bottom = MulDiv(prr->bottom,rr.Height(),10000)+rr.top;
	prr->left = MulDiv(prr->left,rr.Width(),10000);
	prr->top = MulDiv(prr->top,rr.Height(),10000);
	prr->right = MulDiv(prr->right,rr.Width(),10000);
	prr->bottom = MulDiv(prr->bottom,rr.Height(),10000);
}


void CReportPrintDlg::PaperToPrinter(CPoint *pPt)
{

	pPt->x = MulDiv(pPt->x,m_rectPrintArea.Width(),10000)+m_rectPrintArea.left;
	pPt->y = MulDiv(pPt->y,m_rectPrintArea.Height(),10000)+m_rectPrintArea.top;

}

void CReportPrintDlg::PaperToPrinter(CRect *prr)
{

	prr->left = MulDiv(prr->left,m_rectPrintArea.Width(),10000)+m_rectPrintArea.left;
	prr->top = MulDiv(prr->top,m_rectPrintArea.Height(),10000)+m_rectPrintArea.top;
	prr->right = MulDiv(prr->right,m_rectPrintArea.Width(),10000)+m_rectPrintArea.left;
	prr->bottom = MulDiv(prr->bottom,m_rectPrintArea.Height(),10000)+m_rectPrintArea.top;
}

void CReportPrintDlg::PaperToPrinterMM(CRect *prr,float *fTop,float *fBottom,float *fLeft,float *fRight)
{

	*fLeft	= m_fPrintingAreaWidthMM * (float)prr->left / 10000.0f;
	*fRight = m_fPrintingAreaWidthMM * (float)prr->right / 10000.0f;
	*fTop	= m_fPrintingAreaHeightMM * (float)prr->top / 10000.0f;
	*fBottom = m_fPrintingAreaHeightMM * (float)prr->bottom / 10000.0f;

}

void CReportPrintDlg::PaperToPrinterMM(CPoint *pt,float *fX,float *fY)
{

	*fX	= m_fPrintingAreaWidthMM * (float)pt->x / 10000.0f;
	*fY = m_fPrintingAreaHeightMM * (float)pt->y / 10000.0f;

}

void CReportPrintDlg::OnTemplateSaveas() 
{
	

	CFileDialog File(FALSE,_T(".tem"),m_FileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Template (*.tem)|*.tem|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_LastSettingsPath;
	if(File.DoModal()==IDOK) {
	
		m_FileName.Format(_T("%s"),File.GetPathName());
		Save(m_FileName);
		UpdateWindowName();

	};		
	
}

void CReportPrintDlg::OnTemplateNew() 
{
	Zero();
	UpdateWindowName();
	Invalidate(FALSE);
}

void CReportPrintDlg::OnTemplateOpen() 
{
	

	CFileDialog File(TRUE,_T(".tem"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Template (*.tem)|*.tem|All Files (*.*)|*.*||"),NULL);
	File.m_ofn.lpstrInitialDir=theApp.m_LastSettingsPath;
	if(File.DoModal()==IDOK) {
		CloseAllChildren();
		
		m_FileName.Format(_T("%s"),File.GetPathName());
		Retrieve(m_FileName);

		UpdateWindowName();
		Invalidate(FALSE);

	};	
	
}


bool CReportPrintDlg::Save(LPCTSTR lpszFileName)
{
	
	CUSLFile file;
	CString Buff;
	char	Identifier[]="Ultrasonic Sciences Generic Scanner Report Template";
	int	nFeatureOffset;
	int nIncrement = 2048;
	int	nn;

	if(file.Open(lpszFileName,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate)) {
		m_FileName = file.GetFileName();

		file.Seek(0,CFile::begin   );
		file.Write(Identifier,strlen(Identifier));
		file.Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

		file.Seek(nFeatureOffset=0x400,CFile::begin);
		file.Write(&m_nFeatureL,sizeof m_nFeatureL);
		for(nn=0;nn<m_nFeatureL;nn++) {
			file.Seek(nFeatureOffset+nn*nIncrement+4,CFile::begin);
			file.Write(&pFeature[nn].rr,sizeof pFeature[nn].rr);
			file.Write(&pFeature[nn].rectView,sizeof pFeature[nn].rectView);
			file.Write(&pFeature[nn].nType,sizeof pFeature[nn].nType);
			file.Write(&pFeature[nn].rgbText,sizeof pFeature[nn].rgbText);
			file.Write(&pFeature[nn].lf,sizeof pFeature[nn].lf);
			file.Write(&pFeature[nn].nLineWidth,sizeof pFeature[nn].nLineWidth);
			file.Write(&pFeature[nn].nLineStyle,sizeof pFeature[nn].nLineStyle);
			file.Write(&pFeature[nn].rgbLine,sizeof pFeature[nn].rgbLine);
			file.Write(&pFeature[nn].nImageType,sizeof pFeature[nn].nImageType);
			file.Write(&pFeature[nn].nImageNumber,sizeof pFeature[nn].nImageNumber);
			file.Write(&pFeature[nn].nBorderType,sizeof pFeature[nn].nBorderType);
			file.Write(&pFeature[nn].nBorderWidth,sizeof pFeature[nn].nBorderWidth);
			file.Write(&pFeature[nn].rgbBorderColor,sizeof pFeature[nn].rgbBorderColor);
			file.Write(&pFeature[nn].bLeftGraticule,sizeof pFeature[nn].bLeftGraticule);
			file.Write(&pFeature[nn].bRightGraticule,sizeof pFeature[nn].bRightGraticule);
			file.Write(&pFeature[nn].bTopGraticule,sizeof pFeature[nn].bTopGraticule);
			file.Write(&pFeature[nn].bBottomGraticule,sizeof pFeature[nn].bBottomGraticule);
			file.Write(&pFeature[nn].nListType,sizeof pFeature[nn].nListType);
			file.Write(&pFeature[nn].nDateFormat,sizeof pFeature[nn].nDateFormat);
			file.Write(&pFeature[nn].nWhichDate,sizeof pFeature[nn].nWhichDate);
			file.Write(&pFeature[nn].rgbBkTextColor,sizeof pFeature[nn].rgbBkTextColor);
			file.Write(&pFeature[nn].nTextBkMode,sizeof pFeature[nn].nTextBkMode);
			file.Write(&pFeature[nn].nCaptionPosition,sizeof pFeature[nn].nCaptionPosition);
			file.Write(&pFeature[nn].nDataValueType,sizeof pFeature[nn].nDataValueType);
			file.Write(&pFeature[nn].nScalePosition,sizeof pFeature[nn].nScalePosition);

			Buff.Format(_T("%s"),*pFeature[nn].pCaption);
			CArchive arWrite(&file,CArchive::store);
			arWrite.WriteString(Buff + _T("\r\n"));
			arWrite.Close();

			file.Write(&pFeature[nn].nNumberRows,sizeof pFeature[nn].nNumberRows);
			file.Write(&pFeature[nn].nNumberColumns,sizeof pFeature[nn].nNumberColumns);
			file.Write(&pFeature[nn].rgbTitle,sizeof pFeature[nn].rgbTitle);
			file.Write(&pFeature[nn].rgbBkTitle,sizeof pFeature[nn].rgbBkTitle);
			file.Write(&pFeature[nn].unFlags,sizeof pFeature[nn].unFlags);
			if(pFeature[nn].pnIDArray) {
				file.Write(pFeature[nn].pnIDArray, pFeature[nn].nNumberRows * pFeature[nn].nNumberColumns * sizeof(int));
			}
			file.Write(&pFeature[nn].n64UtVariables,sizeof pFeature[nn].n64UtVariables);
			file.Write(&pFeature[nn].nPageNumber,sizeof pFeature[nn].nPageNumber);
			file.Write(&pFeature[nn].nLanguage,sizeof pFeature[nn].nLanguage);
		}

		file.Seek(0x100,CFile::begin);
		file.Write(&nFeatureOffset,sizeof nFeatureOffset);
		file.Write(&nIncrement,sizeof nIncrement);

		file.Close();
		return TRUE;
	}
	return FALSE;

}


bool CReportPrintDlg::Retrieve(LPCTSTR lpszFileName)
{
	
	CUSLFile file;
	CString Buff;
	float fVersion;
	char	Identifier[]="Ultrasonic Sciences Generic Scanner Report Template";
	char str[100];
	int	nFeatureOffset;
	int nIncrement = 1024;
	int	nn;
	CFileException e;
	memset(str,0,sizeof str);

	if(!file.Open(lpszFileName,CFile::modeRead|CFile::typeBinary,&e)) {
		Buff.Format(_T("Failed to Open %s"),lpszFileName);
//		MessageBox(Buff,_T("Open Error"),MB_ICONERROR);

	} else {
		file.Seek(0,CFile::begin   );
		file.Read(str,strlen(Identifier));
		file.Read(&fVersion, sizeof fVersion);
		if(strcmp(str,Identifier)) {
			MessageBox(_T("Incorrect Template File type"),_T("Read Error"),MB_ICONERROR);
			file.Close();
			return FALSE;
		}

		m_FileName = file.GetFileName();

		Zero();

		file.Seek(0x100,CFile::begin);
		file.Read(&nFeatureOffset,sizeof nFeatureOffset);
		file.Read(&nIncrement,sizeof nIncrement);

		file.Seek(nFeatureOffset,CFile::begin);
		file.Read(&m_nFeatureL,sizeof m_nFeatureL);


		pFeature = (FeatureStruct *) new FeatureStruct[m_nFeatureL];

		for(nn=0;nn<m_nFeatureL;nn++) {
			ZeroMemory(&pFeature[nn], sizeof FeatureStruct);

			file.Seek(nFeatureOffset+nn*nIncrement+4,CFile::begin);
			file.Read(&pFeature[nn].rr,sizeof pFeature[nn].rr);
			file.Read(&pFeature[nn].rectView,sizeof pFeature[nn].rectView);
			file.Read(&pFeature[nn].nType,sizeof pFeature[nn].nType);
			file.Read(&pFeature[nn].rgbText,sizeof pFeature[nn].rgbText);
			file.Read(&pFeature[nn].lf,sizeof pFeature[nn].lf);
			file.Read(&pFeature[nn].nLineWidth,sizeof pFeature[nn].nLineWidth);
			file.Read(&pFeature[nn].nLineStyle,sizeof pFeature[nn].nLineStyle);
			file.Read(&pFeature[nn].rgbLine,sizeof pFeature[nn].rgbLine);
			file.Read(&pFeature[nn].nImageType,sizeof pFeature[nn].nImageType);
			file.Read(&pFeature[nn].nImageNumber,sizeof pFeature[nn].nImageNumber);
			file.Read(&pFeature[nn].nBorderType,sizeof pFeature[nn].nBorderType);
			file.Read(&pFeature[nn].nBorderWidth,sizeof pFeature[nn].nBorderWidth);
			file.Read(&pFeature[nn].rgbBorderColor,sizeof pFeature[nn].rgbBorderColor);
			file.Read(&pFeature[nn].bLeftGraticule,sizeof pFeature[nn].bLeftGraticule);
			file.Read(&pFeature[nn].bRightGraticule,sizeof pFeature[nn].bRightGraticule);
			file.Read(&pFeature[nn].bTopGraticule,sizeof pFeature[nn].bTopGraticule);
			file.Read(&pFeature[nn].bBottomGraticule,sizeof pFeature[nn].bBottomGraticule);
			file.Read(&pFeature[nn].nListType,sizeof pFeature[nn].nListType);
			file.Read(&pFeature[nn].nDateFormat,sizeof pFeature[nn].nDateFormat);
			file.Read(&pFeature[nn].nWhichDate,sizeof pFeature[nn].nWhichDate);
			file.Read(&pFeature[nn].rgbBkTextColor,sizeof pFeature[nn].rgbBkTextColor);
			file.Read(&pFeature[nn].nTextBkMode,sizeof pFeature[nn].nTextBkMode);
			file.Read(&pFeature[nn].nCaptionPosition,sizeof pFeature[nn].nCaptionPosition);
			file.Read(&pFeature[nn].nDataValueType,sizeof pFeature[nn].nDataValueType);
			file.Read(&pFeature[nn].nScalePosition,sizeof pFeature[nn].nScalePosition);

			pFeature[nn].pCaption = new CString;
			CArchive arWrite(&file,CArchive::load);
			arWrite.ReadString(Buff);
			arWrite.Close();
			pFeature[nn].pCaption->Format(_T("%s"),Buff);

			file.Read(&pFeature[nn].nNumberRows,sizeof pFeature[nn].nNumberRows);
			file.Read(&pFeature[nn].nNumberColumns,sizeof pFeature[nn].nNumberColumns);
			file.Read(&pFeature[nn].rgbTitle,sizeof pFeature[nn].rgbTitle);
			file.Read(&pFeature[nn].rgbBkTitle,sizeof pFeature[nn].rgbBkTitle);
			file.Read(&pFeature[nn].unFlags,sizeof pFeature[nn].unFlags);
			if((pFeature[nn].nNumberRows * pFeature[nn].nNumberColumns)>0) {
				pFeature[nn].AllocateIDArray();
				file.Read(pFeature[nn].pnIDArray, pFeature[nn].nNumberRows * pFeature[nn].nNumberColumns * sizeof(int));
			} else {
				pFeature[nn].nNumberRows = pFeature[nn].nNumberColumns = 0;
			}
			file.Read(&pFeature[nn].n64UtVariables,sizeof pFeature[nn].n64UtVariables);
			file.Read(&pFeature[nn].nPageNumber,sizeof pFeature[nn].nPageNumber);
			if(fVersion>=4.63f) {
				file.Read(&pFeature[nn].nLanguage,sizeof pFeature[nn].nLanguage);
			}
		}

		file.Close();
		return TRUE;
	}

	return FALSE;
}

void CReportPrintDlg::OnReportProperties() 
{


	CloseAllChildren();

	ScreenToClient(&m_ptClient);

	if(FindFeature(m_ptClient)==TRUE) {
		switch(pFeature[m_nFeaturePtr].nType) {
		case REPORT_LINE:
			m_pLinePropertiesDlg = new CLinePropertiesDlg(this,(CWnd**)&m_pLinePropertiesDlg,&pFeature[m_nFeaturePtr]);
			m_pLinePropertiesDlg->Create(CLinePropertiesDlg::IDD, this);
			break;
		case REPORT_RECTANGLE:
			m_pRectanglePropertiesDlg = new CRectanglePropertiesDlg(this,(CWnd**)&m_pRectanglePropertiesDlg,&pFeature[m_nFeaturePtr]);
			m_pRectanglePropertiesDlg->Create(CRectanglePropertiesDlg::IDD, this);
			break;
		case REPORT_IMAGE:
			m_pImagePropertiesDlg = new CImagePropertiesDlg(this,(CWnd**)&m_pImagePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pImagePropertiesDlg->Create(CImagePropertiesDlg::IDD, this);
			break;
		case REPORT_TEXT:
			m_pTextPropertiesDlg = new CTextPropertiesDlg(this,(CWnd**)&m_pTextPropertiesDlg,&pFeature[m_nFeaturePtr]);
			m_pTextPropertiesDlg->Create(CTextPropertiesDlg::IDD, this);
			break;
		case REPORT_DATE:
			m_pDatePropertiesDlg = new CDatePropertiesDlg(this,(CWnd**)&m_pDatePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pDatePropertiesDlg->Create(CDatePropertiesDlg::IDD, this);
			break;
		case REPORT_LIST:
			m_pListPropertiesDlg = new CListPropertiesDlg(this,(CWnd**)&m_pListPropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pListPropertiesDlg->Create(CListPropertiesDlg::IDD, this);
			break;
		case REPORT_BARGRAPH:
			m_pBarGraphPropertiesDlg = new CBarGraphPropertiesDlg(this,(CWnd**)&m_pBarGraphPropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pBarGraphPropertiesDlg->Create(CBarGraphPropertiesDlg::IDD, this);
			break;
		case REPORT_DATA_VALUE:
			m_pDataValuePropertiesDlg = new CDataValuePropertiesDlg(this,(CWnd**)&m_pDataValuePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pDataValuePropertiesDlg->Create(CDataValuePropertiesDlg::IDD, this);
			break;
		case REPORT_TABLE:
			m_pReportTablePropertiesDlg = new CReportTablePropertiesDlg(this,(CWnd**)&m_pReportTablePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pReportTablePropertiesDlg->Create(CReportTablePropertiesDlg::IDD, this);
			break;
		case REPORT_ULTRASONICS:
			m_pReportUltrasonicsPropertiesDlg = new CReportUltrasonicsPropertiesDlg(this,(CWnd**)&m_pReportUltrasonicsPropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
			m_pReportUltrasonicsPropertiesDlg->Create(CReportUltrasonicsPropertiesDlg::IDD, this);
			break;
		}
	}
}


void CReportPrintDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	int	ii;
	int nModifyL;
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);



	switch(m_nEditOption) {
	case ID_BUTTON_POINTER:
		CloseAllChildren();
		if(FindFeature(point)==TRUE) {
			switch(pFeature[m_nFeaturePtr].nType) {
			case REPORT_LINE:
				m_pLinePropertiesDlg = new CLinePropertiesDlg(this,(CWnd**)&m_pLinePropertiesDlg,&pFeature[m_nFeaturePtr]);
				m_pLinePropertiesDlg->Create(CLinePropertiesDlg::IDD, this);
				break;
			case REPORT_RECTANGLE:
				m_pRectanglePropertiesDlg = new CRectanglePropertiesDlg(this,(CWnd**)&m_pRectanglePropertiesDlg,&pFeature[m_nFeaturePtr]);
				m_pRectanglePropertiesDlg->Create(CRectanglePropertiesDlg::IDD, this);
				break;
			case REPORT_IMAGE:
				m_pImagePropertiesDlg = new CImagePropertiesDlg(this,(CWnd**)&m_pImagePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pImagePropertiesDlg->Create(CImagePropertiesDlg::IDD, this);
				break;
			case REPORT_TEXT:
				m_pTextPropertiesDlg = new CTextPropertiesDlg(this,(CWnd**)&m_pTextPropertiesDlg,&pFeature[m_nFeaturePtr]);
				m_pTextPropertiesDlg->Create(CTextPropertiesDlg::IDD, this);
				break;
			case REPORT_DATE:
				m_pDatePropertiesDlg = new CDatePropertiesDlg(this,(CWnd**)&m_pDatePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pDatePropertiesDlg->Create(CDatePropertiesDlg::IDD, this);
				break;
			case REPORT_LIST:
				m_pListPropertiesDlg = new CListPropertiesDlg(this,(CWnd**)&m_pListPropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pListPropertiesDlg->Create(CListPropertiesDlg::IDD, this);
				break;
			case REPORT_BARGRAPH:
				m_pBarGraphPropertiesDlg = new CBarGraphPropertiesDlg(this,(CWnd**)&m_pBarGraphPropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pBarGraphPropertiesDlg->Create(CBarGraphPropertiesDlg::IDD, this);
				break;
			case REPORT_DATA_VALUE:
				m_pDataValuePropertiesDlg = new CDataValuePropertiesDlg(this,(CWnd**)&m_pDataValuePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pDataValuePropertiesDlg->Create(CDataValuePropertiesDlg::IDD, this);
				break;
			case REPORT_TABLE:
				m_pReportTablePropertiesDlg = new CReportTablePropertiesDlg(this,(CWnd**)&m_pReportTablePropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pReportTablePropertiesDlg->Create(CReportTablePropertiesDlg::IDD, this);
				break;
			case REPORT_ULTRASONICS:
				m_pReportUltrasonicsPropertiesDlg = new CReportUltrasonicsPropertiesDlg(this,(CWnd**)&m_pReportUltrasonicsPropertiesDlg,&pFeature[m_nFeaturePtr],m_pData);
				m_pReportUltrasonicsPropertiesDlg->Create(CReportUltrasonicsPropertiesDlg::IDD, this);
				break;
			}
		}
		Invalidate(FALSE);
		if(m_pFeatureEditDlg) {
			m_pFeatureEditDlg->SetIndex(m_nFeaturePtr);
		}
		break;
	case ID_BUTTON_ERASE:
//		point.x -= rr.left;
//		point.y -= rr.top;
		if(FindFeature(point)==TRUE) {
			DeleteFeature(m_nFeaturePtr);
		}
		Invalidate(FALSE);
		break;
	case ID_BUTTON_RECTANGLE:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_RECTANGLE,point);
		DrawDefineRectangle();
		break;
	case ID_BUTTON_RECTANGLE_SELECTION:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		DrawDefineRectangle(PS_DOT);
		break;
	case ID_BUTTON_IMAGE:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_IMAGE,point);
		break;
	case ID_BUTTON_LINE:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_LINE,point);
		DrawDefineLine();
		break;
	case ID_BUTTON_TEXT:
		point.y-=10;
		point.x+=2;
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_TEXT,point);
		break;
	case ID_BUTTON_MOVE:
		nModifyL=0;
		for(ii=0;ii<m_nFeatureL;ii++) {
			pFeature[ii].rectUndoClient = pFeature[ii].rectClient;
			if(pFeature[ii].bModify==TRUE) nModifyL++;;
		}
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		if(nModifyL<2) {
			if(FindFeature(point)==TRUE) {
				Invalidate(FALSE);
			}
			if(m_pFeatureEditDlg) {
				m_pFeatureEditDlg->SetIndex(m_nFeaturePtr);
			}
		}
		break;
	case ID_BUTTON_DATE:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_DATE,point);
		break;
	case ID_BUTTON_LIST:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_LIST,point);
		break;
	case ID_BUTTON_BARGRAPH:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_BARGRAPH,point);
		break;
	case ID_BUTTON_VARIABLE:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_DATA_VALUE,point);
		break;
	case ID_BUTTON_TABLE:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_TABLE,point);
		break;
	case ID_BUTTON_ULTRASONICS:
		m_rectDefine.TopLeft() = point;
		m_rectDefine.BottomRight() = point;
		AddFeature(REPORT_ULTRASONICS,point);
		break;
	}
	m_nLBOption = m_nEditOption;

	CDialog::OnLButtonDown(nFlags, point);
}

void CReportPrintDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

	switch(m_nLBOption) {
	case ID_BUTTON_RECTANGLE_SELECTION:
		FindFeature(m_rectDefine);
		Invalidate(FALSE);
//		m_rectDefine.SetRectEmpty();
//		DrawDefineRectangle(PS_DOT);
		break;
	}

	m_nLBOption=0;

	
	CDialog::OnLButtonUp(nFlags, point);
}

void CReportPrintDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	switch(m_nLBOption) {
	case ID_BUTTON_POINTER:
		break;
	case ID_BUTTON_RECTANGLE:
		m_rectDefine.BottomRight() = point;
		ClientToPaper(&point);
		pFeature[m_nFeaturePtr].rr.BottomRight() = point;
		DrawDefineRectangle();
		break;
	case ID_BUTTON_RECTANGLE_SELECTION:
		m_rectDefine.BottomRight() = point;
		DrawDefineRectangle(PS_DOT);
		break;
	case ID_BUTTON_IMAGE:
		m_rectDefine.BottomRight() = point;
		ClientToPaper(&point);
		pFeature[m_nFeaturePtr].rr.BottomRight() = point;
		Invalidate(FALSE);
		break;
	case ID_BUTTON_LINE:
		m_rectDefine.BottomRight() = point;
		ClientToPaper(&point);
		pFeature[m_nFeaturePtr].rr.BottomRight() = point;
		DrawDefineLine();
		break;
	case ID_BUTTON_MOVE:
		point.x += rr.left;
		point.y += rr.top;
		m_rectDefine.BottomRight() = point;
		if(m_nMoveAction) {
			MoveFeatures();
			Invalidate(FALSE);
		}
		if(m_pFeatureEditDlg) {
			m_pFeatureEditDlg->SetIndex(m_nFeaturePtr);
		}
		break;
	case ID_BUTTON_BARGRAPH:
		m_rectDefine.BottomRight() = point;
		ClientToPaper(&point);
		pFeature[m_nFeaturePtr].rr.BottomRight() = point;
		Invalidate(FALSE);
		break;
	case ID_BUTTON_TABLE:
	case ID_BUTTON_ULTRASONICS:
		m_rectDefine.BottomRight() = point;
		ClientToPaper(&point);
		pFeature[m_nFeaturePtr].rr.BottomRight() = point;
		Invalidate(FALSE);
		break;
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


void CReportPrintDlg::DrawDefineRectangle(int nStyle,int nWidth)
{
	CDC* pDC = GetDC();
	static CRect rectOld;
	COLORREF rgb;
	if(m_nLBOption==ID_BUTTON_RECTANGLE) {
		rgb= 0x00ffffff;
	} else {
		rgb = 0x00ff00ff;
	}
	CPen pen(nStyle,nWidth,rgb);
	CPen* pOldPen = pDC->SelectObject(&pen);
	int nOldMode=pDC->SetROP2(R2_XORPEN);


	if((m_nLBOption==ID_BUTTON_RECTANGLE) || (m_nLBOption==ID_BUTTON_RECTANGLE_SELECTION)){
		pDC->MoveTo(rectOld.TopLeft());
		pDC->LineTo(rectOld.right,rectOld.top);
		pDC->LineTo(rectOld.BottomRight());
		pDC->LineTo(rectOld.left,rectOld.bottom);
		pDC->LineTo(rectOld.TopLeft());
	}
	rectOld = m_rectDefine;
	pDC->MoveTo(rectOld.TopLeft());
	pDC->LineTo(rectOld.right,rectOld.top);
	pDC->LineTo(rectOld.BottomRight());
	pDC->LineTo(rectOld.left,rectOld.bottom);
	pDC->LineTo(rectOld.TopLeft());


	pDC->SetROP2(nOldMode);
	pDC->SelectObject(pOldPen);

	ReleaseDC(pDC);
}

void CReportPrintDlg::DrawDefineLine()
{
	CDC* pDC = GetDC();
	static CRect rectOld;
	COLORREF rgb = pFeature[m_nFeaturePtr].rgbLine ^ 0x00ffffff;
	CPen pen(pFeature[m_nFeaturePtr].nLineStyle,pFeature[m_nFeaturePtr].nLineWidth,rgb);
	CPen* pOldPen = pDC->SelectObject(&pen);
	int nOldMode=pDC->SetROP2(R2_XORPEN);


	if(m_nLBOption==ID_BUTTON_LINE) {
		pDC->MoveTo(rectOld.TopLeft());
		pDC->LineTo(rectOld.BottomRight());
	}
	rectOld = m_rectDefine;
	pDC->MoveTo(rectOld.TopLeft());
	pDC->LineTo(rectOld.BottomRight());


	pDC->SetROP2(nOldMode);
	pDC->SelectObject(pOldPen);

	ReleaseDC(pDC);
}


bool CReportPrintDlg::FindFeature(CPoint pt)
{
	CRect rr;
	int	ii;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	pt.x-=rr.left;
	pt.y-=rr.top;

	m_nFeaturePtr = -1;
	m_nNumberToBeModified=0;

	for(ii=0;ii<m_nFeatureL;ii++) {
		rr=pFeature[ii].rectClient;
		switch(pFeature[ii].nType) {
		default:
			rr.InflateRect(2,2);
			if(rr.PtInRect(pt)) {
				m_nFeaturePtr=ii;
				m_nNumberToBeModified++;
				pFeature[ii].bModify=TRUE;
			} else {
				pFeature[ii].bModify=FALSE;
			}
			break;
		case REPORT_IMAGE:
		case REPORT_BARGRAPH:
		case REPORT_TABLE:
		case REPORT_RECTANGLE:
		case REPORT_ULTRASONICS:
			rr.InflateRect(10,10);
			if(rr.PtInRect(pt)) {
				m_nFeaturePtr=ii;
				m_nNumberToBeModified++;
				pFeature[ii].bModify=TRUE;
			} else {
				pFeature[ii].bModify=FALSE;
			}
			break;
		case REPORT_HEADER_FOOTER:
			pFeature[ii].bModify=FALSE;
			break;
		}
	}
	if(m_nNumberToBeModified==0) {
		return FALSE;
	}
	return TRUE;
}

bool CReportPrintDlg::FindFeature(CRect rr)
{
	int	ii;
	CRect rrScreen;

	m_staticView.GetWindowRect(&rrScreen);
	ScreenToClient(&rrScreen);
	rr.OffsetRect(-rrScreen.TopLeft());

	m_nFeaturePtr = -1;
	m_nNumberToBeModified=0;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((rr.PtInRect(pFeature[ii].rectClient.TopLeft())) && (rr.PtInRect(pFeature[ii].rectClient.BottomRight()))) {
			m_nFeaturePtr=ii;
			m_nNumberToBeModified++;
			pFeature[ii].bModify=TRUE;
		} else {
			pFeature[ii].bModify=FALSE;
		}
	}
	if(m_nNumberToBeModified==0) {
		return FALSE;
	}
	return TRUE;


}


void CReportPrintDlg::OnReportLeftalign() 
{
	int ii;
	int nX=100000;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			if(nX>pFeature[ii].rr.left) nX=pFeature[ii].rr.left;
		}
	}
	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			pFeature[ii].rr.left = nX;
		}
	}
	Invalidate(FALSE);
}

void CReportPrintDlg::OnReportRightalign() 
{
	int ii;
	int nX=0;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	CSize size;
	CDC* pDC = GetDC();
	CRect rr;
	CString Buff;
	CPoint pt;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			if(nX<pFeature[ii].rectClient.right) nX=pFeature[ii].rectClient.right;
		}
	}

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);

			Buff.Format(_T("%s"),*pFeature[ii].pCaption);
			size = pDC->GetTextExtent(Buff);
			pt.x = nX-size.cx;
			ClientToPaper(&pt);
			pFeature[ii].rr.left = pt.x;

			pDC->SelectObject(pOldFont);
			delete pFont;
		}

	}

	ReleaseDC(pDC);
	Invalidate(FALSE);
}

void CReportPrintDlg::OnReportCenter() 
{
	int ii;
	int nX=0;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	CSize size;
	CDC* pDC = GetDC();
	CRect rr;
	CString Buff;
	CPoint pt;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	nX = (rr.left + rr.right) / 2;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);

			Buff.Format(_T("%s"),*pFeature[ii].pCaption);
			size = pDC->GetTextExtent(Buff);
			pt.x = nX-size.cx/2;
			ClientToPaper(&pt);
			pFeature[ii].rr.left = pt.x;

			pDC->SelectObject(pOldFont);
			delete pFont;
		}
	}
	Invalidate(FALSE);
	
}

void CReportPrintDlg::MoveFeatures()
{
	int	ii;
	CPoint pt;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			switch(m_nMoveAction) {
			case MOVE_LEFT:
				pFeature[ii].rectClient.left = m_rectDefine.Width() + pFeature[ii].rectUndoClient.left;
				pt = pFeature[ii].rectClient.TopLeft();
				ClientToPaper(&pt);
				pFeature[ii].rr.left = pt.x;
				break;
			case MOVE_RIGHT:
				pFeature[ii].rectClient.right = m_rectDefine.Width() + pFeature[ii].rectUndoClient.right;
				pt = pFeature[ii].rectClient.BottomRight();
				ClientToPaper(&pt);
				pFeature[ii].rr.right = pt.x;
				break;
			case MOVE_TOP:
				pFeature[ii].rectClient.top = m_rectDefine.Height() + pFeature[ii].rectUndoClient.top;
				pt = pFeature[ii].rectClient.TopLeft();
				ClientToPaper(&pt);
				pFeature[ii].rr.top = pt.y;
				break;
			case MOVE_BOTTOM:
				pFeature[ii].rectClient.bottom = m_rectDefine.Height() + pFeature[ii].rectUndoClient.bottom;
				pt = pFeature[ii].rectClient.BottomRight();
				ClientToPaper(&pt);
				pFeature[ii].rr.bottom = pt.y;
				break;
			case MOVE_ALL:
				pFeature[ii].rectClient.left = m_rectDefine.Width() + pFeature[ii].rectUndoClient.left;
				pFeature[ii].rectClient.top = m_rectDefine.Height() + pFeature[ii].rectUndoClient.top;
				pFeature[ii].rectClient.right = m_rectDefine.Width() + pFeature[ii].rectUndoClient.right;
				pFeature[ii].rectClient.bottom = m_rectDefine.Height() + pFeature[ii].rectUndoClient.bottom;
				pt = pFeature[ii].rectClient.TopLeft();
				ClientToPaper(&pt);
				pFeature[ii].rr.TopLeft() = pt;
				pt = pFeature[ii].rectClient.BottomRight();
				ClientToPaper(&pt);
				pFeature[ii].rr.BottomRight() = pt;
				if(m_pViewPage) m_pViewPage->m_bInitialize=TRUE;
				break;
			}
		}
	}
}


void CReportPrintDlg::DrawImage(CPaintDC *pDC, CRect rr, int nDevice)
{
	
	int	ii;
	CRect rectBorder;
	double RectHWRatio, ImageHWRatio; 

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_IMAGE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);
			if(pFeature[ii].rectClient.left>pFeature[ii].rectClient.right) pFeature[ii].rectClient.left=pFeature[ii].rectClient.right;
			if(pFeature[ii].rectClient.top>pFeature[ii].rectClient.bottom) pFeature[ii].rectClient.top=pFeature[ii].rectClient.bottom;

			switch(theApp.m_LastSettings.nReportImageScale = 0){
				default:
				case 0:
					break;
				case 1: //Fit To Page
					RectHWRatio = (double)pFeature[ii].rectClient.Height()/(double)pFeature[ii].rectClient.Width();
					ImageHWRatio = m_pData->m_fSlowScanSize/m_pData->m_fFastScanSize;
					if(RectHWRatio>ImageHWRatio){
						//reduce height
						pFeature[ii].rectClient.bottom = (int)(pFeature[ii].rectClient.Width()*ImageHWRatio) + pFeature[ii].rectClient.top;
					}else{
						//reduce width
						pFeature[ii].rectClient.right = (int)(pFeature[ii].rectClient.Height()/ImageHWRatio) + pFeature[ii].rectClient.left;
					}
					break;
			}
		
			
			if(pFeature[ii].rectClient.Width()%4) {
				pFeature[ii].rectClient.right=(pFeature[ii].rectClient.right-pFeature[ii].rectClient.left)/4+pFeature[ii].rectClient.left;
				pFeature[ii].rectClient.right=(pFeature[ii].rectClient.right-pFeature[ii].rectClient.left)*4+pFeature[ii].rectClient.left;
			};

			if(m_pViewPage) m_pViewPage->PrintCallBack(pDC,&pFeature[ii].rectClient,nDevice);

			DrawBorder(pDC,&pFeature[ii],nDevice);
			DrawGraticule(pDC,&pFeature[ii],nDevice);
			DrawImageFileName(pDC,&pFeature[ii],nDevice);
			DrawAnnotation(pDC,rr,&pFeature[ii],nDevice);
			DrawImageScale(pDC,&pFeature[ii],nDevice);
//			DrawCAD(pDC,rr,&pFeature[ii],nDevice);
		}
	}

}

void CReportPrintDlg::DrawRectangle(CPaintDC *pDC, CRect rr, int nDevice)
{
	int	ii;
	CPen* pOldPen;
	CPen* pPen;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_RECTANGLE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);
			if(pFeature[ii].bModify==TRUE) {
				pPen = new CPen(PS_SOLID,10,RGB(238,240,155));
				pOldPen = pDC->SelectObject(pPen);
				pDC->MoveTo(pFeature[ii].rectClient.TopLeft());
				pDC->LineTo(pFeature[ii].rectClient.right,pFeature[ii].rectClient.top);
				pDC->LineTo(pFeature[ii].rectClient.BottomRight());
				pDC->LineTo(pFeature[ii].rectClient.left,pFeature[ii].rectClient.bottom);
				pDC->LineTo(pFeature[ii].rectClient.TopLeft());
				pDC->SelectObject(pOldPen);
				delete pPen;
			}
			pPen = new CPen(pFeature[ii].nLineStyle,MulDiv(pFeature[ii].nLineWidth,rr.Width(),pFeature[ii].rectView.Width())/4,pFeature[ii].rgbLine);
			pOldPen = pDC->SelectObject(pPen);
			switch(pFeature[ii].nBorderType) {
			case 0:
				pDC->MoveTo(pFeature[ii].rectClient.TopLeft());
				pDC->LineTo(pFeature[ii].rectClient.right,pFeature[ii].rectClient.top);
				pDC->LineTo(pFeature[ii].rectClient.BottomRight());
				pDC->LineTo(pFeature[ii].rectClient.left,pFeature[ii].rectClient.bottom);
				pDC->LineTo(pFeature[ii].rectClient.TopLeft());
				break;
			case 1:
			case 2:
				DrawBorder(pDC,&pFeature[ii],nDevice);
				break;
			case 3: pDC->FillSolidRect(pFeature[ii].rectClient,pFeature[ii].rgbLine);
				break;
			}
			pDC->SelectObject(pOldPen);
			delete pPen;

		}
	}

}

void CReportPrintDlg::DrawLine(CPaintDC *pDC, CRect rr, int nDevice)
{
	int	ii;
	CPen* pOldPen;
	CPen* pPen;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_LINE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);
			if(pFeature[ii].bModify==TRUE) {
				pPen = new CPen(PS_SOLID,8,RGB(238,240,155));
				pOldPen = pDC->SelectObject(pPen);
				pDC->MoveTo(pFeature[ii].rectClient.TopLeft());
				pDC->LineTo(pFeature[ii].rectClient.BottomRight());
				pDC->SelectObject(pOldPen);
				delete pPen;
			}
			pPen = new CPen(pFeature[ii].nLineStyle,MulDiv(pFeature[ii].nLineWidth,rr.Width(),pFeature[ii].rectView.Width())/4,pFeature[ii].rgbLine);
			pOldPen = pDC->SelectObject(pPen);
			pDC->MoveTo(pFeature[ii].rectClient.TopLeft());
			pDC->LineTo(pFeature[ii].rectClient.BottomRight());
			pDC->SelectObject(pOldPen);
			delete pPen;

		}
	}
}

void CReportPrintDlg::DrawTable(CPaintDC *pDC, CRect rr, int nDevice)
{
	
	int	ii;
	CPen* pOldPen;
	CPen* pPen;
	int nPnX,nPnY,nRow,nColumn,nItem,nNumberRows;
	CRect rrTitle;
	CString Buff;
	CSize size;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_TABLE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);

			lf = pFeature[ii].lf;
			nNumberRows = pFeature[ii].nNumberRows;
			if(theApp.m_LastSettings.nBothLanguagesOneColumn == 1) nNumberRows*=2;
			if(pFeature[ii].unFlags & FEATURE_TITLEBAR) {
				lf.lfHeight = MulDiv(1,pFeature[ii].rectClient.Height(),nNumberRows + 1);
			} else {
				lf.lfHeight = MulDiv(1,pFeature[ii].rectClient.Height(),nNumberRows);
			}
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);


			if(pFeature[ii].bModify==TRUE) {
				pPen = new CPen(PS_SOLID,10,RGB(238,240,155));
				pOldPen = pDC->SelectObject(pPen);
				pDC->MoveTo(pFeature[ii].rectClient.TopLeft());
				pDC->LineTo(pFeature[ii].rectClient.right,pFeature[ii].rectClient.top);
				pDC->LineTo(pFeature[ii].rectClient.BottomRight());
				pDC->LineTo(pFeature[ii].rectClient.left,pFeature[ii].rectClient.bottom);
				pDC->LineTo(pFeature[ii].rectClient.TopLeft());
				pDC->SelectObject(pOldPen);
				delete pPen;
			}
			pPen = new CPen(pFeature[ii].nLineStyle,MulDiv(pFeature[ii].nLineWidth,rr.Width(),pFeature[ii].rectView.Width())/4,pFeature[ii].rgbLine);
			pOldPen = pDC->SelectObject(pPen);
			switch(pFeature[ii].nBorderType) {
			case 0:
				DrawModernTable(pDC,&pFeature[ii],nDevice);
				if(pFeature[ii].pnIDArray) {
					for(nRow=0;nRow<pFeature[ii].nNumberRows;nRow++) {

						if(pFeature[ii].unFlags & FEATURE_TITLEBAR) {
							nPnY = MulDiv(nRow+1,pFeature[ii].rectClient.Height(),pFeature[ii].nNumberRows + 1) + pFeature[ii].rectClient.top;
						} else {
							nPnY = MulDiv(nRow,pFeature[ii].rectClient.Height(),pFeature[ii].nNumberRows) + pFeature[ii].rectClient.top;
						}

						for(nColumn=0;nColumn<pFeature[ii].nNumberColumns;nColumn++) {
							
							switch(theApp.m_LastSettings.nColumnJustification) {
							case 0:	nPnX = MulDiv(nColumn, pFeature[ii].rectClient.Width(),pFeature[ii].nNumberColumns) + pFeature[ii].rectClient.left + 2;
								break;
							case 1:	nPnX = MulDiv(nColumn+1, pFeature[ii].rectClient.Width(),pFeature[ii].nNumberColumns) + pFeature[ii].rectClient.left - 2;
								break;
							case 2: nPnX = MulDiv(nColumn, pFeature[ii].rectClient.Width(),pFeature[ii].nNumberColumns) + pFeature[ii].rectClient.left + 2;
								nPnX += (MulDiv(1, pFeature[ii].rectClient.Width(),pFeature[ii].nNumberColumns) / 2);
								break;
							}

							if(theApp.m_LastSettings.nBothLanguagesOneColumn == 0) {
								nItem = nRow * pFeature[ii].nNumberColumns + nColumn;
								Buff = m_pData->m_DataProperties.GetReportFromID(pFeature[ii].pnIDArray[nItem]-1);
								size = pDC->GetTextExtent(Buff);
								switch(theApp.m_LastSettings.nColumnJustification) {
								case 0: pDC->TextOut(nPnX,nPnY,Buff);
									break;
								case 1: pDC->TextOut(nPnX-size.cx,nPnY,Buff);
									break;
								case 2: pDC->TextOut(nPnX-size.cx/2,nPnY,Buff);
									break;
								}
							} else {
								switch(nColumn) {
								case 0:
									nItem = nRow * pFeature[ii].nNumberColumns + nColumn;
									Buff = m_pData->m_DataProperties.GetReportFromID(pFeature[ii].pnIDArray[nItem]-1);
									size = pDC->GetTextExtent(Buff);
									switch(theApp.m_LastSettings.nColumnJustification) {
									case 0: pDC->TextOut(nPnX,nPnY,Buff);
										break;
									case 1: pDC->TextOut(nPnX-size.cx,nPnY,Buff);
										break;
									case 2: pDC->TextOut(nPnX-size.cx/2,nPnY,Buff);
										break;
									}
									size = pDC->GetTextExtent(Buff);
									Buff = m_pData->m_DataProperties.GetReportFromID(pFeature[ii].pnIDArray[nItem]);
									size = pDC->GetTextExtent(Buff);
									switch(theApp.m_LastSettings.nColumnJustification) {
									case 0: pDC->TextOut(nPnX,nPnY+size.cy-2,Buff);
										break;
									case 1: pDC->TextOut(nPnX-size.cx,nPnY+size.cy-2,Buff);
										break;
									case 2: pDC->TextOut(nPnX-size.cx/2,nPnY+size.cy-2,Buff);
										break;
									}
									break;
								case 1:
									nItem = nRow * pFeature[ii].nNumberColumns + nColumn;
									Buff = m_pData->m_DataProperties.GetReportFromID(pFeature[ii].pnIDArray[nItem]-1);
									size = pDC->GetTextExtent(Buff);
									switch(theApp.m_LastSettings.nColumnJustification) {
									case 0: pDC->TextOut(nPnX,nPnY+size.cy/2,Buff);
										break;
									case 1: pDC->TextOut(nPnX-size.cx,nPnY+size.cy/2,Buff);
										break;
									case 2: pDC->TextOut(nPnX-size.cx/2,nPnY+size.cy/2,Buff);
										break;
									}
									break;
								}
							}

						}
					}
				}

				break;
			case 1:
			case 2:
				DrawBorder(pDC,&pFeature[ii],nDevice);
				break;
			}
			pDC->SelectObject(pOldFont);
			pDC->SelectObject(pOldPen);
			delete pPen;
			delete pFont;
		}
	}

}


void CReportPrintDlg::DrawUltrasonics(CPaintDC *pDC, CRect rr, int nDevice)
{
	
	int	ii;
	CPen* pOldPen;
	CPen* pPen;
	int nPnX,nPnY,nRow,nColumn;
	CRect rrTitle;
	CString Buff,Value;
	CSize size;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	int nBit;
	__int64 n64Mask;
	int nTS = 0;
	int nImageType = 0;
	int nImageNumber = 0;

	if(m_pViewPage) {
		nTS = m_pViewPage->m_nTS;
		nImageType = m_pViewPage->m_nImageType;
		nImageNumber = m_pViewPage->m_nImageNumber;
	}

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_ULTRASONICS) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);

			lf = pFeature[ii].lf;
			if(pFeature[ii].unFlags & FEATURE_TITLEBAR) {
				lf.lfHeight = MulDiv(1,pFeature[ii].rectClient.Height(),pFeature[ii].nNumberRows + 1);
			} else {
				lf.lfHeight = MulDiv(1,pFeature[ii].rectClient.Height(),pFeature[ii].nNumberRows);
			}
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);

			pFeature[ii].nNumberRows=0;
			for(nBit=0,n64Mask=1;nBit<64;nBit++,n64Mask<<=1) {
				if(pFeature[ii].n64UtVariables & n64Mask) pFeature[ii].nNumberRows++;
			};
			if(pFeature[ii].nNumberRows == 0) pFeature[ii].nNumberRows = 1;


			if(pFeature[ii].bModify==TRUE) {
				pPen = new CPen(PS_SOLID,10,RGB(238,240,155));
				pOldPen = pDC->SelectObject(pPen);
				pDC->MoveTo(pFeature[ii].rectClient.TopLeft());
				pDC->LineTo(pFeature[ii].rectClient.right,pFeature[ii].rectClient.top);
				pDC->LineTo(pFeature[ii].rectClient.BottomRight());
				pDC->LineTo(pFeature[ii].rectClient.left,pFeature[ii].rectClient.bottom);
				pDC->LineTo(pFeature[ii].rectClient.TopLeft());
				pDC->SelectObject(pOldPen);
				delete pPen;
			}
			pPen = new CPen(pFeature[ii].nLineStyle,MulDiv(pFeature[ii].nLineWidth,rr.Width(),pFeature[ii].rectView.Width())/4,pFeature[ii].rgbLine);
			pOldPen = pDC->SelectObject(pPen);
			switch(pFeature[ii].nBorderType) {
			case 0:
				DrawModernTable(pDC,&pFeature[ii],nDevice);

				for(nBit=0,n64Mask=1,nRow=0;nBit<20;nBit++,n64Mask<<=1,nRow++) {
					if(pFeature[ii].n64UtVariables & n64Mask) {
						if(pFeature[ii].unFlags & FEATURE_TITLEBAR) {
							nPnY = MulDiv(nRow+1,pFeature[ii].rectClient.Height(),pFeature[ii].nNumberRows + 1) + pFeature[ii].rectClient.top;
						} else {
							nPnY = MulDiv(nRow,pFeature[ii].rectClient.Height(),pFeature[ii].nNumberRows) + pFeature[ii].rectClient.top;
						}

						Buff.Empty();
						Value.Empty();
						nColumn = 0;
						nPnX = MulDiv(nColumn, pFeature[ii].rectClient.Width(),pFeature[ii].nNumberColumns) + pFeature[ii].rectClient.left + 2;
						switch(pFeature[ii].n64UtVariables & n64Mask) {
						case FEATURE_UT_PREAMP_GAIN:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Preamp Gain";
							} else {
								Buff.LoadString(IDS_Preamp_Gain);
							}
							theApp.m_UtUser.Pr30GetPreampGain(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_TX_RX:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Tx Rx";
							} else {
								Buff.LoadString(IDS_Tx_Rx);
							}
							theApp.m_UtUser.Pr30GetTxRx(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_DAMPING:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Damping";
							} else {
								Buff.LoadString(IDS_Damping);
							}
							theApp.m_UtUser.Pr30GetDamping(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_PULSE_WIDTH:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Pulse Width";
							} else {
								Buff.LoadString(IDS_Pulse_Width);
							}
							theApp.m_UtUser.Pr30GetPulseWidth(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_PULSE_VOLTAGE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Pulse Voltage";
							} else {
								Buff.LoadString(IDS_Pulse_Voltage);
							}
							theApp.m_UtUser.Pr30GetPulseVoltage(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_GAIN:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Main Gain";
							} else {
								Buff.LoadString(IDS_Main_Gain);
							}
							theApp.m_UtUser.Pr30GetOutputModeAndGain(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_OUTPUT_MODE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Output Mode";
							} else {
								Buff.LoadString(IDS_Output_Mode);
							}
							theApp.m_UtUser.Pr30GetOutputMode(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_FILTER_TYPE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Filter Type";
							} else {
								Buff.LoadString(IDS_Filter_Type);
							}
							theApp.m_UtUser.Pr30GetFilterType(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_FILTER_RANGE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Filter Range";
							} else {
								Buff.LoadString(IDS_Filter_Range);
							}
							theApp.m_UtUser.Pr30GetFilterRange(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_FILTER_CENTER:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Filter Center";
							} else {
								Buff.LoadString(IDS_Filter_Center);
							}
							theApp.m_UtUser.Pr30CalculateRealWorldCentreFrequency(&m_pData->m_TS[nTS].Pr30,&Value);
							break;
						case FEATURE_UT_FILTER_CUT_OFF:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Filter Cut Off";
							} else {
								Buff.LoadString(IDS_Filter_Cut_Off);
							}
							theApp.m_UtUser.Pr30GetLinearTimeConstant(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_DAC_MODE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "DAC Mode";
							} else {
								Buff.LoadString(IDS_DAC_Mode);
							}
							theApp.m_UtUser.Pr30GetDACMode(&m_pData->m_TS[nTS].Pr30,&Value,false);
							break;
						case FEATURE_UT_GATE_MODE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Gate Mode";
							} else {
								Buff.LoadString(IDS_Gate_Mode);
							}
							theApp.m_UtUser.GateGetTriggerMode(&m_pData->m_TS[nTS].Gate, nImageNumber,&Value,false);
							break;
						case FEATURE_UT_GATE_DELAY:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Delay";
							} else {
								Buff.LoadString(IDS_Delay);
							}
							theApp.m_UtUser.GateGetDelay(&m_pData->m_TS[nTS].Gate, nImageNumber,&Value,false);
							break;
						case FEATURE_UT_GATE_WIDTH:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Width";
							} else {
								Buff.LoadString(IDS_Width);
							}
							theApp.m_UtUser.GateGetWidth(&m_pData->m_TS[nTS].Gate, nImageNumber,&Value,false);
							break;
						case FEATURE_UT_GATE_THRESHOLD:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Threshold";
							} else {
								Buff.LoadString(IDS_Threshold);
							}
							theApp.m_UtUser.GateGetThreshold(&m_pData->m_TS[nTS].Gate, nImageNumber,&Value,false);
							break;
						case FEATURE_UT_GATE_FAR_DEAD_ZONE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "Dead Zone";
							} else {
								Buff.LoadString(IDS_Dead_Zone);
							}
							break;
						case FEATURE_UT_TOF_MODE:
							if(pFeature[ii].nLanguage == 0) {
								Buff = "TOF Mode";
							} else {
								Buff.LoadString(IDS_TOF_Mode);
							}
							break;
						}
						pDC->TextOut(nPnX,nPnY,Buff);


						nColumn=1;
						nPnX = MulDiv(nColumn, pFeature[ii].rectClient.Width(),pFeature[ii].nNumberColumns) + pFeature[ii].rectClient.left + 2;
						pDC->TextOut(nPnX,nPnY,Value);
					}
				}

				break;
			case 1:
			case 2:
				DrawBorder(pDC,&pFeature[ii],nDevice);
				break;
			}
			pDC->SelectObject(pOldFont);
			pDC->SelectObject(pOldPen);
			delete pPen;
			delete pFont;
		}
	}

}



BOOL CReportPrintDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	
	int	ii;
	CPoint	point;
	CRect rr;
	GetCursorPos(&point);
	m_staticView.GetWindowRect(&rr);


	switch(m_nEditOption) {
	case ID_BUTTON_RECTANGLE:
	case ID_BUTTON_LINE:
	case ID_BUTTON_IMAGE:
	case ID_BUTTON_DATE:
	case ID_BUTTON_LIST:
	case ID_BUTTON_BARGRAPH:
		if(rr.PtInRect(point) == TRUE) {
			SetCursor(m_hcursorCross);
			return TRUE;
		}
		break;
	case ID_BUTTON_TEXT:
		if(rr.PtInRect(point) == TRUE) {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));

			return TRUE;
		}
		break;
	case ID_BUTTON_MOVE:
		if(m_nLBOption==0) {
			m_nMoveAction = 0;
			ScreenToClient(&point);
			ScreenToClient(&rr);
			point.x-=rr.left;
			point.y-=rr.top;
			for(ii=0;ii<m_nFeatureL;ii++) {
				switch(pFeature[ii].nType) {
				case REPORT_IMAGE:
				case REPORT_RECTANGLE:
				case REPORT_BARGRAPH:
				case REPORT_TABLE:
				case REPORT_ULTRASONICS:
					rr=pFeature[ii].rectClient;
					rr.InflateRect(10,10);
					rr.right=rr.left+10;
					if(rr.PtInRect(point)) m_nMoveAction|=MOVE_LEFT;
					rr=pFeature[ii].rectClient;
					rr.InflateRect(10,10);
					rr.left=rr.right-10;
					if(rr.PtInRect(point)) m_nMoveAction|=MOVE_RIGHT;
					rr=pFeature[ii].rectClient;
					rr.InflateRect(10,10);
					rr.bottom=rr.top+10;
					if(rr.PtInRect(point)) m_nMoveAction|=MOVE_TOP;
					rr=pFeature[ii].rectClient;
					rr.InflateRect(10,10);
					rr.top=rr.bottom-10;
					if(rr.PtInRect(point)) m_nMoveAction|=MOVE_BOTTOM;
					if(pFeature[ii].rectClient.PtInRect(point)) m_nMoveAction=MOVE_ALL;
					break;
				default:
					if(pFeature[ii].rectClient.PtInRect(point)) {
						m_nMoveAction|=MOVE_ALL;
					}
					break;
				}
			}
		}


		switch(m_nMoveAction) {
		case MOVE_LEFT:
		case MOVE_RIGHT:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
			break;
		case MOVE_TOP:
		case MOVE_BOTTOM:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			return TRUE;
			break;
		case MOVE_ALL:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CReportPrintDlg::OnTemplatePrint() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff;
	CSize size;
	CPoint pt;
	CPaintDC* pDC = (CPaintDC*)&m_dcPrinter;

	DOCINFO docinfo;
	memset(&docinfo,0,sizeof docinfo);
	docinfo.cbSize = sizeof(docinfo);
	docinfo.lpszDocName = _T("Generic scanner image");


	if(m_dcPrinter.StartDoc(&docinfo) < 0) {
		MessageBox(_T("Printer wouldn't initialize"));
		return;
	}


	if(m_dcPrinter.StartPage() < 0) {
		MessageBox(_T("Could not start page"));
		m_dcPrinter.AbortDoc();
		return;
	}

	CWaitCursor Wait;

	int nOldPageNumber = m_nPageNumber;

	for(m_nPageNumber=0;m_nPageNumber<=m_nMaxPageNumber;m_nPageNumber++) {

		DrawHeaderFooter((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawRectangle((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawText((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawImage((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawBladePlan((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawLine((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawDate((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawList((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawBarGraph((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawDataValues((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawTable((CPaintDC*)pDC,m_rectPrintArea,PRINTER);
		DrawUltrasonics((CPaintDC*)pDC,m_rectPrintArea,PRINTER);

		m_dcPrinter.EndPage();
	}

	m_dcPrinter.EndDoc();

	Wait.Restore();
	
	m_nPageNumber = nOldPageNumber;
}



void CReportPrintDlg::AddFeature(int nType, CPoint pt,int nVerbose)
{
	
	CRect rr;
	int nBit;
	__int64 n64Mask;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	int	nn=m_nFeatureL++;
	int	ii;
	FeatureStruct* pFeatureTemp;

	if(nn==0) {
		pFeature = (FeatureStruct *) new FeatureStruct[m_nFeatureL];
	} else {
		pFeatureTemp = (FeatureStruct *) new FeatureStruct[nn];
		for(ii=0;ii<nn;ii++) {
			pFeatureTemp[ii] = pFeature[ii];
		}
		delete pFeature;
		pFeature = (FeatureStruct *) new FeatureStruct[m_nFeatureL];
		for(ii=0;ii<nn;ii++) {
			pFeature[ii] = pFeatureTemp[ii];
		}
		delete pFeatureTemp;
	}

	ZeroMemory(&pFeature[nn],sizeof pFeature[nn]);
	pFeature[nn].nStructSize = sizeof FeatureStruct;
	pFeature[nn].rectView = rr;
	pFeature[nn].bModify=FALSE;
	pFeature[nn].nType = nType;

	pFeature[nn].pCaption = new CString("Static");
	pFeature[nn].lf = theApp.m_LastSettings.lfReport;

	pFeature[nn].rr.TopLeft() = pt;
	pFeature[nn].rr.BottomRight() = pt;
	ClientToPaper(&pFeature[nn].rr);

	pFeature[nn].nLineWidth = 1;
	pFeature[nn].rgbLine = GetSysColor(COLOR_BTNFACE);
	pFeature[nn].nLineStyle = PS_SOLID;

	pFeature[nn].nImageType = 0;
	pFeature[nn].nImageNumber = 0;

	pFeature[nn].nBorderType = 0;
	pFeature[nn].nBorderWidth = 1;
	pFeature[nn].rgbBorderColor = GetSysColor(COLOR_BTNFACE);

	pFeature[nn].nDateFormat = 0;
	pFeature[nn].nWhichDate = 0;

	pFeature[nn].nPageNumber = m_nPageNumber;
	pFeature[nn].nLanguage = 0;


	CloseAllChildren();

	if(nVerbose==TRUE) {
		switch(pFeature[nn].nType) {
		case REPORT_LINE:
			m_pLinePropertiesDlg = new CLinePropertiesDlg(this,(CWnd**)&m_pLinePropertiesDlg,&pFeature[nn]);
			m_pLinePropertiesDlg->Create(CLinePropertiesDlg::IDD, this);
			break;
		case REPORT_RECTANGLE:
			m_pRectanglePropertiesDlg = new CRectanglePropertiesDlg(this,(CWnd**)&m_pRectanglePropertiesDlg,&pFeature[nn]);
			m_pRectanglePropertiesDlg->Create(CRectanglePropertiesDlg::IDD, this);
			break;
		case REPORT_IMAGE:
			m_pImagePropertiesDlg = new CImagePropertiesDlg(this,(CWnd**)&m_pImagePropertiesDlg,&pFeature[nn],m_pData);
			m_pImagePropertiesDlg->Create(CImagePropertiesDlg::IDD, this);
			break;
		case REPORT_TEXT:
			m_pTextPropertiesDlg = new CTextPropertiesDlg(this,(CWnd**)&m_pTextPropertiesDlg,&pFeature[nn]);
			m_pTextPropertiesDlg->Create(CTextPropertiesDlg::IDD, this);
			break;
		case REPORT_DATE:
			m_pDatePropertiesDlg = new CDatePropertiesDlg(this,(CWnd**)&m_pDatePropertiesDlg,&pFeature[nn],m_pData);
			m_pDatePropertiesDlg->Create(CDatePropertiesDlg::IDD, this);
			break;
		case REPORT_LIST:
			m_pListPropertiesDlg = new CListPropertiesDlg(this,(CWnd**)&m_pListPropertiesDlg,&pFeature[nn],m_pData);
			m_pListPropertiesDlg->Create(CListPropertiesDlg::IDD, this);
			pFeature[nn].pCaption->Empty();
			break;
		case REPORT_BARGRAPH:
			m_pBarGraphPropertiesDlg = new CBarGraphPropertiesDlg(this,(CWnd**)&m_pBarGraphPropertiesDlg,&pFeature[nn],m_pData);
			m_pBarGraphPropertiesDlg->Create(CBarGraphPropertiesDlg::IDD, this);
			pFeature[nn].pCaption->Empty();
			break;
		case REPORT_DATA_VALUE:
			m_pDataValuePropertiesDlg = new CDataValuePropertiesDlg(this,(CWnd**)&m_pDataValuePropertiesDlg,&pFeature[nn],m_pData);
			m_pDataValuePropertiesDlg->Create(CDataValuePropertiesDlg::IDD, this);
			pFeature[nn].pCaption->Empty();
			break;
		case REPORT_TABLE:
			pFeature[nn].nNumberRows = 2;
			pFeature[nn].nNumberColumns = 2;
			pFeature[nn].unFlags |= FEATURE_TITLEBAR;
			pFeature[nn].rgbLine = RGB(0,0,0);
			pFeature[nn].rgbTitle = GetSysColor(COLOR_WINDOWTEXT);
			pFeature[nn].rgbBkTitle = GetSysColor(COLOR_ACTIVECAPTION);
			pFeature[nn].lf.lfUnderline = 0;
			pFeature[nn].AllocateIDArray();
			m_pReportTablePropertiesDlg = new CReportTablePropertiesDlg(this,(CWnd**)&m_pReportTablePropertiesDlg,&pFeature[nn],m_pData);
			m_pReportTablePropertiesDlg->Create(CReportTablePropertiesDlg::IDD, this);
			break;
		case REPORT_ULTRASONICS:
			pFeature[nn].n64UtVariables = theApp.m_LastSettings.n64FeatureUtVariables;
			pFeature[nn].nNumberRows=0;
			for(nBit=0,n64Mask=1;nBit<64;nBit++,n64Mask<<=1) {
				if(pFeature[nn].n64UtVariables & n64Mask) pFeature[nn].nNumberRows++;
			};
			if(pFeature[nn].nNumberRows == 0) pFeature[nn].nNumberRows = 1;
			pFeature[nn].nNumberColumns = 2;
			pFeature[nn].unFlags |= FEATURE_TITLEBAR;
			pFeature[nn].rgbLine = RGB(0,0,0);
			pFeature[nn].rgbTitle = GetSysColor(COLOR_WINDOWTEXT);
			pFeature[nn].rgbBkTitle = GetSysColor(COLOR_ACTIVECAPTION);
			pFeature[nn].lf.lfUnderline = 0;
			m_pReportUltrasonicsPropertiesDlg = new CReportUltrasonicsPropertiesDlg(this,(CWnd**)&m_pReportUltrasonicsPropertiesDlg,&pFeature[nn],m_pData);
			m_pReportUltrasonicsPropertiesDlg->Create(CReportUltrasonicsPropertiesDlg::IDD, this);
			break;
		case REPORT_HEADER_FOOTER:
			pFeature[nn].rrHeader = CRect(0,0,10000,500);
			pFeature[nn].rrFooter = CRect(0,9500,10000,10000);
			m_pReportHeaderFooterDlg = new CReportHeaderFooterDlg(this,(CWnd**)&m_pReportHeaderFooterDlg,&pFeature[nn]);
			m_pReportHeaderFooterDlg->Create(CReportHeaderFooterDlg::IDD, this);
			break;
		}
	}

	m_nFeaturePtr = nn;

	if(m_pFeatureEditDlg) {
		m_pFeatureEditDlg->m_pFeature = pFeature;
		m_pFeatureEditDlg->FillList();
	}

}


void CReportPrintDlg::DeleteFeature(int nPtr)
{
	int	ii;

	if(m_nFeatureL==1) {
		SAFE_DELETE( pFeature[0].pCaption );
		SAFE_DELETE( pFeature[0].pnIDArray );
		SAFE_DELETE_ARRAY( pFeature );
		m_nFeatureL = 0;
		return;
	}

	for(ii=nPtr;ii<(m_nFeatureL-1);ii++) {
		pFeature[ii] = &pFeature[ii+1];
	};
	if(pFeature) {
		SAFE_DELETE( pFeature[ii].pCaption );
		SAFE_DELETE( pFeature[ii].pnIDArray );
	}
	m_nFeatureL--;

	if(m_pFeatureEditDlg) {
		m_pFeatureEditDlg->m_pFeature = pFeature;
		m_pFeatureEditDlg->FillList();
	}

}

void CReportPrintDlg::DrawBorder(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	int nn,nEdge,nWidth;;
	CRect rectBorder=pFeature->rectClient;

	switch(nDevice) {
	case SCREEN:
		nWidth = pFeature->nBorderWidth;
		nEdge = 2;
		break;
	case PRINTER:
		nWidth = MulDiv(pFeature->nBorderWidth,m_rectPrintArea.Width(),pFeature->rectView.Width());
		nEdge = MulDiv(2,m_rectPrintArea.Width(),pFeature->rectView.Width());
		break;
	}

	switch(pFeature->nBorderType) {
	case 0:
		break;
	case 1:	//Raised
		for(nn=0;nn<nEdge;nn++) {
			pDC->Draw3dRect(rectBorder,RGB(0,0,0),RGB(255,255,255));
			rectBorder.InflateRect(1,1);
		}
		for(nn=0;nn<nWidth;nn++) {
			pDC->Draw3dRect(rectBorder,pFeature->rgbBorderColor,pFeature->rgbBorderColor);
			rectBorder.InflateRect(1,1);
		}
		for(nn=0;nn<1;nn++) {
			pDC->Draw3dRect(rectBorder,RGB(255,255,255),RGB(0,0,0));
			rectBorder.InflateRect(1,1);
		}
		for(nn=0;nn<1;nn++) {
			pDC->Draw3dRect(rectBorder,RGB(0,0,0),RGB(0,0,0));
			rectBorder.InflateRect(1,1);
		}
		break;
	case 2:	//Sunken
		for(nn=0;nn<nEdge;nn++) {
			pDC->Draw3dRect(rectBorder,RGB(255,255,255),RGB(0,0,0));
			rectBorder.InflateRect(1,1);
		}
		for(nn=0;nn<nWidth;nn++) {
			pDC->Draw3dRect(rectBorder,pFeature->rgbBorderColor,pFeature->rgbBorderColor);
			rectBorder.InflateRect(1,1);
		}
		for(nn=0;nn<1;nn++) {
			pDC->Draw3dRect(rectBorder,RGB(0,0,0),RGB(255,255,255));
			rectBorder.InflateRect(1,1);
		}
		for(nn=0;nn<1;nn++) {
			pDC->Draw3dRect(rectBorder,RGB(0,0,0),RGB(0,0,0));
			rectBorder.InflateRect(1,1);
		}
		break;
	}


}


void CReportPrintDlg::DrawGraticule(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	
	CString	Buff;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;
	CRect rr;
	float fH,fV;
	float fHSize,fVSize;
	float fHStep,fVStep;
	int nX,nY;
	int nLength = 3;
	int	nTextStep;

	if(m_pViewPage) m_pViewPage->GetGraticuleSize(&fHSize,&fVSize,&fHStep,&nTextStep,theApp.m_fUnits);

	m_staticView.GetWindowRect(&rr);
	rr.OffsetRect(-rr.left,-rr.top);

	if(nDevice == PRINTER) {
		nLength = MulDiv(nLength,m_rectPrintArea.Width(),pFeature->rectView.Width());
		rr = m_rectPrintArea;
	}


	if(fHSize<=10000) fHStep = 500.0;
	if(fHSize<=1000) fHStep = 100.0;
	if(fHSize<=500) fHStep = 100.0;
	if(fHSize<=200) fHStep = 50.0;
	if(fHSize<=100) fHStep = 10.0;
	int nHStep = (int)fHStep;

	if(fVSize<=10000) fVStep = 500.0;
	if(fVSize<=1000) fVStep = 100.0;
	if(fVSize<=500) fVStep = 100.0;
	if(fVSize<=200) fVStep = 50.0;
	if(fVSize<=100) fVStep = 10.0;
	int nVStep = (int)fVStep;

	if(pFeature->bBottomGraticule || pFeature->bTopGraticule){
		lf = pFeature->lf;
		lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature->rectView.Width());
		pFont = (CFont *) new CFont;
		pFont->CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(pFeature->rgbText);
		pDC->SetBkMode(TRANSPARENT);


		for(fH=0;fH<fHSize;fH+=fHStep) {
			nX=MulDiv((int)fH,pFeature->rectClient.Width(),(int)fHSize)+pFeature->rectClient.left;
			if(pFeature->bBottomGraticule) {
				pDC->MoveTo(nX,pFeature->rectClient.bottom);
				pDC->LineTo(nX,pFeature->rectClient.bottom+nLength);
				if(((int)fH%nHStep) == 0) {
					Buff.Format(_T("%.01f"),fH);
					size=pDC->GetTextExtent(Buff);
					pDC->TextOut(nX-size.cx/2,pFeature->rectClient.bottom+nLength,Buff);
				};
			};
			if(pFeature->bTopGraticule) {
				pDC->MoveTo(nX,pFeature->rectClient.top);
				pDC->LineTo(nX,pFeature->rectClient.top-nLength);
				if(((int)fH%nHStep) == 0) {
					Buff.Format(_T("%.01f"),fH);
					size=pDC->GetTextExtent(Buff);
					pDC->TextOut(nX-size.cx/2,pFeature->rectClient.top-nLength-size.cy,Buff);
				};
			};
		}

		pDC->SelectObject(pOldFont);
		delete pFont;
	}

	if(pFeature->bLeftGraticule || pFeature->bRightGraticule){
		lf = pFeature->lf;
		lf.lfEscapement  = 900;
		lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature->rectView.Width());
		pFont = (CFont *) new CFont;
		pFont->CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(pFeature->rgbText);
		pDC->SetBkMode(TRANSPARENT);


		for(fV=0;fV<fVSize;fV+=fVStep) {
			nY=MulDiv((int)fV,pFeature->rectClient.Height(),(int)fVSize)+pFeature->rectClient.top;
			if(pFeature->bLeftGraticule) {
				pDC->MoveTo(pFeature->rectClient.left,nY);
				pDC->LineTo(pFeature->rectClient.left-nLength,nY);
				if(((int)fV%nVStep) == 0) {
					Buff.Format(_T("%.01f"),fV);
					size=pDC->GetTextExtent(Buff);
					pDC->TextOut(pFeature->rectClient.left-nLength-size.cy,nY+size.cx/2,Buff);
				};
			};
			if(pFeature->bRightGraticule) {
				pDC->MoveTo(pFeature->rectClient.right,nY);
				pDC->LineTo(pFeature->rectClient.right+nLength,nY);
				if(((int)fV%nVStep) == 0) {
					Buff.Format(_T("%.01f"),fV);
					size=pDC->GetTextExtent(Buff);
					pDC->TextOut(pFeature->rectClient.right+nLength,nY+size.cx/2,Buff);
				};
			};
		}

		pDC->SelectObject(pOldFont);
		delete pFont;
	}
	pDC->SetTextColor(rgbTextColor);

}


void CReportPrintDlg::DrawList(CPaintDC *pDC, CRect rr, int nDevice)
{
	int	ii;
	CString	Buff,Temp;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_LIST) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());
			pFont = (CFont *) new CFont;
			pFont->CreateFontIndirect(&lf);
			pOldFont = pDC->SelectObject(pFont);
			pDC->SetTextColor(pFeature[ii].rgbText);
			pDC->SetBkMode(TRANSPARENT);

			switch(pFeature[ii].nListType) {
			case 0: DrawPr30List(pDC,&pFeature[ii],nDevice);
				break;
			case 1: DrawCADElementList(pDC,&pFeature[ii],nDevice);
				break;
			case 2: DrawPl15List(pDC,&pFeature[ii],nDevice);
				break;
			case 3: DrawComponentDetailsList(pDC,&pFeature[ii],nDevice);
				break;
			case 4: DrawDefectTableList(pDC,&pFeature[ii],nDevice);
				break;
			case 5:// DrawMultipleResultsList(pDC,&pFeature[ii],nDevice);
				break;
			case 6:// DrawCTGAnalysisList(pDC,&pFeature[ii],nDevice);
				break;
			case 7: DrawReportList(pDC,&pFeature[ii],nDevice);
				break;
			}
			pDC->SelectObject(pOldFont);
			delete pFont;

			DrawBorder(pDC,&pFeature[ii],nDevice);

		}
	}
	pDC->SetTextColor(rgbTextColor);

}


void CReportPrintDlg::DrawPr30List(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice)
{
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	int	ii;

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s"),*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	for(ii=0;ii<7;ii++) {
		switch(ii) {
		case 0:	theApp.m_UtUser.Pr30GetMainGain(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		case 1:	theApp.m_UtUser.Pr30GetPreampGain(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		case 2:	theApp.m_UtUser.Pr30GetPulseWidth(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		case 3:	theApp.m_UtUser.Pr30GetPulseVoltage(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		case 4:	theApp.m_UtUser.Pr30GetDamping(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		case 5:	theApp.m_UtUser.Pr30GetFilterRange(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		case 6:	theApp.m_UtUser.Pr30GetLowPassFilter(&m_pData->m_TS[0].Pr30,&Buff,VERBOSE, pFeature->nLanguage);
			break;
		}
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;
}

void CReportPrintDlg::DrawPr15List(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice)
{
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	int	ii;

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s"),*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}

	for(ii=0;ii<7;ii++) {
		switch(ii) {
		case 0:	theApp.m_UtUser.Pr15GetMainGain(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		case 1:	theApp.m_UtUser.Pr15GetPreampGain(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		case 2:	theApp.m_UtUser.Pr15GetPulseWidth(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		case 3:	theApp.m_UtUser.Pr15GetPulseVoltage(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		case 4:	theApp.m_UtUser.Pr15GetDamping(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		case 5:	theApp.m_UtUser.Pr15GetFilterRange(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		case 6:	theApp.m_UtUser.Pr15GetLowPassFilter(&m_pData->m_TS[0].Pr15,&Buff,VERBOSE);
			break;
		}
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;
}

void CReportPrintDlg::DrawPl15List(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice)
{
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	int	ii;

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s"),*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}

	for(ii=0;ii<7;ii++) {
		switch(ii) {
		case 0:	theApp.m_UtUser.Pl15GetMainGain(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		case 1:	theApp.m_UtUser.Pl15GetPreampGain(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		case 2:	theApp.m_UtUser.Pl15GetPulseWidth(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		case 3:	theApp.m_UtUser.Pl15GetPulseVoltage(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		case 4:	theApp.m_UtUser.Pl15GetDamping(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		case 5:	theApp.m_UtUser.Pl15GetFilterRange(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		case 6:	theApp.m_UtUser.Pl15GetLowPassFilter(&m_pData->m_TS[0].Pl15,&Buff,VERBOSE,pFeature->nLanguage);
			break;
		}
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;
}

void CReportPrintDlg::DrawImageFileName(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	CString	Buff;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;
	CRect rr;
	int	nX,nY;

	m_staticView.GetWindowRect(&rr);
	rr.OffsetRect(-rr.left,-rr.top);

	if(nDevice == PRINTER) {
		rr = m_rectPrintArea;
	}



	if(pFeature->nCaptionPosition){
		lf = pFeature->lf;
		lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature->rectView.Width());
		pFont = (CFont *) new CFont;
		pFont->CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(pFeature->rgbText);
		pDC->SetBkMode(TRANSPARENT);
		Buff.Format(_T("%s %s"),*pFeature->pCaption,m_pData->m_FileName);
		size = pDC->GetTextExtent(Buff);

		switch(pFeature->nCaptionPosition) {
		case 1:
			nX = pFeature->rectClient.left;
			nY = pFeature->rectClient.top - size.cy - size.cy/2;
			break;
		case 2:
			nX = (pFeature->rectClient.left+pFeature->rectClient.right) / 2 - size.cx/2;
			nY = pFeature->rectClient.top - size.cy - size.cy/2;
			break;
		case 3:
			nX = pFeature->rectClient.right - size.cx;
			nY = pFeature->rectClient.top - size.cy - size.cy/2;
			break;
		case 4:
			nX = pFeature->rectClient.left;
			nY = pFeature->rectClient.bottom + size.cy + size.cy / 2;
			break;
		case 5:
			nX = (pFeature->rectClient.left+pFeature->rectClient.right) / 2 - size.cx/2;
			nY = pFeature->rectClient.bottom + size.cy + size.cy / 2;
			break;
		case 6:
			nX = pFeature->rectClient.right - size.cx;
			nY = pFeature->rectClient.bottom + size.cy + size.cy / 2;
			break;
		}
		pDC->TextOut(nX,nY,Buff);

		pDC->SelectObject(pOldFont);
		delete pFont;
	}

	pDC->SetTextColor(rgbTextColor);

}

void CReportPrintDlg::DrawImageScale(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	
	CString	Buff;
	CPoint pt;
	LOGFONT lf;
	CFont* pFont;
	CFont* pOldFont;
	COLORREF	rgbTextColor = pDC->GetTextColor();
	CSize size;
	CRect rr;
	int	nX,nY,nTextStep;
	float fXSize, fYSize, fStep,fTop,fBottom,fLeft,fRight,fHScale,fVScale;
	CString Scale,Horizontal,Vertical;


	m_staticView.GetWindowRect(&rr);
	rr.OffsetRect(-rr.left,-rr.top);

	if(nDevice == PRINTER) {
		rr = m_rectPrintArea;
	}



	if(pFeature->nScalePosition){

		Scale.LoadString(IDS_Scale);
		Horizontal.LoadString(IDS_Horizontal);
		Vertical.LoadString(IDS_Vertical);

		if(m_pViewPage) m_pViewPage->GetGraticuleSize(&fXSize, &fYSize, &fStep, &nTextStep,theApp.m_fUnits);
		PaperToPrinterMM(&pFeature->rr,&fTop,&fBottom,&fLeft,&fRight);

		fHScale = fabs((fRight-fLeft) / fXSize);
		fVScale = fabs((fBottom-fTop) / fYSize);

		lf = pFeature->lf;
		lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature->rectView.Width());
		pFont = (CFont *) new CFont;
		pFont->CreateFontIndirect(&lf);
		pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(pFeature->rgbText);
		pDC->SetBkMode(TRANSPARENT);

		Buff.Format(_T("%s %s:%.02f %s:%.02f"),Scale,Horizontal,fHScale,Vertical,fVScale);
		size = pDC->GetTextExtent(Buff);

		switch(pFeature->nScalePosition) {
		case 1:
			nX = pFeature->rectClient.CenterPoint().x - size.cx/2;
			nY = pFeature->rectClient.top - size.cy - size.cy/2;
			break;
		case 2:
			nX = pFeature->rectClient.CenterPoint().x - size.cx/2;
			nY = pFeature->rectClient.bottom + size.cy / 2;
			break;
		case 3:
			nX = pFeature->rectClient.right - size.cx;
			nY = pFeature->rectClient.top - size.cy - size.cy/2;
			break;
		}
		pDC->TextOut(nX,nY,Buff);

		pDC->SelectObject(pOldFont);
		delete pFont;
	}

	pDC->SetTextColor(rgbTextColor);

}

void CReportPrintDlg::DrawBarGraph(CPaintDC *pDC, CRect rr, int nDevice)
{
	int nImageType = 0;
	CAmplitudePalettePage AmpDlg;
	CThicknessPalettePage ThickDlg;

	int	ii;
	CRect rectBorder;
	LOGFONT lf;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_BARGRAPH) && (pFeature[ii].nPageNumber == m_nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);

			if(pFeature[ii].rectClient.left>pFeature[ii].rectClient.right) pFeature[ii].rectClient.left=pFeature[ii].rectClient.right;
			if(pFeature[ii].rectClient.top>pFeature[ii].rectClient.bottom) pFeature[ii].rectClient.top=pFeature[ii].rectClient.bottom;

			lf = pFeature[ii].lf;
			lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());

			pDC->SetTextColor(RGB(0,0,0));
			switch(pFeature[ii].nImageNumber) {
			default:
				AmpDlg.RefreshBarGraph(pDC,pFeature[ii].rectClient,pFeature[ii].nImageNumber,&lf,nDevice);
				break;
			case 8:
				ThickDlg.RefreshBarGraph(pDC,pFeature[ii].rectClient);
				break;
			}

			DrawBorder(pDC,&pFeature[ii],nDevice);
//			DrawGraticule(pDC,&pFeature[ii],nDevice);
//			DrawImageFileName(pDC,&pFeature[ii],nDevice);

		}
	}

}

void CReportPrintDlg::DrawComponentStatsList(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	/*
	
	CString Buff,Temp,Name,Value;
	CPoint pt;
	CSize size;
	int	ii;
	CString m_TotalFBH, m_InclusionLength, m_TSA, m_VolumeDm, m_TotalTargetsPerKg;

	m_pData->CalculateMass();
	m_pData->CalculateTSA();
	double dVolume = m_pData->CalculateVolume();
	double m_dFbhTotal = 0.0;
	double m_dLengthTotal = 0.0;
	double m_dEDATotal = 0.0;
	for(ii=0;ii<m_pData->m_nDefL;ii++) {
		m_dFbhTotal += m_pData->m_Def[ii].dFBH;
		m_dLengthTotal += m_pData->m_Def[ii].dLength;
		m_dEDATotal += (m_pData->m_Def[ii].dLength*m_pData->m_Def[ii].dFBH);
	}
	m_TotalFBH.Format(_T("%.02f",m_dFbhTotal);
	m_InclusionLength.Format(_T("%.01f mm",m_pData->m_dInclusionLength);
	m_TSA.Format(_T("%.01f",m_pData->m_dTSA);
	m_VolumeDm.Format(_T("%.02f",(m_pData->m_dTestMass / theApp.m_LastSettings.dMaterialDensity) / 1e6  );
	m_TotalTargetsPerKg.Format(_T("%.02f",(double)m_pData->m_nDefL / m_pData->m_dTestMass);

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s",*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	Buff = "HHHHHHHHHHHHHHH";
	size = pDC->GetTextExtent(Buff);
	int nNameWidth = size.cx;
	int nNameHeight = size.cy;
	for(ii=0;ii<19;ii++) {
		switch(ii) {
		case 0:	Name = "Cast";
			Value.Format(_T("%s",m_pData->m_Cast);
			break;
		case 1: Name = "Customer";
			Value.Format(_T("%s",m_pData->m_Customer);
			break;
		case 2: Name = "Grade";
			Value.Format(_T("%s",m_pData->m_Grade);
			break;
		case 3:	Name = "Order Number";
			Value.Format(_T("%s",m_pData->m_OrderNumber);
			break;
		case 4:	Name = "Sample Ref";
			Value.Format(_T("%s",m_pData->m_SampleRef);
			break;
		case 5:	Name = "Focal Depth Fe";
			Value.Format(_T("%s",m_pData->m_FocalDepth);
			break;
		case 6:	Name = "Focal Length H20";
			Value.Format(_T("%s",m_pData->m_FocalLength);
			break;
		case 7:	Name = "Probe";
			Value.Format(_T("%s",m_pData->m_Probe);
			break;
		case 8:	Name = "Ref FBH";
			Value.Format(_T("%s",m_pData->m_RefFBH);
			break;
		case 9:	Name = "Sensitivity";
			Value.Format(_T("%s",m_pData->m_SensitivityFBH);
			break;
		case 10: Name = "Ref Gain (dB)";
			Value.Format(_T("%s","Ref Gain (dB)",m_pData->m_RefGain);
			break;
		case 11: Name = "Test Gain (dB)";
			Value.Format(_T("%s",m_pData->m_TestGain);
			break;
		case 12: Name = "Test Date";
			Value.Format(_T("%s",m_pData->m_TestDate);
			break;
		case 13: Name = "Mass";
			Value.Format(_T("%.02f",m_pData->m_dTestMass);
			break;
		case 14: Name = "Volume/Dm^3";
			Value.Format(_T("%.02f"),(m_pData->m_dTestMass / theApp.m_LastSettings.dMaterialDensity) / 1e6  );
			break;
		case 15: Name = "Calculated Results";
			Value = " ";
			break;
		case 16: Name = "Total Inc' Length/Dm^3";
			Value.Format(_T("%s",m_InclusionLength);
			break;
		case 17: Name = "TSN/10Kg";
			Value.Format(_T("%s",m_TSA);
			break;
		case 18: Name = "Total Targets /Kg";
			Value.Format(_T("%s",m_TotalTargetsPerKg);
			break;
		case 19: Name = "TEDA / Dm3";
			Value.Format(_T("%.02f",m_dEDATotal/dVolume);
			break;
		}
		pDC->TextOut(pt.x,pt.y,Name);
		pDC->TextOut(pt.x+nNameWidth,pt.y,Value);
		size = pDC->GetTextExtent(Value);
		pt.y += nNameHeight;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x+nNameWidth + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + nNameWidth + size.cx;
	}
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;
	*/

}




void CReportPrintDlg::DrawDefectTableList(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	char Class[5] = {" ABC"};

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s"),*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	Buff = "Comment_";
	size = pDC->GetTextExtent(Buff);
	int nNameWidth = size.cx;
	int nNameHeight = size.cy;
	int	nX = pt.x;

	for(int nItem = 0 ; nItem<6;nItem++) {
		switch(nItem) {
		case 0: Buff = "No.";
			nX = pt.x;
			break;
		case 1: Buff = "X";
			nX+=((nNameWidth*3)/2);
			break;
		case 2: Buff = "Y";
			nX+=nNameWidth;
			break;
		case 3: Buff.LoadString(IDS_Amplitude);
			nX+=nNameWidth;
			break;
		case 4: Buff.LoadString(IDS_Depth);
			nX+=nNameWidth;
			break;
		case 5: Buff.LoadString(IDS_Comment);
			nX+=nNameWidth;
			break;
		}
		size = pDC->GetTextExtent(Buff);
		if(nItem==0) {
			pDC->TextOut(nX,pt.y,Buff);
		} else {
			pDC->TextOut(nX-size.cx,pt.y,Buff);
		}
	}
	pt.y += size.cy;
	pFeature->rectClient.bottom = pt.y;
	double dFBH=0.0;
	double dLength = 0.0;
	double dEDA = 0.0;
/*
	for(ii=0;ii<m_pData->m_nDefL;ii++) {
		for(int nItem = 0 ; nItem<6;nItem++) {
			switch(nItem) {
			case 0: Buff.Format(_T("%d"),ii+1);
				nX = pt.x;
				break;
			case 1: Buff.Format(_T("%.01f"),m_pData->m_Def[ii].dXp);
				nX+=(nNameWidth/2 + nNameWidth);
				break;
			case 2: Buff.Format(_T("%.01f"),m_pData->m_Def[ii].dYp);
				nX+=nNameWidth;
				break;
			case 3: Buff.Format(_T("%d%%"),m_pData->m_Def[ii].nAmp[1]);
				nX+=nNameWidth;
				break;
			case 4: Buff.Format(_T("%.01fmm"),m_pData->m_Def[ii].dDepth);
				nX+=nNameWidth;
				break;
			case 5: Buff.Format(_T("%s"),m_pData->m_Def[ii].String);
				nX+=((nNameWidth*5)/2);
				break;
			}
			if(!Buff.IsEmpty()) size = pDC->GetTextExtent(Buff);
			if(nItem==0) {
				pDC->TextOut(nX,pt.y,Buff);
			} else {
				pDC->TextOut(nX-size.cx,pt.y,Buff);
			}
		}
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
	}
	pFeature->rectClient.right = nX+2;
	*/
/*
	pt.y += size.cy;
	for(nItem = 0 ; nItem<6;nItem++) {
		switch(nItem) {
		case 0: Buff.Format(_T("%d"),m_pData->m_nDefL);
			nX = pt.x;
			break;
		case 1: Buff.Empty();
			nX+=(nNameWidth/2 + nNameWidth);
			break;
		case 2:
			nX+=nNameWidth;
			break;
		case 3:
			nX+=nNameWidth;
			break;
		case 4:
			nX+=nNameWidth;
			break;
		case 5:
			nX+=(nNameWidth*2);
			break;
		}
		size = pDC->GetTextExtent(Buff);
		if(nItem==0) {
			pDC->TextOut(nX,pt.y,Buff);
		} else {
			pDC->TextOut(nX-size.cx,pt.y,Buff);
		}
	}
	*/
	pt.y += size.cy;
	pFeature->rectClient.bottom = pt.y;

	
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;

}




void CReportPrintDlg::DrawReportList(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice)
{
	/*
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	int	ii,nX,nTextWidth,nTextHeight;
	CCTGScanDetailsDlg Details(NULL,NULL,1,m_pData);

	Details.InitializeFields();

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s",*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	Buff = "9999999999999999%";
	size = pDC->GetTextExtent(Buff);
	nTextWidth = size.cx;
	nTextHeight = size.cy;


	for(ii=0;ii<21;ii++) {
		for(int nItem = 0 ; nItem<2;nItem++) {
			switch(nItem) {
			case 0: Buff = Details.m_strField[ii];
				nX = pt.x;
				break;
			case 1: Buff = m_pData->m_strReport[ii+m_pData->m_nUsedTimeSlot*30];
				nX += nTextWidth;
				break;
			}
			pDC->TextOut(nX,pt.y,Buff);
			size = pDC->GetTextExtent(Buff);
			if(pFeature->rectClient.right < (nX+size.cx)) pFeature->rectClient.right = (nX+size.cx);
		}
		pt.y += nTextHeight;
		pFeature->rectClient.bottom = pt.y;
	}
	*/
}

//??????
void CReportPrintDlg::MMtoCoords(float fTop,float fLeft,float fBottom,float fRight,CRect *prr)
{
	float fLeftMargin=0.0f;
	float fRightMargin=0.0f;
	float fTopMargin=0.0f;
	float fBottomMargin=0.0f;

	prr->left = (int)((fLeft + fLeftMargin) * 10000.0f / (m_fPrintingAreaWidthMM-fRightMargin-fLeftMargin));
	prr->right = (int)((fRight + fLeftMargin) * 10000.0f / (m_fPrintingAreaWidthMM-fRightMargin-fLeftMargin));

	prr->top = (int)((fTop + fTopMargin) * 10000.0f / (m_fPrintingAreaHeightMM-fTopMargin-fBottomMargin));
	prr->bottom = (int)((fBottom + fTopMargin) * 10000.0f / (m_fPrintingAreaHeightMM-fTopMargin-fBottomMargin));
}


void CReportPrintDlg::RectToMM(CRect rr, float *fTop, float *fLeft, float *fBottom, float *fRight)
{
	float fLeftMargin=0.0f;
	float fRightMargin=0.0f;
	float fTopMargin=0.0f;
	float fBottomMargin=0.0f;

	*fLeft = ((float)rr.left * (m_fPrintingAreaWidthMM-fRightMargin-fLeftMargin) / 10000.0f) - fLeftMargin;
	*fRight = ((float)rr.right * (m_fPrintingAreaWidthMM-fRightMargin-fLeftMargin) / 10000.0f) - fLeftMargin;

	*fTop = ((float)rr.top * (m_fPrintingAreaHeightMM-fTopMargin-fBottomMargin) / 10000.0f) - fTopMargin;
	*fBottom = ((float)rr.bottom * (m_fPrintingAreaHeightMM-fTopMargin-fBottomMargin) / 10000.0f) - fTopMargin;
}


void CReportPrintDlg::AddLine(float fTop, float fLeft, float fBottom, float fRight, int nWidth, COLORREF rgb)
{
	CRect rr;

	AddFeature(REPORT_LINE, rr.TopLeft(),FALSE);
	MMtoCoords(fTop,fLeft,fBottom,fRight,&rr);
	pFeature[m_nFeaturePtr].nLineWidth = nWidth;
	pFeature[m_nFeaturePtr].rr = rr;
	pFeature[m_nFeaturePtr].rgbLine = rgb;

}

void CReportPrintDlg::AddText(CString Buff, float fTop, float fLeft, int nSize, COLORREF rgb,int nWeight,WCHAR *pFaceName, int nPageNumber)
{
	CRect rr;

	AddFeature(REPORT_TEXT, rr.TopLeft(),FALSE);
	MMtoCoords(fTop,fLeft,0.0f,0.0f,&rr);
	pFeature[m_nFeaturePtr].rr = rr;
	pFeature[m_nFeaturePtr].rgbText = rgb;
	pFeature[m_nFeaturePtr].lf.lfHeight = nSize;
	if(pFeature[m_nFeaturePtr].pCaption) delete pFeature[m_nFeaturePtr].pCaption;
	pFeature[m_nFeaturePtr].pCaption = new CString(Buff);
	pFeature[m_nFeaturePtr].lf.lfWeight=nWeight;
	if(pFaceName!= NULL) {
		wcscpy_s(pFeature[m_nFeaturePtr].lf.lfFaceName,32,(WCHAR*)pFaceName);
	}
	pFeature[m_nFeaturePtr].nPageNumber = nPageNumber;
	if(m_nMaxPageNumber < nPageNumber) m_nMaxPageNumber = nPageNumber;

}


void CReportPrintDlg::AddDataValue(int nDataValueType, float fTop, float fLeft, int nSize, COLORREF rgb,int nWeight)
{
	CRect rr;

	AddFeature(REPORT_DATA_VALUE, rr.TopLeft(),FALSE);
	MMtoCoords(fTop,fLeft,0.0f,0.0f,&rr);
	pFeature[m_nFeaturePtr].rr = rr;
	pFeature[m_nFeaturePtr].rgbText = rgb;
	pFeature[m_nFeaturePtr].lf.lfHeight = nSize;
	pFeature[m_nFeaturePtr].nDataValueType = nDataValueType;
	pFeature[m_nFeaturePtr].lf.lfWeight=nWeight;


}

void CReportPrintDlg::VSMPOPageZero()
{
	CString Buff;
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(10,10);

	AddFeature(REPORT_RECTANGLE, rr.TopLeft(),FALSE);
	pFeature[m_nFeaturePtr].nLineWidth = 2;
	MMtoCoords(0,0,281,192,&rr);
	pFeature[m_nFeaturePtr].rr = rr;
	pFeature[m_nFeaturePtr].rgbLine = RGB(0,0,0);

	//Horizontal Lines
	AddLine(10,0,10,192,2,RGB(0,0,0));
	AddLine(20,0,20,192,2,RGB(0,0,0));
	AddLine(25,0,25,192,2,RGB(0,0,0));
	AddLine(50,0,50,192,1,RGB(0,0,0));
	AddLine(60,0,60,192,1,RGB(0,0,0));
	AddLine(70,0,70,192,1,RGB(0,0,0));
	AddLine(80,0,80,192,1,RGB(0,0,0));
	AddLine(90,0,90,192,2,RGB(0,0,0));
	AddLine(95,0,95,192,2,RGB(0,0,0));
	AddLine(105,0,105,192,1,RGB(0,0,0));
	AddLine(115,0,115,192,1,RGB(0,0,0));
	AddLine(125,0,125,192,2,RGB(0,0,0));
	AddLine(130,0,130,192,2,RGB(0,0,0));
	AddLine(150,0,150,192,2,RGB(0,0,0));
	AddLine(160,0,160,192,2,RGB(0,0,0));
	AddLine(170,0,170,192,1,RGB(0,0,0));
	AddLine(180,0,180,192,1,RGB(0,0,0));
	AddLine(190,0,190,192,1,RGB(0,0,0));
	AddLine(200,0,200,192,1,RGB(0,0,0));
	AddLine(210,0,210,192,1,RGB(0,0,0));
	AddLine(220,0,220,192,2,RGB(0,0,0));
	AddLine(230,0,230,192,2,RGB(0,0,0));
	AddLine(240,0,240,192,1,RGB(0,0,0));
	AddLine(250,0,250,192,1,RGB(0,0,0));
	AddLine(260,0,260,192,1,RGB(0,0,0));
	AddLine(270,0,270,192,1,RGB(0,0,0));
	AddLine(280,0,280,192,1,RGB(0,0,0));

	//Vertical Lines
	AddLine(25,97.5,90,97.5,1,RGB(0,0,0));
	AddLine(95,97.5,115,97.5,1,RGB(0,0,0));
	AddLine(160,48.75,210,48.75,1,RGB(0,0,0));
	AddLine(160,97.5,210,97.5,1,RGB(0,0,0));
	AddLine(160,146.25,210,146.25,1,RGB(0,0,0));
	AddLine(230,48.75,280,48.75,1,RGB(0,0,0));
	AddLine(230,75,280,75,1,RGB(0,0,0));
	AddLine(230,146.25,280,146.25,1,RGB(0,0,0));

	Buff.LoadString(IDS_Print_Russian_Title);
	AddText(Buff,0,70,FONT_SIZE,RGB(0,0,0),900);
	Buff.LoadString(IDS_Print_English_Title);
	AddText(Buff,5,50,FONT_SIZE,RGB(0,0,0),900);
	Buff.LoadString(IDS_Print_Report_Number);
	AddText(Buff,12,2,FONT_SIZE,RGB(0,0,0),900);
	Buff.LoadString(IDS_Print_Russian_Customer);
	AddText(Buff,26,2,FONT_SIZE,RGB(0,0,0));
	Buff.LoadString(IDS_Print_English_Customer);
	AddText(Buff,31,2,FONT_SIZE,RGB(0,0,0));
	Buff.LoadString(IDS_Print_Russian_Contract_No);
	AddText(Buff,36,2,FONT_SIZE,RGB(0,0,0));
	Buff.LoadString(IDS_Print_English_Contract_No);
	AddText(Buff,41,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",50,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Item",55,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",60,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Item code/dimensions;mm",65,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",70,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Item number (if needed)",75,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",80,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Amout of Items in a lot; pcs",84,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",95,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Type of inspection unit",100,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",105,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"RS number",110,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",115,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Type and dimensions of reflectors in RS",120,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",130,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Background noise level, %(dB)",135,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",140,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"Backwall echo variation, %(dB)",144,2,FONT_SIZE,RGB(0,0,0));

	AddText(L"????????",26,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"Specification",36,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",50,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"Insp.Technique Sheet No",55,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",60,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"Heat No",65,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",70,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"Lot No",75,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",80,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"Alloy",85,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",95,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"Type of flaw detector",100,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"????????",105,99,FONT_SIZE,RGB(0,0,0));
	AddText(L"transducer types",110,99,FONT_SIZE,RGB(0,0,0));

	AddText(L"??????????????????? (General information)",20,70,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"??????????????????? (Inspection Data)",90,70,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"??????????????????? (Inspection Results)",125,70,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"???????????????????",150,70,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"(Items Meeting Specification Requirements)",155,70,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"???????????????????",220,70,FONT_SIZE,RGB(0,0,0));
	AddText(L"(Items Not Meeting Specification Requirements)",225,70,FONT_SIZE,RGB(0,0,0),900);

	AddDataValue(REPORT_CUSTOMER_DETAILS,25,30,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_CONTRACT_DETAILS,35,30,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_ITEM_DETAILS,50,30,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_ITEM_CODE_DETAILS,60,30,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_NUMBER_ITEMS_IN_LOT_DETAILS,80,30,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_TECHNIQUE_DETAILS,50,120,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_HEAT_NO_DETAILS,60,120,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_LOT_DETAILS,70,120,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_ALLOY_DETAILS,80,120,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(REPORT_SPECIFICATION_DETAILS,25,120,FONT_SIZE,RGB(0,0,0),900);

}


void CReportPrintDlg::VSMPOPageOne()
{
	int nTS;
	CString Buff;
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(10,10);

	AddFeature(REPORT_RECTANGLE, rr.TopLeft(),FALSE);
	pFeature[m_nFeaturePtr].nLineWidth = 2;
	MMtoCoords(0,0,281,192,&rr);
	pFeature[m_nFeaturePtr].rr = rr;
	pFeature[m_nFeaturePtr].rgbLine = RGB(0,0,0);

	//Horizontal Lines
	AddLine(5,0,5,192,2,RGB(0,0,0));
	AddLine(10,0,10,192,2,RGB(0,0,0));
	AddLine(15,0,15,192,2,RGB(0,0,0));
	AddLine(41,0,41,192,1,RGB(0,0,0));
	AddLine(54.5,0,54.5,192,2,RGB(0,0,0));
	AddLine(59,0,59,192,2,RGB(0,0,0));
	AddLine(68,0,68,192,2,RGB(0,0,0));
	AddLine(72.5,0,72.5,192,2,RGB(0,0,0));
	AddLine(77,0,77,192,1,RGB(0,0,0));
	AddLine(81.5,0,81.5,192,1,RGB(0,0,0));
	AddLine(86,0,86,192,1,RGB(0,0,0));
	AddLine(90.5,0,90.5,192,1,RGB(0,0,0));
	AddLine(95,0,95,192,1,RGB(0,0,0));
	AddLine(99.5,0,99.5,192,1,RGB(0,0,0));
	AddLine(104,0,104,192,1,RGB(0,0,0));
	AddLine(108.5,0,108.5,192,1,RGB(0,0,0));
	AddLine(113,0,113,192,2,RGB(0,0,0));
	AddLine(117.5,0,117.5,192,2,RGB(0,0,0));
	AddLine(135,0,135,192,2,RGB(0,0,0));
	AddLine(144,0,144,192,1,RGB(0,0,0));
	AddLine(148.5,0,148.5,192,1,RGB(0,0,0));
	AddLine(153,0,153,192,1,RGB(0,0,0));
	AddLine(157.5,0,157.5,192,1,RGB(0,0,0));
	AddLine(162,0,162,192,1,RGB(0,0,0));
	AddLine(166.5,0,166.5,192,1,RGB(0,0,0));
	AddLine(171,0,171,192,2,RGB(0,0,0));
	AddLine(175.5,0,175.5,192,2,RGB(0,0,0));
	AddLine(180,0,180,192,2,RGB(0,0,0));
	AddLine(189,0,189,192,2,RGB(0,0,0));
	AddLine(193.5,0,193.5,192,2,RGB(0,0,0));
	AddLine(202.5,0,202.5,192,1,RGB(0,0,0));
	AddLine(207,0,207,192,2,RGB(0,0,0));
	AddLine(211.5,0,211.5,192,2,RGB(0,0,0));
	AddLine(230,0,230,192,2,RGB(0,0,0));

	//Vertical Lines
	AddLine(15,97.5,41,97.5,1,RGB(0,0,0));
	AddLine(59,97.5,68,97.5,1,RGB(0,0,0));

	AddLine(72.5,13,113,13,1,RGB(0,0,0));
	AddLine(72.5,49,113,49,1,RGB(0,0,0));
	AddLine(72.5,63,113,63,1,RGB(0,0,0));
	AddLine(72.5,97.5,113,97.5,1,RGB(0,0,0));
	AddLine(72.5,125,113,125,1,RGB(0,0,0));
	AddLine(72.5,145,113,145,1,RGB(0,0,0));

	AddLine(117.5,97.5,135,97.5,1,RGB(0,0,0));
	AddLine(135,13,180,13,1,RGB(0,0,0));
	AddLine(135,43,180,43,1,RGB(0,0,0));
	AddLine(135,73,180,73,1,RGB(0,0,0));
	AddLine(135,103,180,103,1,RGB(0,0,0));
	AddLine(135,133,180,133,1,RGB(0,0,0));
	AddLine(135,163,180,163,1,RGB(0,0,0));

	AddText(L"Probe",72.5,2,FONT_SIZE,RGB(0,0,0));
	AddText(L"1",77,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"2",81.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"3",86,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"4",90.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"5",95,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"6",99.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"7",104,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"8",108.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"Serial Number",72.5,15,FONT_SIZE,RGB(0,0,0));
	AddDataValue(PROBE_SERIAL_0,77,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_1,81.5,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_2,86,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_3,90.5,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_4,95,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_5,99.5,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_6,104,20,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SERIAL_7,108.5,20,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Frequency",72.5,65,FONT_SIZE,RGB(0,0,0));
	AddDataValue(PROBE_FREQ_0,77,  70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_1,81.5,70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_2,86,  70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_3,90.5,70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_4,95,  70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_5,99.5,70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_6,104, 70,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_FREQ_7,108.5,70,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Diameter",72.5,100,FONT_SIZE,RGB(0,0,0));
	AddDataValue(PROBE_DIA_0,77,  100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_1,81.5,100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_2,86,  100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_3,90.5,100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_4,95,  100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_5,99.5,100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_6,104, 100,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_DIA_7,108.5,100,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Foc' Length",72.5,128,FONT_SIZE,RGB(0,0,0));
	AddDataValue(PROBE_LENGTH_0,77,  130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_1,81.5,130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_2,86,  130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_3,90.5,130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_4,95,  130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_5,99.5,130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_6,104, 130,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_LENGTH_7,108.5,130,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Spot Size",72.5,150,FONT_SIZE,RGB(0,0,0));
	AddDataValue(PROBE_SPOT_0,77,  160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_1,81.5,160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_2,86,  160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_3,90.5,160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_4,95,  160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_5,99.5,160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_6,104, 160,FONT_SIZE,RGB(0,0,0),900);
	AddDataValue(PROBE_SPOT_7,108.5,160,FONT_SIZE,RGB(0,0,0),900);

	AddText(L"VSMPO-AVISMA Corporation NDT Lab (shop 23)",0.5,30,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Report",5.5,3,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"General",10.5,80,FONT_SIZE,RGB(0,0,0),900);


	AddText(L"1",144,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"2",148.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"3",153,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"4",157.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"5",162,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"6",166.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"7",171,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"8",175.5,5,FONT_SIZE,RGB(0,0,0));
	AddText(L"Hole Depth",135,15,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_DEPTH_0,139.5,15,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Hole Depth",135,45,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_DEPTH_1,139.5,45,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Hole Depth",135,75,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_DEPTH_2,139.5,75,FONT_SIZE,RGB(0,0,0),900);

	AddText(L"Nominal Gain",135,105,FONT_SIZE,RGB(0,0,0));
	AddText(L"Compensation",135,135,FONT_SIZE,RGB(0,0,0));
	AddText(L"Inspection Gain",135,165,FONT_SIZE,RGB(0,0,0));

	for(nTS=0;nTS<8;nTS++) {
		AddDataValue(REPORT_AMP_AT_DEPTH_0+nTS,144.0f+(float)nTS*4.5f,15,FONT_SIZE,RGB(0,0,0),900);
	}
	for(nTS=0;nTS<8;nTS++) {
		AddDataValue(REPORT_AMP_AT_DEPTH_1+nTS,144.0f+(float)nTS*4.5f,45,FONT_SIZE,RGB(0,0,0),900);
	}
	for(nTS=0;nTS<8;nTS++) {
		AddDataValue(REPORT_AMP_AT_DEPTH_2+nTS,144.0f+(float)nTS*4.5f,75,FONT_SIZE,RGB(0,0,0),900);
	}
	for(nTS=0;nTS<8;nTS++) {
		AddDataValue(REPORT_NOMINAL_GAIN+nTS,144.0f+(float)nTS*4.5f,105,FONT_SIZE,RGB(0,0,0),900);
	}
	for(nTS=0;nTS<8;nTS++) {
		AddDataValue(REPORT_COMPENSATION_GAIN+nTS,144.0f+(float)nTS*4.5f,135,FONT_SIZE,RGB(0,0,0),900);
	}
	for(nTS=0;nTS<8;nTS++) {
		AddDataValue(REPORT_INSPECTION_GAIN+nTS,144.0f+(float)nTS*4.5f,165,FONT_SIZE,RGB(0,0,0),900);
	}


//	AddLine(135,103,180,103,1,RGB(0,0,0));
//	AddLine(135,133,180,133,1,RGB(0,0,0));
//	AddLine(135,163,180,163,1,RGB(0,0,0));

	AddText(L"File Name",15,3,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_FILE_NAME,15,60,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Customer",20,3,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_CUSTOMER_DETAILS,20,60,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Contract No",25,3,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_CONTRACT_DETAILS,25,60,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Item",30,3,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_ITEM_DETAILS,30,60,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Inspection Procedure",35,3,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_TECHNIQUE_DETAILS,35,60,FONT_SIZE,RGB(0,0,0),900);

	AddText(L"US Specification",41,3,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_SPECIFICATION_DETAILS,41,100,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Additional Documents",45.5,3,FONT_SIZE,RGB(0,0,0));
	AddText(L"Inspection Technique",50,3,FONT_SIZE,RGB(0,0,0));


	AddText(L"Equipment",55,85,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Flaw Detector Type",59.5,3,FONT_SIZE,RGB(0,0,0));
	AddText(L"Unit Type",64,3,FONT_SIZE,RGB(0,0,0));
	AddText(L"Serial Number",59.5,100,FONT_SIZE,RGB(0,0,0));

	AddText(L"Probes",68,85,FONT_SIZE,RGB(0,0,0),900);

	AddText(L"Heat Number",15,99,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_HEAT_NO_DETAILS,15,120,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Lot Number",19.5,99,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_ITEM_DETAILS,19.5,120,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Alloy Grade",24,99,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_LOT_DETAILS,24,120,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Plate Size",28.5,99,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_SIZE_DETAILS,28.5,120,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Lot Code",33,99,FONT_SIZE,RGB(0,0,0));
	AddDataValue(REPORT_ITEM_CODE_DETAILS,33,120,FONT_SIZE,RGB(0,0,0),900);

	AddText(L"Setup",113.5,85,FONT_SIZE,RGB(0,0,0),900);
	AddText(L"Test Block Number",118,3,FONT_SIZE,RGB(0,0,0));
	AddText(L"Hole Type",122.5,3,FONT_SIZE,RGB(0,0,0));
	AddText(L"Wave Type",127,3,FONT_SIZE,RGB(0,0,0));
	AddText(L"Alloy Grade",118,100,FONT_SIZE,RGB(0,0,0));
	AddText(L"Hole Size",122.5,100,FONT_SIZE,RGB(0,0,0));


}

void CReportPrintDlg::OnExamplesPlateReport() 
{
	Zero();
	VSMPOPageZero();
	Invalidate(FALSE);
}

void CReportPrintDlg::OnExamplesSetupreport() 
{
	Zero();
	VSMPOPageOne();	
	Invalidate(FALSE);
}

void CReportPrintDlg::OnTemplateEdit() 
{
	

	if (m_pFeatureEditDlg == NULL) {
		m_pFeatureEditDlg = new CFeatureEditDlg(this,(CWnd**)&m_pFeatureEditDlg,pFeature,&m_nFeatureL);
		m_pFeatureEditDlg->Create(CFeatureEditDlg::IDD, this);	
	} else {
		m_pFeatureEditDlg->SendMessage(WM_CLOSE);
	}
	
}



void CReportPrintDlg::UpdateWindowName()
{
	SetWindowText(m_FileName);

}

void CReportPrintDlg::CloseAllChildren()
{
	if(m_pTextPropertiesDlg) m_pTextPropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pLinePropertiesDlg) m_pLinePropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pRectanglePropertiesDlg) m_pRectanglePropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pImagePropertiesDlg) m_pImagePropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pDatePropertiesDlg) m_pDatePropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pListPropertiesDlg) m_pListPropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pBarGraphPropertiesDlg) m_pBarGraphPropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pDataValuePropertiesDlg) m_pDataValuePropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pFeatureEditDlg) m_pFeatureEditDlg->SendMessage(WM_CLOSE);
	if(m_pReportTablePropertiesDlg) m_pReportTablePropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pReportUltrasonicsPropertiesDlg) m_pReportUltrasonicsPropertiesDlg->SendMessage(WM_CLOSE);
	if(m_pReportHeaderFooterDlg) m_pReportHeaderFooterDlg->SendMessage(WM_CLOSE);

}

void CReportPrintDlg::OnSelectDelete() 
{
	int ii;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].bModify==TRUE) {
			DeleteFeature(ii);
			ii=0;
		}
	}
	Invalidate(FALSE);

}

void CReportPrintDlg::OnSelectMove() 
{
	

	m_nEditOption = ID_BUTTON_MOVE;
}

BOOL CReportPrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	int ii;

	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 27:
			for(ii=0;ii<m_nFeatureL;ii++) {
				pFeature[ii].bModify=FALSE;
			}
			Invalidate(FALSE);
			return TRUE;
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CReportPrintDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_POINTER:  strTipText.LoadString(IDS_Pointer);
		break;
	case ID_BUTTON_LINE:  strTipText.LoadString(IDS_Line);
		break;
	case ID_BUTTON_TEXT:  strTipText.LoadString(IDS_Text);
		break;
	case ID_BUTTON_ERASE:  strTipText.LoadString(IDS_Erase);
		break;
	case ID_BUTTON_RECTANGLE:  strTipText.LoadString(IDS_Rectangle);
		break;
	case ID_BUTTON_IMAGE:  strTipText.LoadString(IDS_Image);
		break;
	case ID_BUTTON_DATE:  strTipText.LoadString(IDS_Date);
		break;
	case ID_BUTTON_MOVE:  strTipText.LoadString(IDS_Move);
		break;
	case ID_BUTTON_LIST:  strTipText.LoadString(IDS_List);
		break;
	case ID_BUTTON_BARGRAPH:  strTipText.LoadString(IDS_Palette);
		break;
	case ID_BUTTON_VARIABLE:  strTipText.LoadString(IDS_Variables);
		break;
	case ID_TEMPLATE_EXPORTTOWORD: strTipText.LoadString(IDS_Export_to_Word);
		break;
	case ID_BUTTON_TABLE: strTipText.LoadString(IDS_Properties);
		break;
	case ID_BUTTON_ULTRASONICS: strTipText.LoadString(IDS_Ultrasonics);
		break;
	case ID_BUTTON_HEADER_FOOTER: strTipText.LoadString(IDS_Header_Footer);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CReportPrintDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_POINTER))>=0)
		m_nEditOption==ID_BUTTON_POINTER ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_LINE))>=0)
		m_nEditOption==ID_BUTTON_LINE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_TEXT))>=0)
		m_nEditOption==ID_BUTTON_TEXT ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_ERASE))>=0)
		m_nEditOption==ID_BUTTON_ERASE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_RECTANGLE))>=0)
		m_nEditOption==ID_BUTTON_RECTANGLE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_IMAGE))>=0)
		m_nEditOption==ID_BUTTON_IMAGE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_DATE))>=0)
		m_nEditOption==ID_BUTTON_DATE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_MOVE))>=0)
		m_nEditOption==ID_BUTTON_MOVE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_LIST))>=0)
		m_nEditOption==ID_BUTTON_LIST ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_BARGRAPH))>=0)
		m_nEditOption==ID_BUTTON_BARGRAPH ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_VARIABLE))>=0)
		m_nEditOption==ID_BUTTON_VARIABLE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_LANDSCAPE))>=0)
		m_nOrientation==DMORIENT_LANDSCAPE ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPrintToolBar.CommandToIndex(ID_BUTTON_PORTRAIT))>=0)
		m_nOrientation==DMORIENT_PORTRAIT ? m_wndPrintToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPrintToolBar.SetButtonStyle(nIndex,0);

}


void CReportPrintDlg::OnButtonBargraph() 
{
	m_nEditOption=ID_BUTTON_BARGRAPH;
	SetToolBarCheckedState();	
}

void CReportPrintDlg::OnButtonDate() 
{
	m_nEditOption=ID_BUTTON_DATE;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonErase() 
{
	m_nEditOption=ID_BUTTON_ERASE;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonImage() 
{
	m_nEditOption=ID_BUTTON_IMAGE;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonLine() 
{
	m_nEditOption=ID_BUTTON_LINE;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonList() 
{
	m_nEditOption=ID_BUTTON_LIST;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonMove() 
{
	m_nEditOption=ID_BUTTON_MOVE;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonPointer() 
{
	m_nEditOption=ID_BUTTON_POINTER;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonRectangle() 
{
	m_nEditOption=ID_BUTTON_RECTANGLE;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonRectangleSelection() 
{
	m_nEditOption=ID_BUTTON_RECTANGLE_SELECTION;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonText() 
{
	m_nEditOption=ID_BUTTON_TEXT;
	SetToolBarCheckedState();	
	
}

void CReportPrintDlg::OnButtonVariable() 
{
	m_nEditOption=ID_BUTTON_VARIABLE;
	SetToolBarCheckedState();	
	
}


void CReportPrintDlg::DrawComponentDetailsList(CPaintDC *pDC, FeatureStruct *pFeature, int nDevice)
{
	
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	int	nn;

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s"),*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	for(nn=0;nn<theApp.m_nDescriptionL;nn++) {
		switch(theApp.m_pDescription[nn].nCtrlType) {
		default:
			Buff = *theApp.m_pDescription[nn].pCaption + m_pData->m_Details[nn];
			break;
		case 1:
			break;
		}
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;
}


void CReportPrintDlg::WorldToClient(CRect rr,float fX, float fY,CPoint *pt)
{
	
	float fWidth = m_pData->m_fFastIncrement * (float)m_pViewPage->m_prrSamples->Width();
	float fHeight = m_pData->m_fSlowIncrement * (float)m_pViewPage->m_prrSamples->Height();

	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		pt->x = rr.left + (int)((fX - ((float)m_pViewPage->m_prrSamples->left * m_pData->m_fFastIncrement)) * (float)(rr.Width()-1) / fWidth);
		pt->y = rr.top  + (int)((fY - ((float)m_pViewPage->m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(rr.Height()-1) / fHeight);
		break;
	case 1:
		pt->x = rr.left + (int)((fX - ((float)m_pViewPage->m_prrSamples->left * m_pData->m_fFastIncrement)) * (float)(rr.Width()-1) / fWidth);
		pt->y =rr.bottom - (int)((fY - ((float)m_pViewPage->m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(rr.Height()-1) / fHeight);
		break;
	case 2:
		pt->x = rr.right - (int)((fX - ((float)m_pViewPage->m_prrSamples->left * m_pData->m_fFastIncrement)) * (float)(rr.Width()-1) / fWidth);
		pt->y = rr.top  + (int)((fY - ((float)m_pViewPage->m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(rr.Height()-1) / fHeight);
		break;
	case 3:
		pt->x = rr.right - (int)((fX - ((float)m_pViewPage->m_prrSamples->left * m_pData->m_fFastIncrement)) * (float)(rr.Width()-1) / fWidth);
		pt->y =rr.bottom - (int)((fY - ((float)m_pViewPage->m_prrSamples->top * m_pData->m_fSlowIncrement)) * (float)(rr.Height()-1) / fHeight);
		break;
	}
}

void CReportPrintDlg::AlicePage0()
{
	CString Buff;
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(10,10);

//	AddFeature(REPORT_RECTANGLE, rr.TopLeft(),FALSE);
//	pFeature[m_nFeaturePtr].nLineWidth = 2;
//	MMtoCoords(0,0,281,192,&rr);
//	pFeature[m_nFeaturePtr].rr = rr;
//	pFeature[m_nFeaturePtr].rgbLine = RGB(0,0,0);

	//Horizontal Lines
	AddLine(26,0,26,192,2,RGB(0,0,0));

	AddText(L"GENERALITAT VALENCIANA",0.0f,10.0f,8,RGB(0,0,0),3,L"Arial Narrow");
	AddText(L"C",5.0f,10.0f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"O",5.0f,12.3f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"N",5.0f,14.6f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"S",5.0f,16.9f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"E",5.0f,19.2f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"L",5.0f,21.5f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"L",5.0f,23.8f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"L",5.0f,26.1f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"E",5.0f,28.4f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"R",5.0f,30.7f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"I",5.0f,33.3f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"A",5.0f,35.3f,4,RGB(0,0,0),3,_T("Arial"));

	AddText(L"D",5.0f,37.0f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"'",5.0f,39.3f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"E",5.0f,41.6f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"D",5.0f,43.9f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"U",5.0f,46.2f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"C",5.0f,48.5f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"A",5.0f,50.8f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"C",5.0f,53.1f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"I",5.0f,55.4f,4,RGB(0,0,0),3,_T("Arial"));
	AddText(L"\x0d3",5.0f,57.7f,4,RGB(0,0,0),3,_T("Arial"));

}

void CReportPrintDlg::SetOrientation(int nOrientation) 
{

	CString testPrinter = m_pPrintDlg->GetDeviceName();
	DEVMODE   devmodeIn    = {0};                    
	PDEVMODE  pDevmodeWork = NULL;              

	CPrinterUtils PrinterUtils;

    if(!(pDevmodeWork = PrinterUtils.ChangePrinterProperties(testPrinter, &devmodeIn, NULL))) {
         TRACE(_T("Can't get printer settings\r\n"));
         return;
    }
        

    if( pDevmodeWork->dmFields & DM_ORIENTATION )
    {
        pDevmodeWork->dmOrientation = nOrientation;
        pDevmodeWork->dmFields |= DM_ORIENTATION;
    }



    if(!PrinterUtils.SetPrinterSetting(testPrinter, *pDevmodeWork) ) {
		TRACE(_T("Failed to Set Printer Setting Ok\r\n"));
    }


    ::free(pDevmodeWork);

	m_dcPrinter.Detach();
	SAFE_DELETE( m_pPrintDlg );
	m_pPrintDlg = new CPrintDialog(FALSE);
	m_pPrintDlg->GetDefaults();
	m_hdcPrinter = m_pPrintDlg->GetPrinterDC();
	if(m_hdcPrinter == NULL) {
		MessageBox(_T("No printer Drivers installed"));
	} else {
		m_dcPrinter.Attach(m_hdcPrinter);
	}

	GetPaperStatistics();
	SetAspectRatio();
	SetToolBarCheckedState();
    return;
	
}

void CReportPrintDlg::OnButtonLandscape() 
{
	SetOrientation( DMORIENT_LANDSCAPE );
	
}

void CReportPrintDlg::OnButtonPortrait() 
{
	SetOrientation( DMORIENT_PORTRAIT );
	
}

void CReportPrintDlg::DrawAnnotation(CPaintDC *pDC,CRect rr, FeatureStruct *pFeature, int nDevice)
{
	
	int	ii;
	CRect rectBorder=pFeature->rectClient;
	CString	Buff,Temp;
	CPoint pt;
	LOGFONT lf;
	int nn;
	CSize size;

	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	if(pViewSheet->m_bRenderAnnotation!=TRUE) return;

	for(ii=0;ii<m_pData->m_nAnnotationL;ii++) {
		CFont* pFont = (CFont *) new CFont;

		lf = m_pData->m_pAnnotation[ii].lf;
		switch(nDevice) {
		case 0: lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature->rectView.Width()) / 4;
			break;
		case 1: lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature->rectView.Width()) / 4;
			break;
		}
		MinMax(&theApp.m_LastSettings.nReportAnnotationMultiple,1,6);
		lf.lfHeight *= theApp.m_LastSettings.nReportAnnotationMultiple;


		pFont->CreateFontIndirect(&lf);
		CFont* pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(m_pData->m_pAnnotation[ii].rgbTextColor);
		pDC->SetBkMode(TRANSPARENT);

		WorldToClient(rectBorder,m_pData->m_pAnnotation[ii].fXp,m_pData->m_pAnnotation[ii].fYp, &pt);
		Temp.Format(L"%s",m_pData->m_pAnnotation[ii].Text);
		Buff.Empty();
		for(nn=0;nn<Temp.GetLength(); nn++) {
			if(Temp.GetAt(nn) != '\r') {
				Buff += Temp.GetAt(nn);
			} else {
				if(rectBorder.PtInRect(pt)) {
					pDC->TextOut(pt.x,pt.y,Buff);
				}
				size = pDC->GetTextExtent(Buff);
				pt.y += size.cy;
				Buff.Empty();
			}
		}
		if(Buff.GetLength()>0) {
			if(rectBorder.PtInRect(pt)) {
				pDC->TextOut(pt.x,pt.y,Buff);
			}
		}

		pDC->SelectObject(pOldFont);
		delete pFont;
	}
}


void CReportPrintDlg::DrawCAD(CPaintDC *pDC, CRect rr, FeatureStruct *pFeature, int nDevice)
{
	
	int	ii;
	CRect rectBorder=pFeature->rectClient;
	CString	Buff;
	CPoint pt;
	LOGFONT lf;

	CViewSheet* pViewSheet = (CViewSheet*)m_pParent;
	CViewPage* pViewPage = (CViewPage*)pViewSheet->GetActivePage();

	for(ii=0;ii<m_pData->m_nAnnotationL;ii++) {
		CFont* pFont = (CFont *) new CFont;

		lf = pFeature->lf;
		lf.lfHeight = MulDiv(lf.lfHeight,rr.Width(),pFeature[ii].rectView.Width());

		pFont->CreateFontIndirect(&lf);
		CFont* pOldFont = pDC->SelectObject(pFont);
		pDC->SetTextColor(m_pData->m_pAnnotation[ii].rgbTextColor);
		pDC->SetBkMode(TRANSPARENT);

		WorldToClient(rectBorder,m_pData->m_pAnnotation[ii].fXp,m_pData->m_pAnnotation[ii].fYp, &pt);

		Buff.Format(_T("%s "),m_pData->m_pAnnotation[ii].Text);
		pDC->TextOut(pt.x,pt.y,Buff);
		pDC->SelectObject(pOldFont);
		delete pFont;
	}
}

void CReportPrintDlg::OnReport11() 
{
	
	int ii;
	int nX=0;
	CSize size;
	CDC* pDC = GetDC();
	CRect rr;
	CString Buff;
	CPoint pt;
	float fXSize, fYSize, fStep;
	int nTextStep;
	double ImageHWRatio;

	if(m_pViewPage) m_pViewPage->GetGraticuleSize(&fXSize, &fYSize, &fStep, &nTextStep,theApp.m_fUnits);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	nX = (rr.left + rr.right) / 2;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].nType == REPORT_IMAGE) {
			ImageHWRatio = fYSize/fXSize;

			if(m_nOrientation==DMORIENT_LANDSCAPE) {
				pFeature[ii].rr.bottom = (int)(((double)pFeature[ii].rr.Width()*ImageHWRatio) * 1.6) + pFeature[ii].rr.top;
			} else {
				pFeature[ii].rr.bottom = (int)((double)pFeature[ii].rr.Width()*ImageHWRatio) + pFeature[ii].rr.top;
			}
			
			if(pFeature[ii].rr.Width()%4) {
				pFeature[ii].rr.right=(pFeature[ii].rr.right-pFeature[ii].rr.left)/4+pFeature[ii].rr.left;
				pFeature[ii].rr.right=(pFeature[ii].rr.right-pFeature[ii].rr.left)*4+pFeature[ii].rr.left;
			};

		}
	}
	Invalidate(FALSE);
}

void CReportPrintDlg::OnTemplateExporttoword() 
{
	CRect rr;
	CDC  dcMem;
	CBitmap bmpScreen;

	m_staticView.GetWindowRect(rr);
	rr.DeflateRect(1,1);
	ScreenToClient(rr);
	CDC *pDCScreen = GetDC();

	bmpScreen.CreateCompatibleBitmap(pDCScreen,rr.Width(),rr.Height());
	dcMem.CreateCompatibleDC(pDCScreen);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmpScreen);

	dcMem.BitBlt(0,0,rr.Width(),rr.Height(),pDCScreen,rr.left,rr.top,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


	if ( OpenClipboard() )
	{
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,bmpScreen.m_hObject);
		CloseClipboard();
	}

	ReleaseDC(pDCScreen);
	
}

void CReportPrintDlg::OnReportScalex1() 
{
	OnReportScale(1.0f);
}

void CReportPrintDlg::OnReportScale12() 
{
	OnReportScale(0.5f);
}

void CReportPrintDlg::OnReportScale14() 
{
	OnReportScale(0.25f);
}


void CReportPrintDlg::OnReportScale(float fScale)
{
	
	int ii;
	CPoint ptCursor;
	float fLeft,fRight,fTop,fBottom,fXSize,fYSize,fStep;
	int nTextStep;
	int nCursorLine,nCursorSample;
	float fXCursor,fYCursor;

	if(m_pViewPage == NULL) return;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_IMAGE) && (pFeature[ii].nPageNumber == m_nPageNumber)) {

			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,SCREEN);

			PaperToPrinterMM(&pFeature[ii].rr,&fTop,&fBottom,&fLeft,&fRight);

			ptCursor = m_ptClient;
			ScreenToClient(&ptCursor);
			ClientToPaper(&ptCursor);
			PaperToPrinterMM(&ptCursor,&fXCursor,&fYCursor);

			switch(theApp.m_LastSettings.nImageOrientation) {
			case 0:
				nCursorSample = MulDiv((ptCursor.x - pFeature[ii].rr.left),m_pViewPage->m_prrSamples->Width(),pFeature[ii].rr.Width()) + m_pViewPage->m_prrSamples->left;
				nCursorLine = MulDiv((ptCursor.y - pFeature[ii].rr.top),m_pViewPage->m_prrSamples->Height(),pFeature[ii].rr.Height()) + m_pViewPage->m_prrSamples->top;
				m_pViewPage->m_prrSamples->left = nCursorSample - (int)(((fXCursor - fLeft) / m_pViewPage->m_pData->m_fFastIncrement) / fScale);
				m_pViewPage->m_prrSamples->top = nCursorLine - (int)(((fYCursor - fTop) / m_pViewPage->m_pData->m_fSlowIncrement) / fScale);

				MinMax(&m_pViewPage->m_prrSamples->left, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->top, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				m_pViewPage->m_prrSamples->right = m_pViewPage->m_prrSamples->left + (int)(((fRight - fLeft) / m_pViewPage->m_pData->m_fFastIncrement) / fScale);
				m_pViewPage->m_prrSamples->bottom = m_pViewPage->m_prrSamples->top + (int)(((fBottom - fTop) / m_pViewPage->m_pData->m_fSlowIncrement) / fScale);

				if(m_pViewPage->m_prrSamples->right >= m_pViewPage->m_pData->m_nSamplesLine) {
					MinMax(&m_pViewPage->m_prrSamples->right, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
					m_pViewPage->GetGraticuleSize(&fXSize, &fYSize, &fStep, &nTextStep,theApp.m_fUnits);
					pFeature[ii].rr.right = pFeature[ii].rr.left + (int)(((fXSize * fScale) / m_fPrintingAreaWidthMM) * 10000.0f);
				}
				if(m_pViewPage->m_prrSamples->bottom >= m_pViewPage->m_pData->m_nNumberLines) {
					MinMax(&m_pViewPage->m_prrSamples->bottom, 0, m_pViewPage->m_pData->m_nNumberLines-1);
					m_pViewPage->GetGraticuleSize(&fXSize, &fYSize, &fStep, &nTextStep,theApp.m_fUnits);
					pFeature[ii].rr.bottom = pFeature[ii].rr.top + (int)(((fYSize * fScale) / m_fPrintingAreaHeightMM) * 10000.0f);
				}
				break;
			case 1:
				nCursorSample = MulDiv((ptCursor.x - pFeature[ii].rr.left),m_pViewPage->m_prrSamples->Width(),pFeature[ii].rr.Width()) + m_pViewPage->m_prrSamples->left;
				nCursorLine = MulDiv((pFeature[ii].rr.bottom - ptCursor.y),m_pViewPage->m_prrSamples->Height(),pFeature[ii].rr.Height()) + m_pViewPage->m_prrSamples->top;
				m_pViewPage->m_prrSamples->left = nCursorSample - (int)(((fXCursor - fLeft) / m_pViewPage->m_pData->m_fFastIncrement) / fScale);
				m_pViewPage->m_prrSamples->bottom = nCursorLine + (int)(((fBottom - fYCursor) / m_pViewPage->m_pData->m_fSlowIncrement) / fScale);
				MinMax(&m_pViewPage->m_prrSamples->left, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->bottom, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				m_pViewPage->m_prrSamples->right = m_pViewPage->m_prrSamples->left + (int)(((fRight - fLeft) / m_pViewPage->m_pData->m_fFastIncrement) / fScale);
				m_pViewPage->m_prrSamples->top = m_pViewPage->m_prrSamples->bottom - (int)(((fBottom - fTop) / m_pViewPage->m_pData->m_fSlowIncrement) / fScale);
				if(m_pViewPage->m_prrSamples->right >= m_pViewPage->m_pData->m_nSamplesLine) {
					MinMax(&m_pViewPage->m_prrSamples->right, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
					m_pViewPage->GetGraticuleSize(&fXSize, &fYSize, &fStep, &nTextStep,theApp.m_fUnits);
					pFeature[ii].rr.right = pFeature[ii].rr.left + (int)(((fXSize * fScale) / m_fPrintingAreaWidthMM) * 10000.0f);
				}
				if(m_pViewPage->m_prrSamples->top < 0) {
					MinMax(&m_pViewPage->m_prrSamples->top, 0, m_pViewPage->m_pData->m_nNumberLines-1);
					m_pViewPage->GetGraticuleSize(&fXSize, &fYSize, &fStep, &nTextStep,theApp.m_fUnits);
					pFeature[ii].rr.bottom = pFeature[ii].rr.top + (int)(((fYSize * fScale) / m_fPrintingAreaHeightMM) * 10000.0f);
				}
				break;
			}
		}
	}
	m_pViewPage->m_bRenderView=TRUE;
	Invalidate(FALSE);	
}

void CReportPrintDlg::OnReportCenterpoint() 
{
	
	int ii;
	CPoint ptCursor;
	float fLeft,fRight,fTop,fBottom;
	int nCursorLine,nCursorSample;
	float fXCursor,fYCursor;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].nType == REPORT_IMAGE) {

			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,SCREEN);

			PaperToPrinterMM(&pFeature[ii].rr,&fTop,&fBottom,&fLeft,&fRight);

			ptCursor = m_ptClient;
			ScreenToClient(&ptCursor);
			ClientToPaper(&ptCursor);
			PaperToPrinterMM(&ptCursor,&fXCursor,&fYCursor);
			int nSamplesWidth = m_pViewPage->m_prrSamples->Width();
			int	nLinesHeight = m_pViewPage->m_prrSamples->Height();

			switch(theApp.m_LastSettings.nImageOrientation) {
			case 0:
				nCursorSample = MulDiv((ptCursor.x - pFeature[ii].rr.left),nSamplesWidth,pFeature[ii].rr.Width()) + m_pViewPage->m_prrSamples->left;
				nCursorLine = MulDiv((ptCursor.y - pFeature[ii].rr.top),nLinesHeight,pFeature[ii].rr.Height()) + m_pViewPage->m_prrSamples->top;
				m_pViewPage->m_prrSamples->left = nCursorSample - (nSamplesWidth/2);
				m_pViewPage->m_prrSamples->top = nCursorLine - (nLinesHeight/2);
				MinMax(&m_pViewPage->m_prrSamples->left, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->top, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				m_pViewPage->m_prrSamples->right = m_pViewPage->m_prrSamples->left + nSamplesWidth;
				m_pViewPage->m_prrSamples->bottom = m_pViewPage->m_prrSamples->top + nLinesHeight;
				MinMax(&m_pViewPage->m_prrSamples->right, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->bottom, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				MinMax(&m_pViewPage->m_prrSamples->left, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->top, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				break;
			case 1:
				nCursorSample = MulDiv((ptCursor.x - pFeature[ii].rr.left),nSamplesWidth,pFeature[ii].rr.Width()) + m_pViewPage->m_prrSamples->left;
				nCursorLine = MulDiv((pFeature[ii].rr.bottom - ptCursor.y),nLinesHeight,pFeature[ii].rr.Height()) + m_pViewPage->m_prrSamples->top;
				m_pViewPage->m_prrSamples->left = nCursorSample - (nSamplesWidth/2);
				m_pViewPage->m_prrSamples->top = nCursorLine - (nLinesHeight/2);
				MinMax(&m_pViewPage->m_prrSamples->left, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->top, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				m_pViewPage->m_prrSamples->right = m_pViewPage->m_prrSamples->left + nSamplesWidth;
				m_pViewPage->m_prrSamples->bottom = m_pViewPage->m_prrSamples->top + nLinesHeight;
				MinMax(&m_pViewPage->m_prrSamples->right, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->bottom, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				MinMax(&m_pViewPage->m_prrSamples->left, 0, m_pViewPage->m_pData->m_nSamplesLine-1);
				MinMax(&m_pViewPage->m_prrSamples->top, 0, m_pViewPage->m_pData->m_nNumberLines-1);
				break;
			}
		}
	}
	m_pViewPage->m_bRenderView=TRUE;
	Invalidate(FALSE);	
}



void CReportPrintDlg::OnReportOrigionalimage() 
{
	

	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
	case 1:

		m_pViewPage->m_prrSamples->left=0;
		m_pViewPage->m_prrSamples->top=0;
		m_pViewPage->m_prrSamples->right = m_pViewPage->m_pData->m_nSamplesLine-1;
		m_pViewPage->m_prrSamples->bottom = m_pViewPage->m_pData->m_nNumberLines-1;
		break;
	}

	m_pViewPage->m_bRenderView=TRUE;
	Invalidate(FALSE);	
	
}


void CReportPrintDlg::DrawCADElementList(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice)
{
	
	CString Buff,Temp;
	CPoint pt;
	CSize size;
	int	nn;
	int nColumnPos[20];
	int	xx,ii;

	if(m_pData->IsCADElement() == false) return;
	CStringArray *Str = (CStringArray *) new CStringArray[m_pData->m_nCADElementL + 1];
	ZeroMemory(nColumnPos,sizeof nColumnPos);

	pt = pFeature->rr.TopLeft();
	PaperToDevice(&pt,nDevice);
	pFeature->rectClient.TopLeft() = pt;
	pFeature->rectClient.BottomRight() = pt;
	if(pFeature->pCaption->IsEmpty()==FALSE) {
		Buff.Format(_T("%s"),*pFeature->pCaption);
		pDC->TextOut(pt.x,pt.y,Buff);
		size = pDC->GetTextExtent(Buff);
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
		if((pt.x + size.cx) > pFeature->rectClient.right) pFeature->rectClient.right = pt.x + size.cx;
	}

	Buff.LoadString(IDS_Title);			Str[0].Add(Buff);
	Buff.LoadString(IDS_Length);		Str[0].Add(Buff);
	Buff.LoadString(IDS_Width);			Str[0].Add(Buff);
	Buff.LoadString(IDS_Height);		Str[0].Add(Buff);
	Buff.LoadString(IDS_Area);			Str[0].Add(Buff);
	Buff.LoadString(IDS_Ref_Level);		Str[0].Add(Buff);
	Buff.LoadString(IDS_Defect_Level);	Str[0].Add(Buff);
	Buff.LoadString(IDS_Defect_Area);	Str[0].Add(Buff);
	Buff.LoadString(IDS_Defect_Width);	Str[0].Add(Buff);
	Buff.LoadString(IDS_Defect_Height);	Str[0].Add(Buff);


	for(nn=0;nn<m_pData->m_nCADElementL;nn++) {
		Buff.Format(_T("%s"),m_pData->m_pCADElement[nn].m_wText);
		Str[nn+1].Add(Buff);
	
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_LENGTH) {
			m_pData->GetCADElementLength(nn,&Buff);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		size = pDC->GetTextExtent(Buff);
		if(nColumnPos[2] < (xx = nColumnPos[1] + size.cx + 10)) nColumnPos[2] = xx;

		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_WIDTH) {
			m_pData->GetCADElementWidth(nn,&Buff);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_HEIGHT) {
			m_pData->GetCADElementHeight(nn,&Buff);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_TOTAL_AREA) {
			m_pData->GetCADElementArea(nn,&Buff);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_REF_LEVEL) {
			m_pData->GetHistogramRefLevel(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_LEVEL) {
			m_pData->GetHistogramDefectLevel(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_AREA) {
			m_pData->GetHistogramDefectArea(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_WIDTH) {
			m_pData->GetHistogramDefectWidth(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
		if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_HEIGHT) {
			m_pData->GetHistogramDefectHeight(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
			Str[nn+1].Add(Buff);
		} else {
			Str[nn+1].Add(L" ");
		}
	}

	nColumnPos[0] = pt.x;
	for(nn=0;nn<=m_pData->m_nCADElementL;nn++) {
		for(ii=0;ii<10;ii++) {
			size = pDC->GetTextExtent(Str[nn].GetAt(ii));
			if(nColumnPos[ii+1] < (nColumnPos[ii] + size.cx + 10)) {
				int nExtra = (nColumnPos[ii] + size.cx + 10) - nColumnPos[ii+1];

				for(int jj=ii+1;jj<11;jj++) nColumnPos[jj] += nExtra;
			}
		}
	}

	
	for(nn=0;nn<=m_pData->m_nCADElementL;nn++) {
		for(ii=0;ii<10;ii++) {
			pDC->TextOut(nColumnPos[ii],pt.y,Str[nn].GetAt(ii));
		}
		pt.y += size.cy;
		pFeature->rectClient.bottom = pt.y;
	}

	pFeature->rectClient.right = nColumnPos[10];
	pFeature->rectClient.left -= 2;
	pFeature->rectClient.top -= 2;


	SAFE_DELETE_ARRAY( Str );
}

void CReportPrintDlg::OnButtonTable() 
{
	m_nEditOption=ID_BUTTON_TABLE;
	SetToolBarCheckedState();	
}



void CReportPrintDlg::OnButtonUltrasonics() 
{
	m_nEditOption=ID_BUTTON_ULTRASONICS;
	SetToolBarCheckedState();	
}

void CReportPrintDlg::DrawModernTable(CPaintDC *pDC, FeatureStruct* pFeature, int nDevice)
{
	int nRow,nColumn,nPnY,nPnX;
	CString Buff;
	CSize size;
	CRect rrTitle;

	pDC->MoveTo(pFeature->rectClient.TopLeft());
	pDC->LineTo(pFeature->rectClient.right,pFeature->rectClient.top);
	pDC->LineTo(pFeature->rectClient.BottomRight());
	pDC->LineTo(pFeature->rectClient.left,pFeature->rectClient.bottom);
	pDC->LineTo(pFeature->rectClient.TopLeft());

	//Print rows
	for(nRow=0;nRow<=pFeature->nNumberRows;nRow++) {
		if(pFeature->unFlags & FEATURE_TITLEBAR) {
			nPnY = MulDiv(nRow+1,pFeature->rectClient.Height(),pFeature->nNumberRows + 1) + pFeature->rectClient.top;
			if(nRow==0) {
				rrTitle = pFeature->rectClient;
				rrTitle.bottom = nPnY;
				pDC->FillSolidRect(rrTitle.left+1,rrTitle.top+1,rrTitle.Width()-1,rrTitle.Height(),pFeature->rgbBkTitle);
				Buff.Format(_T("%s"),*pFeature->pCaption);
				size = pDC->GetTextExtent(Buff);
				pDC->SetTextColor(pFeature->rgbTitle);
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(pFeature->rectClient.CenterPoint().x - size.cx/2, pFeature->rectClient.top, Buff);
			}
		} else {
			nPnY = MulDiv(nRow,pFeature->rectClient.Height(),pFeature->nNumberRows) + pFeature->rectClient.top;
		}
		pDC->MoveTo(pFeature->rectClient.left,nPnY);
		pDC->LineTo(pFeature->rectClient.right,nPnY);
	}

	//Print columns
	for(nColumn=0;nColumn<=pFeature->nNumberColumns;nColumn++) {
		nPnX = MulDiv(nColumn, pFeature->rectClient.Width(),pFeature->nNumberColumns) + pFeature->rectClient.left;
		if(pFeature->unFlags & FEATURE_TITLEBAR) {
			nPnY = MulDiv(1,pFeature->rectClient.Height(),pFeature->nNumberRows + 1) + pFeature->rectClient.top;
		} else {
			nPnY = MulDiv(0,pFeature->rectClient.Height(),pFeature->nNumberRows) + pFeature->rectClient.top;
		}
		pDC->MoveTo(nPnX,nPnY);
		pDC->LineTo(nPnX,pFeature->rectClient.bottom);
	}

}

void CReportPrintDlg::OnButtonHeaderFooter() 
{
	m_nEditOption=0;

	for(int ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].nType == REPORT_HEADER_FOOTER) {
			m_pReportHeaderFooterDlg = new CReportHeaderFooterDlg(this,(CWnd**)&m_pReportHeaderFooterDlg,&pFeature[m_nFeaturePtr=ii]);
			m_pReportHeaderFooterDlg->Create(CReportHeaderFooterDlg::IDD, this);
			return;
			break;
		}
	}
	
	AddFeature(REPORT_HEADER_FOOTER,TRUE);


}


void CReportPrintDlg::DrawHeaderFooter(CPaintDC *pDC, CRect rr, int nDevice)
{
	int	ii;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if(pFeature[ii].nType == REPORT_HEADER_FOOTER) {

			if(pFeature[ii].bHeader == true) {
				pFeature[ii].rectClient=pFeature[ii].rrHeader;
				PaperToDevice(&pFeature[ii].rectClient,nDevice);
				pDC->FillRect(pFeature[ii].rectClient, &CBrush(pFeature[ii].rgbHeader));
			}
			if(pFeature[ii].bFooter == true) {
				pFeature[ii].rectClient=pFeature[ii].rrFooter;
				PaperToDevice(&pFeature[ii].rectClient,nDevice);
				pDC->FillRect(pFeature[ii].rectClient, &CBrush(pFeature[ii].rgbFooter));
			}
		}
	}

}

void CReportPrintDlg::FormatBladeSectionReport()
{
	
	CString Buff;
	CRect rr;
	CString strSide[2] = {L"Outside form", L"Inside form"};
	float fTab[6] = {5.0f,15.0f,25.0f,35.0f,45.0f,55.0f};
	CString strColumnName[6] = {L"#",L"X Pos", L"Actual", L"Min", L"Max", L"Error"};
	int nColumn,nSample;
	CCoord Cp;
	float fX,fMinThickness,fMaxThickness,fNomThickness;
	int nType,nTL;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(10,10);

	Zero();

	float fTop = 0.0f;
	float fLineSep = 4.0f;
	int	nPageNumber = 0;

	fTop = RRBladeHeader(fTop, fLineSep, nPageNumber);
	NextLine(&fTop, &nPageNumber, fLineSep);

	for(int nLine=0;nLine<m_pData->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
		
		if(m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToPrint == 1) {

			Buff.Format(L"Section %d  Y:%s  Side:%s",nLine + 1, m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetString(),strSide[theApp.m_Thread.m_nBladeSide]);
			AddText(Buff,fTop,5,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
			NextLine(&fTop,&nPageNumber,fLineSep);

			for(nColumn = 0;nColumn<6; nColumn++) {
				AddText(strColumnName[nColumn], fTop, fTab[nColumn], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
			}
			NextLine(&fTop,&nPageNumber,fLineSep);

			for(nSample=0,nTL=0;nSample<m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetSize();nSample++) {
				m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetCoord(nSample,&Cp);

				fX = Cp.Side0.fX - m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_Matrix.m[3][0];
				m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetNominalThickness(fX,NULL,&fNomThickness,&fMinThickness,&fMaxThickness,&nType);

				if(nType & TRAFFIC_LIGHT) {
					Buff.Format(L"%d",nTL + 1);									AddText(Buff, fTop, fTab[0], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
					Buff.Format(L"%.01f",fX);									AddText(Buff, fTop, fTab[1], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
					Buff.Format(L"%.02f",Cp.fActThickness);						AddText(Buff, fTop, fTab[2], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
					Buff.Format(L"%.02f",fMinThickness);						AddText(Buff, fTop, fTab[3], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
					Buff.Format(L"%.02f",fMaxThickness);						AddText(Buff, fTop, fTab[4], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
					Buff.Format(L"%.02f",Cp.fActThickness - fNomThickness);		AddText(Buff, fTop, fTab[5], FONT_SIZE, RGB(0,0,0), 900,NULL,nPageNumber);
					NextLine(&fTop,&nPageNumber,fLineSep);
					nTL++;
				};
			}
			NextLine(&fTop,&nPageNumber,fLineSep);
		}
		
	}

}

float CReportPrintDlg::RRBladeHeader(float fTop, float fLineSep, int nPageNumber)
{
	
	CString Buff;
	CRect rr;
	CString strSide[2] = {L"Outside form", L"Inside form"};

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(10,10);


	Buff = L"Blade Filename:";					AddText(Buff,fTop,5,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	Buff.Format(L"%s",m_pData->m_FileName);		AddText(Buff,fTop,35,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	NextLine(&fTop,&nPageNumber,fLineSep);

	Buff = L"Profile Filename:";				AddText(Buff,fTop,5,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	Buff.Format(L"%s",m_pData->m_ProfileName);	AddText(Buff,fTop,35,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	NextLine(&fTop,&nPageNumber,fLineSep);

	Buff = L"Inspection Side:";					AddText(Buff,fTop,5,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
												AddText(strSide[theApp.m_Thread.m_nBladeSide],fTop,35,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	NextLine(&fTop,&nPageNumber,fLineSep);

	Buff = L"Inspection Date:";					AddText(Buff,fTop,5,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	Buff.Format(L"%s",m_pData->m_TestDateTime);	AddText(Buff,fTop,35,FONT_SIZE,RGB(0,0,0),900,NULL,nPageNumber);
	NextLine(&fTop,&nPageNumber,fLineSep);

	return fTop;

}

void CReportPrintDlg::FormatBladePlanReport()
{
	
	CString Buff;
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(10,10);

	Zero();

	float fTop = 0.0f;
	float fLineSep = 4.0f;
	int	nPageNumber = 0;
	float fLeft = 5.0;
	float fRight = m_fPrintingAreaWidthMM - 5.0f;
	float fBottom = m_fPrintingAreaHeightMM - fTop - 5.0f;

	fTop = RRBladeHeader(fTop, fLineSep, nPageNumber);
	NextLine(&fTop, &nPageNumber, fLineSep);


	AddFeature(REPORT_BLADE_PLAN, rr.TopLeft(),FALSE);
	MMtoCoords(fTop,fLeft,fBottom,fRight,&rr);
	pFeature[m_nFeaturePtr].rr = rr;


}

void CReportPrintDlg::DrawBladePlan(CPaintDC *pDC, CRect rr, int nDevice)
{
	
	int	ii;
	CRect rectBorder;
	CViewBladeThickneessDlg *pViewBlade;

	for(ii=0;ii<m_nFeatureL;ii++) {
		if((pFeature[ii].nType == REPORT_BLADE_PLAN) &&(m_nPageNumber == pFeature[ii].nPageNumber)) {
			pFeature[ii].rectClient=pFeature[ii].rr;
			PaperToDevice(&pFeature[ii].rectClient,nDevice);
			if(pFeature[ii].rectClient.left>pFeature[ii].rectClient.right) pFeature[ii].rectClient.left=pFeature[ii].rectClient.right;
			if(pFeature[ii].rectClient.top>pFeature[ii].rectClient.bottom) pFeature[ii].rectClient.top=pFeature[ii].rectClient.bottom;

			
			if(pFeature[ii].rectClient.Width()%4) {
				pFeature[ii].rectClient.right=(pFeature[ii].rectClient.right-pFeature[ii].rectClient.left)/4+pFeature[ii].rectClient.left;
				pFeature[ii].rectClient.right=(pFeature[ii].rectClient.right-pFeature[ii].rectClient.left)*4+pFeature[ii].rectClient.left;
			};

			if(m_pParent) {
				pViewBlade = (CViewBladeThickneessDlg*)m_pParent;
				pViewBlade->PrintCallBack(pDC,&pFeature[ii].rectClient,nDevice);
			}
		}
	}

}

void CReportPrintDlg::OnChangeEditPageNumber()
{
	
	CString	Buff;
	int nTemp = m_nPageNumber;

	CEdit* pEdit = (CEdit* ) m_wndPageNumberDlgBar.GetDlgItem(IDC_EDIT_PAGE_NUMBER);
	pEdit->GetWindowText(Buff);

	m_nPageNumber = _ttoi(Buff) - 1;
	MinMax(&m_nPageNumber,0,20);

	if(nTemp - m_nPageNumber ) {

		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndPageNumberDlgBar.GetDlgItem(IDC_SPIN_PAGE_NUMBER);
		pSpin->SetPos(m_nPageNumber);
		Invalidate(false);
	}

}

void CReportPrintDlg::OnDeltaposSpinPageNumber(NMHDR* pNMHDR, LRESULT* pResult)
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTemp = m_nPageNumber;

	m_nPageNumber += pNMUpDown->iDelta;
	MinMax(&m_nPageNumber,0,20);

	if(nTemp - m_nPageNumber) {
		Buff.Format(_T("%d"),m_nPageNumber + 1);
		CEdit* pEdit = (CEdit* ) m_wndPageNumberDlgBar.GetDlgItem(IDC_EDIT_PAGE_NUMBER);
		pEdit->SetWindowText(Buff);
		Invalidate(false);

	}
	*pResult = 0;
}

void CReportPrintDlg::CheckPageNumber(int *pnPageNumber, float *pfTop)
{
	CString Buff;

	if(*pfTop > m_fPrintingAreaHeightMM) {
		Buff.Format(L"- Page %d -",*pnPageNumber + 1);
		AddText(Buff,m_fPrintingAreaHeightMM - 4.0f,m_fPrintingAreaWidthMM - 20.0f,FONT_SIZE,RGB(0,0,0),900,NULL,*pnPageNumber);

		*pfTop = 0.0f;
		*pnPageNumber = *pnPageNumber + 1;
	}
}

void CReportPrintDlg::NextLine(float *pfTop, int *pnPageNumber, float fLineSep)
{
	CString Buff;

	*pfTop = *pfTop + fLineSep;
	if(*pfTop > (m_fPrintingAreaHeightMM - fLineSep)) {
		if(*pnPageNumber == 0) {
			Buff.Format(L"- Page %d -",*pnPageNumber + 1);
			AddText(Buff,m_fPrintingAreaHeightMM - 8.0f,m_fPrintingAreaWidthMM - 30.0f,FONT_SIZE,RGB(0,0,0),900,NULL,*pnPageNumber);
		}

		*pfTop = 0.0f;
		*pnPageNumber = *pnPageNumber + 1;

		Buff.Format(L"- Page %d -",*pnPageNumber + 1);
		AddText(Buff,m_fPrintingAreaHeightMM - 8.0f,m_fPrintingAreaWidthMM - 30.0f,FONT_SIZE,RGB(0,0,0),900,NULL,*pnPageNumber);
	}

}

void CReportPrintDlg::OnAnnotationX1() 
{
	
	
	theApp.m_LastSettings.nReportAnnotationMultiple = 1;
	Invalidate(FALSE);
}

void CReportPrintDlg::OnAnnotationX2() 
{
	
	
	theApp.m_LastSettings.nReportAnnotationMultiple = 2;
	Invalidate(FALSE);
}

void CReportPrintDlg::OnAnnotationX4() 
{
	
	
	theApp.m_LastSettings.nReportAnnotationMultiple = 4;
	Invalidate(FALSE);
}

void CReportPrintDlg::OnAnnotationX6() 
{
	
	
	theApp.m_LastSettings.nReportAnnotationMultiple = 6;
	Invalidate(FALSE);

}
