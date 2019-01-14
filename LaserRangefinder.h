// LaserRangefinder.h: interface for the CLaserRangefinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LASERRANGEFINDER_H__0F6F46F6_7F0D_4B13_8C9B_B131DF12F9CE__INCLUDED_)
#define AFX_LASERRANGEFINDER_H__0F6F46F6_7F0D_4B13_8C9B_B131DF12F9CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLaserRangefinder : public CSerial
{
public:
	CLaserRangefinder();
	virtual ~CLaserRangefinder();


	void ResetErrorCode();
	int GetLastLaserError();
	float GetLastDistance();
	
	virtual void Measure() = 0;
	virtual void MeasureCont() = 0;
	virtual void StopClear() = 0;
	virtual bool Initialize(CString strPortName);
	virtual bool IStringReceived();
	virtual bool GetStr(CString *pBuff);

	void WriteBytes(char* bytes, int length);
	void WriteBytes(BYTE* bytes, int length);
	void SetPort(int iPort);
	void SetPort(CString sPort);
	char* GetBytesSent(int* nBytes);
	
	CString GetReceived();
	CString GetData();
	
	bool Read();
	bool Start();
	bool Close();


	bool IsRunningContinuously();

protected:
	void SetRunningContinuously(bool val);
	CCriticalSection m_Crit;
	CString m_sHeader;
	CString m_sData;
	CString m_sError;
	int m_errorCode;
	float m_fDistance;
	CString m_sPort;

private:
	//Pure virtual functions that define how each rangefinder handles incoming messages.
	virtual void HandleMessage(CString buff) = 0;
	virtual void OnMeasurementReceived(CString meas) = 0;
	virtual void OnErrorReceived(CString sErr) = 0;
	//Defines the test for end of line byte 
	virtual void TestEndOfLineByte(BYTE val);
	virtual void ResetEndOfLineByte(BYTE val);
	virtual void ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered);

	//Threads
	typedef struct THREADSTRUCT
	{
		CLaserRangefinder* _this;
	}THREADSTRUCT;
	static UINT SerialPortReadThread(LPVOID pParam);
	void SuspendThread();
	CWinThread* m_hSerialPortReadThread;
	THREADSTRUCT *m_pParam;


	CString m_receiveBuff; //The received bytes
	wchar_t* pBuff;


	char* m_sendBuff; //Bytes sent
	int m_nBytesSent; 



	bool m_bRunningContinuously;

};

#endif // !defined(AFX_LASERRANGEFINDER_H__0F6F46F6_7F0D_4B13_8C9B_B131DF12F9CE__INCLUDED_)
