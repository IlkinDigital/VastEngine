#pragma once

#include "Camera/Camera.h"

#include "Math/MathOperations.h"

namespace Vast {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic };
	public:
		SceneCamera();
		~SceneCamera() override = default;

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; CalculateProjection(); }

		void SetViewportSize(uint32 width, uint32 height);

		/**
		* Perspective Projection
		*/

		float GetPerspectiveFOV() const { return m_PerspProps.VerticalFOV; }
		float GetPerspectiveNearClip() const { return m_PerspProps.NearClip; }
		float GetPerspectiveFarClip() const { return m_PerspProps.FarClip; }

		void SetPerspectiveFOV(float verticalFOV) { m_PerspProps.VerticalFOV = verticalFOV; CalculateProjection(); }
		void SetPerspectiveNearClip(float nearClip) { m_PerspProps.NearClip = nearClip; CalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_PerspProps.FarClip = farClip; CalculateProjection(); }

		/**
		* Orthographic Projection
		*/

		float GetOrthographicSize() const { return m_OrthoProps.Size; }
		float GetOrthographicNearClip() const { return m_OrthoProps.NearClip; }
		float GetOrthographicFarClip() const { return m_OrthoProps.FarClip; }

		void SetOrthographicSize(float size) { m_OrthoProps.Size = size; CalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { m_OrthoProps.NearClip = nearClip; CalculateProjection(); }
		void SetOrthographicFarClip(float farClip) { m_OrthoProps.FarClip = farClip; CalculateProjection(); }

	private:
		void CalculateProjection();
	public:

		struct Perspective
		{
			float VerticalFOV = 1.0f;
			float NearClip = 0.01f, FarClip = 1000.0f;
		};

		struct Orthograpic
		{
			float Size = 10.0f;
			float NearClip = -1.0f, FarClip = 1.0f;
		};
	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		Perspective m_PerspProps;
		Orthograpic m_OrthoProps;

		float m_AspectRatio = 0.0f;
	};

}
