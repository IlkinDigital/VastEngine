#include "vastpch.h"
#include "EditorCamera.h"

#include "Input/Input.h"

#include <glm/gtx/quaternion.hpp>

namespace Vast {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_Projection = Math::Perspective(Math::Radians(fov), aspectRatio, nearClip, farClip);
		CalculateView();
	}

	void EditorCamera::CalculateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = Math::Perspective(Math::Radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::CalculateView()
	{
		m_ViewMatrix = Math::Translate(Mat4(1.0f), m_Position)
			* Math::Rotate(Mat4(1.0f), m_Roll,  { 0.0f, 0.0f, 1.0f })
			* Math::Rotate(Mat4(1.0f), m_Yaw,   { 0.0f, 1.0f, 0.0f })
			* Math::Rotate(Mat4(1.0f), m_Pitch, { 1.0f, 0.0f, 0.0f });
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}


	void EditorCamera::OnUpdate(Timestep ts)
	{
		Vector2 mouse = Input::GetMousePosition();
		Vector2 delta = (mouse - m_InitialMousePosition) * 0.3f;
		m_InitialMousePosition = mouse;

		if (Input::IsPressed(Mouse::Right))
		{
			MouseGaze(ts, delta);

			float speed = 10.0f;

			if (Input::IsPressed(Key::W))
				m_Position += speed * GetForwardDirection() * (float)ts;
			if (Input::IsPressed(Key::S))
				m_Position += speed * -GetForwardDirection() * (float)ts;
			if (Input::IsPressed(Key::D))
				m_Position += speed * GetRightDirection() * (float)ts;
			if (Input::IsPressed(Key::A))
				m_Position += speed * -GetRightDirection() * (float)ts;
			if (Input::IsPressed(Key::Q))
				m_Position += speed * GetUpDirection() * (float)ts;
			if (Input::IsPressed(Key::E))
				m_Position += speed * -GetUpDirection() * (float)ts;
		}

		CalculateView();
	}

	void EditorCamera::MouseGaze(Timestep ts, const Vector2& delta)
	{
		m_Yaw -= 5.0f * delta.x * ts;
		m_Pitch -= 5.0f * delta.y * ts;
	}


	Vector3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), Vector3(0.0f, 1.0f, 0.0f));
	}

	Vector3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), Vector3(1.0f, 0.0f, 0.0f));
	}

	Vector3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), Vector3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(Vector3(m_Pitch, m_Yaw, m_Roll));
	}

}