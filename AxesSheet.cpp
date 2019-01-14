// AxesSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AxesSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesSheet

IMPLEMENT_DYNAMIC(CAxesSheet, CPropertySheet)

CAxesSheet::CAxesSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CAxesSheet::CAxesSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** DlgPtr)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	bool bDisplayStepperStatus=TRUE;
	int nn;
	m_DlgPtr = DlgPtr;
	m_FlagEnableClose = TRUE;
	m_DialogCreatedFlag = FALSE;

	theApp.m_Tank.nXtLeftMaster=28;
	theApp.m_Tank.nXtRightMaster=29;
	theApp.m_Tank.nFingerLeftMaster=30;
	theApp.m_Tank.nFingerRightMaster=31;


	ZeroMemory(m_pAxis, sizeof m_pAxis);
	ZeroMemory(m_pAxisPmac, sizeof m_pAxisPmac);
	ZeroMemory(m_pPPMACPage, sizeof m_pPPMACPage);

	m_pAxesStatus=NULL;
	m_pSi6Page=NULL;
	m_pFokkerPumpPage=NULL;
	m_pMoveLadderPage=NULL;
	m_pAxesVirtualPage = NULL;


	switch(theApp.m_nMotorType) {
	case 1:
	case 2:
		break;
	case FIELD_BUS:
		CreateFieldBusSheet();
		return;
		break;
	}


	nn=0;
	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType == SERVO) {
				if((theApp.m_Tank.nScannerDescription==SINGLE_BRIDGE_TANK) || (theApp.m_Tank.nScannerDescription==SINGLE_BRIDGE_TANK_WITH_R) || (theApp.m_Tank.nScannerDescription == SINGLE_BRIDGE_TT_ROLLERS)){
//					wcscpy(theApp.m_Axes[theApp.m_Tank.nXLeft].cName,L"X");	
					m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_X, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXLeft);
				}else{
//					wcscpy(theApp.m_Axes[theApp.m_Tank.nXLeft].cName,L"X Left");	
					m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_X_AXIS_LEFT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXLeft);
				}
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_X_AXIS_LEFT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}
	
		if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable){	
			if(theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYLeft].cName,L"Y");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nZLeft].cName,L"Z");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nZLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nZLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType == SERVO) {
				if(theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA) {
//					wcscpy(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName,L"Rotate");
					m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_ROTATE, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXtLeft);
				}else{
//					wcscpy(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName,L"Xtip");
					m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXtLeft);
				}
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXtLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType == SERVO) {
				if(theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA) {
//					wcscpy(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName,L"Theta");
					m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_THETA, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYtLeft);
				}else{
//					wcscpy(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName,L"Ztip");
					m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_ZT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYtLeft);
				}
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_ZT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYtLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXRight].cName,L"X Right");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_X_AXIS_RIGHT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_X_AXIS_RIGHT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYRight].cName,L"Y");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

	
		if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nZRight].cName,L"Z");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nZRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nZRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXtRight].cName,L"Xtip");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXtRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXtRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYtRight].cName,L"Ztip");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_YT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYtRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_YT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYtRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nXBridge].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXBridge].cName,L"X Bridge");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_X_BRIDGE, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXBridge);
				AddPage(m_pAxisPmac[nn]);
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nRLeft].cName,L"Rotate");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_R, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nRLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_R, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nRLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable){
			if(theApp.m_Axes[theApp.m_Tank.nRRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nRLeft].cName,L"Rotate");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_ROLLER, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nRRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_ROLLER, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nRRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if( (theApp.m_Axes[theApp.m_Tank.nLeftPump].bAvailable) || (theApp.m_Axes[theApp.m_Tank.nRightPump].bAvailable) ) {
			m_pFokkerPumpPage = new CFokkerPumpPage();
			AddPage(m_pFokkerPumpPage);
		}
		break;
	case 1:
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXLeft].cName,L"X Right");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_X_AXIS_RIGHT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_X_AXIS_LEFT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYLeft].cName,L"Y");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}
	
		if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nZLeft].cName,L"Z");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nZLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nZLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName,L"Xtip");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXtLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXtLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName,L"Ztip");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_ZT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYtLeft);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_ZT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYtLeft);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXRight].cName,L"X Left");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_X_AXIS_LEFT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_X_AXIS_RIGHT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYRight].cName,L"Y");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Y, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nZRight].cName,L"Z");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nZRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_Z, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nZRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXtRight].cName,L"Xtip");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXtRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_XT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nXtRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nYtRight].cName,L"Ytip");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_YT, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nYtRight);
				AddPage(m_pAxisPmac[nn]);
			} else {
				m_pAxis[nn] = new CAxisDlg(IDS_AXIS_YT, this,(CWnd**) m_pAxis[nn],theApp.m_Tank.nYtRight);
				AddPage(m_pAxis[nn]);
				bDisplayStepperStatus = TRUE;
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nXBridge].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nXBridge].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nXBridge].cName,L"X Bridge");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_X_BRIDGE, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nXBridge);
				AddPage(m_pAxisPmac[nn]);
			}
			nn++;
		}

		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable){		
			if(theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType == SERVO) {
//				wcscpy(theApp.m_Axes[theApp.m_Tank.nRLeft].cName,L"Rotate");
				m_pAxisPmac[nn] = new CAxisPmacDlg(IDS_AXIS_R, this,(CWnd**) m_pAxisPmac[nn],theApp.m_Tank.nRLeft);
				AddPage(m_pAxisPmac[nn]);
			}
			nn++;
		}

		if( (theApp.m_Axes[theApp.m_Tank.nLeftPump].bAvailable) || (theApp.m_Axes[theApp.m_Tank.nRightPump].bAvailable) ) {
			m_pFokkerPumpPage = new CFokkerPumpPage();
			AddPage(m_pFokkerPumpPage);
		}
		break;
	}

	switch(theApp.m_nMotorType) {
	case 1:
	case 2:
	case FIELD_BUS:
		m_pAxesStatus = new CAxesPmacStatus();
		AddPage(m_pAxesStatus);
		break;
	}

	m_pMoveLadderPage = new CMoveLadderPage();
	AddPage( m_pMoveLadderPage );

	m_pAxesVirtualPage = new CAxesVirtualPage();
	AddPage( m_pAxesVirtualPage );
}

CAxesSheet::~CAxesSheet()
{
	for(int nn=0;nn<16; nn++) {
		SAFE_DELETE( m_pAxis[nn] );
		SAFE_DELETE( m_pAxisPmac[nn] );
		SAFE_DELETE( m_pPPMACPage[nn] );
	};
	SAFE_DELETE( m_pAxesStatus );
	SAFE_DELETE( m_pSi6Page );
	SAFE_DELETE( m_pFokkerPumpPage );
	SAFE_DELETE( m_pMoveLadderPage );
	SAFE_DELETE( m_pAxesVirtualPage );
}


void CAxesSheet::CreateFieldBusSheet()
{
	int nn=0;
	switch(theApp.m_nSide0Orientation) {
	case 0:
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nZLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXtLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYtLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nZRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXtRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYtRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nFingerLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nFingerLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nFingerLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nRLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nRLeft);
			nn++;
		}
		break;
	case 1:
		if((theApp.m_Axes[theApp.m_Tank.nXRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nZRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nZRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXtRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXtRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYtRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYtRight);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nRRight].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nZLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nZLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nXtLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nXtLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nYtLeft);
			nn++;
		}
		if((theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable) && (theApp.m_Axes[theApp.m_Tank.nYtLeft].nMotorType == SERVO)) {
			m_pPPMACPage[nn] = new CAxisPowerPmacPage(theApp.m_Tank.nRRight);
			nn++;
		}
		break;
	}

	for(int ii=0;ii<nn;ii++) {
		if(m_pPPMACPage[ii]) AddPage(m_pPPMACPage[ii]);
	}

	m_pAxesStatus = new CAxesPmacStatus();
	AddPage(m_pAxesStatus);

	m_pMoveLadderPage = new CMoveLadderPage();
	AddPage( m_pMoveLadderPage );

	if( (theApp.m_Axes[theApp.m_Tank.nLeftPump].bAvailable) || (theApp.m_Axes[theApp.m_Tank.nRightPump].bAvailable) ) {
		m_pFokkerPumpPage = new CFokkerPumpPage();
		AddPage(m_pFokkerPumpPage);
	}

}

BEGIN_MESSAGE_MAP(CAxesSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAxesSheet)
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesSheet message handlers

void CAxesSheet::PostNcDestroy() 
{

	CPropertySheet::PostNcDestroy();
	delete this;
}

void CAxesSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_AXES_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}
void CAxesSheet::OnDestroy() 
{
	//Store Window Position
	if (IDD_AXES_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_AXES_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_DlgPtr	= NULL;		
}

void CAxesSheet::OnMove(int x, int y) 
{
	CPropertySheet::OnMove(x, y);

	if (IDD_AXES_SHEET < MAXIMUMWINDOWSIDNUMBER&&m_DialogCreatedFlag){
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_AXES_SHEET]);
		if(m_pAxesStatus) {
			if(m_pAxesStatus->m_hWnd) m_pAxesStatus->SendMessage(WM_MOVE);
		}
	}
	
		
}

BOOL CAxesSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	if (IDD_AXES_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_AXES_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

//	int ii= GetPageCount ();
//	bool Flag = SetActivePage (0);
	

	Invalidate(TRUE);
	SetWindowText(_T("Servo Motor Axes"));
	m_DialogCreatedFlag = TRUE;
	SetTabNames();


	
	return bResult;	
}

void CAxesSheet::UpdateStatusSheet()
{

//	m_pAxesStatus->UpdateControls();
}

void CAxesSheet::UpdateAllPages()
{
	for(int nn=0; nn<16; nn++) {
		if(m_pAxisPmac[nn]!=NULL)	m_pAxisPmac[nn]->UpdateAllControls();
		if(m_pAxis[nn]!=NULL)		m_pAxis[nn]->UpdateAllControls();
		if(m_pPPMACPage[nn]!=NULL)	m_pPPMACPage[nn]->UpdateAllControls();
	};

}

void CAxesSheet::SetTabNames()
{
	CString Buff;
	TCITEM TabCtrlItem;
	CTabCtrl* tab = GetTabControl();

	for(int nIndex=0;nIndex<16;nIndex++) {
		if(m_pPPMACPage[nIndex]) {
			
			ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
			tab->GetItem(nIndex,&TabCtrlItem);
			TabCtrlItem.pszText = m_pPPMACPage[nIndex]->getTabName();
			TabCtrlItem.mask=TCIF_TEXT;
			tab->SetItem(nIndex,&TabCtrlItem);
		}
	}
}
