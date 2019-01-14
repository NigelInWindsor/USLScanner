// BladeStatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "BladeStatisticsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBladeStatisticsDlg dialog


CBladeStatisticsDlg::CBladeStatisticsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CData *pData, int nScanType)
	: CDialog(CBladeStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBladeStatisticsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pData = pData;
	m_nScanType = nScanType;
	
	m_pEditSpinItem=NULL;
}


void CBladeStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBladeStatisticsDlg)
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBladeStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CBladeStatisticsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_UPDATE_BLADE_STATISTICS, UpdateMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBladeStatisticsDlg message handlers

void CBladeStatisticsDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CBladeStatisticsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CBladeStatisticsDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CBladeStatisticsDlg::OnDestroy() 
{
	if (CBladeStatisticsDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CBladeStatisticsDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CBladeStatisticsDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;
	

	m_StaticPosList.Initialize(this, m_listCoords, TRUE, TRUE, TRUE, TRUE);

	if (CBladeStatisticsDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CBladeStatisticsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosList.SetNewPos();

	
	CreateColumns();
	FillList();
	UpdateAllControls();
	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBladeStatisticsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosList.SetNewPos();
	
}


void CBladeStatisticsDlg::UpdateAllControls()
{

	SetWindowTitle();
}

void CBladeStatisticsDlg::CreateColumns()
{
	int ColumnWidth[21] = { 0,55,55,55,8,55,55,55,8,55,55,55,8,55,55,55,55,55,55,8,55};
	CString ColumnName[21];
	ColumnName[0]= "#";
	ColumnName[1]= "X Nom";
	ColumnName[2]= "Y";
	ColumnName[3]= "Z";
	ColumnName[4]= "";
	ColumnName[5]= "X Tool";
	ColumnName[6]= "Y";
	ColumnName[7]= "Z";
	ColumnName[8]= "";
	ColumnName[9]= "X Surf";
	ColumnName[10]= "Y";
	ColumnName[11]= "Z";
	ColumnName[12]= "";
	ColumnName[13]= "Nom Th'";
	ColumnName[14]= "Act Th'";
	ColumnName[15]= "Delta Th'";
	ColumnName[16]= "Traffic L''";
	ColumnName[17]= "Min Thick'";
	ColumnName[18]= "Max Thick'";
	ColumnName[19]= "";
	ColumnName[20]= "Amp";

	int nColumnCount = m_listCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}
	for (int ii=0;ii<21;ii++) {
		m_listCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CBladeStatisticsDlg::FillList()
{
	int ii;
	CString Buff;

	if(m_pData->m_LineBlade[m_nScanType] == NULL) return;

	int nCount = m_listCoords.GetItemCount();
	int nCoordL = m_pData->m_LineBlade[m_nScanType][theApp.m_nBladeLine].m_nCoordL;

	if(nCount<nCoordL) {
		for (ii=nCount;ii<nCoordL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
	} else {
		for(ii=nCount-1;ii>=nCoordL;ii--) {
			m_listCoords.DeleteItem(ii);
		}
	}
	m_listCoords.EnsureVisible(theApp.m_nIndex,FALSE);

	m_listCoords.EnsureVisible(m_nIndex,FALSE);
	for(ii=0;ii<nCoordL;ii++) 
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoords.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoords.SetFocus();
	m_listCoords.RedrawItems(theApp.m_nIndex,theApp.m_nIndex+50);

}

void CBladeStatisticsDlg::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CCoord Cp;
	float fX;
	static float fNomThickness,fMinThickness,fMaxThickness;
	static int nType;

	if(pDispInfo->item.iItem < m_pData->m_LineBlade[m_nScanType][theApp.m_nBladeLine].m_nCoordL) {
		m_pData->m_LineBlade[m_nScanType][theApp.m_nBladeLine].GetCoord(pDispInfo->item.iItem, &Cp);

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:	swprintf_s(str,_T("%.01f"),Cp.Side0.fX);
			break;
		case 2:	swprintf_s(str,_T("%.01f"),Cp.Side0.fY);
			break;
		case 3:	swprintf_s(str,_T("%.01f"),Cp.Side0.fZ);
			break;
		case 4: swprintf_s(str,_T("|"));
			break;
		case 5:	swprintf_s(str,_T("%.01f"),Cp.Side1.fX);
			break;
		case 6:	swprintf_s(str,_T("%.01f"),Cp.Side1.fY);
			break;
		case 7:	swprintf_s(str,_T("%.01f"),Cp.Side1.fZ);
			break;
		case 8: swprintf_s(str,_T("|"));
			break;
		case 9:	swprintf_s(str,_T("%.01f"),Cp.Side[2].fX);
			break;
		case 10:	swprintf_s(str,_T("%.01f"),Cp.Side[2].fY);
			break;
		case 11:	swprintf_s(str,_T("%.01f"),Cp.Side[2].fZ);
			break;
		case 12: swprintf_s(str,_T("|"));
			break;
		case 13:
			fX = Cp.Side0.fX - m_pData->m_LineBlade[m_nScanType][theApp.m_nBladeLine].m_Matrix.m[3][0];
			m_pData->m_LineBlade[m_nScanType][theApp.m_nBladeLine].GetNominalThickness(fX,NULL,&fNomThickness,&fMinThickness,&fMaxThickness,&nType);
			swprintf_s(str,_T("%.02f"),fNomThickness);
			break;
		case 14:
			swprintf_s(str,_T("%.02f"),Cp.fActThickness);
			break;
		case 15:	swprintf_s(str,_T("%.02f"),Cp.fActThickness - fNomThickness);
			break;
		case 16:
			if(nType  & TRAFFIC_LIGHT) {
				swprintf_s(str,_T("TL"));
			} else {
				swprintf_s(str,_T(""));
			}
			break;
		case 17:	swprintf_s(str,_T("%.02f"),fMinThickness);
			break;
		case 18:	swprintf_s(str,_T("%.02f"),fMaxThickness);
			break;
		case 19: swprintf_s(str,_T("|"));
			break;
		case 20:	swprintf_s(str,_T("%d"),Cp.nAmp);
			break;
		}
	}
	

	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CBladeStatisticsDlg::SetWindowTitle()
{
	CString	Buff,Temp;
	int nPos;
	CString ScanType[4];
	CCoord Cp;


	ScanType[0] = "Outside";
	ScanType[1] = "Inside";
	ScanType[2] = "Pressure Swept";
	ScanType[3] = "Pressure Parallel";


	Buff = m_pData->m_FileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	Temp.Format(_T(" %s    Line %d    Section %s"),ScanType[m_nScanType],theApp.m_nBladeLine + 1, m_pData->m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetString());
	Buff += Temp;

	if(m_pData->m_bModified) {
		Buff += "    (Not Saved)";
	}


	SetWindowText(Buff);

}

HRESULT CBladeStatisticsDlg::UpdateMessage(WPARAM, LPARAM)
{

	FillList();
	SetWindowTitle();
	return NULL;
}

void CBladeStatisticsDlg::setScanType(int nScanType)
{

	m_nScanType = nScanType;
}
