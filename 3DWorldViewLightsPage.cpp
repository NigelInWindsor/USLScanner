// 3DWorldViewLightsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "3DWorldViewLightsPage.h"
#include "MainFrm.h"


// C3DWorldViewLightsPage

IMPLEMENT_DYNAMIC(C3DWorldViewLightsPage, CPropertyPage)


C3DWorldViewLightsPage::C3DWorldViewLightsPage(CPropertySheet* pToolSheet, CPropertyPage* pViewPage)
	: CPropertyPage(IDD_3D_WORLDVIEW_LIGHTS_PAGE)
{
	m_pToolSheet = pToolSheet;
	m_pViewPage = pViewPage;
}

C3DWorldViewLightsPage::~C3DWorldViewLightsPage()
{
}

void C3DWorldViewLightsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AMBIENT_INTENSITY_SLIDER, m_sliderAmbientIntensity);
	DDX_Control(pDX, IDC_AMBIENT_MFCCOLORBUTTON, m_colorAmbient);
	DDX_Control(pDX, IDC_H_DIRECTION_SLIDER, m_sliderHDirection);
	DDX_Control(pDX, IDC_V_DIRECTION_SLIDER, m_sliderVDirection);
	DDX_Control(pDX, IDC_WHICH_LIGHT_COMBO, m_comboWhichLight);
	DDX_Control(pDX, IDC_DIFFUSE_MFCCOLORBUTTON, m_colourDiffuse);
	DDX_Control(pDX, IDC_SPECULAR_MFCCOLORBUTTON, m_colourSpecular);
	DDX_Control(pDX, IDC_LIGHT_OFF_ON_CHECK, m_checkOffOn);
}


BEGIN_MESSAGE_MAP(C3DWorldViewLightsPage, CPropertyPage)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_AMBIENT_MFCCOLORBUTTON, &C3DWorldViewLightsPage::OnBnClickedAmbientMfccolorbutton)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_DIFFUSE_MFCCOLORBUTTON, &C3DWorldViewLightsPage::OnBnClickedDiffuseMfccolorbutton)
	ON_BN_CLICKED(IDC_SPECULAR_MFCCOLORBUTTON, &C3DWorldViewLightsPage::OnBnClickedSpecularMfccolorbutton)
	ON_CBN_SELCHANGE(IDC_WHICH_LIGHT_COMBO, &C3DWorldViewLightsPage::OnCbnSelchangeWhichLightCombo)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, &C3DWorldViewLightsPage::OnBnClickedDefaultButton)
	ON_BN_CLICKED(IDC_LIGHT_OFF_ON_CHECK, &C3DWorldViewLightsPage::OnBnClickedLightOffOnCheck)
END_MESSAGE_MAP()


// C3DWorldViewLightsPage message handlers

BOOL C3DWorldViewLightsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_comboWhichLight.AddString(L"1");
	m_comboWhichLight.AddString(L"2");
	m_comboWhichLight.AddString(L"3");
	m_comboWhichLight.SetCurSel(0);
	m_sliderHDirection.SetRange(-180, 180, true);
	m_sliderVDirection.SetRange(-90, 90, true);
	m_sliderAmbientIntensity.SetRange(0, 100);

	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DWorldViewLightsPage::UpdateAllControls()
{
	int nRed, nGreen, nBlue;
	if (this->GetSafeHwnd() == NULL) return;

	int nIndex = m_comboWhichLight.GetCurSel();
	int nMask = 1;

	nMask <<= nIndex;

	theApp.m_LastSettings.nProfileLightOffOnMask & nMask ? m_checkOffOn.SetCheck(true) : m_checkOffOn.SetCheck(false);

	float fTemp = -1.0f * atan2f(theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[0], theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[1]) * RAD_TO_DEG;
	m_sliderHDirection.SetPos((int)fTemp);
	fTemp = atan2f(theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[2], hypotf(theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[0], theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[1])) * RAD_TO_DEG;
	m_sliderVDirection.SetPos((int)fTemp);

	m_sliderAmbientIntensity.SetPos((int)(theApp.m_LastSettings.fProfileAmbientIntensity * 100.0f));

	COLORREF rgb;
	rgb = 0;
	nRed = (int)(theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[0] * 255.0f);		rgb |= (nRed << 0);
	nGreen = (int)(theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[1] * 255.0f);		rgb |= (nGreen << 8);
	nBlue = (int)(theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[2] * 255.0f);		rgb |= (nBlue << 16);
	m_colorAmbient.SetColor(rgb);

	rgb = 0;
	nRed = (int)(theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[0] * 255.0f);		rgb |= (nRed << 0);
	nGreen = (int)(theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[1] * 255.0f);	rgb |= (nGreen << 8);
	nBlue = (int)(theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[2] * 255.0f);		rgb |= (nBlue << 16);
	m_colourDiffuse.SetColor(rgb);
	rgb = 0;
	nRed = (int)(theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[0] * 255.0f);		rgb |= (nRed << 0);
	nGreen = (int)(theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[1] * 255.0f);	rgb |= (nGreen << 8);
	nBlue = (int)(theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[2] * 255.0f);	rgb |= (nBlue << 16);
	m_colourSpecular.SetColor(rgb);
}




void C3DWorldViewLightsPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	FRAME;

	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	float fTemp;
	int nIndex = m_comboWhichLight.GetCurSel();

	nTemp = -pSlider->GetPos();

	switch (pSpin->GetDlgCtrlID()) {
	case IDC_H_DIRECTION_SLIDER:
		fTemp = sinf((float)nTemp * DEG_TO_RAD);
		theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[0] = fTemp;
		fTemp = cosf((float)nTemp * DEG_TO_RAD);
		theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[1] = fTemp;
		pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);
		break;
	case IDC_AMBIENT_INTENSITY_SLIDER:
		theApp.m_LastSettings.fProfileAmbientIntensity = (float)nTemp / 100.0f;
		pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);
		break;
	}

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}


void C3DWorldViewLightsPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	FRAME;

	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	float fTemp;
	int nIndex = m_comboWhichLight.GetCurSel();

	nTemp = pSlider->GetPos();

	switch (pSpin->GetDlgCtrlID()) {
	case IDC_V_DIRECTION_SLIDER:
		fTemp = sinf((float)nTemp * DEG_TO_RAD) * hypotf(theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[0], theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[1]);
		theApp.m_LastSettings.vProfileLightDirection[nIndex].v.m128_f32[2] = fTemp;
		pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);
		break;
	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void C3DWorldViewLightsPage::OnBnClickedAmbientMfccolorbutton()
{
	FRAME;

	COLORREF rgb = m_colorAmbient.GetColor();
	theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[0] = (float)GetRValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[1] = (float)GetGValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[2] = (float)GetBValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileAmbientColour.v.m128_f32[3] = 0;
	pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);
}


void C3DWorldViewLightsPage::OnBnClickedDiffuseMfccolorbutton()
{
	FRAME;
	int nIndex = m_comboWhichLight.GetCurSel();

	COLORREF rgb = m_colourDiffuse.GetColor();
	theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[0] = (float)GetRValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[1] = (float)GetGValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[2] = (float)GetBValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileDiffuse[nIndex].v.m128_f32[3] = 0;
	pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);

}


void C3DWorldViewLightsPage::OnBnClickedSpecularMfccolorbutton()
{
	FRAME;
	int nIndex = m_comboWhichLight.GetCurSel();

	COLORREF rgb = m_colourSpecular.GetColor();
	theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[0] = (float)GetRValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[1] = (float)GetGValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[2] = (float)GetBValue(rgb) / 255.0f;
	theApp.m_LastSettings.vProfileSpecular[nIndex].v.m128_f32[3] = 0;
	pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);

}


void C3DWorldViewLightsPage::OnBnClickedLightOffOnCheck()
{
	FRAME;
	int nIndex = m_comboWhichLight.GetCurSel();
	int nMask = 1;

	nMask <<= nIndex;

	theApp.m_LastSettings.nProfileLightOffOnMask ^= nMask;
	UpdateAllControls();
	pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);
}


void C3DWorldViewLightsPage::OnCbnSelchangeWhichLightCombo()
{
	UpdateAllControls();
}


void C3DWorldViewLightsPage::OnBnClickedDefaultButton()
{
	FRAME;

	theApp.m_LastSettings.fProfileAmbientIntensity = 1.0f;
	theApp.m_LastSettings.vProfileAmbientColour = { 0.3f, 0.3f, 0.3f };

	theApp.m_LastSettings.vProfileLightDirection[0] = { 0.5265408f, 0.5735765f, -0.6275069f };
	theApp.m_LastSettings.vProfileLightDirection[1] = { 0.7198464f,  0.3420201f,  0.6040227f };
	theApp.m_LastSettings.vProfileLightDirection[2] = { 0.4545195f, -0.7660444f,  0.4545195f };

	theApp.m_LastSettings.vProfileDiffuse[0] = { 1.0000000f, 0.9607844f, 0.8078432f };
	theApp.m_LastSettings.vProfileDiffuse[1] = { 0.9647059f, 0.7607844f, 0.4078432f };
	theApp.m_LastSettings.vProfileDiffuse[2] = { 0.3231373f, 0.3607844f, 0.3937255f };

	theApp.m_LastSettings.vProfileSpecular[0] = { 1.0000000f, 0.9607844f, 0.8078432f };
	theApp.m_LastSettings.vProfileSpecular[1] = { 0.0000000f, 0.0000000f, 0.0000000f };
	theApp.m_LastSettings.vProfileSpecular[2] = { 0.3231373f, 0.3607844f, 0.3937255f };

	theApp.m_LastSettings.nProfileLightOffOnMask = 0x07;

	UpdateAllControls();
	pFrame->SendMessage(UI_RESTART_3D_WORLDVIEW);
}

