#pragma once

#include "CoreMinimal.h"
#include "Clock/Timestep.h"
#include "Math/Math.h"

namespace Vast {

	class Scene;

	struct DebugLine
	{
		Vector3 Start;
		Vector3 End;
		Vector4 Color;
		float Lifetime = 5.0f;

		float Time = 0.0f;

		DebugLine(const Vector3& start, const Vector3& end, const Vector4& color, float lifetime)
			: Start(start), End(end), Color(color), Lifetime(lifetime) {}

		bool IsAlive() { return Time < Lifetime; }

		void OnUpdate(Timestep ts)
		{
			Time += ts;
		}
	};

	class DebugRenderer
	{
	public:
		DebugRenderer() = default;

		void OnUpdate(Timestep ts);

		static void DrawLine(Scene& scene, const Vector3& start, const Vector3& end, const Vector4& color = { 0.2f, 1.0f, 0.05f, 1.0f }, float lifetime = 5.0f);
	private:
		DArray<DebugLine> m_DebugLines;
	};

}
