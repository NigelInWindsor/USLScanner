// EditSpinItem.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EditSpinItem.h"
#include <Math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSpinItem dialog


CEditSpinItem::CEditSpinItem(CWnd* pParent /*=NULL*/ , CWnd** pDlg,CRect* pRect,int *pnFontHeight)
	: CDialog(CEditSpinItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditSpinItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pnValue=NULL;
	m_pdValue=NULL;
	m_nVariableType=0;
	m_Rect=pRect;
	m_pParent = pParent;
	ASSERT(pDlg!=NULL);
	m_pDlg = pDlg;
	m_pFont = NULL;
	if(pnFontHeight != NULL) {
		m_nFontHeight = *pnFontHeight;
	} else {
		m_nFontHeight = -11;
	}

}

CEditSpinItem::~CEditSpinItem()
{
	SAFE_DELETE( m_pFont );
}

void CEditSpinItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditSpinItem)
	DDX_Control(pDX, IDC_COMBO, m_comboValue);
	DDX_Control(pDX, IDC_SPIN, m_spinValue);
	DDX_Control(pDX, IDC_EDIT, m_editValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditSpinItem, CDialog)
	//{{AFX_MSG_MAP(CEditSpinItem)
	ON_EN_CHANGE(IDC_EDIT, OnChangeEdit)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSpinItem message handlers

BOOL CEditSpinItem::OnInitDialog() 
{
	int	ii;
	CDialog::OnInitDialog();
	CRect	rectCtrl, rectSpin, rectEdit, rr;

	GetWindowRect(&rectCtrl);
	m_editValue.GetWindowRect(&rectEdit);
	m_spinValue.GetWindowRect(&rectSpin);
	ScreenToClient(&rectSpin);

	CreateFont();

	CString	Buff;

	SetWindowPos( &wndTop , m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(), NULL);

	m_editValue.SetWindowPos(NULL,0,0,m_Rect.Width(),m_Rect.Height(),NULL);
	m_spinValue.SetWindowPos(m_spinValue.GetBuddy(),m_Rect.Width()-m_Rect.Height(),0,m_Rect.Height(),m_Rect.Height(),NULL);
	m_comboValue.SetWindowPos(NULL,0,0,m_Rect.Width(),m_Rect.Height(),NULL);

	m_editValue.SetFont(m_pFont);
	m_spinValue.SetFont(m_pFont);
	m_comboValue.SetFont(m_pFont);

	switch(m_nVariableType) {
	case 0: Buff.Format(_T("%d"),*m_pnValue);
		m_editValue.SetWindowText(Buff);
		m_spinValue.SetRange32(m_nMin,m_nMax);
		m_spinValue.SetPos(*m_pnValue);
		m_comboValue.ShowWindow(FALSE);
		break;
	case 1:
		Buff.Format(m_Format,*m_pdValue);
		m_editValue.SetWindowText(Buff);
		m_nMin=(int)(m_dMin/m_dStep);
		m_nMax=(int)(m_dMax/m_dStep);
		m_spinValue.SetRange(0,m_nMax-m_nMin);
		m_spinValue.SetPos((int)((*m_pdValue-m_dMin)/m_dStep));
		m_comboValue.ShowWindow(FALSE);
		break;
	case 2: Buff.Format(m_Format,((double)*m_pnValue)/m_dCoefficient);
		m_editValue.SetWindowText(Buff);
		(m_nMax>32000) ? m_nStep=10 : m_nStep=1;
		m_spinValue.SetRange32(m_nMin/m_nStep,m_nMax/m_nStep);
		m_spinValue.SetPos(*m_pnValue/m_nStep);
		m_comboValue.ShowWindow(FALSE);
		break;
	case 3:
		for(ii=0;ii<m_nNumberItems;ii++) {
			m_comboValue.AddString(m_pBuff[ii]);
		}
		m_comboValue.SetCurSel(*m_pnValue);
		m_editValue.ShowWindow(FALSE);
		m_spinValue.ShowWindow(FALSE);
		break;
	case 4: Buff.Format(_T("%s"),m_pStr);
		m_editValue.SetWindowText(Buff);
		m_spinValue.SetWindowPos(m_spinValue.GetBuddy(), m_Rect.Width(), 0, m_Rect.Height(), m_Rect.Height(), NULL);
		m_spinValue.ShowWindow(FALSE);
		m_comboValue.ShowWindow(FALSE);
		break;
	case 5:
		Buff.Format(m_Format,*m_pfValue);
		m_editValue.SetWindowText(Buff);
		m_nMin=(int)(m_dMin/m_dStep);
		m_nMax=(int)(m_dMax/m_dStep);
		m_spinValue.SetRange(0,m_nMax-m_nMin);
		m_spinValue.SetPos((int)((*m_pfValue-m_dMin)/m_dStep));
		m_comboValue.ShowWindow(FALSE);
		break;
	case 6:
		Buff=m_pBuff->GetBuffer(256);
		m_editValue.SetWindowText(Buff);
		m_spinValue.ShowWindow(FALSE);
		m_comboValue.ShowWindow(FALSE);
		break;
	case 7:
		for(ii=0;ii<m_nNumberItems;ii++) {
			m_comboValue.AddString(m_pBuff[ii]);
		}
		m_comboValue.SetCurSel((int)*m_pcValue);
		m_editValue.ShowWindow(FALSE);
		m_spinValue.ShowWindow(FALSE);
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditSpinItem::Initialize(int *pnValue,int nMin,int nMax,int nStep)
{
	CString Buff;

	m_pnValue = pnValue;
	m_nMin=nMin;
	m_nMax=nMax;
	m_nVariableType=0;
}

void CEditSpinItem::Initialize(int *pnValue,int nMin,int nMax,int nStep,double dCoefficient,WCHAR *Format)
{
	CString Buff;

	m_pnValue = pnValue;
	m_nMin=nMin;
	m_nMax=nMax;
	m_dCoefficient=dCoefficient;
	m_nVariableType=2;
	wcscpy_s(m_Format,Format);
}

void CEditSpinItem::Initialize(double *pdValue,double dMin,double dMax,double dStep,WCHAR *Format)
{
	CString	Buff;

	m_pdValue = pdValue;
	m_dMin=dMin;
	m_dMax=dMax;
	m_dStep=dStep;
	m_nVariableType=1;
	wcscpy_s(m_Format,Format);
}

void CEditSpinItem::Initialize(int *pnValue,CString* Buff,int nNumberItems)
{

	m_pnValue = pnValue;
	m_nVariableType=3;
	m_pBuff=Buff;
	m_nNumberItems=nNumberItems;
}

void CEditSpinItem::Initialize(WCHAR *pStr)
{
	m_nVariableType=4;
	m_pStr=pStr;

}


void CEditSpinItem::Initialize(float *pfValue,float fMin,float fMax,float fStep,WCHAR *Format)
{
	CString	Buff;

	m_pfValue = pfValue;
	m_dMin=fMin;
	m_dMax=fMax;
	m_dStep=fStep;
	m_nVariableType=5;
	wcscpy_s(m_Format,Format);
}

void CEditSpinItem::Initialize(CString *pBuff)
{
	m_nVariableType=6;
	m_pBuff=pBuff;

}

void CEditSpinItem::Initialize(unsigned char *pcValue,CString* Buff,int nNumberItems)
{

	m_pcValue = pcValue;
	m_nVariableType=7;
	m_pBuff=Buff;
	m_nNumberItems=nNumberItems;
}

void CEditSpinItem::OnChangeEdit() 
{
	CString	Buff;
	double dTemp;
	float	fTemp;

	m_editValue.GetWindowText(Buff);
	switch(m_nVariableType) {
	case 0: *m_pnValue=_ttoi(Buff);
		if(*m_pnValue<m_nMin) *m_pnValue=m_nMin;
		if(*m_pnValue>m_nMax) *m_pnValue=m_nMax;
		m_spinValue.SetPos(*m_pnValue);
		m_pParent->SendMessage(UI_ITEMCHANGED);
		break;
	case 1: 
		dTemp=*m_pdValue;
		_WTOD(Buff,*m_pdValue);
		if(*m_pdValue<m_dMin) *m_pdValue=m_dMin;
		if(*m_pdValue>m_dMax) *m_pdValue=m_dMax;
		dTemp=Round(dTemp,3);
		*m_pdValue=Round(*m_pdValue,3);
		if(dTemp != *m_pdValue) {
			m_spinValue.SetPos((int)((*m_pdValue-m_dMin)/m_dStep));
			m_pParent->SendMessage(UI_ITEMCHANGED);
		}
		break;
	case 2: _WTOD(Buff,dTemp);
		*m_pnValue=(int)(dTemp*m_dCoefficient);
		if(*m_pnValue<m_nMin) *m_pnValue=m_nMin;
		if(*m_pnValue>m_nMax) *m_pnValue=m_nMax;
		m_spinValue.SetPos(*m_pnValue/m_nStep);
		m_pParent->SendMessage(UI_ITEMCHANGED);
		break;
	case 4:
		swprintf_s(m_pStr,100,_T("%s"),(LPCWSTR)Buff);
		m_pParent->SendMessage(UI_ITEMCHANGED);
		break;
	case 5: 
		fTemp=*m_pfValue;
		
		_WTOF(Buff,*m_pfValue);
		if(*m_pfValue<m_dMin) *m_pfValue=(float)m_dMin;
		if(*m_pfValue>m_dMax) *m_pfValue=(float)m_dMax;
		fTemp=Round(fTemp,3);
		*m_pfValue=Round(*m_pfValue,3);
		if(fTemp != *m_pfValue) {
			m_spinValue.SetPos((int)((*m_pfValue-m_dMin)/m_dStep));
			m_pParent->SendMessage(UI_ITEMCHANGED);
		}
		break;
	case 6:
		m_pBuff->Format(_T("%s"),Buff);
		m_pParent->SendMessage(UI_ITEMCHANGED);
		break;
	}

}

void CEditSpinItem::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CString	Buff;
	int	nTemp;
	int	nStep;
	double dTemp;
	double fTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN:
		nTemp=pSpin->GetPos()&0xffff;
		switch(m_nVariableType) {
		case 0:
			if(*m_pnValue-nTemp) {
				*m_pnValue=nTemp;
				Buff.Format(_T("%d"),*m_pnValue);
				m_editValue.SetWindowText(Buff);
				m_pParent->SendMessage(UI_ITEMCHANGED);
			}
			break;
		case 1:
			dTemp=*m_pdValue;
			nStep = (int)(1.0/m_dStep);
			nTemp+=m_nMin;
			*m_pdValue=(double)nTemp*m_dStep;
			if(dTemp != *m_pdValue) {
				Buff.Format(m_Format,*m_pdValue);
				m_editValue.SetWindowText(Buff);
				m_pParent->SendMessage(UI_ITEMCHANGED);
			}
			break;
		case 2:	*m_pnValue=nTemp*m_nStep;
			Buff.Format(m_Format,((double)*m_pnValue)/m_dCoefficient);
			break;
		case 5:
			fTemp=*m_pfValue;
			nStep = (int)(1.0/m_dStep);
			nTemp+=m_nMin;
			*m_pfValue=(float)((double)nTemp*m_dStep);
			if(fTemp != *m_pfValue) {
				Buff.Format(m_Format,*m_pfValue);
				m_editValue.SetWindowText(Buff);
				m_pParent->SendMessage(UI_ITEMCHANGED);
			}
			break;
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}



BOOL CEditSpinItem::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==0x100) {

		switch(pMsg->wParam) {
		case 13:
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMFINISHED);
			SendMessage(WM_CLOSE);
//			if(m_pDlg) *m_pDlg = NULL;
			return TRUE;
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CEditSpinItem::OnSelchangeCombo() 
{

	switch(m_nVariableType) {
	case 3:	*m_pnValue=m_comboValue.GetCurSel();
		break;
	case 7:	*m_pcValue=(unsigned char)m_comboValue.GetCurSel();
		break;
	}
	m_pParent->SendMessage(UI_ITEMCHANGED);
	m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMFINISHED);

}

void CEditSpinItem::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CEditSpinItem::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	DestroyWindow();
}

void CEditSpinItem::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pDlg) *m_pDlg = NULL;		
}

double CEditSpinItem::Round(double dValue, int nSd)
{
	int	nValue;

	switch(nSd) {
	case 1:	nValue = (int)(dValue * 10.0);
		dValue = (double)nValue / 10.0;
		break;
	case 2:	nValue = (int)(dValue * 100.0);
		dValue = (double)nValue / 100.0;
		break;
	case 3:	nValue = (int)(dValue * 1000.0);
		dValue = (double)nValue / 1000.0;
		break;
	case 4:	nValue = (int)(dValue * 10000.0);
		dValue = (double)nValue / 10000.0;
		break;
	}

	return dValue;
}

float CEditSpinItem::Round(float dValue, int nSd)
{
	int	nValue;

	switch(nSd) {
	case 1:	nValue = (int)(dValue * 10.0f);
		dValue = (float)nValue / 10.0f;
		break;
	case 2:	nValue = (int)(dValue * 100.0f);
		dValue = (float)nValue / 100.0f;
		break;
	case 3:	nValue = (int)(dValue * 1000.0f);
		dValue = (float)nValue / 1000.0f;
		break;
	case 4:	nValue = (int)(dValue * 10000.0f);
		dValue = (float)nValue / 10000.0f;
		break;
	}

	return dValue;
}

void CEditSpinItem::CreateFont()
{
	SAFE_DELETE( m_pFont );

	MinMax(&theApp.m_LastSettings.nDataPropertiesFontHeight,0,11);

	CFont *pFont = m_pParent->GetFont();
	pFont->GetLogFont(&m_LogFont);

	LOGFONT lf = m_LogFont;
	lf.lfHeight = m_nFontHeight;
	theApp.SetFontLanguage(&lf);
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&lf);

}

