// PrinterDiagnosticsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PrinterDiagnosticsPage.h"
#include "USLPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrinterDiagnosticsPage property page

IMPLEMENT_DYNCREATE(CPrinterDiagnosticsPage, CPropertyPage)

CPrinterDiagnosticsPage::CPrinterDiagnosticsPage() : CPropertyPage(CPrinterDiagnosticsPage::IDD)
{
	//{{AFX_DATA_INIT(CPrinterDiagnosticsPage)
	m_strBytesPerPixel = _T("");
	m_strPaperSize = _T("");
	m_strPrintableArea = _T("");
	m_strResolution = _T("");
	//}}AFX_DATA_INIT
	
}

CPrinterDiagnosticsPage::~CPrinterDiagnosticsPage()
{
}

void CPrinterDiagnosticsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrinterDiagnosticsPage)
	DDX_Text(pDX, IDC_EDIT_BYTES_PER_PIXEL, m_strBytesPerPixel);
	DDX_Text(pDX, IDC_EDIT_PAPER_SIZE, m_strPaperSize);
	DDX_Text(pDX, IDC_EDIT_PRINTABLE_AREA, m_strPrintableArea);
	DDX_Text(pDX, IDC_EDIT_RESOLUTION, m_strResolution);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrinterDiagnosticsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPrinterDiagnosticsPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinterDiagnosticsPage message handlers

BOOL CPrinterDiagnosticsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPrinterDiagnosticsPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();

	return CPropertyPage::OnSetActive();
}

void CPrinterDiagnosticsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CUSLPrint m_Print;
	int nBitsPixel = 0;
	int nBytesPixel = 0;
	int	nPrintingAreaWidthMM = 0;
	int	nPrintingAreaHeightMM = 0;
	int nXRes = 0;
	int nYRes = 0;
	int nPrintingAreaWidth = 0;
	int nPrintingAreaHeight = 0;

	int	nPhysicalPageWidth = 0;
	int	nPhysicalPageHeight = 0;

	int	nPrintingAreaOffsetLeft = 0;
	int	nPrintingAreaOffsetTop = 0;


	if(1) {
		m_Print.StartDoc(L"USLScanner Image");
		nBitsPixel = m_Print.m_dc.GetDeviceCaps(BITSPIXEL);
		nBytesPixel = nBitsPixel/8;
		if(nBytesPixel<0) nBytesPixel=1;

		nPrintingAreaWidthMM = m_Print.m_dc.GetDeviceCaps(HORZSIZE);		//908 //287mm
		nPrintingAreaHeightMM = m_Print.m_dc.GetDeviceCaps(VERTSIZE);	//1213 //405mm
		nXRes = m_Print.m_dc.GetDeviceCaps(LOGPIXELSX);					//600 //300 pixels / inch
		nYRes = m_Print.m_dc.GetDeviceCaps(LOGPIXELSY);					//600 //300 pixels / inch
		nPrintingAreaWidth	= m_Print.m_dc.GetDeviceCaps(HORZRES);		//21460 //3387 Number of dots
		nPrintingAreaHeight  = m_Print.m_dc.GetDeviceCaps(VERTRES);		//28860 //4786 Number of dots

		nPhysicalPageWidth   = m_Print.m_dc.GetDeviceCaps(PHYSICALWIDTH);	//21600 //3507
		nPhysicalPageHeight  = m_Print.m_dc.GetDeviceCaps(PHYSICALHEIGHT);	//28800 //4960

		nPrintingAreaOffsetLeft	= m_Print.m_dc.GetDeviceCaps(PHYSICALOFFSETX);	//70 //60
		nPrintingAreaOffsetTop	= m_Print.m_dc.GetDeviceCaps(PHYSICALOFFSETY);	//70 //36
		m_Print.EndDoc();
	}

	m_strBytesPerPixel.Format(_T("%d"),nBytesPixel);
	m_strPrintableArea.Format(_T("Width:%.01fmm Height:%.01fmm"),(float)nPrintingAreaWidthMM,(float)nPrintingAreaHeightMM);
	m_strPaperSize.Format(_T("Width:%.01fmm Height:%.01fmm"),(float)nPhysicalPageWidth/(float)nXRes*25.4f,(float)nPhysicalPageHeight/(float)nYRes*25.4f);
	m_strResolution.Format(_T("Width:%d Height:%d"),nXRes,nYRes);

	UpdateData(FALSE);
}
