#if !defined(AFX_MAINTENANCEDIAGNOSTICSSHEET_H__0AA2110A_A0DC_431D_9BD6_B97205CDB1FC__INCLUDED_)
#define AFX_MAINTENANCEDIAGNOSTICSSHEET_H__0AA2110A_A0DC_431D_9BD6_B97205CDB1FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenanceDiagnosticsSheet.h : header file
//
#include "ActivityLogPage.h"
#include "SerialJoystickDlg.h"
#include "ServoDrivesPage.h"
#include "PrinterDiagnosticsPage.h"
#include "EthernetPage.h"
#include "MaintenancePage.h"
#include "AcqirisDlg.h"
#include "FaroTrackerPage.h"
#include "ProtractorPage.h"
#include "RemotePage.h"
#include "MarkerPenPage.h"
#include "MachineMonitorPage.h"
#include "SI10Page.h"
#include "EgismosRangefinderPage.h"
#include "FLSC10Page.h"
#include "CV120Page.h"
#include "AOSPhasedArrayPage.h"
#include "PM40Page.h"

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDiagnosticsSheet

class CMaintenanceDiagnosticsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMaintenanceDiagnosticsSheet)

// Construction
public:
	CMaintenanceDiagnosticsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, CWnd** pDlg = NULL);
	CMaintenanceDiagnosticsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintenanceDiagnosticsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	CWnd** m_pDlg;
	inline void Construct(CWnd** pDlg);
	CSerialJoystickDlg*		m_pSerialJoystickDlg;
	CActivityLogPage*		m_pActivityLogPage;
	CServoDrivesPage*		m_pServoDrivesPage;
	CPrinterDiagnosticsPage* m_pPrinterDiagnosticsPage;
	CEthernetPage*			m_pEthernetPage;
	CMaintenancePage*		m_pMaintenancePage;
	CAcqirisDlg*			m_pAcqirisDlg;
	CFaroTrackerPage*		m_pFaroTrackerPage;
	CProtractorPage*		m_pProtractorPage;
	CRemotePage*            m_pRemotePage;
	CMarkerPenPage*			m_pMarkerPenPage;
	CMachineMonitorPage*	m_pMachineMonitorPage;
	CSI10Page*				m_pSI10Page;
	CEgismosRangefinderPage* m_pEgismosPage;
	CFLSC10Page*            m_pFLSC10Page;
	CPM40Page*				m_pPM40Page;
#ifdef NPTRACKINGTOOLS
	CV120Page*       m_pV120Page;
#endif
	CAOSPhasedArrayPage*	m_pAOSPhasedArrayPage;

	virtual ~CMaintenanceDiagnosticsSheet();
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CMaintenanceDiagnosticsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LRESULT RemoteHandlerFLSC10(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteHandlerEgismosRF(WPARAM Wp, LPARAM Lp);
	LRESULT RemotePageUpdateControls(WPARAM Wp, LPARAM Lp);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTENANCEDIAGNOSTICSSHEET_H__0AA2110A_A0DC_431D_9BD6_B97205CDB1FC__INCLUDED_)
