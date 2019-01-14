// DataValuePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DataValuePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataValuePropertiesDlg dialog


CDataValuePropertiesDlg::CDataValuePropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature,CData *pData)
	: CDialog(CDataValuePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataValuePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFeature = pFeature;
	m_pDlg = pDlg;
	m_pParent = pParent;
	m_pData = pData;
	
}


void CDataValuePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataValuePropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_VARIABLE, m_comboVariable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataValuePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CDataValuePropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_VARIABLE, OnSelchangeComboVariable)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataValuePropertiesDlg message handlers

void CDataValuePropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CDataValuePropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CDataValuePropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CDataValuePropertiesDlg::OnDestroy() 
{
	if (CDataValuePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CDataValuePropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CDataValuePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff,Temp;
	
	if (CDataValuePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CDataValuePropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}


	for(int nn=0;nn<theApp.m_nDescriptionL;nn++) {
		Buff.Empty();
		switch(theApp.m_pDescription[nn].nCtrlType) {
		default:
			Buff = *theApp.m_pDescription[nn].pCaption;
			m_comboVariable.AddString(Buff);
			break;
		case 100:	//Just to shut the compiler up
			break;
		}
	}



	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDataValuePropertiesDlg::UpdateAllControls()
{

	m_comboVariable.SetCurSel(m_pFeature->nDataValueType);

}

void CDataValuePropertiesDlg::OnSelchangeComboVariable() 
{
	CString Buff;

	m_pFeature->nDataValueType = m_comboVariable.GetCurSel();
	if(m_pFeature->pCaption==NULL)  m_pFeature->pCaption = new CString;
	m_pFeature->pCaption->Format(_T("%s"),*theApp.m_pDescription[m_pFeature->nDataValueType].pCaption);
	m_pFeature->nCaptionPosition=0;
	m_pParent->Invalidate(FALSE);
	
}

void CDataValuePropertiesDlg::OnButtonFont() 
{
	CClientDC dc(this);
	CString Buff;

	CFontDialog dlg(&m_pFeature->lf);
	dlg.m_cf.rgbColors = m_pFeature->rgbText;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&m_pFeature->lf);
		m_pFeature->rgbText = dlg.GetColor();

		m_pParent->Invalidate(FALSE);
	}
	
}

void CDataValuePropertiesDlg::SetIndex(int nIndex)
{
}
