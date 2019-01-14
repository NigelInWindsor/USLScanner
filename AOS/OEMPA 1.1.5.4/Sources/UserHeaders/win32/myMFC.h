
#pragma once

#if !defined(_COM_DECL_)
#ifdef _UTSOCK_EXPORT_
	#define _COM_DECL_ __declspec(dllexport)
#else _UTSOCK_EXPORT_
	#define _COM_DECL_ __declspec(dllimport)
#endif _UTSOCK_EXPORT_
#endif _COM_DECL_

class _COM_DECL_ myCString
{
public:
	myCString();
	myCString(myCString&);
	myCString(wchar_t*);
	~myCString();
	void Format( LPCTSTR lpszFormat, ... );
    operator const wchar_t *();
	myCString& operator=(const wchar_t*);
	myCString& operator=(const myCString&);
	bool operator==(const myCString&);
	bool operator!=(const myCString&);
	bool operator==(const wchar_t *);
	bool operator!=(const wchar_t *);
	void operator+=(const myCString&);
	void operator+=(const wchar_t *);
	int GetLength() const;
	myCString Left( int nCount ) const;
	myCString Right( int nCount ) const;
	wchar_t GetAt( int nIndex ) const;
	int Find( const wchar_t *) const;
	int Find( const wchar_t *, int ) const;
	bool Alloc(int iSize);
	wchar_t *GetBuffer();
	bool IsEmpty() const;
private:
	void Desalloc();
	int m_iSize;
	wchar_t *m_pData;
};

#define	nUNICODE_BOM	0xFEFF		// Unicode "byte order mark" which goes at start of file
class _COM_DECL_ myCFile
{
public:
	enum OpenFlags {
		modeRead =         (int) 0x00000,
		modeWrite =        (int) 0x00001,
		modeReadWrite =    (int) 0x00002,
		shareCompat =      (int) 0x00000,
		shareExclusive =   (int) 0x00010,
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		shareDenyNone =    (int) 0x00040,
		modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000,
		modeNoTruncate =   (int) 0x02000,
		typeText =         (int) 0x04000, // typeText and typeBinary are
		typeBinary =       (int) 0x08000, // used in derived classes only
		osNoBuffer =       (int) 0x10000,
		osWriteThrough =   (int) 0x20000,
		osRandomAccess =   (int) 0x40000,
		osSequentialScan = (int) 0x80000,
fileUnicode=(int)0x10000000
		};
	myCFile();
	myCFile(const wchar_t *, int );
	myCFile(HANDLE, int);
	~myCFile();
	bool Open(const wchar_t *, int );
	void SeekToEnd();
	virtual void Close();
	virtual UINT Read(void* lpBuf,UINT nCount);
	virtual void Write(const void* lpBuf,UINT nCount);
	bool IsOpened();
	bool IsText();
	bool TextWrite(const wchar_t *pString,DWORD &dwNumberOfBytesWritten);
	bool TextRead(myCString &string);
	HANDLE GetHandle();
	DWORD GetLength();
private:
	bool TextReadW(myCString &string);
	bool TextReadA(myCString &string);
	void SetFlags(int flags,DWORD &dwDesiredAccess,DWORD &dwSharedMode,DWORD &dwCreationDisposition,bool &bText);
	myCString m_strName;
	HANDLE m_hFile;
	bool m_bText;
	bool m_bExtern;
	bool m_bUnicode;
};

class _COM_DECL_ myCStdioFile : public myCFile
{
public:
	myCStdioFile();
	myCStdioFile(const wchar_t *, int );
	myCStdioFile(HANDLE, int);
	bool WriteString(const wchar_t *);
	bool WriteString(myCString &);
	bool ReadString(myCString &);
private:
};

class _COM_DECL_ myCDWordArray
{
public:
	myCDWordArray();
	~myCDWordArray();

	void RemoveAll();
	bool SetSize(int);
	int GetCount();
	DWORD& operator[](int iIndex);
	bool Add(DWORD);
	bool Set(int iIndex,int iCount,DWORD *pData);
	bool Append(myCDWordArray &a);
	bool RemoveAt(int iIndex);
private:
	bool Alloc(int iSize);
	void Desalloc();
	int m_iGrowBy;
	int m_iAllocSize;
	int m_iSize;
	DWORD *m_pData;
};

int AfxMessageBox(
   wchar_t *wcText,
   UINT nType = MB_OK,
   UINT nIDHelp = 0 
);


class _COM_DECL_ _myCSocket 
{
private:
	ULONGLONG m_ullPrivateMemory[1];

protected:

};

class _COM_DECL_ myCSocket : public _myCSocket
{

public:
	myCSocket();
	~myCSocket();

	bool Connect(wchar_t* lpszHostAddress, UINT nHostPort, bool bPing, bool bDisplayError);
	bool Disconnect();
	bool IsConnected();
	int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	bool IOCtl(long lCommand, DWORD* lpArgument = NULL);
	int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnReceive();
	virtual void OnClose();
	virtual void OnConnected();

private:
};
