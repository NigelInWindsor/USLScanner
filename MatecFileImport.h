#pragma once

class CMatecFileImport 
{
public:
	CMatecFileImport();
	~CMatecFileImport();
	bool Retrieve(CUSLFile * pFile, CData * pData);
	bool ImportScanFile(CUSLFile * pFile, CData * pData);
	bool ImportAnnotationFile(CUSLFile * pFile, CData * pData);
	HRESULT VariantFromString(PCWSTR wszValue, VARIANT & Variant);
	HRESULT CreateAndInitDOM(IXMLDOMDocument ** ppDoc);
	void Old_loadDOMScanFile(CString FilePath, CData * pData);
	void loadDOMScanFile(CString FilePath, CData * pData);
	void loadDOMAnnotationFile(CString FilePath, CData * pData);
	void ExtractAnnotationVariables(IXMLDOMNode * pXMLBaseNode, CData * pData);
	bool GetFloatValue(IXMLDOMNode * pXMLNode, float * fTemp);
	void SwapRedBlue(COLORREF * rgb);
	bool FindInThisNode(CComBSTR strFind, IXMLDOMNode * pXMLBaseNode, IXMLDOMNode ** pScanSetupNode);
};

