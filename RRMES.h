// RRMES.h: interface for the CRRMES class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRMES_H__E24C9237_B90E_4A8B_9146_BDBA050A103A__INCLUDED_)
#define AFX_RRMES_H__E24C9237_B90E_4A8B_9146_BDBA050A103A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	MES_CONTROL_TO_GAUGE	0
#define	MES_ASSOCIATED_TO_GAUGE	1
#define	MES_MESSAGES_TO_MES		2
#define	MES_ASSOCIATED_TO_MES	3

class CRRMES  
{
public:
	bool GetWIPValue(CString Label,CString &Value);
	void ExtractLabelsAndValues(CString &Buff,int nStart);
	bool ReadMessageFromMES(CString FileName);
	bool StateMachine();
	bool FillFileNameList(int nFileType, CStringArray *pArray);
	bool IsThereAMessageFromMES(CString &FileName);
	void SaveRegitryVariables();
	void CreateRegistryVariables();
	void RetrieveRegistryVariables();

	CStringArray m_WIPLabels;
	CStringArray m_WIPValues;
	int		m_nStateAction;
	bool	m_bEnabled;
	CString m_ControlPathToGauge;
	CString m_AssociatedPathToGauge;
	CString m_MessagePathToMes;
	CString m_AssociatedPathToMes;
	CString m_strResourceName;

	
	CRRMES();
	virtual ~CRRMES();

};

#endif // !defined(AFX_RRMES_H__E24C9237_B90E_4A8B_9146_BDBA050A103A__INCLUDED_)
