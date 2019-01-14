// ListItemEdit.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ListItemEdit.h"
#include "afxdialogex.h"


// CListItemEdit dialog

IMPLEMENT_DYNAMIC(CListItemEdit, CDialogEx)

CListItemEdit::CListItemEdit(CWnd* pParent, CWnd** pDlg, CListCtrl *pListCtrl, LPNMITEMACTIVATE pNMItemActivate, CRect *pRect)
	: CDialogEx(CListItemEdit::IDD, pParent)
{
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pListCtrl = pListCtrl;
	m_pNMItemActivate = pNMItemActivate;
	m_pFont = NULL;
	m_pRect = pRect;
}

CListItemEdit::~CListItemEdit()
{
	SAFE_DELETE(m_pFont);
}

void CListItemEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_comboValue);
	DDX_Control(pDX, IDC_EDIT_ITEM, m_editValue);
	DDX_Control(pDX, IDC_SPIN_ITEM, m_spinValue);
}


BEGIN_MESSAGE_MAP(CListItemEdit, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ITEM, &CListItemEdit::OnDeltaposSpinItem)
	ON_EN_CHANGE(IDC_EDIT_ITEM, &CListItemEdit::OnChangeEditItem)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM, &CListItemEdit::OnSelchangeComboItem)
END_MESSAGE_MAP()


// CListItemEdit message handlers


void CListItemEdit::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::PostNcDestroy();
	delete this;
}

void CListItemEdit::OnClose()
{
	m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMFINISHED, 0);

	CDialogEx::OnClose();
	DestroyWindow();
}

void CListItemEdit::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pDlg) *m_pDlg = NULL;
}

BOOL CListItemEdit::Create(int nIID, CWnd* pParentWnd, float* pfValue, float fMin, float fMax, float fStep, CString Format)
{
	m_pfValue = pfValue;
	m_fMin = fMin;
	m_fMax = fMax;
	m_fStep = fStep;
	m_Format = Format;
	m_Type = FLOAT;

	return CDialogEx::Create(nIID, pParentWnd);
}

BOOL CListItemEdit::Create(int nIID, CWnd* pParentWnd, double* pdValue, float fMin, float fMax, float fStep, CString Format)
{
	m_pdValue = pdValue;
	m_fMin = fMin;
	m_fMax = fMax;
	m_fStep = fStep;
	m_Format = Format;
	m_Type = DOUBLE;

	return CDialogEx::Create(nIID, pParentWnd);
}

BOOL CListItemEdit::Create(int nIID, CWnd* pParentWnd, int* pnValue, int nMin, int nMax, int nStep, CString Format)
{
	m_pnValue = pnValue;
	m_nMin = nMin;
	m_nMax = nMax;
	m_nStep = nStep;
	m_Format = Format;
	m_Type = INT;

	return CDialogEx::Create(nIID, pParentWnd);
}

BOOL CListItemEdit::Create(int nIID, CWnd* pParentWnd, int *pnValue, CString* Buff, int nNumberItems)	//Combo
{

	m_pnValue = pnValue;
	m_Type = COMBO;
	m_pComboStrings = Buff;
	m_nNumberItems = nNumberItems;

	return CDialogEx::Create(nIID, pParentWnd);
}

BOOL CListItemEdit::Create(int nIID, CWnd* pParentWnd, WCHAR *pwcArray, int nMaxLength, CString Format)
{
	m_Type = _WCHAR;
	m_pwcArray = pwcArray;
	m_nMaxArrayLength = nMaxLength;
	m_Format = Format;

	return CDialogEx::Create(nIID, pParentWnd);
}

BOOL CListItemEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect	rectCtrl, rectSpin, rectEdit, rr;
	int ii;

	GetWindowRect(&rectCtrl);
	m_editValue.GetWindowRect(&rectEdit);
	m_spinValue.GetWindowRect(&rectSpin);
	ScreenToClient(&rectSpin);

	CreateFont();

	CString	Buff;
	if (m_pListCtrl) {
		m_pListCtrl->GetWindowRect(rectCtrl);
		m_pListCtrl->GetSubItemRect(m_pNMItemActivate->iItem, m_pNMItemActivate->iSubItem, LVIR_BOUNDS, rr);
		rr.OffsetRect(rectCtrl.left, rectCtrl.top);
		rr.InflateRect(-1, 0, -1, 0);
	}
	else {
		if (m_pRect) {
			rr = *m_pRect;
		}
	}

	SetWindowPosition(rr);

	m_editValue.SetFont(&m_Font);
	m_spinValue.SetFont(&m_Font);
	m_comboValue.SetFont(&m_Font);

	switch (m_Type) {
	case INT: 
		m_comboValue.ShowWindow(FALSE);
		m_editValue.ShowWindow(true);
		m_spinValue.ShowWindow(true);
		m_spinValue.SetBuddy(&m_editValue);
		m_spinValue.SetRange32(m_nMin, m_nMax);
		m_spinValue.SetPos(*m_pnValue);
		Buff.Format(_T("%d"), *m_pnValue);
		m_editValue.SetWindowText(Buff);
		break;
	case FLOAT:
		m_comboValue.ShowWindow(FALSE);
		m_editValue.ShowWindow(true);
		m_spinValue.ShowWindow(true);
		m_spinValue.SetBuddy(&m_editValue);
		m_nMin = (int)(m_fMin / m_fStep);
		m_nMax = (int)(m_fMax / m_fStep);
		m_spinValue.SetRange(0, m_nMax - m_nMin);
		m_spinValue.SetPos((int)((*m_pfValue - m_fMin) / m_fStep));
		Buff.Format(m_Format, *m_pfValue);
		m_editValue.SetWindowText(Buff);
		break;
	case DOUBLE:
		m_comboValue.ShowWindow(FALSE);
		m_editValue.ShowWindow(true);
		m_spinValue.ShowWindow(true);
		m_spinValue.SetBuddy(&m_editValue);
		m_nMin = (int)(m_fMin / m_fStep);
		m_nMax = (int)(m_fMax / m_fStep);
		m_spinValue.SetRange(0, m_nMax - m_nMin);
		m_spinValue.SetPos((int)((*m_pdValue - (double)m_fMin) / (double)m_fStep));
		Buff.Format(m_Format, *m_pdValue);
		m_editValue.SetWindowText(Buff);
		break;
	case COMBO:
		m_comboValue.ShowWindow(true);
		m_editValue.ShowWindow(false);
		m_spinValue.ShowWindow(false);
		for (ii = 0; ii<m_nNumberItems; ii++) {
			m_comboValue.AddString(m_pComboStrings[ii]);
		}
		m_comboValue.SetCurSel(*m_pnValue);
		break;
	case _WCHAR:
		m_comboValue.ShowWindow(FALSE);
		m_editValue.ShowWindow(true);
		m_spinValue.ShowWindow(FALSE);
		Buff.Format(m_Format, m_pwcArray);
		m_editValue.SetWindowText(Buff);
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CListItemEdit::SetWindowPosition(CRect rr)
{

	SetWindowPos(&wndTop, rr.left, rr.top, rr.Width(), rr.Height(), NULL);
	m_editValue.SetWindowPos(NULL, 0, 0, rr.Width(), rr.Height(), NULL);
	m_spinValue.SetWindowPos(m_spinValue.GetBuddy(), rr.Width() - rr.Height(), 0, rr.Height(), rr.Height(), NULL);
	m_comboValue.SetWindowPos(NULL, 0, 0, rr.Width(), rr.Height(), NULL);
	m_rrWnd = rr;
}

void CListItemEdit::CreateFont()
{
	::DeleteObject(m_Font.Detach());

	if (theApp.m_LastSettings.bUseAlernativeToSystemFont == false) {
		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;

		m_Font.CreateFontIndirect(&lf);
	}
	else {
		m_Font.CreateFontIndirect(&theApp.m_LastSettings.lfMenus);
	}

}


void CListItemEdit::OnDeltaposSpinItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString Buff;
	float fTemp;
	int nTemp;
	double dTemp;

	switch (m_Type) {
	case INT: nTemp = *m_pnValue;
		*m_pnValue += pNMUpDown->iDelta;
		MinMax(m_pnValue, m_nMin, m_nMax);
		if (nTemp - *m_pnValue) {
			Buff.Format(m_Format, *m_pnValue);
			m_editValue.SetWindowText(Buff);
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		};
		break;

	case FLOAT:
		fTemp = *m_pfValue;
		*m_pfValue += ((float)pNMUpDown->iDelta * m_fStep);
		MinMax(m_pfValue, m_fMin, m_fMax);
		if (fTemp - *m_pfValue) {
			Buff.Format(m_Format, *m_pfValue);
			m_editValue.SetWindowText(Buff);
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		};
		break;

	case DOUBLE:
		dTemp = *m_pdValue;
		*m_pdValue += ((double)pNMUpDown->iDelta * (double)m_fStep);
		MinMax(m_pdValue, (double)m_fMin, (double)m_fMax);
		if (dTemp - *m_pdValue) {
			Buff.Format(m_Format, *m_pdValue);
			m_editValue.SetWindowText(Buff);
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		};
		break;
	}

	*pResult = 0;
}






void CListItemEdit::OnChangeEditItem()
{
	CString Buff;
	float fTemp;
	int nTemp;
	double dTemp;

	m_editValue.GetWindowText(Buff);

	switch (m_Type) {
	case INT: nTemp = *m_pnValue;
		*m_pnValue = _ttoi(Buff);
		MinMax(m_pnValue, m_nMin, m_nMax);
		if (nTemp - *m_pnValue) {
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		};
		break;

	case FLOAT:
		fTemp = *m_pfValue;
		_WTOF(Buff, *m_pfValue);
		MinMax(m_pfValue, m_fMin, m_fMax);
		if (fTemp - *m_pfValue) {
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		};
		break;

	case DOUBLE:
		dTemp = *m_pdValue;
		_WTOD(Buff, *m_pdValue);
		MinMax(m_pdValue, (double)m_fMin, (double)m_fMax);
		if (dTemp - *m_pdValue) {
			m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		};
		break;

	case _WCHAR:
		swprintf_s(m_pwcArray, m_nMaxArrayLength, L"%s", (LPCWSTR)Buff);
		m_pParent->SendMessage(UI_ITEMCHANGED, WM_ITEMCHANGED);
		break;
	}


}


void CListItemEdit::OnSelchangeComboItem()
{
	switch (m_Type) {
	case COMBO:	*m_pnValue = m_comboValue.GetCurSel();
		break;
	}

	m_pParent->SendMessage(UI_ITEMCHANGED , WM_ITEMCHANGED, 0);
}


BOOL CListItemEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == 0x100) {

		switch (pMsg->wParam) {
		case 13:
			SendMessage(WM_CLOSE);
			return TRUE;
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
