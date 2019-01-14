// ToolChangers.h: interface for the CToolChangers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLCHANGERS_H__AD735D0A_2E42_48C0_97C4_1FCA70CBF6C5__INCLUDED_)
#define AFX_TOOLCHANGERS_H__AD735D0A_2E42_48C0_97C4_1FCA70CBF6C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EndEffector.h"

class CToolChangers  
{
public:
	void			setProbeVector(int nIndex, D3DXVECTOR3* vProbeLength);
	void			getProbeVector(int nIndex, D3DXVECTOR3* vProbeLength);
	void			setArmVector(int nIndex, D3DXVECTOR3* vArmLength);
	void			getArmVector(int nIndex, D3DXVECTOR3* vArmLength);
	int				setType(int nIndex, int nType);
	int				getTypeFromID(int nID);
	int				getIndexFromID(int nID, int *nStatus);
	void			InvalidateKinematicWaterPath(int nSide);
	CString&		getWaterPath(int nIndex, float *fLength);
	float			getWaterPath(int nID);
	void			setWaterPath(int nIndex, float fEdit);
	float			getClampToolTipLength(int nID);
	void			InitializeTypeStrings();
	void			Delete(int nIndex);
	void			Sort(int nColumn, int nDir);
	void			SwapEndEffector(int j, int k);
	void			setClampToolTipLength(int nIndex, float fLength);
	CString&		getTypeFromID(int nID, int *nStatus);
	CString&		getClampToolTipLength(int nIndex, float *fLength);
	CString&		getDescriptionFromID(int nID, int *nStatus);
	void			getParkPos(int nIndex, CCoord *pCp);
	void			RefreshParkPosition(int nIndex);
	int				getSide(int nIndex);
	int				*getPtrSide(int nIndex);
	float			getLength(int nIndex);
	float			*getPtrLength(int nIndex);
	int				getUniqueID(int nIndex);
	int				*getPtrUniqueID(int nIndex);
	int				*getPtrType(int nIndex);
	CString&		getType(int nIndex);
	CString			*getPtrDescription(int nIndex);
	CString			&getDescription(int nIndex);
	void			Zero();
	int				AddEmptyEndEffector();
	int				getSideEndEffectorID(int nSide);
	CString&		getCoordAtParkPos(int nIndex,char cAxis);
	CString&		getCoordAtParkPos(int nIndex);
	CString&		getSideEndEffectorDescription(int nSide);
	void			setEndEffectorSize(int nSize);
	int				getEndEffectorSize();
	int				m_nEndEffectorSize;
	CEndEffector	m_EndEffector[32];
	int				m_nSideID[2];
	CString			m_strType[20];

	HRESULT Retrieve(CString FilePath);
	HRESULT Save(CString FilePath);
	HRESULT Retrieve(CUSLFile* pFile);
	HRESULT Save(CUSLFile* pFile);
	CToolChangers();
	virtual ~CToolChangers();

};

#endif // !defined(AFX_TOOLCHANGERS_H__AD735D0A_2E42_48C0_97C4_1FCA70CBF6C5__INCLUDED_)
