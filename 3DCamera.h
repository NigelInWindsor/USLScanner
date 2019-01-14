#pragma once


//extern XMVECTOR m_qDown;             // Quaternion before button down
//extern XMVECTOR m_qNow;              // Composite quaternion for current drag
//extern XMVECTOR m_vDownPt;           // starting point of rotation arc
//extern XMVECTOR m_vCurrentPt;        // current point of rotation arc

//Base camera
extern XMVECTOR m_vDefaultEye;          // Default camera eye position
extern XMVECTOR m_vDefaultLookAt;       // Default LookAt position
extern XMVECTOR m_vDefaultUp;       // Default Up Vetcor
extern XMVECTOR m_vEye;                 // Camera eye position
extern XMVECTOR m_vLookAt;              // LookAt position
extern XMVECTOR m_vUp;              // Up Vector
									//extern XMVECTOR m_vCameraTranslation;

extern bool g_bRightHanded;

#define	DRAG_VIEW		0
#define DRAG_PROFILE	1
#define	DRAG_SLICER		2

__declspec(align(16)) class CD3DArcBall_11
{
public:
	CD3DArcBall_11();
	~CD3DArcBall_11();
	void                            Reset();
	void							ZeroTranslation();
	void                            SetTranslationRadius(FLOAT fRadiusTranslation)
	{
		m_fRadiusTranslation = fRadiusTranslation;
	}
	void                            SetRadius(FLOAT fRadius)
	{
		m_fRadius = fRadius;
	}
	void SetTranslation(XMVECTOR* pTranslation);
	XMVECTOR* GetTranslation();
	void                            SetWindow(INT nWidth, INT nHeight, FLOAT fRadius = 0.9f)
	{
		m_nWidth = nWidth; m_nHeight = nHeight; m_fRadius = fRadius;
		m_vCenter = XMFLOAT2(m_nWidth / 2.0f, m_nHeight / 2.0f);
	}
	void                            SetOffset(INT nX, INT nY)
	{
		m_Offset.x = nX; m_Offset.y = nY;
	}

	// Call these from client and use GetRotationMatrix() to read new rotation matrix
	void                            OnBegin(int nX, int nY, int nMouseButtons);  // start the rotation (pass current mouse position)
	void                            OnMove(int nX, int nY);   // continue the rotation (pass current mouse position)
	void                            OnEnd();                    // end the rotation 

																// Or call this to automatically handle left, middle, right buttons
	LRESULT                         HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Functions to get/set state
	const XMMATRIX GetRotationMatrix()
	{
		return XMMatrixRotationQuaternion(m_qNow);
	};
	const XMMATRIX GetRotationMatrixStartOfDrag()
	{
		return XMMatrixRotationQuaternion(m_qStartOfDrag);
	};
	const XMMATRIX GetTranslationMatrix() const
	{
		return m_mTranslation;
	}
	const XMMATRIX GetTranslationDeltaMatrix() const
	{
		return m_mTranslationDelta;
	}
	bool                            IsBeingDragged() const
	{
		return m_bDrag;
	}
	XMVECTOR                  GetQuatNow() const
	{
		return m_qNow;
	}
	void                            SetQuatNow(XMVECTOR q)
	{
		m_qNow = q;
	}
	void                            SetQuatStartOfDrag(XMVECTOR q)
	{
		m_qStartOfDrag = q;
	}

	//	static XMVECTOR WINAPI    QuatFromBallPoints(const XMVECTOR& vFrom, const XMVECTOR& vTo);
	XMVECTOR    QuatFromBallPoints(XMVECTOR& vFrom, XMVECTOR& vTo);


protected:
	XMVECTOR m_qDown;             // Quaternion before button down
	XMVECTOR m_qNow;              // Composite quaternion for current drag
	XMVECTOR m_qStartOfDrag;
	XMVECTOR m_vDownPt;           // starting point of rotation arc
	XMVECTOR m_vCurrentPt;        // current point of rotation arc

	XMMATRIX m_mRotation;         // Matrix for arc ball's orientation
	XMMATRIX m_mTranslation;      // Matrix for arc ball's position
	XMMATRIX m_mTranslationDelta; // Matrix for arc ball's position

	POINT m_Offset;   // window offset, or upper-left corner of window
	INT m_nWidth;   // arc ball's window width
	INT m_nHeight;  // arc ball's window height
	XMFLOAT2 m_vCenter;  // center of arc ball 
	FLOAT m_fRadius;  // arc ball's radius in screen coords
	FLOAT m_fRadiusTranslation; // arc ball's radius for translating the target

	bool m_bDrag;             // Whether user is dragging arc ball
	bool m_bMouseLButtonDown;    // True if left button is down 
	bool m_bMouseMButtonDown;    // True if middle button is down 
	bool m_bMouseRButtonDown;    // True if right button is down 
	POINT m_ptLastMouse;      // position of last mouse point

	XMVECTOR                     ScreenToVector(float fScreenPtX, float fScreenPtY);
};

enum CameraKeys
{
	CAM_STRAFE_LEFT = 0,
	CAM_STRAFE_RIGHT,
	CAM_MOVE_FORWARD,
	CAM_MOVE_BACKWARD,
	CAM_MOVE_UP,
	CAM_MOVE_DOWN,
	CAM_RESET,
	CAM_CONTROLDOWN,
	CAM_MAX_KEYS,
	CAM_UNKNOWN = 0xFF
};

#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04
#define MOUSE_WHEEL         0x08

__declspec(align(16)) class CBaseCamera
{
public:
	CBaseCamera();
	~CBaseCamera();
	// Call these from client and use Get*Matrix() to read new matrices
	virtual LRESULT             HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void                FrameMove(FLOAT fElapsedTime) = 0;

	// Functions to change camera matrices
	virtual void                Reset();
	virtual void                SetViewParams(XMVECTOR* pvEyePt, XMVECTOR* pvLookatPt, XMVECTOR* pvUp);
	virtual void                SetProjParams(FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane, FLOAT fFarPlane);
	virtual void				SetViewLookAt(XMVECTOR* pvLookatPt);

	// Functions to change behavior
	virtual void                SetDragRect(RECT& rc)
	{
		m_rcDrag = rc;
	}
	void                        SetInvertPitch(bool bInvertPitch)
	{
		m_bInvertPitch = bInvertPitch;
	}
	void                        SetDrag(bool bMovementDrag, FLOAT fTotalDragTimeToZero = 0.25f)
	{
		m_bMovementDrag = bMovementDrag; m_fTotalDragTimeToZero = fTotalDragTimeToZero;
	}
	void                        SetEnableYAxisMovement(bool bEnableYAxisMovement)
	{
		m_bEnableYAxisMovement = bEnableYAxisMovement;
	}
	void                        SetEnablePositionMovement(bool bEnablePositionMovement)
	{
		m_bEnablePositionMovement = bEnablePositionMovement;
	}
	void                        SetClipToBoundary(bool bClipToBoundary, XMVECTOR* pvMinBoundary,
		XMVECTOR* pvMaxBoundary)
	{
		m_bClipToBoundary = bClipToBoundary; if (pvMinBoundary) m_vMinBoundary = *pvMinBoundary;
		if (pvMaxBoundary) m_vMaxBoundary = *pvMaxBoundary;
	}
	void                        SetScalers(FLOAT fRotationScaler = 0.01f, FLOAT fMoveScaler = 5.0f)
	{
		m_fRotationScaler = fRotationScaler; m_fMoveScaler = fMoveScaler;
	}
	void                        SetNumberOfFramesToSmoothMouseData(int nFrames)
	{
		if (nFrames > 0) m_fFramesToSmoothMouseData = (float)nFrames;
	}
	void                        SetResetCursorAfterMove(bool bResetCursorAfterMove)
	{
		m_bResetCursorAfterMove = bResetCursorAfterMove;
	}

	// Functions to get state
	const XMMATRIX* GetSlicerMatrix() const
	{
		return &m_mSlicer;
	}
	const XMMATRIX* GetProfileMatrix() const
	{
		return &m_mProfile;
	}
	const XMMATRIX* GetViewMatrix() const
	{
		return &m_mView;
	}
	const XMMATRIX* GetProjMatrix() const
	{
		return &m_mProj;
	}
	const XMVECTOR* GetEyePt() const
	{
		return &m_vEye;
	}
	const XMVECTOR* GetLookAtPt() const
	{
		return &m_vLookAt;
	}
	float                       GetNearClip() const
	{
		return m_fNearPlane;
	}
	float                       GetFarClip() const
	{
		return m_fFarPlane;
	}

	bool                        IsBeingDragged() const
	{
		return (m_bMouseLButtonDown || m_bMouseMButtonDown || m_bMouseRButtonDown);
	}
	bool                        IsMouseLButtonDown() const
	{
		return m_bMouseLButtonDown;
	}
	bool                        IsMouseMButtonDown() const
	{
		return m_bMouseMButtonDown;
	}
	bool                        IsMouseRButtonDown() const
	{
		return m_bMouseRButtonDown;
	}

protected:
	// Functions to map a WM_KEYDOWN key to a D3DUtil_CameraKeys enum
	virtual CameraKeys  MapKey(UINT nKey);
	bool                        IsKeyDown(BYTE key) const
	{
		return((key & KEY_IS_DOWN_MASK) == KEY_IS_DOWN_MASK);
	}
	bool                        WasKeyDown(BYTE key) const
	{
		return((key & KEY_WAS_DOWN_MASK) == KEY_WAS_DOWN_MASK);
	}

	void                        ConstrainToBoundary(XMVECTOR* pV);
	void                        UpdateMouseDelta();
	void                        UpdateVelocity(float fElapsedTime);
	void                        GetInput(bool bGetKeyboardInput, bool bGetMouseInput, bool bGetGamepadInput,
		bool bResetCursorAfterMove);

	XMMATRIX m_mView;              // View matrix 
	XMMATRIX m_mProfile;
	XMMATRIX m_mSlicer;
	XMMATRIX m_mProj;              // Projection matrix

								   //	DXUT_GAMEPAD                m_GamePad[DXUT_MAX_CONTROLLERS];  // XInput controller state
	XMFLOAT3 m_vGamePadLeftThumb;
	XMFLOAT3 m_vGamePadRightThumb;
	//	double                      m_GamePadLastActive[DXUT_MAX_CONTROLLERS];

	int m_cKeysDown;            // Number of camera keys that are down.
	BYTE                        m_aKeys[CAM_MAX_KEYS];  // State of input - KEY_WAS_DOWN_MASK|KEY_IS_DOWN_MASK
	XMVECTOR m_vKeyboardDirection;   // Direction vector of keyboard input
	POINT m_ptLastMousePosition;  // Last absolute position of mouse cursor
	bool m_bMouseLButtonDown;    // True if left button is down 
	bool m_bMouseMButtonDown;    // True if middle button is down 
	bool m_bMouseRButtonDown;    // True if right button is down 
	int m_nCurrentButtonMask;   // mask of which buttons are down
	int m_nMouseWheelDelta;     // Amount of middle wheel scroll (+/-) 
	XMVECTOR m_vMouseDelta;          // Mouse relative delta smoothed over a few frames
	float m_fFramesToSmoothMouseData; // Number of frames to smooth mouse data over

	float m_fCameraYawAngle;      // Yaw angle of camera
	float m_fCameraPitchAngle;    // Pitch angle of camera

	RECT m_rcDrag;               // Rectangle within which a drag can be initiated.
	XMVECTOR m_vVelocity;            // Velocity of camera
	bool m_bMovementDrag;        // If true, then camera movement will slow to a stop otherwise movement is instant
	XMVECTOR m_vVelocityDrag;        // Velocity drag force
	FLOAT m_fDragTimer;           // Countdown timer to apply drag
	FLOAT m_fTotalDragTimeToZero; // Time it takes for velocity to go from full to 0
	XMVECTOR m_vRotVelocity;         // Velocity of camera

	float m_fFOV;                 // Field of view
	float m_fAspect;              // Aspect ratio
	float m_fNearPlane;           // Near plane
	float m_fFarPlane;            // Far plane

	float m_fRotationScaler;      // Scaler for rotation
	float m_fMoveScaler;          // Scaler for movement

	bool m_bInvertPitch;         // Invert the pitch axis
	bool m_bEnablePositionMovement; // If true, then the user can translate the camera/model 
	bool m_bEnableYAxisMovement; // If true, then camera can move in the y-axis

	bool m_bClipToBoundary;      // If true, then the camera will be clipped to the boundary
	XMVECTOR m_vMinBoundary;         // Min point in clip boundary
	XMVECTOR m_vMaxBoundary;         // Max point in clip boundary

	bool m_bResetCursorAfterMove;// If true, the class will reset the cursor position so that the cursor always has space to move 

};

__declspec(align(16)) class CModelViewerCamera :
	public CBaseCamera
{
public:
	CModelViewerCamera();
	~CModelViewerCamera();
	void SliceMatrixIdentity();
	void ProfileMatrixIdentity();
	// Call these from client and use Get*Matrix() to read new matrices
	virtual LRESULT HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	XMVECTOR * getPickEyePt();
	XMVECTOR * getPickEyeDir();
	void pickRayVector(float mouseX, float mouseY, XMVECTOR & pickRayInWorldSpacePos, XMVECTOR & pickRayInWorldSpaceDir);
	virtual void    FrameMove(FLOAT fElapsedTime);
	virtual	void	FeatureMove(FLOAT fElapsedTime);

	// Functions to change behavior
	virtual void    SetDragRect(RECT& rc);
	void            Reset();
	VOID			FeatureArcBallEqualViewBall();
	void            SetViewParams(XMVECTOR* pvEyePt, XMVECTOR* pvLookatPt, XMVECTOR* pvUp);
	void			SetViewTranslation(XMVECTOR* pvTranslation);
	XMVECTOR*		GetViewTranslation();
	void SetProfileTranslation(XMVECTOR * pvTranslation);
	XMVECTOR * GetProfileTranslation();
	void            SetButtonMasks(int nRotateModelButtonMask = 0, int nZoomButtonMask = MOUSE_WHEEL,
		int nRotateCameraButtonMask = MOUSE_LEFT_BUTTON, int nTranslateCameraButtonMask = MOUSE_MIDDLE_BUTTON | MOUSE_RIGHT_BUTTON)
	{
		m_nRotateModelButtonMask = nRotateModelButtonMask, m_nZoomButtonMask = nZoomButtonMask;
		m_nRotateCameraButtonMask = nRotateCameraButtonMask;
		m_nTranslateCameraButtonMask = nTranslateCameraButtonMask;
	}
	void            SetAttachCameraToModel(bool bEnable = false)
	{
		m_bAttachCameraToModel = bEnable;
	}
	void            SetWindow(int nWidth, int nHeight, float fArcballRadius = 0.9f)
	{
		m_WorldArcBall.SetWindow(nWidth, nHeight, fArcballRadius);
		m_ViewArcBall.SetWindow(nWidth, nHeight, fArcballRadius);
		m_FeatureArcBall.SetWindow(nWidth, nHeight, fArcballRadius);
		m_fClientWidth = (float)nWidth;
		m_fClientHeight = (float)nHeight;

	}
	void            SetRadius(float fDefaultRadius = 5.0f, float fMinRadius = 1.0f, float fMaxRadius = FLT_MAX)
	{
		m_fDefaultRadius = m_fRadius = fDefaultRadius; m_fMinRadius = fMinRadius; m_fMaxRadius = fMaxRadius;
		m_bDragSinceLastUpdate = true;
	}
	void						SetDragWhich(int nWhich)
	{
		m_nDragWhichFeature = nWhich;
	}
	int						GetDragWhich()
	{
		return m_nDragWhichFeature;
	}

	float	GetRadius()
	{
		return m_fRadius;
	}
	void            SetModelCenter(XMVECTOR vModelCenter)
	{
		m_vModelCenter = vModelCenter;
	}
	void            SetLimitPitch(bool bLimitPitch)
	{
		m_bLimitPitch = bLimitPitch;
	}
	void            SetViewQuat(XMVECTOR q)
	{
		m_ViewArcBall.SetQuatNow(q); m_bDragSinceLastUpdate = true;
	}
	void            SetWorldQuat(XMVECTOR q)
	{
		m_WorldArcBall.SetQuatNow(q); m_bDragSinceLastUpdate = true;
	}
	XMVECTOR         GetViewQuat()
	{
		return m_ViewArcBall.GetQuatNow();
	}
	XMVECTOR         GetWorldQuat()
	{
		return m_WorldArcBall.GetQuatNow();
	}

	// Functions to get state
	const XMMATRIX* GetWorldMatrix() const
	{
		return &m_mWorld;
	}
	void            SetWorldMatrix(XMMATRIX& mWorld)
	{
		m_mWorld = mWorld; m_bDragSinceLastUpdate = true;
	}

protected:
	CD3DArcBall_11 m_WorldArcBall;
	CD3DArcBall_11 m_ViewArcBall;
	CD3DArcBall_11 m_FeatureArcBall;
	XMVECTOR m_vModelCenter;
	XMMATRIX m_mModelLastRot;        // Last arcball rotation matrix for model 
	XMMATRIX m_mModelRot;            // Rotation matrix of model
	XMMATRIX m_mWorld;               // World matrix of model

	int m_nRotateModelButtonMask;
	int m_nZoomButtonMask;
	int m_nRotateCameraButtonMask;
	int m_nTranslateCameraButtonMask;

	bool m_bAttachCameraToModel;
	bool m_bLimitPitch;
	float m_fRadius;              // Distance from the camera to model 
	float m_fDefaultRadius;       // Distance from the camera to model 
	float m_fMinRadius;           // Min radius
	float m_fMaxRadius;           // Max radius
	bool m_bDragSinceLastUpdate; // True if mouse drag has happened since last time FrameMove is called.
	int m_nDragWhichFeature;
	float m_fClientWidth;
	float m_fClientHeight;

	XMMATRIX m_mCameraRotLast;
	XMMATRIX m_mCameraTranslationLast;
};

class CAnimate
{
	enum {
		RotateView = 0,
		TranslateLookAtPt,
		TransformProfile,
	};
protected:
	int	m_nAnimateStyle;
	XMVECTOR m_vStart;
	XMVECTOR m_vFinish;
	XMVECTOR m_qStart;
	XMVECTOR m_qFinish;
	XMVECTOR m_vCenterOfRotate;
	int	m_nCurrentFrame;
	int m_nTotalFrames;
	bool m_bProcess;

public:
	void setProfileTransform(CModelViewerCamera* pCamera, XMVECTOR *pqRotate, XMVECTOR *pvCenterOfRotate, XMVECTOR *pvTranslation);
	void FrameMove(CModelViewerCamera * pCamera);
	bool NeedsAnimating();
	void setViewRotate(CModelViewerCamera* pCamera, XMVECTOR qFinish);
	void setLookAt(CModelViewerCamera* pCamera, XMVECTOR vFinish);
	CAnimate();
	~CAnimate();
};


