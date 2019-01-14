// ReportTablePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ReportTablePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	_LIST_TABLE			0x01
#define	_CLASSIC			0x02
#define	_LANGUAGE0			0x04
#define	_LANGUAGE1			0x08

/////////////////////////////////////////////////////////////////////////////
// CReportTablePropertiesDlg dialog

CReportTablePropertiesDlg::CReportTablePropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature,CData *pData)
	: CDialog(CReportTablePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataValuePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFeature = pFeature;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pData = pData;
	
	m_nRow = 0;
	m_nColumn = 0;
}


void CReportTablePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportTablePropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_JUSTIFY, m_comboJustify);
	DDX_Control(pDX, IDC_CHECK_BOTH_LANGUAGES, m_checkBothLanguagesOneColumn);
	DDX_Control(pDX, IDC_COMBO_ID_ITEM, m_comboIDItem);
	DDX_Control(pDX, IDC_EDIT_ROW_COLUMN, m_editRowColumn);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_editTitle);
	DDX_Control(pDX, IDC_STATIC_TITLE_TEXT, m_staticTitleText);
	DDX_Control(pDX, IDC_STATIC_TITLE_BK, m_staticTitleBk);
	DDX_Control(pDX, IDC_SPIN_ROWS, m_spinNumberRows);
	DDX_Control(pDX, IDC_SPIN_COLUMNS, m_spinNumberColumns);
	DDX_Control(pDX, IDC_CHECK_TITLE_BAR, m_checkTitleBar);
	DDX_Control(pDX, IDC_EDIT_COLUMNS, m_editNumberColumns);
	DDX_Control(pDX, IDC_EDIT_ROWS, m_editNumberRows);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportTablePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CReportTablePropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_COLUMNS, OnChangeEditColumns)
	ON_EN_CHANGE(IDC_EDIT_ROWS, OnChangeEditRows)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_TITLE_BAR, OnCheckTitleBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_TITLE, OnChangeEditTitle)
	ON_EN_CHANGE(IDC_EDIT_ROW_COLUMN, OnChangeEditRowColumn)
	ON_CBN_SELCHANGE(IDC_COMBO_ID_ITEM, OnSelchangeComboIdItem)
	ON_BN_CLICKED(IDC_BUTTON_AUTOMATIC, OnButtonAutomatic)
	ON_BN_CLICKED(IDC_CHECK_BOTH_LANGUAGES, OnCheckBothLanguages)
	ON_CBN_SELCHANGE(IDC_COMBO_JUSTIFY, OnSelchangeComboJustify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportTablePropertiesDlg message handlers

void CReportTablePropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CReportTablePropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CReportTablePropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CReportTablePropertiesDlg::OnDestroy() 
{
	if (CReportTablePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CReportTablePropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CReportTablePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int ii;
	CString Buff;

	m_spinNumberRows.SetRange(1,200);
	m_spinNumberColumns.SetRange(1,200);

	m_comboIDItem.AddString(L"");
	for(ii=0;ii<m_pData->m_DataProperties.GetSize();ii++) {
		Buff.Format(L"Item 1 ..%s",m_pData->m_DataProperties.GetCaptionAt(0,ii));
		m_comboIDItem.AddString(Buff);
		Buff.Format(L"Item 2 ..%s",m_pData->m_DataProperties.GetCaptionAt(1,ii));
		m_comboIDItem.AddString(Buff);
		Buff.Format(L"Value ...%s",m_pData->m_DataProperties.GetValueAt(ii));
		m_comboIDItem.AddString(Buff);
	}

	Buff.LoadString(IDS_Left);		m_comboJustify.AddString(Buff);
	Buff.LoadString(IDS_Right);		m_comboJustify.AddString(Buff);
	Buff.LoadString(IDS_Centre);	m_comboJustify.AddString(Buff);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportTablePropertiesDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	int nIndex=0;

	Buff.Format(L"%d",m_pFeature->nNumberRows);
	m_editNumberRows.SetWindowText(Buff);

	Buff.Format(L"%d",m_pFeature->nNumberColumns);
	m_editNumberColumns.SetWindowText(Buff);

	m_spinNumberRows.SetPos( m_pFeature->nNumberRows);
	m_spinNumberColumns.SetPos( m_pFeature->nNumberColumns);

	m_pFeature->unFlags & FEATURE_TITLEBAR ? m_checkTitleBar.SetCheck(true) : m_checkTitleBar.SetCheck(false);

	m_editTitle.SetWindowText( *m_pFeature->pCaption );

	Buff.Format(L"%d X %d",m_nRow + 1, m_nColumn+1);
	m_editRowColumn.SetWindowText(Buff);


	if(m_pFeature->pnIDArray) {
		nIndex = m_pFeature->pnIDArray[m_nRow * m_pFeature->nNumberColumns + m_nColumn];
	}
	m_comboIDItem.SetCurSel(nIndex);

	m_checkBothLanguagesOneColumn.SetCheck(	theApp.m_LastSettings.nBothLanguagesOneColumn );

	m_comboJustify.SetCurSel(theApp.m_LastSettings.nColumnJustification);

}

void CReportTablePropertiesDlg::OnChangeEditRows() 
{
	CString Buff;
	int nTemp = m_pFeature->nNumberRows;

	m_editNumberRows.GetWindowText(Buff);
	m_pFeature->nNumberRows = _ttoi(Buff);

	MinMax(&m_pFeature->nNumberRows,1,30);

	if(m_pFeature->nNumberRows - nTemp) {
		m_spinNumberRows.SetPos(m_pFeature->nNumberRows);
		UpdateParent();
	}

}

void CReportTablePropertiesDlg::OnChangeEditColumns() 
{
	CString Buff;
	int nTemp = m_pFeature->nNumberRows;

	m_editNumberColumns.GetWindowText(Buff);
	m_pFeature->nNumberColumns = _ttoi(Buff);

	MinMax(&m_pFeature->nNumberColumns,1,30);

	if(m_pFeature->nNumberColumns - nTemp) {
		m_spinNumberColumns.SetPos(m_pFeature->nNumberColumns);
		UpdateParent();
	}
}

void CReportTablePropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_ROWS:
		nTemp=m_pFeature->nNumberRows;
		m_pFeature->nNumberRows=pSpin->GetPos()&0xffff;
		if(nTemp-m_pFeature->nNumberRows) {
			Buff.Format(_T("%d"),m_pFeature->nNumberRows);
			m_editNumberRows.SetWindowText(Buff);
			UpdateParent();
		}
		break;
	case IDC_SPIN_COLUMNS:
		nTemp=m_pFeature->nNumberColumns;
		m_pFeature->nNumberColumns=pSpin->GetPos()&0xffff;
		if(nTemp-m_pFeature->nNumberColumns) {
			Buff.Format(_T("%d"),m_pFeature->nNumberColumns);
			m_editNumberColumns.SetWindowText(Buff);
			UpdateParent();
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CReportTablePropertiesDlg::UpdateParent()
{

	m_pParent->Invalidate(FALSE);


}

void CReportTablePropertiesDlg::OnCheckTitleBar() 
{
	if(m_checkTitleBar.GetCheck() == TRUE) {
		m_pFeature->unFlags |= FEATURE_TITLEBAR;
	} else {
		m_pFeature->unFlags &= ~FEATURE_TITLEBAR;
	}
	UpdateParent();
	
}

void CReportTablePropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticTitleText.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbTitle=dlg.GetColor();
			Invalidate(FALSE);
			UpdateParent();
		}
	}

	m_staticTitleBk.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbBkTitle=dlg.GetColor();
			Invalidate(FALSE);
			UpdateParent();
		}
	}
		
	CDialog::OnLButtonDown(nFlags, point);
}

void CReportTablePropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	BlockFill(&dc,&m_staticTitleText,m_pFeature->rgbTitle);
	BlockFill(&dc,&m_staticTitleBk,m_pFeature->rgbBkTitle);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CReportTablePropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CReportTablePropertiesDlg::OnChangeEditTitle() 
{
	CString Buff;

	m_editTitle.GetWindowText(Buff);
	if(*m_pFeature->pCaption != Buff) {
		*m_pFeature->pCaption = Buff;
		UpdateParent();
	}
}

void CReportTablePropertiesDlg::OnChangeEditRowColumn() 
{
	CString Buff;
	WCHAR cc;

	m_editRowColumn.GetWindowText(Buff);

	for(int ii=0;ii<Buff.GetLength();ii++) {
		cc=Buff.GetAt(ii);
		if(!isdigit(cc) && (cc!='.') && (cc!='-')) {
			Buff.Replace(cc,' ');
		}
	}
	if(Buff.GetLength() > 0) {
		swscanf_s(Buff,L"%d %d",&m_nRow,&m_nColumn);
		m_nRow-=1;
		m_nColumn-=1;
	}

}

void CReportTablePropertiesDlg::OnSelchangeComboIdItem() 
{

	int	nIndex = m_nRow * m_pFeature->nNumberColumns + m_nColumn;

	if((nIndex >= 0) && (nIndex<(m_pFeature->nNumberColumns * m_pFeature->nNumberRows))) {

		if(m_pFeature->pnIDArray == NULL) m_pFeature->AllocateIDArray();

		m_pFeature->pnIDArray[nIndex] = m_comboIDItem.GetCurSel();
		UpdateParent();
	}
}

void CReportTablePropertiesDlg::OnButtonAutomatic() 
{
	int nIndex;

	m_pFeature->nNumberColumns = 1;
	if(	theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) m_pFeature->nNumberColumns++;
	if(	theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1) m_pFeature->nNumberColumns++;
	if( theApp.m_LastSettings.nBothLanguagesOneColumn == 1) m_pFeature->nNumberColumns = 2;

	m_pFeature->nNumberRows = m_pData->m_DataProperties.GetSize();	
	SAFE_DELETE_ARRAY( m_pFeature->pnIDArray ) m_pFeature->AllocateIDArray();

	for(int nRow=0;nRow<m_pFeature->nNumberRows;nRow++) {
		for(int nColumn=0;nColumn < m_pFeature->nNumberColumns;nColumn++) {

			nIndex = nRow * m_pFeature->nNumberColumns + nColumn;

			if( theApp.m_LastSettings.nBothLanguagesOneColumn == 0) {
				if(	(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) && !(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1)) {
					switch(nColumn) {
					case 0:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 1;
						break;
					case 1:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 3;
						break;
					}
				}
				if(	!(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) && (theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1)) {
					switch(nColumn) {
					case 0:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 2;
						break;
					case 1:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 3;
						break;
					}
				}
				if(	(theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE0) && (theApp.m_LastSettings.nDataPropertiesMask & _LANGUAGE1)) {
					switch(nColumn) {
					case 0:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 1;
						break;
					case 1:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 2;
						break;
					case 2:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 3;
						break;
					}
				}
			} else {
				switch(nColumn) {
				case 0:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 1;
					break;
				case 1:	m_pFeature->pnIDArray[nIndex] = (nRow * 3) + 3;
					break;
				}
			}
		}
	}
	UpdateParent();
}

void CReportTablePropertiesDlg::OnCheckBothLanguages() 
{
	theApp.m_LastSettings.nBothLanguagesOneColumn = m_checkBothLanguagesOneColumn.GetCheck() & 1;
}

void CReportTablePropertiesDlg::OnSelchangeComboJustify() 
{
	theApp.m_LastSettings.nColumnJustification = m_comboJustify.GetCurSel();
}
