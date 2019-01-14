// ThirdPartyImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ThirdPartyImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThirdPartyImageDlg dialog


CThirdPartyImageDlg::CThirdPartyImageDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CString FilePath)
	: CDialog(CThirdPartyImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThirdPartyImageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CUSLFile File;
	CFileException e;

	m_pDlg = pDlg;
	
	m_pcBuffer = NULL;
	m_FilePath = FilePath;
	m_nFileLength=0;
	m_nCoordL = 5000;
	m_Data.Zero();
	m_Data.m_nNumberAmpImages[0] = 1;
	m_Data.m_nNumberImages = 1;
	m_Data.m_nNumberTimeSlots = 1;
	m_Data.m_nNumberLines = theApp.m_LastSettings.nThirdNumberLines;
	m_Data.m_nSamplesLine = theApp.m_LastSettings.nThirdSamplesLine;

	if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {

	} else {
		m_nFileLength = (int)File.GetLength();
		m_pcBuffer = new unsigned char[m_nFileLength];
		File.Read(m_pcBuffer,m_nFileLength);
		File.Close();
	}

}


void CThirdPartyImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThirdPartyImageDlg)
	DDX_Control(pDX, IDC_LIST_DECODE, m_listDecode);
	DDX_Control(pDX, IDC_COMBO_SIZEOF, m_comboSizeOf);
	DDX_Control(pDX, IDC_EDIT_SEARCH_FOR, m_editSearchFor);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_editResult);
	DDX_Control(pDX, IDC_SPIN_STARTOFFSET, m_spinStartOffset);
	DDX_Control(pDX, IDC_SPIN_NUMBERLINES, m_spinNumberLines);
	DDX_Control(pDX, IDC_SPIN_SAMPLESLINE, m_spinSamplesLine);
	DDX_Control(pDX, IDC_EDIT_STARTOFFSET, m_editStartOffset);
	DDX_Control(pDX, IDC_EDIT_SAMPLESLINE, m_editSamplesLine);
	DDX_Control(pDX, IDC_EDIT_NUMBERLINES, m_editNumberLines);
	DDX_Control(pDX, IDC_EDIT_MIN_VALUE, m_editMinValue);
	DDX_Control(pDX, IDC_EDIT_MAX_VALUE, m_editMaxValue);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
}


BEGIN_MESSAGE_MAP(CThirdPartyImageDlg, CDialog)
	//{{AFX_MSG_MAP(CThirdPartyImageDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_SAMPLESLINE, OnChangeEditSamplesline)
	ON_EN_CHANGE(IDC_EDIT_NUMBERLINES, OnChangeEditNumberlines)
	ON_EN_CHANGE(IDC_EDIT_STARTOFFSET, OnChangeEditStartoffset)
	ON_EN_CHANGE(IDC_EDIT_MIN_VALUE, OnChangeEditMinValue)
	ON_EN_CHANGE(IDC_EDIT_MAX_VALUE, OnChangeEditMaxValue)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_FOR, OnButtonSearchFor)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DECODE, OnGetdispinfoListDecode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThirdPartyImageDlg message handlers

void CThirdPartyImageDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	
	if(m_pcBuffer) delete m_pcBuffer;
	delete this;
}

void CThirdPartyImageDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CThirdPartyImageDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CThirdPartyImageDlg::OnDestroy() 
{
	if (CThirdPartyImageDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CThirdPartyImageDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CThirdPartyImageDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	
	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	if (CThirdPartyImageDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CThirdPartyImageDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	m_StaticPosView.SetNewPos();

	m_spinSamplesLine.SetRange(0,10000);
	m_spinNumberLines.SetRange(0, 10000);
	m_spinStartOffset.SetRange(0, 10000);

	m_comboSizeOf.AddString(_T("1 Byte"));
	m_comboSizeOf.AddString(_T("WORD"));
	m_comboSizeOf.AddString(_T("int"));
	m_comboSizeOf.AddString(_T("float"));
	m_comboSizeOf.AddString(_T("double"));
	m_comboSizeOf.SetCurSel(1);

	CreateColumns();
	FillList();

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThirdPartyImageDlg::UpdateAllControls()
{
	CString Buff,Temp;
	
	if(GetSafeHwnd() == NULL) return;

	Buff.Format(_T("%d"),theApp.m_LastSettings.nThirdSamplesLine);
	m_editSamplesLine.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nThirdNumberLines);
	m_editNumberLines.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nThirdStartOffset);
	m_editStartOffset.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nThirdMin);
	m_editMinValue.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nThirdMax);
	m_editMaxValue.SetWindowText(Buff);

	m_spinSamplesLine.SetPos(theApp.m_LastSettings.nThirdSamplesLine);
	m_spinNumberLines.SetPos(theApp.m_LastSettings.nThirdNumberLines);
	m_spinStartOffset.SetPos(theApp.m_LastSettings.nThirdStartOffset);

	Buff.Empty();
	for (int ii = theApp.m_LastSettings.nThirdStartOffset, nRow = 0; nRow < 10; ii += theApp.m_LastSettings.nThirdSamplesLine, nRow++) {
		for (int jj = 0; jj < theApp.m_LastSettings.nThirdSamplesLine; jj++) {
			Temp.Format(L"%3d ", m_pcBuffer[ii + jj]);
			Buff += Temp;
		}
		Buff += L"\r\n";
	}
	m_editText.SetWindowText(Buff);
}

void CThirdPartyImageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos(); 
	if(GetSafeHwnd()) {
		Invalidate(FALSE);
	}
	
}

void CThirdPartyImageDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	CBitmap bmp;
	CDC  dcMem;
	CBitmap *pOldBitmap;
	int nColor,nOffset,nPtr,xx,yy,ii;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	COLORREF* pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	COLORREF rgbPalette[256];
	for(ii=0;ii<theApp.m_LastSettings.nThirdMin;ii++) {
		rgbPalette[ii]=RGB(0,0,0);
	}
	for(ii;(ii<=theApp.m_LastSettings.nThirdMax)&&(ii<256);ii++) {
		nColor=MulDiv(ii-theApp.m_LastSettings.nThirdMin,255,theApp.m_LastSettings.nThirdMax-theApp.m_LastSettings.nThirdMin+1);
		rgbPalette[ii]=RGB(nColor,nColor,nColor);
	}
	for(ii;ii<256;ii++) {
		rgbPalette[ii]=RGB(255,255,255);
	}
	int nStart=0;
	int nType = 1;
	switch (nType) {
	case 0:	//Byte serial
		for (yy = 0; yy < rr.Height(); yy++) {
			nOffset = theApp.m_LastSettings.nThirdStartOffset + MulDiv(yy, theApp.m_LastSettings.nThirdNumberLines, rr.Height()) * theApp.m_LastSettings.nThirdSamplesLine;
			if ((nOffset + theApp.m_LastSettings.nThirdSamplesLine) < m_nFileLength) {
				for (xx = 0; xx < rr.Width(); xx++) {
					nPtr = MulDiv(xx, theApp.m_LastSettings.nThirdSamplesLine, rr.Width());
					nColor = m_pcBuffer[nOffset + nPtr];
					pArray[yy*rr.Width() + xx] = rgbPalette[nColor];
					if ((nStart == 0) && nColor) nStart = nOffset + nPtr;
				}
			}
		}
		break;
	case 1:	//Maytech probably blocks of 32 bytes per pixel
		for (yy = 0; yy < rr.Height(); yy++) {
			nOffset = theApp.m_LastSettings.nThirdStartOffset + MulDiv(yy, theApp.m_LastSettings.nThirdNumberLines, rr.Height()) * (theApp.m_LastSettings.nThirdSamplesLine * 32);
			if ((nOffset + (theApp.m_LastSettings.nThirdSamplesLine * 32)) < m_nFileLength) {
				for (xx = 0; xx < rr.Width(); xx++) {
					nPtr = MulDiv(xx, theApp.m_LastSettings.nThirdSamplesLine, rr.Width()) * 32;
					nColor = m_pcBuffer[nOffset + nPtr];
					pArray[yy*rr.Width() + xx] = rgbPalette[nColor];
					if ((nStart == 0) && nColor) nStart = nOffset + nPtr;
				}
			}
		}
		break;
	}

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pArray);

	dcMem.CreateCompatibleDC(&dc);
	pOldBitmap = dcMem.SelectObject(&bmp);
	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;

}


void CThirdPartyImageDlg::OnChangeEditSamplesline() 
{
	CString Buff;
	int nTemp = theApp.m_LastSettings.nThirdSamplesLine;

	m_editSamplesLine.GetWindowText(Buff);
	theApp.m_LastSettings.nThirdSamplesLine = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nThirdSamplesLine) {
		m_spinSamplesLine.SetPos(theApp.m_LastSettings.nThirdSamplesLine);
		Invalidate(FALSE);
	}
}

void CThirdPartyImageDlg::OnChangeEditNumberlines() 
{
	CString Buff;

	m_editNumberLines.GetWindowText(Buff);
	theApp.m_LastSettings.nThirdNumberLines = _ttoi(Buff);
	Invalidate(FALSE);
}

void CThirdPartyImageDlg::OnChangeEditStartoffset() 
{
	CString Buff;

	m_editStartOffset.GetWindowText(Buff);
	theApp.m_LastSettings.nThirdStartOffset = _ttoi(Buff);
	Invalidate(FALSE);
	
}

void CThirdPartyImageDlg::OnChangeEditMinValue() 
{
	CString Buff;

	m_editMinValue.GetWindowText(Buff);
	theApp.m_LastSettings.nThirdMin = _ttoi(Buff);
	Invalidate(FALSE);
	
}

void CThirdPartyImageDlg::OnChangeEditMaxValue() 
{
	CString Buff;

	m_editMaxValue.GetWindowText(Buff);
	theApp.m_LastSettings.nThirdMax = _ttoi(Buff);
	Invalidate(FALSE);
	
}


void CThirdPartyImageDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_SAMPLESLINE:
		nTemp = theApp.m_LastSettings.nThirdSamplesLine;
		theApp.m_LastSettings.nThirdSamplesLine=(pSpin->GetPos()&0xffff);
		if(nTemp - theApp.m_LastSettings.nThirdSamplesLine) {
			UpdateAllControls();
			Invalidate(FALSE);
		}
		break;
	case IDC_SPIN_NUMBERLINES:
		nTemp = theApp.m_LastSettings.nThirdNumberLines;
		theApp.m_LastSettings.nThirdNumberLines = (pSpin->GetPos() & 0xffff);
		if (nTemp - theApp.m_LastSettings.nThirdNumberLines) {
			UpdateAllControls();
			Invalidate(FALSE);
		}
		break;
	case IDC_SPIN_STARTOFFSET:
		nTemp = theApp.m_LastSettings.nThirdStartOffset;
		theApp.m_LastSettings.nThirdStartOffset = (pSpin->GetPos() & 0xffff);
		if (nTemp - theApp.m_LastSettings.nThirdStartOffset) {
			UpdateAllControls();
			Invalidate(FALSE);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CThirdPartyImageDlg::OnButtonSearchFor() 
{
	int ii;
	CString Buff,Temp;

	m_editSearchFor.GetWindowText(Buff);

	int nValue = _ttoi(Buff);
	char cValue = nValue & 0xff;
	WORD wValue = nValue & 0xffff;
	float fValue;
	double dValue;
	int* nArray;
	float* fArray;
	WORD* wArray;
	double* dArray;
	int nMode = m_comboSizeOf.GetCurSel();
	_WTOF(Buff,fValue);
	_WTOD(Buff,dValue);

	Buff.Empty();

	for(ii=0;ii<theApp.m_LastSettings.nThirdStartOffset;ii++) {
		switch(nMode) {
		case 0:
			break;
		case 1: wArray = (WORD*)(m_pcBuffer + ii);
			if(wArray[0] == wValue) {
				Temp.Format(_T("%d\r\n"),ii);
				Buff += Temp;
			}
			break;
		case 2: nArray = (int*)(m_pcBuffer + ii);
			if(nArray[0] == nValue) {
				Temp.Format(_T("%d\r\n"),ii);
				Buff += Temp;
			}
			break;
		case 3: fArray = (float*)(m_pcBuffer + ii);
			if(fabs((double)(fArray[0] - fValue))<0.5) {
				Temp.Format(_T("%d\r\n"),ii);
				Buff += Temp;
			}
			break;
		case 4: dArray = (double*)(m_pcBuffer + ii);
			if(fabs(dArray[0] - fValue)<0.5) {
				Temp.Format(_T("%d\r\n"),ii);
				Buff += Temp;
			}
			break;
		}
	}
	m_editResult.SetWindowText(Buff);
}

void CThirdPartyImageDlg::OnGetdispinfoListDecode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int* nArray;
	float* fArray;
	WORD* wArray;
	double* dArray;

	pDispInfo->item.pszText=str;


	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,_T("%d"),pDispInfo->item.iItem);
		break;
	case 1: swprintf_s(str,_T("%c"),m_pcBuffer[pDispInfo->item.iItem]);
		break;
	case 2: swprintf_s(str,_T("%d"),m_pcBuffer[pDispInfo->item.iItem]);
		break;
	case 3:	wArray = (WORD*)(m_pcBuffer + pDispInfo->item.iItem);
			swprintf_s(str,_T("%d"),wArray[0]);
		break;
	case 4:	nArray = (int*)(m_pcBuffer + pDispInfo->item.iItem);
			swprintf_s(str,_T("%d"),nArray[0]);
		break;
	case 5:	fArray = (float*)(m_pcBuffer + pDispInfo->item.iItem);
			swprintf_s(str,_T("%.02f"),fArray[0]);
		break;
	case 6:	dArray = (double*)(m_pcBuffer + pDispInfo->item.iItem);
			swprintf_s(str,100,_T("%.02e"),dArray[0]);
		break;
	}
	pDispInfo->item.pszText=str;

	*pResult = 0;
}

void CThirdPartyImageDlg::CreateColumns()
{
	CString ColumnName[7] = { L"#",L"Ascii", L"char", L"Word", L"int",L"float",L"double"};
	int ColumnWidth[7] = { 40,40,40,50,60,60,60};
	int ii;

	for (ii=0;ii<7;ii++) {
		m_listDecode.InsertColumn(ii,ColumnName[ii], LVCFMT_RIGHT,ColumnWidth[ii]);
	}
	m_listDecode.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CThirdPartyImageDlg::FillList()
{
	CString Buff;
	int ii;

	for (ii=0;ii<m_nCoordL;ii++) {
		Buff.Format(_T("%d"),ii);
		m_listDecode.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listDecode.EnsureVisible(0,FALSE);
	m_listDecode.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listDecode.SetFocus();

}

