#pragma once

#include "Clock/Timestep.h"
#include "Renderer/Texture2D.h"

namespace Vast::Board2D {

	struct KeyFrame
	{
		Ref<Texture2D> Texture;
		float ActivationFraction; // Value from 0.0 - 1.0, at what section in the timeline it activates
		bool Activated = false;
	};

	class Flipbook
	{
	public:
		Flipbook();

		void Update(Timestep ts);

		void SetTimeline(float duration) { m_Timeline = duration; } // Sets duration of the animation
		const Ref<Texture2D>& GetCurrentTexture() const { return m_KeyFrames[m_CurrentKeyIndex].Texture; }

		const DArray<KeyFrame>& GetKeyFrames() const { return m_KeyFrames; }

		void PushKeyFrame(const Ref<Texture2D>& texture, float timePos);
	private:
		float m_Timer = 0.0f;
		float m_Timeline = 1.0f; // Timeline is duration of the whole animation (flipbook)
		uint32 m_CurrentKeyIndex = 0;
		DArray<KeyFrame> m_KeyFrames; // State to texture map
	};

}
