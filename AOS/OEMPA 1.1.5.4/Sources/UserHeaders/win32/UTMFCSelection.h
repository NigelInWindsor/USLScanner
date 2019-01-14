
#pragma once
#include "afxwin.h"

#pragma warning( disable : 4251 )

class CUTMFCDlgSelection;
class CUTMFCTreeView;

class UTKERNEL_MFC_API CUTMFCContainer
{
public:
	CUTMFCContainer();
	~CUTMFCContainer();
	enum enumItemDialog{eSetupLoad,eSetupSave,eCustomPointLoad,eCustomPointSave};
	static bool DisplayDialog(CWnd *pThis,enumItemDialog eItemDlg,CString &strFileName,CString &strFileExt,CString &strFilePath);
	static bool Load2(CWnd *pThis,CString &strFileName);
	static void Load(CWnd *pThis, bool &bLoadDisplay, wchar_t *pFileName=NULL);
	static void Save(CWnd *pThis, wchar_t *pFileName=NULL, int iSelectionRootSize=0, int *aiSelectionRoot=NULL);
	static bool CreateHwnd(HWND hWnd);
	static bool DeleteHwnd(HWND hWnd);
private:

};
extern "C" UTKERNEL_MFC_API void CUTMFCContainer_Save(CWnd *pThis, wchar_t *pFileName, int iSelectionRootSize, int *aiSelectionRoot);



class UTKERNEL_MFC_API_ALIGN _CUTMFCSelection
{
private:
	ULONGLONG m_ullPrivateMemory[27];
public:
	_CUTMFCSelection();

};

class UTKERNEL_MFC_API_ALIGN CUTMFCSelection : public _CUTMFCSelection
{

public:
	CUTMFCSelection(bool bNoLink=false);
	CUTMFCSelection(const CUTMFCSelection &a);
	~CUTMFCSelection();
	CUTMFCSelection& operator=(const CUTMFCSelection &a);
	int operator==(const CUTMFCSelection &a);
	int operator!=(const CUTMFCSelection &a);
	void Constructor(bool bNoLink);
	int GetDiff(const CUTMFCSelection &a,BOOL &bNewState);

	static bool GetTitle(int iRootID,const wchar_t *pNewName,CString &string);

	static void FunctionFormat(wchar_t *pInfo,wchar_t *pRootName,int iRootID);
	static bool FormatName(int iRootID,int iSize,wchar_t *pName);
	static bool FormatNameEx(int iRootID,int iSize,wchar_t *pName);
	void UpdateDisplayListFromKernel(bool bRunningVersionOnly);
	void SelectDisplayList(CUTMFCSelection &display);//what do you want to see in the list at the beginning ?
	void RefreshDisplayList(HWND hWnd,bool bOnNew,bool bOnDelete,bool bOnReName);//what do you want to see in the list at the beginning ?
	void RefreshRelease();
	bool SetDisplayList(CDWordArray &adwRootListID,CDWordArray &aeRootListType,CStringArray &astrNameList);
	void GetDisplayList(CDWordArray &adwRootListID,CDWordArray &aeRootListType,CStringArray &astrNameList);

	bool SetSelect(CDWordArray &adwSelect);
	void GetSelect(CDWordArray &adwSelect);
	bool SetError(CDWordArray &adwError);
	void GetError(CDWordArray &adwError);
	void CheckSelect(CUTMFCSelection &oldState);
	void CheckError();
	BOOL IsSelected(enumRootType eRootID,CUTMFCSelection *pSelect);

	void SetDetail(bool bEnable);

	void SetVersion(BOOL bEnable);
	void SetWizardSystem(BOOL bEnable);
	//void SetReservedForFuture(BOOL bEnable);
	void SetEncoder(BOOL bEnable);
	void SetAxis(BOOL bEnable);
	void SetAcquisitionData(BOOL bEnable);
	void SetAcquisitionBuffer(BOOL bEnable);
	void SetDriver(BOOL bEnable);
	void SetParameters(BOOL bEnable);
	void SetDisplayCursor(BOOL bEnable);
	void SetDisplayZoom(BOOL bEnable);
	void SetDisplayLayout(BOOL bEnable);
	void SetSingleProcessOnly(BOOL bEnable);

	BOOL GetCurrentPreference();
	BOOL GetWizardSystem();
	//BOOL GetReservedForFuture();
	BOOL GetEncoder();
	BOOL GetAxis();
	BOOL GetAcquisitionData();
	BOOL GetAcquisitionBuffer();
	BOOL GetDevice();
	BOOL GetChannels();
	BOOL GetDisplayCursor();
	BOOL GetDisplayZoom();
	BOOL GetDisplayLayout();
	BOOL GetSingleProcessOnly();

protected:

};

#pragma warning( default : 4251 )
