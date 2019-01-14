// ImageProcessorPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ImageProcessorPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessorPage property page

IMPLEMENT_DYNCREATE(CImageProcessorPage, CPropertyPage)

CImageProcessorPage::CImageProcessorPage(CWnd* pParent) : CPropertyPage(CImageProcessorPage::IDD)
{
	//{{AFX_DATA_INIT(CImageProcessorPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ZeroMemory(&theApp.m_nFilter,sizeof theApp.m_nFilter);
	
	m_pArray = NULL;
	m_nRow = -1;
	m_nColumn = -1;
	m_bLButtonDblClk = false;
	m_nDest=0;
	m_nSrc=0;
	m_nImageType=0;

	ZeroMemory(m_nFilter,sizeof m_nFilter);
	int nK[7][5][5] = {
		{{-1,2,0},{0,0,0},{0,0,0}},
		{{0,-1,0},{-1,5,-1},{0,-1,0}},
		{{0,1,0},{1,-4,1},{0,1,0}},
		{{-2,-1,-0},{-1,1,1},{0,1,2}},
		{{-1,-4,-1},{-4,26,-4},{-1,-4,-1}},
		{{1,1,1},{1,5,1},{1,1,1}},
		{{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,24,-1,-1},{-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1}}
	};

	for(int ii=0;ii<7;ii++) {
		CopyMemory(m_nFilter[ii],nK[ii],sizeof nK[0]);
	}

	CopyMemory(&theApp.m_nFilter,&m_nFilter[theApp.m_LastSettings.nConvolutionFilter],sizeof theApp.m_nFilter);

	m_pParent = pParent;
	
	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	m_pData = &pSheet->m_Data;

}

CImageProcessorPage::~CImageProcessorPage()
{
	SAFE_DELETE( m_pArray );
}

void CImageProcessorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageProcessorPage)
	DDX_Control(pDX, IDC_COMBO_SRC_TYPE, m_comboSrcType);
	DDX_Control(pDX, IDC_COMBO_SRC_IMAGE, m_comboSourceImage);
	DDX_Control(pDX, IDC_COMBO_DESTINATION, m_comboDestination);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_sliderThreshold);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edit);
	DDX_Control(pDX, IDC_STATIC_FILTER, m_staticFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageProcessorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CImageProcessorPage)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_EDIT_VALUE, OnChangeEditValue)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelchangeComboPreset)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_DIFERENTIATION, OnButtonDiferentiation)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SOBEL, OnButtonSobel)
	ON_CBN_SELCHANGE(IDC_COMBO_SRC_TYPE, OnSelchangeComboSrcType)
	ON_BN_CLICKED(IDC_BUTTON_33_MEDIAN, OnButton33Median)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BUTTON_33_MEAN, OnButton33Mean)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessorPage message handlers

BOOL CImageProcessorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboPreset.AddString(_T("edge enhance"));
	m_comboPreset.AddString(_T("2"));
	m_comboPreset.AddString(_T("3"));
	m_comboPreset.AddString(_T("emboss"));
	m_comboPreset.AddString(_T("Honeycombe"));
	m_comboPreset.AddString(_T("Noise reduction"));
	m_comboPreset.AddString(_T("Leplace edge"));

	m_sliderThreshold.SetRange(0,100);
	m_sliderThreshold.SetTicFreq(0x10);

	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CImageProcessorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CImageProcessorPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	int nAmp,gg,nF;
	CString Amp;	Amp.LoadString(IDS_Amp);

	m_edit.ShowWindow(FALSE);

	m_comboPreset.SetCurSel(theApp.m_LastSettings.nConvolutionFilter);

	m_sliderThreshold.SetPos(100-theApp.m_LastSettings.nDifferentiatorThreshold);

	m_comboSourceImage.ResetContent();
	for(gg=0,nAmp=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_pData->m_cAmpBuffer[gg]!=NULL) {
			if(m_pData->m_ImageName[gg]=="") {
				Buff.Format(_T("%d"),gg+1);
			}else{
				Buff.Format(_T("%s"),m_pData->m_ImageName[gg]); 
			}
			m_comboSourceImage.AddString(Buff);
			nAmp++;
		}
	}
	m_comboSourceImage.SetCurSel(m_nSrc);

	m_comboDestination.ResetContent();
	for(gg=0,nF=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_pData->m_cFilterBuffer[gg]!=NULL) {
			Buff.Format(_T("%d"),gg+1);
			m_comboDestination.AddString(Buff);
			nF++;
		}
	}
	Buff.Format(_T("%d"),nF+1);
	m_comboDestination.AddString(Buff);
	m_comboDestination.SetCurSel(m_nDest);


	m_comboSrcType.ResetContent();
	m_comboSrcType.AddString(_T("Amp"));
	m_comboSrcType.AddString(_T("Filt"));
	m_comboSrcType.SetCurSel(m_nImageType);

}

void CImageProcessorPage::OnPaint() 
{
	CRect rr,rect;
	CPaintDC dc(this); // device context for painting
	CDC			dcMem;
	CBitmap		bmp;
	
	m_staticFilter.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(m_pArray==NULL){
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width();
	rect.bottom = rr.Height();

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	

	DrawGrid((CPaintDC*)&dcMem,rect);


	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
}


void CImageProcessorPage::DrawGrid(CDC* pDC,CRect rr)
{
	CFont HorizontalFont;
	CPen penGrey(PS_SOLID,1,RGB(100,100,100));
	CString Buff = L"999.9";
	CSize size = pDC->GetTextExtent(Buff);
	CPen* pOldPen = pDC->SelectObject(&penGrey);
	CRect rrSquare;

	int nX,nY,pnX,pnY;

	for(nX=0;nX<6;nX++) {
		pnX = MulDiv(nX,rr.Width(),5);
		pDC->MoveTo(pnX,0);
		pDC->LineTo(pnX,rr.Height());
	}

	for(nY=0;nY<6;nY++) {
		pnY = MulDiv(nY,rr.Height(),5);
		pDC->MoveTo(0,pnY);
		pDC->LineTo(rr.Width(),pnY);
	}




	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);


	int nXCentre = MulDiv(1,rr.Width(),5) / 2;
	int nYCentre = MulDiv(1,rr.Height(),5) / 2;

	Buff = "0";
	size =pDC->GetTextExtent(Buff);

	for(nY=0;nY<6;nY++) {
		pnY = MulDiv(nY,rr.Height(),5) + nYCentre;
		for(nX=0;nX<6;nX++) {
			pnX = MulDiv(nX,rr.Width(),5) + nXCentre;
			Buff.Format(_T("%d"),theApp.m_nFilter[nY][nX]);
			pDC->TextOut(pnX-size.cx/2,pnY-size.cy/2,Buff);
		}
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

}

void CImageProcessorPage::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	CPropertyPage::OnLButtonDblClk(nFlags, point);
}

void CImageProcessorPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDblClk = false;
	CRect rr,rrEdit;
	CString Buff;

	m_staticFilter.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(rr.PtInRect(point)) {
		m_bLButtonDblClk = true;

		m_nColumn = MulDiv(point.x-rr.left,4,rr.Width());
		m_nRow = MulDiv(point.y-rr.top,4,rr.Height());


		rrEdit.left = MulDiv(m_nColumn,rr.Width(),5)+rr.left;
		rrEdit.right = MulDiv(m_nColumn+1,rr.Width(),5)+rr.left;
		rrEdit.top = MulDiv(m_nRow,rr.Height(),5)+rr.top;
		rrEdit.bottom = MulDiv(m_nRow+1,rr.Height(),5)+rr.top;
		m_edit.SetWindowPos(NULL,rrEdit.left,rrEdit.top,rrEdit.Width(),rrEdit.Height(),NULL);
		Buff.Format(_T("%d"),theApp.m_nFilter[m_nRow][m_nColumn]);
		m_edit.SetWindowText(Buff);
		m_edit.ShowWindow(TRUE);
		m_edit.SetFocus();
		

		Invalidate(FALSE);
	}

	
	Invalidate(FALSE);
	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CImageProcessorPage::OnChangeEditValue() 
{
	CString Buff;

	m_edit.GetWindowText(Buff);
	theApp.m_nFilter[m_nRow][m_nColumn] = _ttoi(Buff);

}

void CImageProcessorPage::OnSelchangeComboPreset() 
{
	theApp.m_LastSettings.nConvolutionFilter = m_comboPreset.GetCurSel();
	CopyMemory(&theApp.m_nFilter,m_nFilter[theApp.m_LastSettings.nConvolutionFilter],sizeof theApp.m_nFilter);

	m_edit.ShowWindow(FALSE);

	Invalidate(FALSE);
}

void CImageProcessorPage::OnButtonApply() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nSrc = m_comboSourceImage.GetCurSel();
	m_nDest = m_comboDestination.GetCurSel();

	pSheet->CreateImageProcessPage(0,m_nImageType,m_nSrc,m_nDest);
	UpdateAllControls();
}


void CImageProcessorPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	switch(pSlider->GetDlgCtrlID()) {
	case IDC_SLIDER_THRESHOLD:
		theApp.m_LastSettings.nDifferentiatorThreshold=100-(pSlider->GetPos()&0xffff);
		break;
	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImageProcessorPage::OnButtonDiferentiation() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nSrc = m_comboSourceImage.GetCurSel();
	m_nDest = m_comboDestination.GetCurSel();

	pSheet->CreateImageProcessPage(1,m_nImageType,m_nSrc,m_nDest);
	UpdateAllControls();
}

void CImageProcessorPage::OnButtonSobel() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nSrc = m_comboSourceImage.GetCurSel();
	m_nDest = m_comboDestination.GetCurSel();

	pSheet->CreateImageProcessPage(2,m_nImageType,m_nSrc,m_nDest);
	UpdateAllControls();
}


void CImageProcessorPage::OnSelchangeComboSrcType() 
{
	m_nImageType = m_comboSrcType.GetCurSel();
}

void CImageProcessorPage::OnButton33Median() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nSrc = m_comboSourceImage.GetCurSel();
	m_nDest = m_comboDestination.GetCurSel();

	pSheet->CreateImageProcessPage(3,m_nImageType,m_nSrc,m_nDest);
	UpdateAllControls();
}

void CImageProcessorPage::OnButton33Mean() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;

	m_nSrc = m_comboSourceImage.GetCurSel();
	m_nDest = m_comboDestination.GetCurSel();

	pSheet->CreateImageProcessPage(4,m_nImageType,m_nSrc,m_nDest);
	UpdateAllControls();
}

BOOL CImageProcessorPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}
