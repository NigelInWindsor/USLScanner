// PrinterUtils.h: interface for the CPrinterUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTERUTILS_H__6B642930_6073_453C_B38D_8724BDDEA790__INCLUDED_)
#define AFX_PRINTERUTILS_H__6B642930_6073_453C_B38D_8724BDDEA790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <winspool.h>

class CPrinterUtils  
{
public:
	
	BOOL SetPrinterSetting( IN CString& szPrinterName, IN DEVMODE& devMode);
	VOID DevmodeCopy( IN const DEVMODE& fromDevMode, OUT DEVMODE& toDevMode );
	VOID ChangePrinterSettingsForCurrentOS( IN CString& szPrinterName, OUT PRINTER_INFO_2& pi2, IN DEVMODE& srcDevmode, IN HANDLE hPrinter );
	PDEVMODEW ChangePrinterProperties(CString& szPrinterName, PDEVMODE pSrcDevMode, HANDLE hPrinter);
	CPrinterUtils();
	virtual ~CPrinterUtils();

};

#endif // !defined(AFX_PRINTERUTILS_H__6B642930_6073_453C_B38D_8724BDDEA790__INCLUDED_)
