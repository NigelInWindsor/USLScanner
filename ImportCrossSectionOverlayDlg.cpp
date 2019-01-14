// ImportCrossSectionOverlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ImportCrossSectionOverlayDlg.h"
#include "ViewThicknessCrossSectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportCrossSectionOverlayDlg dialog


CImportCrossSectionOverlayDlg::CImportCrossSectionOverlayDlg(CWnd* pParent /*=NULL*/, CWnd** pDlg)
	: CDialog(CImportCrossSectionOverlayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportCrossSectionOverlayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	
	m_pDlg = pDlg;
	m_nFormat = 0;
	m_nLineNumber = 0;
	m_nNumberLines = 0;
	m_nSamplesLine = 0;
	m_pfThickness = NULL;
	m_pfHeader = NULL;
	m_bMirror = false;
	m_nOffset = 0;
	m_nHeaderSize;
}


void CImportCrossSectionOverlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportCrossSectionOverlayDlg)
	DDX_Control(pDX, IDC_SLIDER_OVERLAY_OFFSET, m_sliderOverlayOffset);
	DDX_Control(pDX, IDC_CHECK_MIRROR, m_checkMirror);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_comboFormat);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_editFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportCrossSectionOverlayDlg, CDialog)
	//{{AFX_MSG_MAP(CImportCrossSectionOverlayDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnSelchangeComboFormat)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_MIRROR, OnCheckMirror)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportCrossSectionOverlayDlg message handlers

void CImportCrossSectionOverlayDlg::PostNcDestroy() 
{
	SAFE_DELETE ( m_pfThickness );
	SAFE_DELETE ( m_pfHeader );
	
	CDialog::PostNcDestroy();
	delete this;
}

void CImportCrossSectionOverlayDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CImportCrossSectionOverlayDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CImportCrossSectionOverlayDlg::OnDestroy() 
{
	if (CImportCrossSectionOverlayDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CImportCrossSectionOverlayDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CImportCrossSectionOverlayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;
	CString Micro = L"\x0b5s";

	if (CImportCrossSectionOverlayDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CImportCrossSectionOverlayDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	m_comboFormat.AddString(L"RR Singapore CSV");
	m_sliderOverlayOffset.SetRange(0,2000);
	m_sliderOverlayOffset.SetPos(m_nOffset + 1000);


	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportCrossSectionOverlayDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_comboFormat.SetCurSel(m_nFormat);
	m_spinLineNumber.SetRange(0,m_nNumberLines);
	m_spinLineNumber.SetPos(m_nLineNumber);
	Buff.Format(L"%d",m_nLineNumber+1);
	m_editLineNumber.SetWindowText(Buff);


}

void CImportCrossSectionOverlayDlg::OnSelchangeComboFormat() 
{
	// TODO: Add your control notification handler code here
	
}

void CImportCrossSectionOverlayDlg::OnButtonOpen() 
{
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".*"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_AptSourcePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_AptSourcePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {

			switch(m_nFormat) {
			case 0: ImportRRSingaporeCSV(&File);
				break;
			};
			File.Close();
			m_editFilePath.SetWindowText(FilePath);

			UpdateAllControls();
		}
	}
	
}



void CImportCrossSectionOverlayDlg::OnChangeEditLineNumber() 
{
	CString Buff;
	int nTemp = m_nLineNumber;
	
	m_editLineNumber.GetWindowText(Buff);
	m_nLineNumber = _ttoi(Buff) - 1;
	if(m_nLineNumber<0) m_nLineNumber=0;
	if(m_nLineNumber>=m_nNumberLines) m_nLineNumber = m_nNumberLines - 1;
	if(nTemp - m_nLineNumber) {
		m_spinLineNumber.SetPos(m_nLineNumber);
		InvalidateParent();
	}
}

void CImportCrossSectionOverlayDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_NUMBER:
		nTemp = m_nLineNumber;
		m_nLineNumber = pSpin->GetPos()&0xffff;
		if(nTemp - m_nLineNumber) {
			if(m_nLineNumber<0) m_nLineNumber=0;
			if(m_nLineNumber>=m_nNumberLines) m_nLineNumber = m_nNumberLines-1;
			Buff.Format(L"%d",m_nLineNumber+1);
			m_editLineNumber.SetWindowText(Buff);
			InvalidateParent();
		}
		break;

	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CImportCrossSectionOverlayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp;

	switch(pSlider->GetDlgCtrlID()) {
	case IDC_SLIDER_OVERLAY_OFFSET:
		nTemp = m_nOffset;
		m_nOffset=(pSlider->GetPos()&0xffff) - 1000;
		if(nTemp - m_nOffset) {
			InvalidateParent();
		}
		break;

	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

bool CImportCrossSectionOverlayDlg::ImportRRSingaporeCSV(CFile *pFile)
{
	CString Buff,Temp;
	char *pText;
	int nn,nLine,nSample;
	WCHAR wc;
	float fValue;
	USES_CONVERSION;

	m_strTextRows.RemoveAll();

	//read the entire file
	int nFileLength = (int)pFile->GetLength();
	pText = new char[nFileLength+1];
	pFile->Read(pText,nFileLength);
	
	
	//Seperate in to individual rows
	Buff.Empty();
	for(nn=0;nn<nFileLength;nn++) {
		if(isprint(pText[nn])) {
			Buff += pText[nn];
		} else {
			if(Buff.GetLength()>0) {
				m_strTextRows.Add(Buff);
				Buff.Empty();
			}
		}
	}
	if(Buff.GetLength()>0) {
		m_strTextRows.Add(Buff);
		Buff.Empty();
	}

	delete pText;

	//Find how many scan lines
	m_nNumberLines=0;
	Buff = m_strTextRows.GetAt(12);
	for(nn=0;nn<Buff.GetLength();nn++) {
		Temp.Empty();
		for( ;(nn<Buff.GetLength()) && Buff.GetAt(nn)!=',';nn++) {
			Temp += Buff.GetAt(nn);
		}
		m_nNumberLines++;
	}
	
	m_nSamplesLine = m_strTextRows.GetSize() - 12;

	//extract the header
	SAFE_DELETE( m_pfHeader );
	m_nHeaderSize = 11;
	m_pfHeader = new float[m_nHeaderSize * m_nNumberLines];
	ZeroMemory(m_pfHeader,m_nHeaderSize * m_nNumberLines * sizeof (float));


	for(int hh=0;hh<m_nHeaderSize; hh++) {
		nLine=0;
		Buff = m_strTextRows.GetAt(hh);
		for(nn=0;nn<Buff.GetLength();nn++) {
			Temp.Empty();
			for( ;nn<Buff.GetLength();nn++) {
				wc = Buff.GetAt(nn);
				if(wc == ',') break;
				switch(wc) {
				default:Temp += wc;
					break;
				case '*':
					break;
				}
			}
			_WTOF(Temp,fValue);
			m_pfHeader[nLine * m_nHeaderSize + hh] = fValue;
			nLine++;
		}
	}


	

	//extract the values and stick them in a square array
	SAFE_DELETE( m_pfThickness );
	m_pfThickness = new float[m_nSamplesLine * m_nNumberLines];
	ZeroMemory(m_pfThickness,m_nSamplesLine * m_nNumberLines * sizeof (float));

	for(nSample = 0;nSample<m_nSamplesLine;nSample++) {
		Buff = m_strTextRows.GetAt(12 + nSample);
		nLine=0;
		for(nn=0;nn<Buff.GetLength();nn++) {
			Temp.Empty();
			for( ;nn<Buff.GetLength();nn++) {
				wc = Buff.GetAt(nn);
				if(wc == ',') break;
				switch(wc) {
				default:Temp += wc;
					break;
				case '*':
					break;
				}
			}
			_WTOF(Temp,fValue);
			m_pfThickness[nLine * m_nSamplesLine + nSample] = fValue;
			nLine++;
		}
	}

	//Remove Line 1
	m_nNumberLines--;
	CopyMemory(m_pfThickness,&m_pfThickness[1 * m_nSamplesLine],m_nNumberLines * m_nSamplesLine * sizeof (float));
	CopyMemory(m_pfHeader,&m_pfHeader[1 * m_nHeaderSize],m_nNumberLines * m_nHeaderSize * sizeof (float));

	//Remove empty lines by checking to see if th e2nd number is zero
	for(nLine=0;nLine<m_nNumberLines;nLine++) {
		if(m_pfThickness[nLine * m_nSamplesLine + 1] == 0.0f) {
			m_nNumberLines--;
			CopyMemory((void*)&m_pfThickness[nLine * m_nSamplesLine],(void*)&m_pfThickness[(nLine+1) * m_nSamplesLine],(m_nNumberLines-nLine) * m_nSamplesLine * sizeof (float));
			CopyMemory((void*)&m_pfHeader[nLine * m_nHeaderSize],(void*)&m_pfHeader[(nLine+1) * m_nHeaderSize],(m_nNumberLines-nLine) * m_nHeaderSize * sizeof (float));
		}
	}

	
	
	return true;
}

bool CImportCrossSectionOverlayDlg::GetLine(int nMaxLength, float *pArray)
{
	int nLength,nn,nPtr;

	if(m_pfThickness == NULL) {
		return false;
	}

	if(nMaxLength >= m_nSamplesLine) {
		nLength = m_nSamplesLine;
	} else {
		nLength = nMaxLength;
	}

	if(m_bMirror == false) {
		for(nn=0,nPtr=m_nOffset;nn<nLength;nn++,nPtr++) {
			if((nPtr>=0) && (nPtr<nMaxLength)) {
				pArray[nPtr] = m_pfThickness[m_nLineNumber * m_nSamplesLine + nn];
			};
		}
	} else {
		for(nn=0,nPtr=m_nOffset;nn<nLength;nn++,nPtr++) {
			if((nPtr>=0) && (nPtr<nMaxLength)) {
				pArray[nPtr] = m_pfThickness[m_nLineNumber * m_nSamplesLine + ((nLength-1)-nn)];
			}
		}
	}


	return true;
}



int CImportCrossSectionOverlayDlg::getSamplesLine()
{

	return m_nSamplesLine;

}

void CImportCrossSectionOverlayDlg::InvalidateParent()
{
	CViewThicknessCrossSectionDlg *pCrossSection = (CViewThicknessCrossSectionDlg*)m_pParent;

	pCrossSection->Invalidate(false);
}

void CImportCrossSectionOverlayDlg::OnCheckMirror() 
{
	
	if(m_checkMirror.GetCheck()) {
		m_bMirror = true;
	} else {
		m_bMirror = false;
	}
	InvalidateParent();
	
}


bool CImportCrossSectionOverlayDlg::GetHeader(float *pArray)
{

	if(m_pfHeader) {
		CopyMemory(pArray,&m_pfHeader[m_nLineNumber * m_nHeaderSize],m_nHeaderSize * sizeof (float));
		return true;
	} else {
		return false;
	}
}
