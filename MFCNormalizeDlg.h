#pragma once
class CMFCNormalizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMFCNormalizeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMFCNormalizeDlg(CWnd* pParent = NULL);
	virtual ~CMFCNormalizeDlg();

	enum { IDD = IDD_NORMALIZE_PAGE };

protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:

};

class CDialogPane : public CDockablePane
{
	DECLARE_DYNAMIC(CDialogPane)
	DECLARE_MESSAGE_MAP()
public:
	CDialogPane();
	virtual ~CDialogPane();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CMFCNormalizeDlg m_wndDlg;
};

