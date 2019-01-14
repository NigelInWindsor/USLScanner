#pragma once



class C3DWorldViewLightsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(C3DWorldViewLightsPage)

public:
	CPropertySheet * m_pToolSheet;
	CPropertyPage* m_pViewPage;
	C3DWorldViewLightsPage(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);
	virtual ~C3DWorldViewLightsPage();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_3D_WORLDVIEW_LIGHTS_PAGE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderAmbientIntensity;
	CMFCColorButton m_colorAmbient;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedAmbientMfccolorbutton();
	CSliderCtrl m_sliderHDirection;
	CSliderCtrl m_sliderVDirection;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDiffuseMfccolorbutton();
	afx_msg void OnBnClickedSpecularMfccolorbutton();
	CComboBox m_comboWhichLight;
	void UpdateAllControls();
	afx_msg void OnCbnSelchangeWhichLightCombo();
	CMFCColorButton m_colourDiffuse;
	CMFCColorButton m_colourSpecular;
	afx_msg void OnBnClickedDefaultButton();
	afx_msg void OnBnClickedLightOffOnCheck();
	CButton m_checkOffOn;
};
