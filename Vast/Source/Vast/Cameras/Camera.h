#pragma once

#include "Math/Math.h"

namespace Vast {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		const Mat4& GetProjection() const { return m_Projection; }
	protected:
		Mat4 m_Projection;
	};

}