// EditBladeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "EditBladeDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"
#include "FileThumbNailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04
#define UPDATE_CHILDREN 0x08

#define	MINIMUM		0
#define	NOMINAL		1
#define	MAXIMUM		2

/////////////////////////////////////////////////////////////////////////////
// CEditBladeDlg dialog

CEditBladeDlg::CEditBladeDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop)
	: CDialog(CEditBladeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrainEditDlg)
	//}}AFX_DATA_INIT
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pDlg = pDlg;
}



void CEditBladeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditBladeDlg)
	DDX_Control(pDX, IDC_CHECK_ROTATE, m_checkRotate);
	DDX_Control(pDX, IDC_CHECK_DECIMATE, m_checkDecimate);
	DDX_Control(pDX, IDC_EDIT_FAST_INCREMENT, m_editFastIncrement);
	DDX_Control(pDX, IDC_BUTTON_IMPORT_MIN, m_buttonMin);
	DDX_Control(pDX, IDC_BUTTON_IMPORT_MAX, m_buttonMax);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditBladeDlg, CDialog)
	//{{AFX_MSG_MAP(CEditBladeDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_MAX, OnButtonImportMax)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_NOM, OnButtonImportNom)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_MIN, OnButtonImportMin)
	ON_BN_CLICKED(IDC_BUTTON_ADD_180_DEGREES, OnButtonAdd180Degrees)
	ON_BN_CLICKED(IDC_BUTTON_COPY_TO_SCANLINES, OnButtonCopyToScanlines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBladeDlg message handlers
void CEditBladeDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CEditBladeDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CEditBladeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
	
}

BOOL CEditBladeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditBladeDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	SetAccessPrivelage();

	Buff.Format(L"%d",theApp.m_nLine+1);
	m_editLineNumber.SetWindowText(Buff);

	m_spinLineNumber.SetRange(0,PROFILE->m_nScanLineL-1);
	m_spinLineNumber.SetPos(theApp.m_nLine);

	Buff.Format(L"%.01f mm",PROFILE->m_fFastIncrement);
	m_editFastIncrement.SetWindowText(Buff);

}


void CEditBladeDlg::OnChangeEditLineNumber() 
{
	CString Buff;
	int nLine = theApp.m_nLine;

	m_editLineNumber.GetWindowText(Buff);
	nLine = _ttoi(Buff) - 1;
	if(nLine-theApp.m_nLine) {
		theApp.m_nLine = nLine;
		m_spinLineNumber.SetPos(theApp.m_nLine);
		UpdateParent(FILLLIST|INVALIDATE);
		::SetFocus(m_editLineNumber.m_hWnd);
	}
	
}

void CEditBladeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	CString Buff;


	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_NUMBER:
		nTemp = theApp.m_nLine;
		theApp.m_nLine=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_nLine) {
			Buff.Format(_T("%d"),theApp.m_nLine+1);
			m_editLineNumber.SetWindowText(Buff);
			UpdateParent(INVALIDATE|FILLLIST|UPDATE_CHILDREN);
			::SetFocus(m_editLineNumber.m_hWnd);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEditBladeDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);
	if(nAction & UPDATE_CHILDREN) pParent->UpdateAllControls();

}

void CEditBladeDlg::OnButtonImportMax() 
{
	switch(theApp.m_nEditLineType) {
	case OUTSIDELINES:	ImportOverlay(OUTSIDE,MAXIMUM);
		break;
	case INSIDELINES:	ImportOverlay(INSIDE,MAXIMUM);
		break;
	}
	UpdateParent(INVALIDATE);
	
}

void CEditBladeDlg::OnButtonImportNom() 
{
	CString Buff;

	m_editFastIncrement.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_fFastIncrement);

	switch(theApp.m_nEditLineType) {
	case OUTSIDELINES:	ImportOverlay(OUTSIDE,NOMINAL);
		break;
	case INSIDELINES:	ImportOverlay(INSIDE,NOMINAL);
		break;
	}
	UpdateParent(FILLLIST|INVALIDATE);
	SetAccessPrivelage();

}

void CEditBladeDlg::OnButtonImportMin() 
{
	switch(theApp.m_nEditLineType) {
	case OUTSIDELINES:	ImportOverlay(OUTSIDE,MINIMUM);
		break;
	case INSIDELINES:	ImportOverlay(INSIDE,MINIMUM);
		break;
	}
	UpdateParent(INVALIDATE);

}



void CEditBladeDlg::ImportOverlay(int nSide, int nWhichThickness)
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString FilePath,Buff;
	int	nPos,nStackingAxisSample;
	CUSLFile File;
	CFileException e;
	float	*pfThickness = NULL;
	int nLine,nSamplesLine,nStartIndex;
	D3DXMATRIXA16 matRotate;
	float fAngle;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	CFileThumbNailDlg FileDlg(TRUE,_T(".csv"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("CSV (*.csv)|*.csv|All Files (*.*)|*.*|"),NULL);
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
			 theApp.m_RRImportCSVThicknessOverlay[nSide].Retrieve(&File);
			File.Close();

			pfThickness = new float[nSamplesLine= theApp.m_RRImportCSVThicknessOverlay[nSide].getSamplesLine()];
			for(nLine=0;nLine<PROFILE->m_nLineBladeL[nSide];nLine++) {
				ZeroMemory(pfThickness,sizeof (float) * nSamplesLine);
				 theApp.m_RRImportCSVThicknessOverlay[nSide].getThicknessLine(nLine,pfThickness);



				switch(nWhichThickness) {
				case MINIMUM:
					nStackingAxisSample = PROFILE->m_LineBlade[nSide][nLine].FindNearestCoord(0.0f,PROFILE->m_LineBlade[nSide][nLine].GetSize()/2);
					switch(nSide) {
					case 0:	nStartIndex = nStackingAxisSample -  theApp.m_RRImportCSVThicknessOverlay[nSide].getStackingAxisToLE(nLine);
						break;
					case 1:	nStartIndex = nStackingAxisSample +  theApp.m_RRImportCSVThicknessOverlay[nSide].getStackingAxisToLE(nLine);
						break;
					}
					PROFILE->m_LineBlade[nSide][nLine].setAllMinimumThickness(pfThickness,nSamplesLine,nStartIndex,nSide);
					break;
				case NOMINAL:
					PROFILE->m_LineBlade[nSide][nLine].SetString(theApp.m_RRImportCSVThicknessOverlay[nSide].getSection(nLine));

					if(m_checkRotate.GetCheck()) {
						PROFILE->m_LineBlade[nSide][nLine].SetAllRPos(PORTSIDE,fAngle =  theApp.m_RRImportCSVThicknessOverlay[nSide].getBladeFormAngle(nLine) * -1.0f);
						D3DXMatrixRotationY(&matRotate,fAngle*DEG_TO_RAD);
						switch(nSide) {
						case 0: PROFILE->ComponentMoveLine(matRotate,OUTSIDELINES,nLine);
							break;
						case 1: PROFILE->ComponentMoveLine(matRotate,INSIDELINES,nLine);
							break;
						};
						PROFILE->m_LineBlade[nSide][nLine].Sort(0);
					}
					if(m_checkDecimate.GetCheck()) {
						PROFILE->m_LineBlade[nSide][nLine].DecimateEitherSideOfZero(PROFILE->m_fFastIncrement);
					}

					nStackingAxisSample = PROFILE->m_LineBlade[nSide][nLine].FindNearestCoord(0.0f,PROFILE->m_LineBlade[nSide][nLine].GetSize()/2);
					switch(nSide) {
					case 0:	nStartIndex = nStackingAxisSample -  theApp.m_RRImportCSVThicknessOverlay[nSide].getStackingAxisToLE(nLine);
						break;
					case 1:	nStartIndex = nStackingAxisSample +  theApp.m_RRImportCSVThicknessOverlay[nSide].getStackingAxisToLE(nLine);
						break;
					}
					PROFILE->m_LineBlade[nSide][nLine].setAllNominalThickness(pfThickness,nSamplesLine,nStartIndex,nSide);

					break;
				case MAXIMUM:
					nStackingAxisSample = PROFILE->m_LineBlade[nSide][nLine].FindNearestCoord(0.0f,PROFILE->m_LineBlade[nSide][nLine].GetSize()/2);
					switch(nSide) {
					case 0:	nStartIndex = nStackingAxisSample -  theApp.m_RRImportCSVThicknessOverlay[nSide].getStackingAxisToLE(nLine);
						break;
					case 1:	nStartIndex = nStackingAxisSample +  theApp.m_RRImportCSVThicknessOverlay[nSide].getStackingAxisToLE(nLine);
						break;
					}
					PROFILE->m_LineBlade[nSide][nLine].setAllMaximumThickness(pfThickness,nSamplesLine,nStartIndex,nSide);
					break;
				}
			}
			delete pfThickness;
		}
	}


}

void CEditBladeDlg::SetAccessPrivelage()
{
	bool bFlag = false;

	switch(theApp.m_nEditLineType) {
	case OUTSIDELINES: if( theApp.m_RRImportCSVThicknessOverlay[OUTSIDE].m_nNumberHeaderRows>0) bFlag = true;
		break;
	case INSIDELINES: if( theApp.m_RRImportCSVThicknessOverlay[INSIDE].m_nNumberHeaderRows>0) bFlag = true;
		break;
	}
	m_buttonMin.EnableWindow(bFlag);
	m_buttonMax.EnableWindow(bFlag);
}

void CEditBladeDlg::OnButtonAdd180Degrees() 
{
	switch(theApp.m_nEditLineType) {
	case OUTSIDELINES: PROFILE->AddRAngleToBlade(180.0f,OUTSIDE);
		break;
	case INSIDELINES: PROFILE->AddRAngleToBlade(180.0f,INSIDE);
		break;
	}
	UpdateParent(INVALIDATE);

}



void CEditBladeDlg::OnButtonCopyToScanlines() 
{
	switch(theApp.m_nEditLineType) {
	case OUTSIDELINES:	PROFILE->CopyAllLines(SCANLINES, OUTSIDELINES);
		break;
	case INSIDELINES:	PROFILE->CopyAllLines(SCANLINES, INSIDELINES);
		break;
	}
	UpdateParent(INVALIDATE);
	
}
