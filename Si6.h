// Si6.h: interface for the CSi6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SI6_H__8D87A36F_816A_44D3_90C0_3BC2E7CB319D__INCLUDED_)
#define AFX_SI6_H__8D87A36F_816A_44D3_90C0_3BC2E7CB319D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Stuff Added By Vince
//Stepping Motors  Axis
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_ROT 3
#define PI 3.14159
#define PIf 3.14159f
#define AXISNUMBER 4



class CSi6  
{
public:
	void Retrieve(CUSLFile *pFile);
	int Save(CUSLFile* pFile,int nOffset=0);
	void SetRevCounter(int nValue);
	int GetRevCounter();
	void StartContinuousRotate();
	void GetRollerPos(COORDDATA *pCp);
	void GetTurntableCoord(COORDDATA *pCp);
	float ReturnCosine(float dAngle);
	void SetScanSpeed(float dScanSpeed,bool bArchive = TRUE);
	void ResetMaxSpeed();
	void StopCollectingData();
	void CollectData(int nWhichAxis,float dStart,float dFinish,float dIncrement);
	void GetCoord( COORDDATA *pCp);
	void MoveToSafely( COORDDATA CpMoveTo);
	void MoveToHomePos();
	void MoveToCoord(HoldingFixtureCoord Cp);
	void MoveToCoord( COORDDATA cp);
	int ReadLimitSwitches();
	void EnableHomeFind(int nAxis = -1);
	bool TestAxisRunning(int nWhich);
	BOOL FreeAndDeallocateDataMemory();
	BOOL AllocateAndShareDataMemory();
	bool StartDriver();
	CSi6();
	virtual ~CSi6();
	void Initialize();
	BOOL GetHandleToDevice(int OS);
	HANDLE GetDeviceViaInterface( GUID* pGuid, DWORD instance);
	WORD Inp(int nOffset);
	WORD Inpw(int nOffset);
	void Outp(int nOffset, int nValue);
	void GetVersion(CString* Version);
	void GetVersion(char *Version,int Length);
	void EnableInterrupts();
	void DisableInterrupts();
	BOOL UserSi6PtrToDriver();
	BOOL ShareUserSi6Struct();
	BOOL FreeUserSi6Struct();
	BOOL ShareTimeSlotStruct();
	BOOL FreeTimeSlotStruct();
	LONG GetIrqCount();
	void CounterStart(int number,int value);
	void EnableAxis(int nWhich,int action);
	void DisableAxis(int nWhich);
	void CounterInterruptMask(int irq,int action);
	void EnableJoystick();
	void StopAllMotorsDead();
	void AxisDefaults(int nWhich);
	int	ReadEncoder(struct AxisData *axis);
	int ReadJoystick(int nWhich);
	void ZeroPosition(int nWhich);
	void EncoderZero(struct AxisData *pAxis);
	void EncoderResetMode(struct AxisData *pAxis);

	//Methods Added by Vince
	/////////////////////////////////////////////////
	BOOL IsAxisInPosition(int nWhich, int Pos);
	bool TestAllAxisRunning();
	float FromCurrentPosToUnit(int nWhich);
	float FromPosToUnit(int nWhich, int Pos, int UnitSelected = -1);
	int	FromUnitToPos(int nWhich, float UnitPos, int UnitSelected = -1);
	int ReadJoystickPourcent(int nWhich);


	int FromVelocityUnitToMotor();
	int FromVelocityMotorToUnit();


	HANDLE	m_hSi6;
	struct	SI6Data	m_Si6;
	char	Array[50000];
	int		m_nCorrectMaxSpeed;

};

#define	SI6_CLK_PORT			0
#define	SI6_DIR_PORT			1
#define	SI6_CLK_ENABLE			2
#define	SI6_INT_CLR				3
#define	SI6_PIC					4
#define	SI6_COUNT_0				8
#define	SI6_COUNT_1				9
#define	SI6_COUNT_2				0x0a
#define	SI6_COUNT_3				0x0c
#define	SI6_COUNT_4				0x0d
#define	SI6_COUNT_5				0x0e
#define	SI6_CNTRL_012			0x0b
#define	SI6_CNTRL_345			0x0f
#define	SI6_JOYSTICK			0x10
#define	SI6_PIA					0x14
#define	SI6_ENCODER_SELECT		0x18
#define	SI6_ENCODER_MODE		0x19

#define	PIC_CNTRL				0x20
#define	PIC_CNTRL_BYTE			0x20
#define	PIC_MASK				0x21
#define	PIC2_CNTRL				0xa0
#define	PIC2_CNTRL_BYTE			0xa0
#define	PIC2_MASK				0xa1
#define	PIC_NOP					0x40
#define	PIC_EOI					0x20
/*
#define	XAXIS					0
#define	YAXIS					1
#define	ZAXIS					2
#define	RAXIS					3
#define	XTAXIS					4
#define	YTAXIS					5
*/
#define	MOVETO					1
#define	HOMETO					2
#define	JOYSTICK				3

#define	DISABLE					0
#define	ENABLE					1





#endif // !defined(AFX_SI6_H__8D87A36F_816A_44D3_90C0_3BC2E7CB319D__INCLUDED_)
