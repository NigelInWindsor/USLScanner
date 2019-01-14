// InclusionsResultsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "InclusionsResultsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInclusionsResultsPage property page

IMPLEMENT_DYNCREATE(CInclusionsResultsPage, CPropertyPage)

CInclusionsResultsPage::CInclusionsResultsPage(CInclusions* pInclusions, CData *pData) : CPropertyPage(CInclusionsResultsPage::IDD)
{
	//{{AFX_DATA_INIT(CInclusionsResultsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pInclusions = pInclusions;
	m_pEditSpinItem = NULL;
	m_pData = pData;
}

CInclusionsResultsPage::~CInclusionsResultsPage()
{
}

void CInclusionsResultsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInclusionsResultsPage)
	DDX_Control(pDX, IDC_EDIT_RESULTS, m_editResults);
	DDX_Control(pDX, IDC_LIST_RESULTS, m_listResults);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInclusionsResultsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CInclusionsResultsPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULTS, OnClickListResults)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RESULTS, OnDblclkListResults)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RESULTS, OnRclickListResults)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RESULTS, OnGetdispinfoListResults)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_TEXT_FILE, OnButtonTextFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInclusionsResultsPage message handlers
BOOL CInclusionsResultsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	

	CreateColumns();
	FillList();

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInclusionsResultsPage::CreateColumns()
{
	CString Name[10] = {L"#", L"Xpos", L"Ypos", L"Depth", L"Amp %",L"Gain dB", L"FBH mm", L"Length", L"Class", L"Comment"};
	int nWidth[10] = { 30,50,50,50,50,60,60,60,60,140 };

	for(int ii=0;ii<10; ii++) {
		m_listResults.InsertColumn(ii,Name[ii],LVCFMT_LEFT,nWidth[ii]);
	}
	m_listResults.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CInclusionsResultsPage::FillList()
{
	CString Buff;

	if (GetSafeHwnd() == NULL) return;

	m_listResults.DeleteAllItems();
	for(int ii=0;ii<m_pInclusions->GetDefectSize();ii++) {
		Buff.Format(L"%d",ii+1);
		m_listResults.InsertItem(ii,Buff.GetBuffer(255),ii);
	}
	m_listResults.EnsureVisible(m_nIndex,FALSE);
	m_listResults.SetItemState(m_nIndex, LVIS_SELECTED,LVIS_SELECTED);
	m_listResults.SetFocus();
}

void CInclusionsResultsPage::OnGetdispinfoListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static WCHAR str[100];
	CString Buff;
	DefectStruct *pDef;

	str[0]=0;
	if(m_pInclusions->IsDefects() && pDispInfo->item.iItem<m_pInclusions->GetDefectSize()) {

		pDef = m_pInclusions->GetPtrDefect(pDispInfo->item.iItem);

		switch(pDispInfo->item.iSubItem) {
		case 0: swprintf_s(str,L"%d",pDispInfo->item.iItem+1);
			break;
		case 1: swprintf_s(str,L"%.01f",pDef->fXp);
			break;
		case 2: swprintf_s(str,L"%.01f",pDef->fYp);
			break;
		case 3: swprintf_s(str,L"%.01f",pDef->fDepth);
			break;
		case 4: swprintf_s(str,L"%d",pDef->nAmp);
			break;
		case 5: swprintf_s(str,L"%.01f",pDef->fGain);
			break;
		case 6: swprintf_s(str,L"%.02f",pDef->fFBH);
			break;
		case 7: swprintf_s(str,L"%.01f",pDef->fLength);
			break;
		case 8: swprintf_s(str,L"%.02f",pDef->fFBH * pDef->fLength);
			break;
		}
	}
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CInclusionsResultsPage::OnClickListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CInclusionsResultsPage::OnDblclkListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CInclusionsResultsPage::OnRclickListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}



void CInclusionsResultsPage::UpdateAllControls()
{

}

void CInclusionsResultsPage::OnPaint() 
{
	if (GetSafeHwnd() == NULL) return;
	CRect rrView;
	CPaintDC dc(this); // device context for painting
	
	if(GetSafeHwnd()) {
		/*
		m_staticView.GetWindowRect(&rrView);
		ScreenToClient(&rrView);
		rrView.DeflateRect(2,2);

		dc.FillRect(rrView,&CBrush(RGB(255,255,255)));

		LOGFONT lf;
		memset((void*)&lf,0,sizeof(lf));
		lf.lfHeight = 18;
		switch(theApp.m_nLanguage) {
		default: _tcscpy(lf.lfFaceName,_T("Times"));
			break;
		case 2: _tcscpy(lf.lfFaceName,_T("SimSun"));
			break;
		}
		CFont Font;
		Font.CreateFontIndirect(&lf);

		CFont* pOldFont = dc.SelectObject(&Font);
		dc.SetBkMode(TRANSPARENT);
		
		RenderView(&dc, rrView);

		dc.SelectObject(pOldFont);
		*/

		RenderView(&dc, rrView);


	}
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CInclusionsResultsPage::RenderView(CDC *pDC, CRect rrView)
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff, Left, Right, Temp;
	int nY = rrView.top;
	int ii;
	CSize size;
	float fFbhTotal = 0.0;
	float fLengthTotal = 0.0;
	float fEDATotal = 0.0;
	DefectStruct* pDef;


	Buff.Empty();
	Left.Format(L"Cast: %s",m_pInclusions->m_Cast);
	Right.Format(L"Customer: %s",m_pInclusions->m_Customer);
	Temp.Format(L"%-30.30s\t\t%-30.30s\r\n",Left,Right); Buff+= Temp;

	Left.Format(L"Grade: %s",m_pInclusions->m_Grade);
	Right.Format(L"Order Number: %s",m_pInclusions->m_OrderNumber);
	Temp.Format(L"%-30.30s\t\t%-30.30s\r\n",Left,Right); Buff+= Temp;

	Left.Format(L"Sample Ref: %s",m_pInclusions->m_SampleRef);
	Right.Format(L"Focal depth: %s",m_pInclusions->m_FocalDepth);
	Temp.Format(L"%-30.30s\t\t%-30.30s\r\n",Left,Right); Buff+= Temp;

	Left.Format(L"FocalLength: %s",m_pInclusions->m_FocalLength);
	Right.Format(L"Probe: %s",m_pInclusions->m_Probe);
	Temp.Format(L"%-30.30s\t\t%-30.30s\r\n",Left,Right); Buff+= Temp;

	Left.Format(L"Ref FBH: %s",m_pInclusions->m_RefFBH);
	Right.Format(L"RefSensitivityFBH: %s",m_pInclusions->m_RefSensitivityFBH);
	Temp.Format(L"%-30.30s\t\t%-30.30s\r\n",Left,Right); Buff+= Temp;

	Left.Format(L"Ref Gain: %s",m_pInclusions->m_RefGain);
	Right.Format(L"Test Gain: %s",m_pInclusions->m_TestGain);
	Temp.Format(L"%-30.30s\t\t%-30.30s\r\n",Left,Right); Buff+= Temp;

	CTime time(m_pInclusions->m_osRefDate);
	Left = time.Format(_T("Date: %d %b %Y %Hh%M\r\n"));	Buff += Left;

//	switch(m_pInclusions->GetComponentShape(m_pData)) {
//	case 0:

		m_pInclusions->CalculateMass(m_pData);
		m_pInclusions->CalculateTSA(m_pData);

		Temp.Format(L"Mass: %.02f Kg\r\n",m_pInclusions->m_fTestMass); Buff+=Temp;

		for(ii=0;ii<m_pInclusions->GetDefectSize();ii++) {
			pDef = m_pInclusions->GetPtrDefect(ii);
			fFbhTotal += pDef->fFBH;
			fLengthTotal += pDef->fLength;
			fEDATotal += (pDef->fLength * pDef->fFBH);
		}
		Temp.Format(L"FBHTotal: %.02f\r\n",fFbhTotal);	Buff+=Temp;
		Temp.Format(L"Inclusion length: %.01f mm\r\n",m_pInclusions->m_fInclusionLength);	Buff+=Temp;
		Temp.Format(L"TSA/10Kg: %.01f\r\n",m_pInclusions->m_fTSA);	Buff+=Temp;
		Temp.Format(L"Volume/dm^3:%.02f\r\n",(m_pInclusions->m_fTestMass / theApp.m_LastSettings.fMaterialDensity) / 1e6  );	Buff+=Temp;
		Temp.Format(L"Targets/Kg: %.02f\r\n",(float)m_pInclusions->m_nDefL / m_pInclusions->m_fTestMass);	Buff+=Temp;
		
//		break;
//	case 1:
//		break;
//	}

	m_editResults.SetWindowText(Buff);
}

BOOL CInclusionsResultsPage::OnSetActive() 
{

	
	FillList();
	UpdateAllControls();

	return CPropertyPage::OnSetActive();
}

void CInclusionsResultsPage::OnButtonTextFile() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;

	CFileDialog FileDlg(FALSE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text (*.txt)|*.txt|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PmacPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PmacPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			m_pInclusions->SaveToTextFile(&File, m_pData);
			File.Close();
		}
	}
	ShellExecute(NULL,L"Open",L"Notepad.exe",FilePath,NULL,SW_SHOW);

	
}


