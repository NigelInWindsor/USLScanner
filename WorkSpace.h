// WorkSpace.h: interface for the CWorkSpace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACE_H__E13B1777_2427_4EAA_B617_CA45DA7A9172__INCLUDED_)
#define AFX_WORKSPACE_H__E13B1777_2427_4EAA_B617_CA45DA7A9172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	_STANDARD			0x0001
#define	_USE_ULTRASONICS	0x0002

#define	_SERIAL				0
#define	_PARTITION			1

#define	_SCANNING			1
#define	_FINISHED			2
#define	_SAVED				3

struct	PlayListItem {
	int nStructSize;

	int nProfileIndex;
	int nUsageMask;
	int	nStatus;
	int	nOldStatus;
	CString strComponentUniqueIdentifier;
	CString strComponentPathName;
	CString	strComponentFileName;
	CPolyCoord LadderToItem;

	inline void operator = (PlayListItem Item)	{ 
		nStructSize =  Item.nStructSize;
		nProfileIndex = Item.nProfileIndex;
		nUsageMask = Item.nUsageMask;
		nStatus = Item.nStatus;
		strComponentUniqueIdentifier = Item.strComponentUniqueIdentifier;
		strComponentPathName = Item.strComponentPathName;
		strComponentFileName = Item.strComponentFileName;
		LadderToItem = &Item.LadderToItem;
	};

	void Zero() {
		nProfileIndex = 0;
		nUsageMask = 0;
		nStatus = 0;
		strComponentUniqueIdentifier.Empty();
		strComponentPathName.Empty();
		strComponentFileName.Empty();
		LadderToItem.Zero();
	};

	void Save(CUSLFile *pFile,float fVersion) {
		int nHeaderSize = (sizeof fVersion) + (sizeof nProfileIndex) + (sizeof nUsageMask);
		pFile->Write(&nHeaderSize,sizeof nHeaderSize);
		pFile->Write(&fVersion, sizeof fVersion);
		pFile->Write(&nProfileIndex, sizeof nProfileIndex);
		pFile->Write(&nUsageMask, sizeof nUsageMask);

		CArchive* pArchFile = new CArchive(pFile,CArchive::store);
		pArchFile->WriteString(strComponentUniqueIdentifier+"\r\n");
		pArchFile->WriteString(strComponentPathName+"\r\n");
		pArchFile->WriteString(strComponentFileName+"\r\n");
		pArchFile->Close();
		delete pArchFile;

		LadderToItem.Save(pFile);
	};

	void Retrieve(CUSLFile *pFile) {
		float fVersion;
		int nHeaderSize;
		int	nStartPtr = (int)pFile->GetPosition();
		pFile->Read(&nHeaderSize,sizeof nHeaderSize);
		pFile->Read(&fVersion, sizeof fVersion);
		pFile->Read(&nProfileIndex, sizeof nProfileIndex);
		pFile->Read(&nUsageMask, sizeof nUsageMask);

		pFile->Seek(nStartPtr+nHeaderSize,CFile::begin);
		CArchive* pArchFile = new CArchive(pFile,CArchive::load);
		pArchFile->ReadString(strComponentUniqueIdentifier);
		pArchFile->ReadString(strComponentPathName);
		pArchFile->ReadString(strComponentFileName);
		pArchFile->Close();
		delete pArchFile;

		LadderToItem.Retrieve(pFile);
	};

};


class CWorkSpace  
{
public:
	void CheckSwitchOffPumps();
	bool AddToRefArrayAndCheck(float fValue);
	void ResetAllStatus();
	void CalculateDangerZone();
	bool IsPartitionScan();
	time_t SetCalibrationTime();
	int	TimeTillCalibration(CString *pBuff);
	bool IsCalibrationExpired();
	void ClearAllPlayListStandards();
	void EraseAllLadderFileNames();
	int GetNextPlayListItem();
	int GetSize();
	bool DeleteLadder(int nStartIndex, int nFinishIndex);
	bool DeleteLadder(int nIndex);
	void DeleteAllLadderItemsWithIndex( int nIndex );
	bool GetProfileDescription(int nIndex, CString *pBuff);
	void AddToLadder(int nStartIndex,int nAddLength,int *pnArray);
	void AddToLadder();
	int GetLadderSize();
	void PasteProfile();
	void CopyProfile();
	bool Retrieve(CUSLFile *pFile);
	bool Save(CUSLFile *pFile);
	bool DeleteProfile(int nIndex = -1);
	bool SelectLastProfile();
	bool SelectProfile(int nIndex);
	bool RetrieveProfile(CUSLFile *pFile);
	void AddProfile();
	void Zero();

	int	m_nPlayListL;
	PlayListItem *m_pPlayList;

	CString m_strRefImageName;
	CString m_FileName;
	bool m_bProfileModified;
	bool m_bDataModified;
	int	m_nPlayListIndex;
	int	m_nDataIndex;
	int	m_nProfileIndex;
	int	m_nDataL;
	int	m_nProfileL;
	int m_nScanMethod;
	int m_osTimeBetweenStandards;
	int m_nRefTimeSlot;
	int m_nRefGate;
	int	m_nRefImage;
	int m_nFlag;
	float m_fAttenuationRange[2];
	float m_fdBRefLevel;
	float m_fdBRefLevelArray[100];
	int	 m_ndBRefLevelL;
	CRect m_rrHistogramSample;
	CSize m_SampleSize;
	CSize m_ScanSize;
	CData* m_pCopyData;
	CProfile* m_pCopyProfile;
	CData* m_pData;
	CProfile* m_pProfile;
	CData* m_pDataArray;
	CProfile* m_pProfileArray;
	CWorkSpace();
	virtual ~CWorkSpace();

};

#endif // !defined(AFX_WORKSPACE_H__E13B1777_2427_4EAA_B617_CA45DA7A9172__INCLUDED_)
