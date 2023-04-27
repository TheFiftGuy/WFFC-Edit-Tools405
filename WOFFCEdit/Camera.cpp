#include "Camera.h"
#include "InputCommands.h"
#include <cmath>


using namespace DirectX::SimpleMath;


Camera::Camera()
{
	////functional
	m_movespeed = 0.30;
	//m_camRotRate = 3.0;
	m_camSensitivity = 0.1;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;//pitch
	m_camOrientation.y = 0;//yaw
	m_camOrientation.z = 0;//roll

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	prevMouseX = 0;
	prevMouseY = 0;

	LerpTime = 0.f;
	RemainingLerpTime = 0.f;
	LerpFrom = Vector3();
	LerpTo = Vector3();
}

Camera::~Camera()
{
}

void Camera::Update(DX::StepTimer const& timer)
{

	if (postUpdateMoveCam) {
		postUpdateMoveCam = false;
		return;
	}

	if ((m_InputCommands->mouse_x > 0 || m_InputCommands->mouse_y > 0) && m_InputCommands->mouse_RB_Down == true) {

		if (prevMouseX <= 0 && prevMouseY <= 0) {
			//if no previous, use current as previous and dont move cam until next frame
			prevMouseX = m_InputCommands->mouse_x;
			prevMouseY = m_InputCommands->mouse_y;
			return;
		}
		float deltaX = m_InputCommands->mouse_x - prevMouseX;
		float deltaY = m_InputCommands->mouse_y - prevMouseY;

		deltaX *= m_camSensitivity;
		deltaY *= m_camSensitivity;

		m_camOrientation.y -= deltaX;
		m_camOrientation.x += deltaY;

		if (m_camOrientation.x > 89.0f) {
			m_camOrientation.x = 89.0f;
		}
		if (m_camOrientation.x < -89.0f) {
			m_camOrientation.x = -89.0f;
		}

		if (m_camOrientation.y < 0.0f) {
			m_camOrientation.y += 360.0f;
		}
		if (m_camOrientation.y > 360.0f) {
			m_camOrientation.y -= 360.0f;
		}


		prevMouseX = m_InputCommands->mouse_x;
		prevMouseY = m_InputCommands->mouse_y;
	}
	else {
		prevMouseX = 0;
		prevMouseY = 0;
	}

	//forward.x = cos(radians(yaw)) * cos(radians(pitch));
	//forward.y = sin(radians(pitch));
	//forward.z = sin(radians(yaw)) * cos(radians(pitch));
	// 
	//create look direction from Euler angles in m_camOrientation
	//m_camLookDirection.x = sin((m_camOrientation.y) * 3.1415 / 180);
	//m_camLookDirection.z = cos((m_camOrientation.y) * 3.1415 / 180);
	//m_camLookDirection.Normalize();

	m_camLookDirection.x = cos((m_camOrientation.y) * 3.1415 / 180) * cos((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.y = sin((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.z = sin((m_camOrientation.y) * 3.1415 / 180) * cos((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.Normalize();
	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (m_InputCommands->forward)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}
	if (m_InputCommands->back)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}
	if (m_InputCommands->right)
	{
		m_camPosition += m_camRight * m_movespeed;
	}
	if (m_InputCommands->left)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}
	//moves the Camera staright up down in world space, not in camera up direction
	if (m_InputCommands->up)
	{
		m_camPosition += Vector3::UnitY * m_movespeed;
	}
	if (m_InputCommands->down)
	{
		m_camPosition -= Vector3::UnitY * m_movespeed;
	}

	//D new cam lerp
	if (RemainingLerpTime > 0.f) {
		InterpolateUpdate(timer);
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}

void Camera::SetInputCommand(InputCommands* Input)
{
	m_InputCommands = Input;
}

DirectX::SimpleMath::Matrix Camera::GetCameraViewMat() const
{
	return m_view;
}

void Camera::LookAtObject(DirectX::SimpleMath::Vector3 ObjectPos)
{

	
	postUpdateMoveCam = true;

	Vector3 junk;
	Quaternion rot;
	m_view.Decompose(junk, rot, junk);
	rot.Normalize();
	Vector3 eul = rot.ToEuler();

	m_view = Matrix::CreateLookAt(m_camPosition, ObjectPos, Vector3::UnitY);

	m_view.Decompose(junk, rot, junk);
	rot.Normalize();
	Vector3 eul2 = rot.ToEuler();
	

	eul *= (180 / 3.141592);
	eul2 *= (180 / 3.141592);

	Vector3 deltaEul = eul2 - eul;
	deltaEul.x = 0;

	m_camOrientation.x -= deltaEul.z;
	m_camOrientation.y += deltaEul.y;

	if (m_camOrientation.x > 89.0f) {
		m_camOrientation.x = 89.0f;
	}
	if (m_camOrientation.x < -89.0f) {
		m_camOrientation.x = -89.0f;
	}

	if (m_camOrientation.y < 0.0f) {
		m_camOrientation.y += 360.0f;
	}
	if (m_camOrientation.y > 360.0f) {
		m_camOrientation.y -= 360.0f;
	}

	prevMouseX = 0;
	prevMouseY = 0;
	
}

void Camera::InterpolateNearPosition(Vector3 TargetPosition, float NearDistance, float Time)
{

	float distance = Vector3::Distance(m_camPosition, TargetPosition);
	if (distance > NearDistance)
		distance -= NearDistance;
	else {
		LerpTime = 0.f;
		RemainingLerpTime = 0.f;
		LerpTo = Vector3();
		LerpFrom = Vector3();
		return;
	}
	LerpTime = Time;
	RemainingLerpTime = Time;
	LerpFrom = m_camPosition;
	Vector3 NearPos = TargetPosition - m_camPosition;

	NearPos.Normalize();
	NearPos = m_camPosition + (distance * NearPos);
	LerpTo = NearPos;
}

void Camera::InterpolateUpdate(DX::StepTimer const& timer)
{
	
	RemainingLerpTime -= timer.GetElapsedSeconds();
	float lerpFactor = (LerpTime - RemainingLerpTime) / LerpTime;
	m_camPosition = Vector3::Lerp(LerpFrom, LerpTo, lerpFactor);
}


