#include "stdafx.h"
#include "USLScanner.h"
#include "3DCamera.h"
#include "MainFrm.h"

#undef min // use __min instead
#undef max // use __max instead

//Arc

//Base camera
XMVECTOR m_vDefaultEye;          // Default camera eye position
XMVECTOR m_vDefaultLookAt;       // Default LookAt position
XMVECTOR m_vDefaultUp;       // Default Up Vetcor
XMVECTOR m_vEye;                 // Camera eye position
XMVECTOR m_vLookAt;              // LookAt position
XMVECTOR m_vUp;              // Up Vector
							 //XMVECTOR m_vCameraTranslation;

XMVECTOR g_vPickEyePt, g_vPickEyeDir;

CD3DArcBall_11::CD3DArcBall_11()
{
	Reset();
	m_vDownPt = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_vCurrentPt = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_Offset.x = m_Offset.y = 0;

	RECT rc;
	GetClientRect(GetForegroundWindow(), &rc);
	SetWindow(rc.right, rc.bottom);
	m_bMouseLButtonDown = false;
	m_bMouseMButtonDown = false;
	m_bMouseRButtonDown = false;

}


CD3DArcBall_11::~CD3DArcBall_11()
{
}



//--------------------------------------------------------------------------------------
void CD3DArcBall_11::Reset()
{
	m_qDown = XMQuaternionIdentity();
	m_qNow = XMQuaternionIdentity();
	m_mRotation = XMMatrixIdentity();
	m_mTranslation = XMMatrixIdentity();
	m_mTranslationDelta = XMMatrixIdentity();
	m_bDrag = FALSE;
	m_fRadiusTranslation = 1.0f;
	m_fRadius = 1.0f;
	m_bMouseLButtonDown = false;
	m_bMouseMButtonDown = false;
	m_bMouseRButtonDown = false;
}

void CD3DArcBall_11::ZeroTranslation()
{
	m_mTranslation.r[3].m128_f32[0] = 0.0f;
	m_mTranslation.r[3].m128_f32[1] = 0.0f;
	m_mTranslation.r[3].m128_f32[2] = 0.0f;

}

void CD3DArcBall_11::SetTranslation(XMVECTOR* pvTranslation)
{
	m_mTranslation.r[3].m128_f32[0] = pvTranslation->m128_f32[0];
	m_mTranslation.r[3].m128_f32[1] = pvTranslation->m128_f32[1];
	m_mTranslation.r[3].m128_f32[2] = pvTranslation->m128_f32[2];
}

XMVECTOR* CD3DArcBall_11::GetTranslation()
{
	return &m_mTranslation.r[3];
}

XMVECTOR CD3DArcBall_11::ScreenToVector(float fScreenPtX, float fScreenPtY)
{
	// Scale to screen
	FLOAT x = -(fScreenPtX - m_Offset.x - m_nWidth / 2) / (m_fRadius * m_nWidth / 2);
	FLOAT y = (fScreenPtY - m_Offset.y - m_nHeight / 2) / (m_fRadius * m_nHeight / 2);

	if (g_bRightHanded == true)
	{
		x = -x;
		y = -y;
	}

	FLOAT z = 0.0f;
	FLOAT mag = x * x + y * y;

	if (mag > 1.0f)
	{
		FLOAT scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
		z = sqrtf(1.0f - mag);

	// Return vector
	XMVECTOR vResult = XMLoadFloat3(&XMFLOAT3(x, y, z));
	return vResult;
}




//--------------------------------------------------------------------------------------
XMVECTOR CD3DArcBall_11::QuatFromBallPoints(XMVECTOR& vFrom, XMVECTOR& vTo)
{

	XMVECTOR vPart;
	XMVECTOR vDot;

	vDot = XMVector3Dot(vFrom, vTo);
	vPart = XMVector3Cross(vFrom, vTo);

	vPart.m128_f32[3] = vDot.m128_f32[0];

	return vPart;
}




//--------------------------------------------------------------------------------------
void CD3DArcBall_11::OnBegin(int nX, int nY, int nMouseButtons)
{
	// Only enter the drag state if the click falls
	// inside the click rectangle.
	if (nX >= m_Offset.x &&
		nX < m_Offset.x + m_nWidth &&
		nY >= m_Offset.y &&
		nY < m_Offset.y + m_nHeight)
	{
		m_bDrag = true;
		m_qDown = m_qNow;
		m_vDownPt = ScreenToVector((float)nX, (float)nY);
	}
	if (nMouseButtons == WM_LBUTTONDOWN || nMouseButtons == WM_LBUTTONDBLCLK) m_bMouseLButtonDown = true;
	if (nMouseButtons == WM_MBUTTONDOWN || nMouseButtons == WM_MBUTTONDBLCLK) m_bMouseMButtonDown = true;
	if (nMouseButtons == WM_RBUTTONDOWN || nMouseButtons == WM_RBUTTONDBLCLK) m_bMouseRButtonDown = true;

}




//--------------------------------------------------------------------------------------
void CD3DArcBall_11::OnMove(int nX, int nY)
{
	if (m_bDrag)
	{
		if (m_bMouseLButtonDown == true)
		{
			m_vCurrentPt = ScreenToVector((float)nX, (float)nY);
			m_qNow = XMQuaternionMultiply(m_qDown, QuatFromBallPoints(m_vDownPt, m_vCurrentPt));
		}
		else if ((m_bMouseRButtonDown == true) || (m_bMouseMButtonDown == true))
		{
			// Normalize based on size of window and bounding sphere radius
			FLOAT fDeltaX = (m_ptLastMouse.x - nX) * m_fRadiusTranslation / m_nWidth;
			FLOAT fDeltaY = (m_ptLastMouse.y - nY) * m_fRadiusTranslation / m_nHeight;
			if (m_bMouseRButtonDown == true)
			{
				m_mTranslationDelta = XMMatrixTranslation(-2 * fDeltaX, 2 * fDeltaY, 0.0f);
				m_mTranslation = XMMatrixMultiply(m_mTranslation, m_mTranslationDelta);
			}
			else  // wParam & MK_MBUTTON
			{
				m_mTranslationDelta = XMMatrixTranslation(0.0f, 0.0f, 5 * fDeltaY);
				m_mTranslation = XMMatrixMultiply(m_mTranslation, m_mTranslationDelta);
			}

		}
	}
	// Store mouse coordinate
	m_ptLastMouse.x = nX;
	m_ptLastMouse.y = nY;
}




//--------------------------------------------------------------------------------------
void CD3DArcBall_11::OnEnd()
{
	m_bMouseLButtonDown = false;
	m_bMouseMButtonDown = false;
	m_bMouseRButtonDown = false;
	m_bDrag = false;
}

LRESULT CD3DArcBall_11::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Current mouse position
	int iMouseX = (short)LOWORD(lParam);
	int iMouseY = (short)HIWORD(lParam);

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		SetCapture(hWnd);
		OnBegin(iMouseX, iMouseY, uMsg);
		return TRUE;

	case WM_LBUTTONUP:
		ReleaseCapture();
		OnEnd();
		return TRUE;
	case WM_CAPTURECHANGED:
		if ((HWND)lParam != hWnd)
		{
			ReleaseCapture();
			OnEnd();
		}
		return TRUE;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		SetCapture(hWnd);
		// Store off the position of the cursor when the button is pressed
		m_ptLastMouse.x = iMouseX;
		m_ptLastMouse.y = iMouseY;
		return TRUE;

	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		ReleaseCapture();
		return TRUE;

	case WM_MOUSEMOVE:
		if (MK_LBUTTON & wParam)
		{
			//			OnMove();
		}
		else if ((MK_RBUTTON & wParam) || (MK_MBUTTON & wParam))
		{
			// Normalize based on size of window and bounding sphere radius
			FLOAT fDeltaX = (m_ptLastMouse.x - iMouseX) * m_fRadiusTranslation / m_nWidth;
			FLOAT fDeltaY = (m_ptLastMouse.y - iMouseY) * m_fRadiusTranslation / m_nHeight;

			if (wParam & MK_RBUTTON)
			{
				m_mTranslationDelta = XMMatrixTranslation(-2 * fDeltaX, 2 * fDeltaY, 0.0f);
				m_mTranslation = XMMatrixMultiply(m_mTranslation, m_mTranslationDelta);
			}
			else  // wParam & MK_MBUTTON
			{
				m_mTranslationDelta = XMMatrixTranslation(0.0f, 0.0f, 5 * fDeltaY);
				m_mTranslation = XMMatrixMultiply(m_mTranslation, m_mTranslationDelta);
			}

			// Store mouse coordinate
			m_ptLastMouse.x = iMouseX;
			m_ptLastMouse.y = iMouseY;
		}
		return TRUE;
	}

	return FALSE;
}




CBaseCamera::CBaseCamera()
{
	m_cKeysDown = 0;
	ZeroMemory(m_aKeys, sizeof(BYTE) * CAM_MAX_KEYS);

	// Set attributes for the view matrix
	XMVECTOR vEyePt = XMLoadFloat3(&XMFLOAT3(0.0f, -2000.0f, 0.0f));
	XMVECTOR vLookatPt = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	XMVECTOR vUp = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Setup the view matrix
	SetViewParams(&vEyePt, &vLookatPt, &vUp);

	// Setup the projection matrix
	SetProjParams(XM_PIDIV4, 1.0f, 1.0f, 20000.0f);

	GetCursorPos(&m_ptLastMousePosition);
	m_bMouseLButtonDown = false;
	m_bMouseMButtonDown = false;
	m_bMouseRButtonDown = false;
	m_nCurrentButtonMask = 0;
	m_nMouseWheelDelta = 0;

	m_fCameraYawAngle = 0.0f;
	m_fCameraPitchAngle = 0.0f;

	SetRect(&m_rcDrag, LONG_MIN, LONG_MIN, LONG_MAX, LONG_MAX);
	m_vVelocity = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_bMovementDrag = false;
	m_vVelocityDrag = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_fDragTimer = 0.0f;
	m_fTotalDragTimeToZero = 0.25;
	m_vRotVelocity = XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f));

	m_fRotationScaler = 0.01f;
	m_fMoveScaler = 5.0f;

	m_bInvertPitch = false;
	m_bEnableYAxisMovement = true;
	m_bEnablePositionMovement = true;

	m_vMouseDelta = XMLoadFloat2(&XMFLOAT2(0.0f, 0.0f));
	m_fFramesToSmoothMouseData = 2.0f;

	m_bClipToBoundary = false;
	m_vMinBoundary = XMLoadFloat3(&XMFLOAT3(-1.0f, -1.0f, -1.0f));
	m_vMaxBoundary = XMLoadFloat3(&XMFLOAT3(1.0f, 1.0f, 1.0f));

	m_bResetCursorAfterMove = false;

}


CBaseCamera::~CBaseCamera()
{
}

//--------------------------------------------------------------------------------------
// Client can call this to change the position and direction of camera
//--------------------------------------------------------------------------------------
VOID CBaseCamera::SetViewParams(XMVECTOR* pvEyePt, XMVECTOR* pvLookatPt, XMVECTOR* pvUp)
{
	if (NULL == pvEyePt || NULL == pvLookatPt)
		return;

	m_vDefaultEye = m_vEye = *pvEyePt;
	m_vDefaultLookAt = m_vLookAt = *pvLookatPt;
	m_vDefaultUp = m_vUp = *pvUp;

	// Calc the view matrix
	if (g_bRightHanded == true) {
		m_mView = XMMatrixLookAtRH(*pvEyePt, *pvLookatPt, *pvUp);
	}
	else {
		m_mView = XMMatrixLookAtLH(*pvEyePt, *pvLookatPt, *pvUp);
	}
	XMMATRIX mInvView;
	mInvView = XMMatrixInverse(NULL, m_mView);

	// The axis basis vectors and camera position are stored inside the 
	// position matrix in the 4 rows of the camera's world matrix.
	// To figure out the yaw/pitch of the camera, we just need the Z basis vector
	XMFLOAT3* pZBasis = (XMFLOAT3*)&mInvView.r[2];

	m_fCameraYawAngle = atan2f(pZBasis->x, pZBasis->y);
	float fLen = sqrtf(pZBasis->y * pZBasis->y + pZBasis->x * pZBasis->x);
	m_fCameraPitchAngle = -atan2f(pZBasis->z, fLen);
}


VOID CBaseCamera::SetViewLookAt(XMVECTOR* pvLookatPt)
{
	if (NULL == pvLookatPt)
		return;

	m_vDefaultLookAt = m_vLookAt = *pvLookatPt;
}

//--------------------------------------------------------------------------------------
// Calculates the projection matrix based on input params
//--------------------------------------------------------------------------------------
VOID CBaseCamera::SetProjParams(FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane,
	FLOAT fFarPlane)
{
	// Set attributes for the projection matrix
	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	if (g_bRightHanded == true) {
		m_mProj = XMMatrixPerspectiveFovRH(fFOV, fAspect, fNearPlane, fFarPlane);
	}
	else {
		m_mProj = XMMatrixPerspectiveFovLH(fFOV, fAspect, fNearPlane, fFarPlane);
	}
}




//--------------------------------------------------------------------------------------
// Call this from your message proc so this class can handle window messages
//--------------------------------------------------------------------------------------
LRESULT CBaseCamera::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		// Map this key to a D3DUtil_CameraKeys enum and update the
		// state of m_aKeys[] by adding the KEY_WAS_DOWN_MASK|KEY_IS_DOWN_MASK mask
		// only if the key is not down
		CameraKeys mappedKey = MapKey((UINT)wParam);
		if (mappedKey != CAM_UNKNOWN)
		{
			if (FALSE == IsKeyDown(m_aKeys[mappedKey]))
			{
				m_aKeys[mappedKey] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
				++m_cKeysDown;
			}
		}
		break;
	}

	case WM_KEYUP:
	{
		// Map this key to a D3DUtil_CameraKeys enum and update the
		// state of m_aKeys[] by removing the KEY_IS_DOWN_MASK mask.
		CameraKeys mappedKey = MapKey((UINT)wParam);
		if (mappedKey != CAM_UNKNOWN && (DWORD)mappedKey < 8)
		{
			m_aKeys[mappedKey] &= ~KEY_IS_DOWN_MASK;
			--m_cKeysDown;
		}
		break;
	}

	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	{
		// Compute the drag rectangle in screen coord.
		POINT ptCursor =
		{
			(short)LOWORD(lParam), (short)HIWORD(lParam)
		};

		// Update member var state
		if ((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && PtInRect(&m_rcDrag, ptCursor))
		{
			m_bMouseLButtonDown = true; m_nCurrentButtonMask |= MOUSE_LEFT_BUTTON;
		}
		if ((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && PtInRect(&m_rcDrag, ptCursor))
		{
			m_bMouseMButtonDown = true; m_nCurrentButtonMask |= MOUSE_MIDDLE_BUTTON;
		}
		if ((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && PtInRect(&m_rcDrag, ptCursor))
		{
			m_bMouseRButtonDown = true; m_nCurrentButtonMask |= MOUSE_RIGHT_BUTTON;
		}

		// Capture the mouse, so if the mouse button is 
		// released outside the window, we'll get the WM_LBUTTONUP message
		SetCapture(hWnd);
		GetCursorPos(&m_ptLastMousePosition);
		return TRUE;
	}

	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_LBUTTONUP:
	{
		// Update member var state
		if (uMsg == WM_LBUTTONUP)
		{
			m_bMouseLButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
		}
		if (uMsg == WM_MBUTTONUP)
		{
			m_bMouseMButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
		}
		if (uMsg == WM_RBUTTONUP)
		{
			m_bMouseRButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
		}

		// Release the capture if no mouse buttons down
		if (!m_bMouseLButtonDown &&
			!m_bMouseRButtonDown &&
			!m_bMouseMButtonDown)
		{
			ReleaseCapture();
		}
		break;
	}

	case WM_CAPTURECHANGED:
	{
		if ((HWND)lParam != hWnd)
		{
			if ((m_nCurrentButtonMask & MOUSE_LEFT_BUTTON) ||
				(m_nCurrentButtonMask & MOUSE_MIDDLE_BUTTON) ||
				(m_nCurrentButtonMask & MOUSE_RIGHT_BUTTON))
			{
				m_bMouseLButtonDown = false;
				m_bMouseMButtonDown = false;
				m_bMouseRButtonDown = false;
				m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
				m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
				m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
				ReleaseCapture();
			}
		}
		break;
	}

	case WM_MOUSEWHEEL:
	case UI_MOUSEWHEEL_FROM_PARENT:
		// Update member var state
		m_nMouseWheelDelta += (short)HIWORD(wParam);
		break;
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void CBaseCamera::GetInput(bool bGetKeyboardInput, bool bGetMouseInput, bool bGetGamepadInput,
	bool bResetCursorAfterMove)
{
	m_vKeyboardDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	if (bGetKeyboardInput)
	{
		// Update accceleration vector based on keyboard state
		if (IsKeyDown(m_aKeys[CAM_MOVE_FORWARD]))
			m_vKeyboardDirection.m128_f32[2] += 1.0f;
		if (IsKeyDown(m_aKeys[CAM_MOVE_BACKWARD]))
			m_vKeyboardDirection.m128_f32[2] -= 1.0f;
		if (m_bEnableYAxisMovement)
		{
			if (IsKeyDown(m_aKeys[CAM_MOVE_UP]))
				m_vKeyboardDirection.m128_f32[1] += 1.0f;
			if (IsKeyDown(m_aKeys[CAM_MOVE_DOWN]))
				m_vKeyboardDirection.m128_f32[1] -= 1.0f;
		}
		if (IsKeyDown(m_aKeys[CAM_STRAFE_RIGHT]))
			m_vKeyboardDirection.m128_f32[0] += 1.0f;
		if (IsKeyDown(m_aKeys[CAM_STRAFE_LEFT]))
			m_vKeyboardDirection.m128_f32[0] -= 1.0f;
	}

	if (bGetMouseInput)
	{
		UpdateMouseDelta();
	}

}


//--------------------------------------------------------------------------------------
// Figure out the mouse delta based on mouse movement
//--------------------------------------------------------------------------------------
void CBaseCamera::UpdateMouseDelta()
{
	POINT ptCurMouseDelta;
	POINT ptCurMousePos;

	// Get current position of mouse
	GetCursorPos(&ptCurMousePos);

	// Calc how far it's moved since last frame
	ptCurMouseDelta.x = ptCurMousePos.x - m_ptLastMousePosition.x;
	ptCurMouseDelta.y = ptCurMousePos.y - m_ptLastMousePosition.y;

	// Record current position for next time
	m_ptLastMousePosition = ptCurMousePos;

	//	if (m_bResetCursorAfterMove && DXUTIsActive())
	if (m_bResetCursorAfterMove)
	{
		// Set position of camera to center of desktop, 
		// so it always has room to move.  This is very useful
		// if the cursor is hidden.  If this isn't done and cursor is hidden, 
		// then invisible cursor will hit the edge of the screen 
		// and the user can't tell what happened
		POINT ptCenter;

		// Get the center of the current monitor
		MONITORINFO mi;
		mi.cbSize = sizeof(MONITORINFO);
		//		DXUTGetMonitorInfo(DXUTMonitorFromWindow(DXUTGetHWND(), MONITOR_DEFAULTTONEAREST), &mi);

		mi.rcMonitor.left = 0;
		mi.rcMonitor.right = 1280;
		mi.rcMonitor.top = 0;
		mi.rcMonitor.bottom = 1024;

		ptCenter.x = (mi.rcMonitor.left + mi.rcMonitor.right) / 2;
		ptCenter.y = (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
		SetCursorPos(ptCenter.x, ptCenter.y);
		m_ptLastMousePosition = ptCenter;
	}

	// Smooth the relative mouse data over a few frames so it isn't 
	// jerky when moving slowly at low frame rates.
	float fPercentOfNew = 1.0f / m_fFramesToSmoothMouseData;
	float fPercentOfOld = 1.0f - fPercentOfNew;
	m_vMouseDelta.m128_f32[0] = m_vMouseDelta.m128_f32[0] * fPercentOfOld + ptCurMouseDelta.x * fPercentOfNew;
	m_vMouseDelta.m128_f32[1] = m_vMouseDelta.m128_f32[1] * fPercentOfOld + ptCurMouseDelta.y * fPercentOfNew;

	m_vRotVelocity = m_vMouseDelta * m_fRotationScaler;
}




//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void CBaseCamera::UpdateVelocity(float fElapsedTime)
{
	XMMATRIX mRotDelta;

	UNREFERENCED_PARAMETER(mRotDelta);

	m_vRotVelocity = m_vMouseDelta * m_fRotationScaler;

	XMVECTOR vAccel = m_vKeyboardDirection;

	// Normalize vector so if moving 2 dirs (left & forward), 
	// the camera doesn't move faster than if moving in 1 dir
	vAccel = XMVector3Normalize(vAccel);

	// Scale the acceleration vector
	vAccel *= m_fMoveScaler;

	if (m_bMovementDrag)
	{
		// Is there any acceleration this frame?
		if (XMVector3LengthSq(vAccel).m128_f32[0] > 0.0f)
		{
			// If so, then this means the user has pressed a movement key\
						            // so change the velocity immediately to acceleration 
// upon keyboard input.  This isn't normal physics
// but it will give a quick response to keyboard input
			m_vVelocity = vAccel;
			m_fDragTimer = m_fTotalDragTimeToZero;
			m_vVelocityDrag = vAccel / m_fDragTimer;
		}
		else
		{
			// If no key being pressed, then slowly decrease velocity to 0
			if (m_fDragTimer > 0)
			{
				// Drag until timer is <= 0
				m_vVelocity -= m_vVelocityDrag * fElapsedTime;
				m_fDragTimer -= fElapsedTime;
			}
			else
			{
				// Zero velocity
				m_vVelocity = XMVectorSet(0, 0, 0, 0);
			}
		}
	}
	else
	{
		// No drag, so immediately change the velocity
		m_vVelocity = vAccel;
	}
}




//--------------------------------------------------------------------------------------
// Clamps pV to lie inside m_vMinBoundary & m_vMaxBoundary
//--------------------------------------------------------------------------------------
void CBaseCamera::ConstrainToBoundary(XMVECTOR* pV)
{
	// Constrain vector to a bounding box 
	pV->m128_f32[0] = __max(pV->m128_f32[0], m_vMinBoundary.m128_f32[0]);
	pV->m128_f32[1] = __max(pV->m128_f32[1], m_vMinBoundary.m128_f32[1]);
	pV->m128_f32[2] = __max(pV->m128_f32[2], m_vMinBoundary.m128_f32[2]);

	pV->m128_f32[0] = __min(pV->m128_f32[0], m_vMaxBoundary.m128_f32[0]);
	pV->m128_f32[1] = __min(pV->m128_f32[1], m_vMaxBoundary.m128_f32[1]);
	pV->m128_f32[2] = __min(pV->m128_f32[2], m_vMaxBoundary.m128_f32[2]);
}




//--------------------------------------------------------------------------------------
// Maps a windows virtual key to an enum
//--------------------------------------------------------------------------------------
CameraKeys CBaseCamera::MapKey(UINT nKey)
{
	// This could be upgraded to a method that's user-definable but for 
	// simplicity, we'll use a hardcoded mapping.
	switch (nKey)
	{
	case VK_CONTROL:
		return CAM_CONTROLDOWN;
	case VK_LEFT:
		return CAM_STRAFE_LEFT;
	case VK_RIGHT:
		return CAM_STRAFE_RIGHT;
	case VK_UP:
		return CAM_MOVE_FORWARD;
	case VK_DOWN:
		return CAM_MOVE_BACKWARD;
	case VK_PRIOR:
		return CAM_MOVE_UP;        // pgup
	case VK_NEXT:
		return CAM_MOVE_DOWN;      // pgdn

	case 'A':
		return CAM_STRAFE_LEFT;
	case 'D':
		return CAM_STRAFE_RIGHT;
	case 'W':
		return CAM_MOVE_FORWARD;
	case 'S':
		return CAM_MOVE_BACKWARD;
	case 'Q':
		return CAM_MOVE_DOWN;
	case 'E':
		return CAM_MOVE_UP;

	case VK_NUMPAD4:
		return CAM_STRAFE_LEFT;
	case VK_NUMPAD6:
		return CAM_STRAFE_RIGHT;
	case VK_NUMPAD8:
		return CAM_MOVE_FORWARD;
	case VK_NUMPAD2:
		return CAM_MOVE_BACKWARD;
	case VK_NUMPAD9:
		return CAM_MOVE_UP;
	case VK_NUMPAD3:
		return CAM_MOVE_DOWN;

	case VK_HOME:
		return CAM_RESET;
	}

	return CAM_UNKNOWN;
}




//--------------------------------------------------------------------------------------
// Reset the camera's position back to the default
//--------------------------------------------------------------------------------------
VOID CBaseCamera::Reset()
{
	SetViewParams(&m_vDefaultEye, &m_vDefaultLookAt, &m_vDefaultUp);
}






CModelViewerCamera::CModelViewerCamera()
{
	m_mWorld = XMMatrixIdentity();
	m_mModelRot = XMMatrixIdentity();
	m_mModelLastRot = XMMatrixIdentity();
	m_mCameraRotLast = XMMatrixIdentity();
	m_mCameraTranslationLast = XMMatrixIdentity();

	m_mProfile = XMMatrixIdentity();
	m_mSlicer = XMMatrixIdentity();

	m_vModelCenter = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_fRadius = 5.0f;
	m_fDefaultRadius = 5.0f;
	m_fMinRadius = 1.0f;
	m_fMaxRadius = FLT_MAX;
	m_bLimitPitch = false;
	m_bEnablePositionMovement = false;
	m_bAttachCameraToModel = false;

	m_nRotateModelButtonMask = MOUSE_LEFT_BUTTON;
	m_nZoomButtonMask = MOUSE_WHEEL;
	m_nRotateCameraButtonMask = MOUSE_RIGHT_BUTTON;
	m_bDragSinceLastUpdate = true;
}

CModelViewerCamera::~CModelViewerCamera()
{
}

void CModelViewerCamera::SliceMatrixIdentity()
{
	m_mSlicer = XMMatrixIdentity();
}

void CModelViewerCamera::ProfileMatrixIdentity()
{
	m_mProfile = XMMatrixIdentity();
}


void CModelViewerCamera::SetViewTranslation(XMVECTOR* pvTranslation)
{
	m_ViewArcBall.SetTranslation(pvTranslation);
	m_bDragSinceLastUpdate = true;
}

XMVECTOR* CModelViewerCamera::GetViewTranslation()
{
	return m_ViewArcBall.GetTranslation();
}

void CModelViewerCamera::SetProfileTranslation(XMVECTOR* pvTranslation)
{
	m_FeatureArcBall.SetTranslation(pvTranslation);
	m_bDragSinceLastUpdate = true;
}

XMVECTOR* CModelViewerCamera::GetProfileTranslation()
{
	return m_FeatureArcBall.GetTranslation();
}

//--------------------------------------------------------------------------------------
// Update the view matrix & the model's world matrix based 
//       on user input & elapsed time
//--------------------------------------------------------------------------------------
VOID CModelViewerCamera::FrameMove(FLOAT fElapsedTime)
{
	if (IsKeyDown(m_aKeys[CAM_RESET]))
		Reset();

	// If no dragged has happend since last time FrameMove is called,
	// and no camera key is held down, then no need to handle again.
	if (!m_bDragSinceLastUpdate && 0 == m_cKeysDown)
		return;
	m_bDragSinceLastUpdate = false;

	//// If no mouse button is held down, 
	//// Get the mouse movement (if any) if the mouse button are down
	//if( m_nCurrentButtonMask != 0 ) 
	//    UpdateMouseDelta( fElapsedTime );

	GetInput(m_bEnablePositionMovement, m_nCurrentButtonMask != 0, true, false);

	// Get amount of velocity based on the keyboard input and drag (if any)
	UpdateVelocity(fElapsedTime);

	// Simple euler method to calculate position delta
	XMVECTOR vPosDelta = m_vVelocity * fElapsedTime;

	// Change the radius from the camera to the model based on wheel scrolling
	if (m_nMouseWheelDelta && m_nZoomButtonMask == MOUSE_WHEEL)
		m_fRadius -= m_nMouseWheelDelta * m_fRadius * 0.1f / 120.0f;
	m_fRadius = __min(m_fMaxRadius, m_fRadius);
	m_fRadius = __max(m_fMinRadius, m_fRadius);
	m_nMouseWheelDelta = 0;
	m_ViewArcBall.SetTranslationRadius(m_fRadius / 1.414f);

	// Get the inverse of the arcball's rotation matrix
	XMMATRIX mCameraRot, mCameraTranslation;
	mCameraRot = XMMatrixInverse(NULL, m_ViewArcBall.GetRotationMatrix());
	mCameraTranslation = XMMatrixInverse(NULL, m_ViewArcBall.GetTranslationMatrix());

	// Transform vectors based on camera's rotation matrix
	XMVECTOR vWorldUp, vWorldAhead;
	XMVECTOR vLocalUp = XMLoadFloat3(&XMFLOAT3(0, 1, 0));
	XMVECTOR vLocalAhead = XMLoadFloat3(&XMFLOAT3(0, 0, 1));
	vLocalAhead = XMVector3Normalize(vLocalAhead);
	vLocalAhead = vLocalAhead * -1.0f;

	vWorldUp = XMVector3TransformCoord(vLocalUp, mCameraRot);
	vWorldAhead = XMVector3TransformCoord(vLocalAhead, mCameraRot);

	XMVECTOR vCameraTranslation = XMVector3TransformCoord(vPosDelta, mCameraTranslation);

	m_vLookAt = vCameraTranslation;
	if (m_bClipToBoundary)
		ConstrainToBoundary(&m_vLookAt);

	// Update the eye point based on a radius away from the lookAt position
	m_vEye = m_vLookAt - vWorldAhead * m_fRadius;

	// Update the view matrix
	if (g_bRightHanded == true) {
		m_mView = XMMatrixLookAtRH(m_vEye, m_vLookAt, vWorldUp);
	}
	else {
		m_mView = XMMatrixLookAtLH(m_vEye, m_vLookAt, vWorldUp);
	}
	XMMATRIX mInvView;
	mInvView = XMMatrixInverse(NULL, m_mView);
	mInvView.r[3].m128_f32[0] = mInvView.r[3].m128_f32[1] = mInvView.r[3].m128_f32[2] = 0;

	XMMATRIX mModelLastRotInv;
	mModelLastRotInv = XMMatrixInverse(NULL, m_mModelLastRot);

	// Accumulate the delta of the arcball's rotation in view space.
	// Note that per-frame delta rotations could be problematic over long periods of time.

	XMMATRIX mModelRot;
	switch (m_nDragWhichFeature) {
	case DRAG_VIEW:
		mModelRot = m_WorldArcBall.GetRotationMatrix();
		m_mModelRot *= m_mView * mModelLastRotInv * mModelRot * mInvView;
		break;
	}

	if (m_ViewArcBall.IsBeingDragged() && m_bAttachCameraToModel && !IsKeyDown(m_aKeys[CAM_CONTROLDOWN]))
	{
		// Attach camera to model by inverse of the model rotation
		XMMATRIX mCameraLastRotInv;
		mCameraLastRotInv = XMMatrixInverse(NULL, m_mCameraRotLast);
		XMMATRIX mCameraRotDelta = mCameraLastRotInv * mCameraRot; // local to world matrix
		m_mModelRot *= mCameraRotDelta;
	}
	m_mCameraRotLast = mCameraRot;
	m_mCameraTranslationLast = mCameraTranslation;
	m_mModelLastRot = mModelRot;

	//	m_ViewArcBall.ZeroTranslation();

	// Since we're accumulating delta rotations, we need to orthonormalize 
	// the matrix to prevent eventual matrix skew
	XMVECTOR* pXBasis = (XMVECTOR*)&m_mModelRot.r[0];
	XMVECTOR* pYBasis = (XMVECTOR*)&m_mModelRot.r[1];
	XMVECTOR* pZBasis = (XMVECTOR*)&m_mModelRot.r[2];
	*pXBasis = XMVector3Normalize(*pXBasis);
	*pYBasis = XMVector3Cross(*pZBasis, *pXBasis);
	*pYBasis = XMVector3Normalize(*pYBasis);
	*pZBasis = XMVector3Cross(*pXBasis, *pYBasis);

	// Translate the rotation matrix to the same position as the lookAt position
	m_mModelRot.r[3].m128_f32[0] = m_vLookAt.m128_f32[0];
	m_mModelRot.r[3].m128_f32[1] = m_vLookAt.m128_f32[1];
	m_mModelRot.r[3].m128_f32[2] = m_vLookAt.m128_f32[2];

	// Translate world matrix so its at the center of the model
	XMMATRIX mTrans;
	mTrans = XMMatrixTranslation(-m_vModelCenter.m128_f32[0], -m_vModelCenter.m128_f32[1], -m_vModelCenter.m128_f32[2]);
	m_mWorld = mTrans * m_mModelRot;
}

//--------------------------------------------------------------------------------------
// Update the profile matrix
//       on user input & elapsed time
//--------------------------------------------------------------------------------------
VOID CModelViewerCamera::FeatureMove(FLOAT fElapsedTime)
{
	// If no dragged has happend since last time FrameMove is called,
	// and no camera key is held down, then no need to handle again.
	if (!m_bDragSinceLastUpdate && 0 == m_cKeysDown)
		return;


	if (m_nMouseWheelDelta && m_nZoomButtonMask == MOUSE_WHEEL)
		m_fRadius -= m_nMouseWheelDelta * m_fRadius * 0.1f / 120.0f;
	m_fRadius = __min(m_fMaxRadius, m_fRadius);
	m_fRadius = __max(m_fMinRadius, m_fRadius);
	m_nMouseWheelDelta = 0;
	m_FeatureArcBall.SetTranslationRadius(m_fRadius / 1.414f);

	// Get the inverse of the arcball's rotation matrix
	XMMATRIX mInvRotStartOfDrag, mTran;
	mInvRotStartOfDrag = XMMatrixInverse(NULL, m_FeatureArcBall.GetRotationMatrixStartOfDrag());
	mTran = XMMatrixInverse(NULL, m_FeatureArcBall.GetTranslationMatrix());

	switch (m_nDragWhichFeature) {
	case DRAG_PROFILE:
		m_mProfile = XMMatrixMultiply(m_FeatureArcBall.GetRotationMatrix(), mInvRotStartOfDrag);
		break;
	case DRAG_SLICER:
		m_mSlicer = XMMatrixMultiply(m_FeatureArcBall.GetRotationMatrix(), mInvRotStartOfDrag);
		break;
	};
}

void CModelViewerCamera::SetDragRect(RECT& rc)
{
	CBaseCamera::SetDragRect(rc);

	m_WorldArcBall.SetOffset(rc.left, rc.top);
	m_ViewArcBall.SetOffset(rc.left, rc.top);
	m_FeatureArcBall.SetOffset(rc.left, rc.top);
	SetWindow(rc.right - rc.left, rc.bottom - rc.top);
}


//--------------------------------------------------------------------------------------
// Reset the camera's position back to the default
//--------------------------------------------------------------------------------------
VOID CModelViewerCamera::Reset()
{
	CBaseCamera::Reset();

	m_mWorld = XMMatrixIdentity();
	m_mModelRot = XMMatrixIdentity();
	m_mModelLastRot = XMMatrixIdentity();
	m_mCameraRotLast = XMMatrixIdentity();
	m_mCameraTranslationLast = XMMatrixIdentity();

	m_fRadius = m_fDefaultRadius;
	m_WorldArcBall.Reset();
	m_ViewArcBall.Reset();
	m_FeatureArcBall.Reset();
}

VOID CModelViewerCamera::FeatureArcBallEqualViewBall()
{
	m_FeatureArcBall.SetQuatNow(m_ViewArcBall.GetQuatNow());
	m_FeatureArcBall.SetQuatStartOfDrag(m_FeatureArcBall.GetQuatNow());
}

//--------------------------------------------------------------------------------------
// Override for setting the view parameters
//--------------------------------------------------------------------------------------
void CModelViewerCamera::SetViewParams(XMVECTOR* pvEyePt, XMVECTOR* pvLookatPt, XMVECTOR* pvUp)
{
	CBaseCamera::SetViewParams(pvEyePt, pvLookatPt, pvUp);

	// Propogate changes to the member arcball
	XMVECTOR quat;
	XMMATRIX mRotation;
	if (g_bRightHanded == true) {
		mRotation = XMMatrixLookAtRH(*pvEyePt, *pvLookatPt, *pvUp);
	}
	else {
		mRotation = XMMatrixLookAtLH(*pvEyePt, *pvLookatPt, *pvUp);
	}
	quat = XMQuaternionRotationMatrix(mRotation);
	m_ViewArcBall.SetQuatNow(quat);

	m_FeatureArcBall.SetQuatNow(quat);
	m_FeatureArcBall.SetQuatStartOfDrag(quat);
	//	m_FeatureArcBall.SetScreenToVectorMode(DRAG_PROFILE);

	// Set the radius according to the distance
	XMVECTOR vEyeToPoint;
	vEyeToPoint = XMVectorSubtract(*pvLookatPt, *pvEyePt);
	float fRadius = XMVector3Length(vEyeToPoint).m128_f32[0];
	SetRadius(fRadius);

	// View information changed. FrameMove should be called.
	m_bDragSinceLastUpdate = true;
}



//--------------------------------------------------------------------------------------
// Call this from your message proc so this class can handle window messages
//--------------------------------------------------------------------------------------
LRESULT CModelViewerCamera::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CBaseCamera::HandleMessages(hWnd, uMsg, wParam, lParam);

	if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
		((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
		((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
	{
		int iMouseX = (short)LOWORD(lParam);
		int iMouseY = (short)HIWORD(lParam);
		m_WorldArcBall.OnBegin(iMouseX, iMouseY, uMsg);
	}

	if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
		((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
		((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
	{
		int iMouseX = (short)LOWORD(lParam);
		int iMouseY = (short)HIWORD(lParam);
		switch (m_nDragWhichFeature) {
		case DRAG_VIEW: m_ViewArcBall.OnBegin(iMouseX, iMouseY, uMsg);
			break;
		case DRAG_SLICER:
		case DRAG_PROFILE:	m_FeatureArcBall.OnBegin(iMouseX, iMouseY, uMsg);
			break;
		}
	}

	if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nTranslateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
		((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nTranslateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
		((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nTranslateCameraButtonMask & MOUSE_RIGHT_BUTTON))
	{
		int iMouseX = (short)LOWORD(lParam);
		int iMouseY = (short)HIWORD(lParam);
		switch (m_nDragWhichFeature) {
		case DRAG_VIEW: m_ViewArcBall.OnBegin(iMouseX, iMouseY, uMsg);
			break;
		case DRAG_SLICER:
		case DRAG_PROFILE:	m_FeatureArcBall.OnBegin(iMouseX, iMouseY, uMsg);
			break;
		}
	}

	if (uMsg == WM_MOUSEMOVE)
	{
		int iMouseX = (short)LOWORD(lParam);
		int iMouseY = (short)HIWORD(lParam);

		switch (m_nDragWhichFeature) {
		case DRAG_VIEW:
			m_WorldArcBall.OnMove(iMouseX, iMouseY);
			m_ViewArcBall.OnMove(iMouseX, iMouseY);
			pickRayVector((float)iMouseX, (float)(iMouseY - 22), g_vPickEyePt, g_vPickEyeDir);
			break;
		case DRAG_SLICER:
		case DRAG_PROFILE:	m_FeatureArcBall.OnMove(iMouseX, iMouseY);
			break;
		}
	}

	if ((uMsg == WM_LBUTTONUP && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
		(uMsg == WM_MBUTTONUP && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
		(uMsg == WM_RBUTTONUP && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
	{
		m_WorldArcBall.OnEnd();
	}
	if ((uMsg == WM_LBUTTONUP && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
		(uMsg == WM_MBUTTONUP && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
		(uMsg == WM_RBUTTONUP && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
	{
		switch (m_nDragWhichFeature) {
		case DRAG_VIEW:
			m_ViewArcBall.OnEnd();
			FeatureArcBallEqualViewBall();
			break;
		case DRAG_SLICER:
		case DRAG_PROFILE:
			m_FeatureArcBall.OnEnd();
			break;
		}
	}
	if ((uMsg == WM_LBUTTONUP && m_nTranslateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
		(uMsg == WM_MBUTTONUP && m_nTranslateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
		(uMsg == WM_RBUTTONUP && m_nTranslateCameraButtonMask & MOUSE_RIGHT_BUTTON))
	{
		switch (m_nDragWhichFeature) {
		case DRAG_VIEW:
			m_ViewArcBall.OnEnd();
			FeatureArcBallEqualViewBall();
			break;
		case DRAG_PROFILE:
		case DRAG_SLICER:
			m_FeatureArcBall.OnEnd();
			break;
		}
	}

	if (uMsg == WM_CAPTURECHANGED)
	{
		if ((HWND)lParam != hWnd)
		{
			if ((m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
				(m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
				(m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
			{
				m_WorldArcBall.OnEnd();
			}

			if ((m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
				(m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
				(m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
			{
				switch (m_nDragWhichFeature) {
				case DRAG_VIEW:
					m_ViewArcBall.OnEnd();
					FeatureArcBallEqualViewBall();
					break;
				case DRAG_PROFILE:
				case DRAG_SLICER:
					m_FeatureArcBall.OnEnd();
					break;
				}
			}

			if ((m_nTranslateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
				(m_nTranslateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
				(m_nTranslateCameraButtonMask & MOUSE_RIGHT_BUTTON))
			{
				switch (m_nDragWhichFeature) {
				case DRAG_VIEW:
					m_ViewArcBall.OnEnd();
					FeatureArcBallEqualViewBall();
					break;
				case DRAG_PROFILE:
				case DRAG_SLICER:
					m_FeatureArcBall.OnEnd();
					break;
				}
			}
		}
	}

	if (uMsg == WM_LBUTTONDOWN ||
		uMsg == WM_LBUTTONDBLCLK ||
		uMsg == WM_MBUTTONDOWN ||
		uMsg == WM_MBUTTONDBLCLK ||
		uMsg == WM_RBUTTONDOWN ||
		uMsg == WM_RBUTTONDBLCLK ||
		uMsg == WM_LBUTTONUP ||
		uMsg == WM_MBUTTONUP ||
		uMsg == WM_RBUTTONUP ||
		uMsg == WM_MOUSEWHEEL ||
		uMsg == WM_MOUSEMOVE)
	{
		m_bDragSinceLastUpdate = true;
	}

	return FALSE;
}

XMVECTOR *CModelViewerCamera::getPickEyePt()
{
	return &g_vPickEyePt;
}

XMVECTOR *CModelViewerCamera::getPickEyeDir()
{
	g_vPickEyeDir = XMVector3Normalize(g_vPickEyeDir);
	return &g_vPickEyeDir;
}


void CModelViewerCamera::pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir)
{
	XMVECTOR pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR pickRayInViewSpacePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float PRVecX, PRVecY, PRVecZ;
	XMMATRIX mProj = *GetProjMatrix();
	XMMATRIX mView = *GetViewMatrix();

	//Transform 2D pick position on screen space to 3D ray in View space
	PRVecX = (((2.0f * mouseX) / m_fClientWidth) - 1) / mProj.r[0].m128_f32[0];
	PRVecY = -(((2.0f * mouseY) / m_fClientHeight) - 1) / mProj.r[1].m128_f32[1];
	PRVecZ = -1.0f;	//View space's Y direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

	pickRayInViewSpaceDir = XMVectorSet(PRVecX, PRVecY, PRVecZ, 0.0f);

	//Uncomment this line if you want to use the center of the screen (client area)
	//to be the point that creates the picking ray (eg. first person shooter)
	//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Transform 3D Ray from View space to 3D ray in World space
	XMMATRIX pickRayToWorldSpaceMatrix;
	XMVECTOR matInvDeter;	//We don't use this, but the xna matrix inverse function requires the first parameter to not be null

	pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, mView);	//Inverse of View Space matrix is World space matrix

	pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix); //eyPt
	pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);

}




CAnimate::CAnimate()
{
	m_bProcess = false;
}


CAnimate::~CAnimate()
{
}

void CAnimate::setViewRotate(CModelViewerCamera* pCamera, XMVECTOR qFinish)
{

	m_qStart = pCamera->GetViewQuat();
	m_qFinish = qFinish;
	m_nAnimateStyle = RotateView;
	m_nCurrentFrame = 0;
	m_nTotalFrames = 8;
	m_bProcess = true;

}

void CAnimate::setLookAt(CModelViewerCamera* pCamera, XMVECTOR vFinish)
{
	m_vStart = *pCamera->GetViewTranslation();
	m_vFinish = vFinish;
	m_nAnimateStyle = TranslateLookAtPt;
	m_nCurrentFrame = 0;
	m_nTotalFrames = 8;
	m_bProcess = true;

}

void CAnimate::setProfileTransform(CModelViewerCamera* pCamera, XMVECTOR *pqRotate, XMVECTOR *pvCenterOfRotate, XMVECTOR *pvTranslation)
{
	XMMATRIX mStart = XMMatrixIdentity();

	m_vStart = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_vFinish = *pvTranslation;
	m_vCenterOfRotate = *pvCenterOfRotate;
	m_qStart = XMQuaternionIdentity();
	m_qFinish = *pqRotate;

	m_nAnimateStyle = TransformProfile;
	m_nCurrentFrame = 0;
	m_nTotalFrames = 8;
	m_bProcess = true;

}

void CAnimate::FrameMove(CModelViewerCamera* pCamera)
{
	XMVECTOR qStep;
	XMMATRIX mComponent;
	XMVECTOR vStep;

	if (m_bProcess == true) {
		switch (m_nAnimateStyle) {
		case RotateView:
			qStep = XMQuaternionSlerp(m_qStart, m_qFinish, (float)m_nCurrentFrame / (float)m_nTotalFrames);
			pCamera->SetViewQuat(XMQuaternionNormalize(qStep));
			break;
		case TranslateLookAtPt:
			vStep = (m_vFinish - m_vStart) * (float)m_nCurrentFrame / (float)m_nTotalFrames;
			vStep += m_vStart;
			pCamera->SetViewTranslation(&vStep);
			break;
		case TransformProfile:
			mComponent = XMMatrixTranslationFromVector(m_vCenterOfRotate * -1.0f);
			qStep = XMQuaternionSlerp(m_qStart, m_qFinish, (float)1 / (float)(m_nTotalFrames + 1));
			XMQuaternionNormalize(qStep);
			mComponent = XMMatrixMultiply(mComponent, XMMatrixRotationQuaternion(qStep));
			mComponent = XMMatrixMultiply(mComponent, XMMatrixTranslationFromVector(m_vCenterOfRotate));

			vStep = (m_vFinish - m_vStart) * (float)1 / (float)m_nTotalFrames;
			mComponent = XMMatrixMultiply(mComponent, XMMatrixTranslation(XMVectorGetX(vStep), XMVectorGetY(vStep), XMVectorGetZ(vStep)));

//			m_WS.MatrixMultiply(-1, mComponent);
			break;
		}
		if (m_nCurrentFrame < m_nTotalFrames) {
			m_nCurrentFrame++;
		}
		else {
			m_bProcess = false;
		}
	}

}

bool CAnimate::NeedsAnimating()
{
	return m_bProcess;
}