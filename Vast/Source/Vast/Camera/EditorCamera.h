#pragma once

#include "Camera.h"

#include "Math/MathOperations.h"
#include "Clock/Timestep.h"

namespace Vast {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);

		void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; CalculateProjection(); CalculateView(); }

		float GetGazeSpeed() const { return m_GazeSpeed; }
		void SetGazeSpeed(float speed) { m_GazeSpeed = speed; }

		const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
		Mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		Vector3 GetUpDirection() const;
		Vector3 GetRightDirection() const;
		Vector3 GetForwardDirection() const;
		glm::quat GetOrientation() const;

		const Vector3& GetPosition() const { return m_Position; }
		void SetPosition(const Vector3& position) { m_Position = position; CalculateView(); }

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
		float GetRoll() const { return m_Roll; }

		void SetPitch(float pitch) { m_Pitch = pitch; CalculateView(); }
		void SetYaw(float yaw) { m_Yaw = yaw; CalculateView(); }
		void SetRoll(float roll) { m_Roll = roll; CalculateView(); }
	private:
		void CalculateProjection();
		void CalculateView();

		void MouseGaze(Timestep ts, const Vector2& delta);
	private:
		float m_GazeSpeed = 5.0f;

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		Mat4 m_ViewMatrix;
		Vector3 m_Position = { 0.0f, 0.0f, 5.0f };

		Vector2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Pitch = 0.0f, m_Yaw = 0.0f, m_Roll = 0.0f;

		float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
	};

}