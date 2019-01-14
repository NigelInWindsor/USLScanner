// PrinterUtils.cpp: implementation of the CPrinterUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "PrinterUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinterUtils::CPrinterUtils()
{

}

CPrinterUtils::~CPrinterUtils()
{

}

PDEVMODEW CPrinterUtils::ChangePrinterProperties(CString &szPrinterName, PDEVMODE pSrcDevMode, HANDLE hPrinter)
{
    DWORD dwNeeded = 0;
    LPDEVMODE pDevMode = NULL;

    if( !hPrinter )
    {
        if ( !::OpenPrinter(szPrinterName.GetBuffer(szPrinterName.GetLength()), &hPrinter, NULL) )
        {
            TRACE(L"Error: OpenPrinter failed");
  //          PRINT_ERROR(::GetLastError());
            return NULL;
        }
    }

    dwNeeded = ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(szPrinterName.GetLength()), NULL, NULL, 0);

    pDevMode = (LPDEVMODE)::malloc(dwNeeded);
    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(szPrinterName.GetLength()), pDevMode, NULL, DM_OUT_BUFFER) )
    {
        ::free(pDevMode);
        ::ClosePrinter(hPrinter);
        TRACE(_T("Error: DocumentProperties failed"));

        return NULL;
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_ORIENTATION;
    pDevMode->dmOrientation = pSrcDevMode->dmOrientation;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(szPrinterName.GetLength()), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_ORIENTATION"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERSIZE;
    pDevMode->dmPaperSize = pSrcDevMode->dmPaperSize;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(szPrinterName.GetLength()), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_PAPERSIZE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERWIDTH;
    pDevMode->dmPaperWidth = pSrcDevMode->dmPaperWidth;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(szPrinterName.GetLength()), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_PAPERWIDTH"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PAPERLENGTH;
    pDevMode->dmPaperLength = pSrcDevMode->dmPaperLength;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_PAPERLENGTH"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_SCALE;
    pDevMode->dmScale = pSrcDevMode->dmScale;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_SCALE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_COPIES;
    pDevMode->dmCopies = pSrcDevMode->dmCopies;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_COPIES"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_PRINTQUALITY;
    pDevMode->dmPrintQuality = pSrcDevMode->dmPrintQuality;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_PRINTQUALITY"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_COLOR;
    pDevMode->dmColor = pSrcDevMode->dmColor;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_COLOR"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_DUPLEX;
    pDevMode->dmDuplex = pSrcDevMode->dmDuplex;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_DUPLEX"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_YRESOLUTION;
    pDevMode->dmYResolution = pSrcDevMode->dmYResolution;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_YRESOLUTION"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_TTOPTION;
    pDevMode->dmTTOption = pSrcDevMode->dmTTOption;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_TTOPTION"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_COLLATE;
    pDevMode->dmCollate = pSrcDevMode->dmCollate;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_COLLATE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_FORMNAME;
    ::memcpy(pDevMode->dmFormName, pSrcDevMode->dmFormName, sizeof(pSrcDevMode->dmFormName));

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_FORMNAME"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_MEDIATYPE;
    pDevMode->dmMediaType = pSrcDevMode->dmMediaType;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_MEDIATYPE"));
    }

    pDevMode->dmFields |= pSrcDevMode->dmFields & DM_DITHERTYPE;
    pDevMode->dmDitherType = pSrcDevMode->dmDitherType;

    if ( IDOK != ::DocumentProperties(NULL, hPrinter, szPrinterName.GetBuffer(1), pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER) )
    {
        TRACE(_T("Fails setting DM_DITHERTYPE"));
    }

    ::ClosePrinter(hPrinter);

    return pDevMode;

    


}

VOID CPrinterUtils::ChangePrinterSettingsForCurrentOS(CString &szPrinterName, PRINTER_INFO_2 &pi2, DEVMODE &srcDevmode, HANDLE hPrinter)
{
    PDEVMODE pDevmodeSupport = NULL;
	int nWinVersion = 2007;
//	OSVERSIONINFO VersionInfo;
	/*
	ZeroMemory(&VersionInfo,sizeof VersionInfo);
	VersionInfo.dwOSVersionInfoSize = sizeof VersionInfo;
	::GetVersionEx(&VersionInfo);

	switch(VersionInfo.dwMajorVersion) {
	case 2000:
        DevmodeCopy(srcDevmode, *pi2.pDevMode);
        pDevmodeSupport = ChangePrinterProperties(szPrinterName, &srcDevmode, hPrinter);
		break;
	case 2003:
	    pi2.pDevMode = &srcDevmode;
        pDevmodeSupport = ChangePrinterProperties(szPrinterName, &srcDevmode, NULL);

        if ( pDevmodeSupport ) {
            pi2.pSecurityDescriptor = NULL; // Do not attempt to set security descriptor...
            pi2.pDevMode->dmFields = pDevmodeSupport->dmFields;
            pi2.pDevMode->dmDriverExtra = 0;
        }
		break;
	case 2008:
        pDevmodeSupport = ChangePrinterProperties(szPrinterName, &srcDevmode, NULL);
        if ( pDevmodeSupport ) {
            pi2.pDevMode = &srcDevmode;
            pi2.pDevMode->dmFields = pDevmodeSupport->dmFields;
        }
		break;
	case 6:	//Windows 7
        DevmodeCopy(srcDevmode, *pi2.pDevMode);
		break;
	default:
        DevmodeCopy(srcDevmode, *pi2.pDevMode);
		break;
    }

    if ( pDevmodeSupport ) {
        ::free(pDevmodeSupport);
    }
	*/
}

VOID CPrinterUtils::DevmodeCopy(const DEVMODE &fromDevMode, DEVMODE &toDevMode)
{
    ::ZeroMemory(&toDevMode, sizeof(toDevMode));

    ::memcpy(toDevMode.dmDeviceName, fromDevMode.dmDeviceName, sizeof(fromDevMode.dmDeviceName));
    ::memcpy(toDevMode.dmFormName,   fromDevMode.dmFormName,   sizeof(fromDevMode.dmFormName));

    toDevMode.dmBitsPerPel         = fromDevMode.dmBitsPerPel;
    toDevMode.dmCollate            = fromDevMode.dmCollate;
    toDevMode.dmColor              = fromDevMode.dmColor;
    toDevMode.dmCopies             = fromDevMode.dmCopies;
    toDevMode.dmDefaultSource      = fromDevMode.dmDefaultSource;
//    toDevMode.dmDisplayFixedOutput = fromDevMode.dmDisplayFixedOutput;
    toDevMode.dmDisplayFlags       = fromDevMode.dmDisplayFlags;
    toDevMode.dmDisplayFrequency   = fromDevMode.dmDisplayFrequency;
    toDevMode.dmOrientation        = fromDevMode.dmOrientation;
    toDevMode.dmDriverExtra        = fromDevMode.dmDriverExtra;
    toDevMode.dmDriverVersion      = fromDevMode.dmDriverVersion;
    toDevMode.dmDuplex             = fromDevMode.dmDuplex;
    toDevMode.dmFields             = fromDevMode.dmFields;
    toDevMode.dmLogPixels          = fromDevMode.dmLogPixels;
//    toDevMode.dmNup                = fromDevMode.dmNup;
    toDevMode.dmOrientation        = fromDevMode.dmOrientation;
    toDevMode.dmPaperLength        = fromDevMode.dmPaperLength;
    toDevMode.dmPaperSize          = fromDevMode.dmPaperSize;
    toDevMode.dmPaperWidth         = fromDevMode.dmPaperWidth;
    toDevMode.dmPrintQuality       = fromDevMode.dmPrintQuality;
    toDevMode.dmSpecVersion        = fromDevMode.dmSpecVersion;
    toDevMode.dmTTOption           = fromDevMode.dmTTOption;
    toDevMode.dmPelsHeight         = fromDevMode.dmPelsHeight;
    toDevMode.dmPelsWidth          = fromDevMode.dmPelsWidth;
    toDevMode.dmScale              = fromDevMode.dmScale;
    toDevMode.dmSize               = fromDevMode.dmSize;
    toDevMode.dmYResolution        = fromDevMode.dmYResolution;
    toDevMode.dmPosition.x         = fromDevMode.dmPosition.x;
    toDevMode.dmPosition.y         = fromDevMode.dmPosition.y;
    toDevMode.dmDitherType         = fromDevMode.dmDitherType;
    toDevMode.dmReserved1          = fromDevMode.dmReserved1;
    toDevMode.dmReserved2          = fromDevMode.dmReserved2;
//    toDevMode.dmPanningHeight      = fromDevMode.dmPanningHeight;
//    toDevMode.dmPanningWidth       = fromDevMode.dmPanningWidth;
    toDevMode.dmICMIntent          = fromDevMode.dmICMIntent;
    toDevMode.dmICMMethod          = fromDevMode.dmICMMethod;
    toDevMode.dmMediaType          = fromDevMode.dmMediaType;

}

BOOL CPrinterUtils::SetPrinterSetting(CString &szPrinterName, DEVMODE &devMode)
{
    HANDLE hPrinter = NULL;
    PRINTER_DEFAULTS pd;
    ::ZeroMemory(&pd, sizeof(pd));
    pd.DesiredAccess = PRINTER_ALL_ACCESS;
	pd.DesiredAccess = PRINTER_ACCESS_USE;

    // Open a handle to the printer
    BOOL bFlag = ::OpenPrinter( szPrinterName.GetBuffer(szPrinterName.GetLength()), &hPrinter, &pd );
    if ( (FALSE == bFlag) || (NULL == hPrinter) )
    {
        TRACE(_T("ERROR: Can't open printer\r\n"));
//        PRINT_ERROR(::GetLastError());

        return FALSE;
    }

    DWORD dwNeeded = 0;
    // Getting buffer size
    bFlag = ::GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
    if ( (FALSE == bFlag) && (ERROR_INSUFFICIENT_BUFFER != ::GetLastError()) || (0 == dwNeeded) )
    {
        TRACE(_T("ERROR: Can't get pi2 struct size\r\n"));
 //       PRINT_ERROR(::GetLastError());

        ::ClosePrinter(hPrinter);
        return FALSE;
    }

    // Allocate a buffer of the correct size.
    PRINTER_INFO_2 *pi2 = (PRINTER_INFO_2 *)::GlobalAlloc(GPTR,dwNeeded);
    if ( NULL == pi2 )
    {
        TRACE(_T("ERROR: Can't get malloc the buffer\r\n"));
 //       PRINT_ERROR(::GetLastError());

        ::ClosePrinter(hPrinter);
        return FALSE;
    }

    // Getting PRINTER_INFO_2
    bFlag = ::GetPrinter(hPrinter, 2, (LPBYTE)pi2, dwNeeded, &dwNeeded);
    if ( FALSE == bFlag )
    {
        TRACE(_T("ERROR: Can't call GetPrinter get PRINTER_INFO_2"));
 //       PRINT_ERROR(::GetLastError());

        ::GlobalFree(pi2);
        ::ClosePrinter(hPrinter);
        return FALSE;
    }

    // Modify PRINTER_INFO_2 for current OS
    ChangePrinterSettingsForCurrentOS(szPrinterName, *pi2, devMode, hPrinter);

    // Update printer information
    bFlag = ::SetPrinter(hPrinter, 2, (LPBYTE)pi2, 0);
    if ( FALSE == bFlag )
    {
        TRACE(_T("Can't call SetPrinter\r\n"));
 //       PRINT_ERROR(::GetLastError());
    }

    // Tell other apps that there was a change
    SendMessageTimeout(HWND_BROADCAST, WM_DEVMODECHANGE, 0L, (LPARAM)szPrinterName.GetBuffer(1), SMTO_NORMAL, 1000, NULL);

    ::GlobalFree(pi2);
    ::ClosePrinter(hPrinter);

    return bFlag;

}
