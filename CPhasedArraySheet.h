#pragma once
#include "ResizableSheet.h"
#include "PhasedArrayProbeDlg.h"
#include "AOSPhasedArrayUTPage.h"
#include "ParksMcClellanPage.h"

class CPhasedArraySheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CPhasedArraySheet)

public:
	CAOSPhasedArrayUTPage * m_pAOSPhasedArrayUTPage;
	CPhasedArrayProbeDlg* m_pPhasedArrayProbePage;
	CParksMcClellanPage* m_pParksMcClellanPage;
	CWnd**	m_pDlg;

	CPhasedArraySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPhasedArraySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, CWnd** pDlg = NULL);
	virtual ~CPhasedArraySheet();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};

