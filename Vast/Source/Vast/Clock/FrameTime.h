#pragma once

#include "Timestep.h"

namespace Vast {

	class FrameTime
	{
	public:
		FrameTime() = default;
		FrameTime(float delayMs)
			: m_Delay(delayMs) {}

		void Update(Timestep ts);
		uint32 GetFPS() const { return 1000.0f / m_FrameTime; }
		float GetFrameTime() const { return m_FrameTime; }

	private:
		float m_Delay = 0.0f; // Delay in ms between calculations
		float m_Wait = 0.0f; // Time in ms passed since last calculation
		float m_FrameTime = 0.0f;
	};

}
