// EndEffector.h: interface for the CEndEffector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENDEFFECTOR_H__ADC5EC70_ED59_4F28_B15E_41A373397412__INCLUDED_)
#define AFX_ENDEFFECTOR_H__ADC5EC70_ED59_4F28_B15E_41A373397412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EndEffectorType {
	NOT_KNOWN = 0,
	SINGLE_PROBE,
	COAXIAL_PROBES,
	ADJACENT_PROBES,
	PHASED_ARRAY_FLAT,
	PHASED_ARRAY_CURVED,
	FINGER_MANIPULATOR,
	ARC_MULTI_POSITION,
	ROTATE_ABOUT_SINGLE_PROBE,
	ROTATE_ABOUT_COAXIAL_PROBE,
};

class CEndEffector  
{

public:
	CString		m_Description;
	CString		m_CarouselLabel;

	void operator = (CEndEffector* pEndEffector) {
		m_Description =			pEndEffector->m_Description;
		m_CarouselLabel =		pEndEffector->m_CarouselLabel;

		m_vectAtParkPosition =		pEndEffector->m_vectAtParkPosition;
		m_vParkPosition =		pEndEffector->m_vParkPosition;
		m_vectAtToolTip =			pEndEffector->m_vectAtToolTip;
		m_vClampToToolTip =		pEndEffector->m_vClampToToolTip;
		m_vToolTipRotate =		pEndEffector->m_vToolTipRotate;
		m_fOverallLength =		pEndEffector->m_fOverallLength;
		m_vClampToProbeRotate = pEndEffector->m_vClampToProbeRotate;
		m_vProbeLength =		pEndEffector->m_vProbeLength;

		CopyMemory(m_vProbePos, pEndEffector->m_vProbePos, sizeof m_vProbePos);
		m_Type =			pEndEffector->m_Type;
		m_nNumberProbes =	pEndEffector->m_nNumberProbes;
		m_nUniqueID =		pEndEffector->m_nUniqueID;
		m_nSide =			pEndEffector->m_nSide;
		m_fWp =				pEndEffector->m_fWp;

	};

public:
	D3DXVECTOR4	m_vectAtParkPosition;
	D3DXVECTOR3	m_vParkPosition;
	D3DXVECTOR4	m_vectAtToolTip;
	D3DXVECTOR3	m_vClampToToolTip;
	D3DXVECTOR4	m_vToolTipRotate;
	D3DXVECTOR3 m_vClampToProbeRotate;
	D3DXVECTOR3 m_vProbeLength;
	float		m_fOverallLength;
	float		m_fWp;

	D3DXVECTOR3		m_vProbePos[32];
	EndEffectorType	m_Type;
	int				m_nNumberProbes;
	int				m_nUniqueID;
	int				m_nSide;

	float		getWaterPath();
	void		setWaterPath(float fEdit);
	void		setClampToolTipLength(float fEdit);
	float		getClampToolTipLength();
	void		Zero();
	HRESULT		Retrieve(CString FilePath);
	HRESULT		Save(CString FilePath);
	HRESULT		Retrieve(CUSLFile* pFile);
	HRESULT		Save(CUSLFile* pFile);
	CEndEffector();
	virtual ~CEndEffector();

};

#endif // !defined(AFX_ENDEFFECTOR_H__ADC5EC70_ED59_4F28_B15E_41A373397412__INCLUDED_)
