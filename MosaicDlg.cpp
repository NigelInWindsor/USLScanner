// MosaicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MosaicDlg.h"
#include "FileThumbNailDlg.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMosaicDlg dialog


CMosaicDlg::CMosaicDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, CData *pData)
	: CDialog(CMosaicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMosaicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;
	
	m_nNumberSamples = 1;
	m_nNumberLines = 1;
}


void CMosaicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMosaicDlg)
	DDX_Control(pDX, IDC_SPIN_NUMBER_SAMPLES, m_spinNumberSamples);
	DDX_Control(pDX, IDC_EDIT_NUMBER_SAMPLES, m_editNumberSamples);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMosaicDlg, CDialog)
	//{{AFX_MSG_MAP(CMosaicDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_APPENDTOBOTTOM, OnButtonAppendtobottom)
	ON_BN_CLICKED(IDC_BUTTON_APPENDTOLEFT, OnButtonAppendtoleft)
	ON_BN_CLICKED(IDC_BUTTON_APPENDTORIGHT, OnButtonAppendtoright)
	ON_BN_CLICKED(IDC_BUTTON_APPENDTOTOP, OnButtonAppendtotop)
	ON_BN_CLICKED(IDC_BUTTON_CROPVERTICALLINE, OnButtonCropverticalline)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_SAMPLES, OnChangeEditNumberSamples)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_CROPLEFT, OnButtonCropleft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMosaicDlg message handlers

void CMosaicDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CMosaicDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CMosaicDlg::IDD & MAXIMUMWINDOWSIDNUMBER] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CMosaicDlg::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CMosaicDlg::IDD & MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
}

BOOL CMosaicDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	

	CRect rect = theApp.m_LastSettings.RectWindowPos[CMosaicDlg::IDD & MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
//		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMosaicDlg::OnButtonAppendtobottom() 
{
	AppendFileTo(APPEND_BOTTOM);
	
}

void CMosaicDlg::OnButtonAppendtoleft() 
{
	AppendFileTo(APPEND_LEFT);
	
}

void CMosaicDlg::OnButtonAppendtoright() 
{
	AppendFileTo(APPEND_RIGHT);
	
}

void CMosaicDlg::OnButtonAppendtotop() 
{
	AppendFileTo(APPEND_TOP);
	
}

void CMosaicDlg::AppendFileTo(int nWhichSide)
{
	CString FilePath,Buff,Language;
	int	nPos;
	CUSLFile File;
	CFileException e;
	CData* pNewData;
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;

	CFileThumbNailDlg FileDlg(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeRead | CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			Language.LoadString(IDS_Error);
			MessageBox(Buff,Language,MB_ICONERROR);
		} else {
			pNewData = new CData;

			pNewData->Retrieve(&File);
			File.Close();

//			pViewSheet->SuspendChildTimers();
			m_pData->AppendDataSet(nWhichSide, pNewData);
			delete pNewData;
			pViewSheet->InvalidateChild();

		}



	}

}


void CMosaicDlg::OnChangeEditNumberSamples() 
{
	CString Buff;
	
	m_editNumberSamples.GetWindowText(Buff);
	m_nNumberSamples = _ttoi(Buff);
}

void CMosaicDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_NUMBER_SAMPLES:
		m_nNumberSamples = pSpin->GetPos()&0xffff;
		Buff.Format(L"%d",m_nNumberSamples);
		m_editNumberSamples.SetWindowText(Buff);
		break;
	};
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMosaicDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(L"%d",m_nNumberSamples);
	m_editNumberSamples.SetWindowText(Buff);

	m_spinNumberSamples.SetRange(1,m_pData->m_nSamplesLine);
	m_spinNumberSamples.SetPos(m_nNumberSamples);
}


void CMosaicDlg::OnButtonCropleft() 
{
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;

	m_pData->CropLeftEdge(m_nNumberSamples);
	pViewSheet->InvalidateChild();
}

void CMosaicDlg::OnButtonCropverticalline() 
{
	CViewSheet *pViewSheet = (CViewSheet*)m_pParent;

	m_pData->CropRightEdge(m_nNumberSamples);
	pViewSheet->InvalidateChild();

}
