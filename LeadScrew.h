// LeadScrew.h: interface for the CLeadScrew class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEADSCREW_H__C4D0452F_779D_41EC_AACE_5279C3772547__INCLUDED_)
#define AFX_LEADSCREW_H__C4D0452F_779D_41EC_AACE_5279C3772547__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Position {
	int	nStructSize;

	int	nActualPos;
	int	nActualNeg;
	int	nMachine;
	float fActualPos;
	float fActualNeg;
	float fMachine;
public:
	void Zero()
	{
		nActualPos = 0;
		nActualNeg = 0;
		nMachine = 0;
		fActualPos = 0.0f;
		fActualNeg = 0.0f;
		fMachine = 0.0f;
	};
};


class CLeadScrew  
{
public:
	int	GetSize();
	void FillTable(float fMin, float fMax, float fStepsize);
	void CopyMasterToNegativePositive();
	void CopyNegativeToPositive();
	void CopyPositiveToNegative();
	void InvalidateFloats();
	void InvalidateIntegers();
	void GetPos(Position *pPos);
	void Add(Position Pos);
	Position* GetPtrPosition(int nIndex);
	void ModifyPosition(int nIndex, Position *Pos);
	void GetPosition(int nIndex, Position *Pos);
	void Delete(int nIndex);
	void Add(int nMachine, int nActualPos, int nActualNeg);
	void Sort();
	Position* m_pPos;
	float m_fStepSize;
	int m_nCoordL;
	bool m_bOffOn;
	int	ActualToMachine(int nDesirePos, int nCurrentPos);
	int ActualPositiveToMachine(int nActualPos);
	int ActualNegativeToMachine(int nActualPos);
	int MachineToActualPos(int nMachinePos);
	int MachineToActualNeg(int nMachinePos);
	bool Retrieve(CUSLFile *pFile);
	bool Save(CUSLFile *pFile);
	void Zero();
	CLeadScrew();
	virtual ~CLeadScrew();

};

int ComparePositionData(const void *Cp0,const void *Cp1);

#endif // !defined(AFX_LEADSCREW_H__C4D0452F_779D_41EC_AACE_5279C3772547__INCLUDED_)
