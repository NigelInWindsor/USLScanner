
#pragma once


class CUTKernelFile;

class UTKERNEL_API_ALIGN _CUTKernelFile
{
private:
	ULONGLONG m_ullPrivateMemory[219];

public:
	_CUTKernelFile();
	~_CUTKernelFile();

};

class UTKERNEL_API_ALIGN CUTKernelFile : public _CUTKernelFile
{
public:
	CUTKernelFile(bool bNoLink=false);
	CUTKernelFile(const CUTKernelFile &a);
	~CUTKernelFile();
	CUTKernelFile& operator=(const CUTKernelFile &a);
	int operator==(const CUTKernelFile &a);
	int operator!=(const CUTKernelFile &a);


	bool LoadPrepare(const wchar_t *pFileName,bool bErrorIfRootNameUsed);//Browse all objects from the file
	bool LoadComplete();//load only objects that have been previously selected from the file
	bool Save(const wchar_t *pFileName);
	void RemoveAll();//remove all object from the list
	int GetCount();//count of objects in the list
	int GetSelectedCount();//count of selected objects in the list
	//Before calling member function "Save", you have to browse the kernel and add any object you want to save
	bool AddObject(CUTKernelRoot *ptr,bool bSelect);//add a new object from the kernel
	bool AddObject(int iRootId,bool bSelect);//add a new object from the kernel
	bool AddObject(const wchar_t *pRootName,enumRootType eRootID,bool bSelect);//add a new object from the kernel
	//* After calling "LoadPrepare", you can browse and select each object to load with member function "Load".
	//If an object of the same name is already created in the kernel, it is automatically deselected.
	//* Before calling "Save" you can browse objects that are already selected to be saved and modify the selection.
	bool GetObject(int iIndex,int iNameSize,wchar_t *pRootName);
	bool GetObject(int iIndex,enumRootType &eRootID);
	bool GetObject(int iIndex,bool &bSelected);
	bool SetObject(int iIndex,bool bSelect);

	static bool DeleteObject(CUTKernelRoot *pRoot,enumRootType eRootType);//please use this function to desallocate one object created by "LoadComplete".
	static bool DeleteObject(int iRootObjectId,enumRootType eRootType);//please use this function to desallocate one object created by "LoadComplete".
	static CUTKernelRoot *NewObject(enumRootType eRootType,wchar_t *pName);
	//static bool NewObject(enumRootType eRootType,wchar_t *pName,int &iRootObjectId);

protected:

};
