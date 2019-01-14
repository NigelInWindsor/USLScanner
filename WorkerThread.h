//WorkerThread.h
#ifndef _WORKERTHREAD_H
#define _WORKERTHREAD_H

#include <afxtempl.h>
#include "Constants.h"

struct BACKGNDINFO
{	
	BOOL bMulti;
	CWnd* pMainWnd;
	BOOL bStopBkgnd;
	int nEventRate;
};


class CWorkerThread
{
public:
	bool IsConnected();
	bool IsCollectingData();
	bool AddCloudCoordinate();
	void GetToolRotateAngles(D3DXVECTOR3 *pvToolRotate, D3DXVECTOR3 *pvRaw);
	void GetFrameRotateAngles(D3DXVECTOR3 *pvFrameRotate, D3DXVECTOR3 *pvRaw);
	void DeleteElements(int nStartIndex, int nFinishIndex);
	BOOL AddCoordinate();
	void InitializeFrameMatrix();
	void RotateToFrameNorm(D3DXVECTOR3 *vNormOut,D3DXVECTOR3 *vNormIn);
	void GetToolCoordinate(D3DXVECTOR3 *pvTool, D3DXVECTOR3 *pvRaw);
	void GetToolCoordinate(CUSLVertex &pvTool, CUSLVertex &pvRaw);
	void GetFrameCoordinate(D3DXVECTOR3 *pvFrame, D3DXVECTOR3 *pvRaw);
	void GetFrameCoordinate(CUSLVertex &pvFrame, CUSLVertex &pvRaw);
	int	GetRawXYZCoordinate(D3DXVECTOR3 *pvRaw);
	int GetRawXYZCoordinate(float *pfX,float *pfY,float *pfZ);
	int GetAZDCoordinate(D3DXVECTOR3 *pvRaw);
	int GetAZDCoordinate(float *pfAzimuth,float *pfZenith,float *pfDistance);
	

	void CreateThread();
	static UINT StartThread(LPVOID pParam);
	void RunThread();

	void OnConnect(WCHAR* IpAddress);
	void OnDisconnect();

	void ExecuteCommandSequence();

	void StartSearch();
	void StartLevelMeasure();

	void Abort();

	void OnAvailableApplications(CList<WCHAR*, WCHAR*> &lstApps);
	void OnRunApplication(WCHAR* appName);
	void OnStopApplication();	
	
	void OnStartMeasurements();
	void OnStopMeasurements();
	void OnStartForegndMeasurements();
	void OnStopForegndMeasurements();
	void OnStartBackgndMeasurements();	
	void OnStopBackgndMeasurements();

	
	D3DXMATRIXA16 m_matFrame;
	D3DXVECTOR3	m_vFrameRotate;
	D3DXVECTOR3	m_vFrameNorm;
	D3DXVECTOR3	m_vFrameOrigin;

	D3DXVECTOR3	m_vToolRotate;
	D3DXVECTOR3	m_vToolNorm;
	D3DXVECTOR3	m_vToolOrigin;


	int	m_nCount;
	int	m_nElementL;
	CVertexArray m_Element[1000];
	bool m_bCollectingBackGround;
	bool m_bConnected;
	bool	m_bNewValueAvailable;
	double m_dAzimuth;
	double m_dZenith;
	double m_dDistance;
	double m_dLevelRX;
	double m_dLevelRY;
	double m_dLevelRZ;
	int		m_nStatus;
	CString	m_strMessage;
	int		m_nInitializationStatus;
private:
	void PostMsgToUpdateStatus(WCHAR* txt);

	CWinThread* m_pWorkerThread;
	CWinThread* m_pBkgndThread;
	TRACKERCOMMANDS tCommand;
	HANDLE m_hSuspendResumeEvent;
	HANDLE m_hCommandDoneEvent;
	CWnd* m_pMyMainWnd;
	WCHAR* m_pIpAdd;
	CList<WCHAR*, WCHAR*>m_Appslist;
	WCHAR* m_AppName;
	const CHAR* m_CharAppName;
	BOOL bMulti;	
	BOOL bStopBkgnd;
	int nEventRate;
	BACKGNDINFO backGndInfo;
	BOOL m_bCommandEventSet;
};
#endif
