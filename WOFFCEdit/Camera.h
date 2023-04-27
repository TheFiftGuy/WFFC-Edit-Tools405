#pragma once
#include "pch.h"
#include "StepTimer.h"

struct InputCommands;

class Camera
{

private:
	//functionality
	float								m_movespeed;
	//camera
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float m_camSensitivity;

	//View matrix
	DirectX::SimpleMath::Matrix			m_view;

	InputCommands*						m_InputCommands;
	
	int prevMouseX, prevMouseY;
	bool postUpdateMoveCam = false;

	//Cam focus lerp stuff
	float LerpTime;
	float RemainingLerpTime;
	DirectX::SimpleMath::Vector3 LerpFrom;
	DirectX::SimpleMath::Vector3 LerpTo;


public:

	Camera();
	~Camera();

	void Update(DX::StepTimer const& timer);

	void SetInputCommand(InputCommands* Input);

	DirectX::SimpleMath::Matrix GetCameraViewMat() const;

	void LookAtObject(DirectX::SimpleMath::Vector3 ObjectPos);

	//Lerp over Time near TargetPosition but keep a distance of NearDistance. (Approach but keep a NearDistance) 
	void InterpolateNearPosition(DirectX::SimpleMath::Vector3 TargetPosition, float NearDistance = 5.f, float Time = 1.f);

private:

	void InterpolateUpdate(DX::StepTimer const& timer);

};

