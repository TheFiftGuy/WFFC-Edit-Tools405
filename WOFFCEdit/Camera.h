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

public:

	Camera();
	~Camera();

	void Update(DX::StepTimer const& timer);

	void SetInputCommand(InputCommands* Input);

	DirectX::SimpleMath::Matrix GetCameraViewMat() const;

};

