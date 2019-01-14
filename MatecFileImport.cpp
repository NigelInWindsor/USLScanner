#include "stdafx.h"
#include "USLScanner.h"
#include "MatecFileImport.h"
#include <objbase.h>
#include <MsXml6.h>
#include "PromptDlg.h"
//#import <msxml6.dll> rename_namespace(_T("MSXML"))


#pragma pack(1)
struct MatecHeader {
	char cName[8];
	int nSamplesLine;
	int nNumberLines;
	char cSpace[57];
	int nStartOffset;
};
#pragma pack()

////////////////////////////////////////////////////////////
/*
The C-scans reside in a .dat file that starts with a header;
Each sample comprises 32 bytes
The first byte is the first amplitude value

All the ultrasonics, palettes and scan sizes are in .scan file which is an xml file

All annotation is in .anns file
*/
////////////////////////////////////////////////////////////

#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)\


CMatecFileImport::CMatecFileImport()
{
}


CMatecFileImport::~CMatecFileImport()
{
}

bool CMatecFileImport::Retrieve(CUSLFile *pFile, CData *pData)
{
	bool bFlag = true;
	MatecHeader Header;
	unsigned int nFileLength = (unsigned int)pFile->GetLength();
	int nSample, nLine;

	pFile->SeekToBegin();
	pFile->Read((PVOID)&Header, sizeof Header);
	if (memcmp(Header.cName, "ARAGDAT", 7) != 0) return false;

	CPromptDlg* pPromptDlg = new CPromptDlg(NULL, L"This is a Matec file", L"Retrieve");
	pPromptDlg->Create(CPromptDlg::IDD, NULL);

	Sleep(2000);

	pData->Zero();
	pData->m_nSamplesLine = Header.nSamplesLine;
	pData->m_nNumberLines = Header.nNumberLines;
	pData->m_nCollectAxis = 0;
	pData->m_nFastAxis = 0;
	pData->m_nSlowAxis = 2;

	if (Header.nSamplesLine > 0 && Header.nNumberLines > 0) {
		pData->m_nNumberAmpImages[0] = 1;
		pData->m_nNumberImages = 1;
		pData->AllocateImageBuffers();

		if (Header.nStartOffset > 0 && (unsigned int)Header.nStartOffset < nFileLength) {
			char *pArray = new char[nFileLength];
			pFile->Seek(Header.nStartOffset, CFile::begin);
			pFile->Read(pArray, nFileLength - Header.nStartOffset);

			int nDest = 0;
			unsigned char *pDest = pData->m_cAmpBuffer[0];
			for (nLine = 0; nLine < pData->m_nNumberLines; nLine++) {
				unsigned int nSrc = nLine * (pData->m_nSamplesLine * 32);
				for (nSample=0; nSample < pData->m_nSamplesLine && nSrc<nFileLength; nSample++, nSrc += 32) {
					pDest[nDest++] = pArray[nSrc];
				}
			}
			delete pArray;
		}
		else {
			bFlag = false;
			goto CleanUp;
		}
	}
	else {
		bFlag = false;
		goto CleanUp;
	}

	ImportScanFile(pFile, pData);
	ImportAnnotationFile(pFile, pData);

CleanUp:
	pPromptDlg->SendMessage(WM_CLOSE);
	SAFE_DELETE(pPromptDlg);

	return bFlag;
}

bool CMatecFileImport::ImportScanFile(CUSLFile *pFile, CData *pData)
{
	CString Buff;
	CString FilePath = pFile->GetFilePath();
	FilePath.Replace(L".dat", L".scan");

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		loadDOMScanFile(FilePath, pData);
		CoUninitialize();
	}

	return true;
}

bool CMatecFileImport::ImportAnnotationFile(CUSLFile *pFile, CData *pData)
{
	CString Buff;
	CString FilePath = pFile->GetFilePath();
	FilePath.Replace(L".dat", L".anns");

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		loadDOMAnnotationFile(FilePath, pData);
		CoUninitialize();
	}

	return true;
}

HRESULT CMatecFileImport::VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
	HRESULT hr = S_OK;
	BSTR bstr = SysAllocString(wszValue);
	CHK_ALLOC(bstr);

	V_VT(&Variant) = VT_BSTR;
	V_BSTR(&Variant) = bstr;

CleanUp:
	return hr;
}

HRESULT CMatecFileImport::CreateAndInitDOM(IXMLDOMDocument **ppDoc)
{
	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
	if (SUCCEEDED(hr))
	{
		// these methods should not fail so don't inspect result
		(*ppDoc)->put_async(VARIANT_FALSE);
		(*ppDoc)->put_validateOnParse(VARIANT_FALSE);
		(*ppDoc)->put_resolveExternals(VARIANT_FALSE);
	}
	return hr;
}

void CMatecFileImport::loadDOMScanFile(CString FilePath, CData *pData)
{
	HRESULT hr = S_OK;
	IXMLDOMDocument*	pXMLDom = NULL;
	IXMLDOMParseError*	pXMLErr = NULL;
	IXMLDOMElement*       pXMLElement = NULL;
	IXMLDOMNodeList*      pXMLNodeList = NULL;
	IXMLDOMNodeList*      pXMLChildNodeList = NULL;
	IXMLDOMNodeList*      pXMLScanNodeList = NULL;
	IXMLDOMNode*          pXMLNode = NULL;
	IXMLDOMNode*		  pXMLChildNode = NULL;
	IXMLDOMNode*		  pXMLScanNode = NULL;


	long lCount,  lScanCount;
	BSTR bstrXML = NULL;
	BSTR bstrErr = NULL;
	CComBSTR bstrName;
	CComBSTR bstrValue;
	VARIANT_BOOL varStatus;
	VARIANT varFileName;
	CComVariant varValue;
	float fTemp;

	VariantInit(&varFileName);
	VariantInit(&varValue);

	CHK_HR(CreateAndInitDOM(&pXMLDom));

	// XML file name to load
	CHK_HR(VariantFromString(FilePath, varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));
	if (varStatus == VARIANT_TRUE)
	{
		CHK_HR(pXMLDom->get_xml(&bstrXML));
		CHK_HR(pXMLDom->get_childNodes(&pXMLNodeList));
		CHK_HR(pXMLNodeList->get_length(&lCount));

		for (long nIndex = 0; nIndex < lCount; nIndex++) {
			CHK_HR(pXMLNodeList->get_item(nIndex, &pXMLNode));
			CHK_HR(pXMLNode->get_nodeName(&bstrName));

			if (bstrName == "Parameters") {

				IXMLDOMNode* pScanSetupNode = NULL;
				if (FindInThisNode("ScanSetup", pXMLNode, &pScanSetupNode) == true) {

					CHK_HR(pScanSetupNode->get_childNodes(&pXMLScanNodeList));
					CHK_HR(pXMLScanNodeList->get_length(&lScanCount));

					for (long nScan = 0; nScan < lScanCount; nScan++) {
						CHK_HR(pXMLScanNodeList->get_item(nScan, &pXMLScanNode));
						CHK_HR(pXMLScanNode->get_nodeName(&bstrName));

						if (bstrName == "ScanStart") {
							if (GetFloatValue(pXMLScanNode, &fTemp) == true) { pData->m_CpStart.Side0.fX = fTemp * 25.4f; }
						}
						if (bstrName == "ScanExtent") {
							if (GetFloatValue(pXMLScanNode, &fTemp) == true) {
								pData->m_fFastScanSize = fTemp * 25.4f;
								pData->m_CpFinish.Side0.fX = pData->m_CpStart.Side0.fX + pData->m_fFastScanSize;
							}
						}
						if (bstrName == "ScanIncrement") {
							if (GetFloatValue(pXMLScanNode, &fTemp) == true) { pData->m_fFastIncrement = fTemp * 25.4f; }
						}

						if (bstrName == "IndexStart") {
							if (GetFloatValue(pXMLScanNode, &fTemp) == true) { pData->m_CpStart.Side0.fZ = fTemp * 25.4f; }
						}
						if (bstrName == "IndexExtent") {
							if (GetFloatValue(pXMLScanNode, &fTemp) == true) {
								pData->m_fSlowScanSize = fTemp * 25.4f;
								pData->m_CpFinish.Side0.fZ = pData->m_CpStart.Side0.fZ + pData->m_fSlowScanSize;
							}
						}
						if (bstrName == "IndexIncrement") {
							if (GetFloatValue(pXMLScanNode, &fTemp) == true) { pData->m_fSlowIncrement = fTemp * 25.4f; }
						}
					}
				}
				SAFE_RELEASE(pScanSetupNode);
			}
		}

	}
	else
	{
		// Failed to load xml, get last parsing error
		CHK_HR(pXMLDom->get_parseError(&pXMLErr));
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		//printf("Failed to load DOM from stocks.xml. %S\n", bstrErr);
	}

CleanUp:
	SAFE_RELEASE(pXMLScanNode);
	SAFE_RELEASE(pXMLScanNodeList);
	SAFE_RELEASE(pXMLChildNode);
	SAFE_RELEASE(pXMLElement);
	SAFE_RELEASE(pXMLNodeList);
	SAFE_RELEASE(pXMLNode);
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pXMLErr);
	SysFreeString(bstrXML);
	SysFreeString(bstrErr);
	VariantClear(&varFileName);
	VariantClear(&varValue);
}


void CMatecFileImport::loadDOMAnnotationFile(CString FilePath, CData *pData)
{
	HRESULT hr = S_OK;
	IXMLDOMDocument*	pXMLDom = NULL;
	IXMLDOMParseError*	pXMLErr = NULL;
	IXMLDOMElement*     pXMLElement = NULL;
	IXMLDOMNodeList*    pXMLNodeList = NULL;
	IXMLDOMNodeList*	pXMlistAnnoNodeList = NULL;
	IXMLDOMNode*        pXMLDrawingNode = NULL;
	IXMLDOMNode*        pXMLNode = NULL;
	IXMLDOMNode*		pXMLChildNode = NULL;
	IXMLDOMNode*		pXMLAnnotationNode = NULL;
	IXMLDOMNode*		pXMLValueNode = NULL;
	IXMLDOMNode*		pXMLAnnoNode = NULL;
	IXMLDOMNode*		pXMlistAnnoNode = NULL;


	long lCount, lListCount;
	BSTR bstrXML = NULL;
	BSTR bstrErr = NULL;
	CComBSTR bstrName;
	CComBSTR bstrValue;
	VARIANT_BOOL varStatus;
	VARIANT varFileName;
	CComVariant varValue;

	VariantInit(&varFileName);
	VariantInit(&varValue);

	CHK_HR(CreateAndInitDOM(&pXMLDom));

	// XML file name to load
	CHK_HR(VariantFromString(FilePath, varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));
	if (varStatus == VARIANT_TRUE)
	{
		CHK_HR(pXMLDom->get_xml(&bstrXML));
		CHK_HR(pXMLDom->get_childNodes(&pXMLNodeList));
		CHK_HR(pXMLNodeList->get_length(&lCount));

		for (long nIndex = 0; nIndex < lCount; nIndex++) {
			CHK_HR(pXMLNodeList->get_item(nIndex, &pXMLNode));
			CHK_HR(pXMLNode->get_nodeName(&bstrName));

			if (bstrName == "Parameters") {

				if(FindInThisNode("DrawingSetup", pXMLNode, &pXMLDrawingNode)) {

					if (FindInThisNode("listAnnotations", pXMLDrawingNode, &pXMlistAnnoNode)) {

						CHK_HR(pXMlistAnnoNode->get_childNodes(&pXMlistAnnoNodeList));
						CHK_HR(pXMlistAnnoNodeList->get_length(&lListCount));

						for (long lList=0; lList < lListCount; lList++) {
							CHK_HR(pXMlistAnnoNodeList->get_item(lList, &pXMLAnnoNode));
							CHK_HR(pXMLAnnoNode->get_nodeName(&bstrName));

							if (bstrName == "AnnotationListItem") {

								ExtractAnnotationVariables(pXMLAnnoNode, pData);
							}
						}
					}
				}
			}
		}


	}
	else
	{
		// Failed to load xml, get last parsing error
		CHK_HR(pXMLDom->get_parseError(&pXMLErr));
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		//		printf("Failed to load DOM from stocks.xml. %S\n", bstrErr);
	}

CleanUp:
	SAFE_RELEASE(pXMlistAnnoNodeList);
	SAFE_RELEASE(pXMlistAnnoNode);
	SAFE_RELEASE(pXMLDrawingNode);
	SAFE_RELEASE(pXMLAnnoNode);
	SAFE_RELEASE(pXMLChildNode);
	SAFE_RELEASE(pXMLNode);
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pXMLErr);
	SysFreeString(bstrXML);
	SysFreeString(bstrErr);
	VariantClear(&varFileName);
	VariantClear(&varValue);
}


void CMatecFileImport::ExtractAnnotationVariables(IXMLDOMNode* pXMLBaseNode, CData* pData)
{
	HRESULT hr = S_OK;
	CComBSTR bstrName, bstrValue;
	IXMLDOMNode*        pXMLFirstNode = NULL;
	IXMLDOMNodeList*    pXMLObjectNodeList = NULL;
	IXMLDOMNode*        pXMLObjectNode = NULL;
	long				lObjectCount;
	IXMLDOMNodeList*    pXMLRectangleNodeList = NULL;
	IXMLDOMNode*        pXMLRectangleNode = NULL;
	long				lRectangleCount;
	float fX = 0.0f, fY = 0.0f;
	CString strID;

	int nBkMode = theApp.m_LastSettings.nAnnotateBkMode;
	COLORREF rgbBkColor = theApp.m_LastSettings.rgbAnnotateBkColor;
	COLORREF rgbTextColor = theApp.m_LastSettings.rgbAnnotation;

	CHK_HR(pXMLBaseNode->get_firstChild(&pXMLFirstNode));
	CHK_HR(pXMLFirstNode->get_nodeName(&bstrName));
	if (bstrName == "AnnotationObject") {

		CHK_HR(pXMLFirstNode->get_childNodes(&pXMLObjectNodeList));
		CHK_HR(pXMLObjectNodeList->get_length(&lObjectCount));

		for (long nObject = 0; nObject < lObjectCount; nObject++) {
			CHK_HR(pXMLObjectNodeList->get_item(nObject, &pXMLObjectNode));
			CHK_HR(pXMLObjectNode->get_nodeName(&bstrName));

			if (bstrName == "Content") {
				CHK_HR(pXMLObjectNode->get_text(&bstrValue));
				strID = bstrValue;
			}
			if (bstrName == "BackgroundColor") {
				CHK_HR(pXMLObjectNode->get_text(&bstrValue));
				rgbBkColor = _ttoi(bstrValue) & 0x00ffffff;
				SwapRedBlue(&rgbBkColor);
			}
			if (bstrName == "ForegroundColor") {
				CHK_HR(pXMLObjectNode->get_text(&bstrValue));
				rgbTextColor = _ttoi(bstrValue) & 0x00ffffff;
				SwapRedBlue(&rgbTextColor);
			}
			if (bstrName == "RectangleObject") {
				CHK_HR(pXMLObjectNode->get_childNodes(&pXMLRectangleNodeList));
				CHK_HR(pXMLRectangleNodeList->get_length(&lRectangleCount));

				for (long nRect = 0; nRect < lRectangleCount; nRect++) {
					CHK_HR(pXMLRectangleNodeList->get_item(nRect, &pXMLRectangleNode));
					CHK_HR(pXMLRectangleNode->get_nodeName(&bstrName));

					if (bstrName == "X") {
						CHK_HR(pXMLRectangleNode->get_text(&bstrValue));
						fX = (float)_ttof(bstrValue) * pData->m_fFastIncrement;
					}
					if (bstrName == "Y") {
						CHK_HR(pXMLRectangleNode->get_text(&bstrValue));
						fY = (float)_ttof(bstrValue) * pData->m_fSlowIncrement;
					}
				}
			}
		}
		pData->AddAnnotation(fX, fY, strID, theApp.m_LastSettings.lfAnnotation, rgbTextColor, 0, 0, theApp.m_LastSettings.nAnnotateApplyToAllImages, nBkMode, rgbBkColor);
	}


CleanUp:
	SAFE_RELEASE(pXMLRectangleNodeList);
	SAFE_RELEASE(pXMLRectangleNode);
	SAFE_RELEASE(pXMLFirstNode);
	SAFE_RELEASE(pXMLObjectNodeList);
	SAFE_RELEASE(pXMLObjectNode);
}

bool CMatecFileImport::GetFloatValue(IXMLDOMNode* pXMLNode, float *fTemp)
{
	bool bFlag = false;
	HRESULT			hr = S_OK;
	IXMLDOMNode*	pXMLValueNode;
	CComBSTR bstrName, bstrValue;

	CHK_HR(pXMLNode->get_firstChild(&pXMLValueNode));
	CHK_HR(pXMLValueNode->get_nodeName(&bstrName));
	if (bstrName == "Value") {
		CHK_HR(pXMLValueNode->get_text(&bstrValue));
		*fTemp = (float)_ttof(bstrValue);
		bFlag = true;
	}
CleanUp:
	SAFE_RELEASE(pXMLValueNode);

	return bFlag;
}


void CMatecFileImport::SwapRedBlue(COLORREF *rgb)
{
	COLORREF rgbInp = *rgb;
	DWORD r, g, b;

	r = GetRValue(rgbInp);
	g = GetGValue(rgbInp);
	b = GetBValue(rgbInp);

	*rgb = (r << 16) | (g << 8) | b;

}


bool CMatecFileImport::FindInThisNode(CComBSTR strFind, IXMLDOMNode* pXMLBaseNode, IXMLDOMNode** pNextNode)
{
	bool bFlag = false;
	HRESULT hr = S_OK;
	IXMLDOMNodeList*      pXMLBaseNodeList = NULL;
	CComBSTR bstrName;
	long lBaseCount;

	CHK_HR(pXMLBaseNode->get_childNodes(&pXMLBaseNodeList));
	CHK_HR(pXMLBaseNodeList->get_length(&lBaseCount));

	for (long nBase = 0; nBase < lBaseCount; nBase++) {
		CHK_HR(pXMLBaseNodeList->get_item(nBase, pNextNode));
		CHK_HR((*pNextNode)->get_nodeName(&bstrName));
		if (bstrName == strFind) {
			bFlag = true;
			goto CleanUp;
		}
	};

CleanUp:
	SAFE_RELEASE(pXMLBaseNodeList);

	return bFlag;
}
