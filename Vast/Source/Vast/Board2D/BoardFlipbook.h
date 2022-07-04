#pragma once

#include "Clock/Timestep.h"
#include "Renderer/Texture2D.h"

namespace Vast::Board2D {

	struct KeyFrame
	{
		Ref<Texture2D> Texture;
	};

	class Flipbook
	{
	public:
		Flipbook();

		void Update(Timestep ts);

		float GetFPS() const { return m_FPS; }
		void SetFPS(float fps) { m_FPS = fps; }

		const Ref<Texture2D>& GetCurrentTexture() const { return m_KeyFrames[m_CurrentKeyIndex].Texture; }

		const DArray<KeyFrame>& GetKeyFrames() const { return m_KeyFrames; }

		void PushKeyFrame(const KeyFrame& key);
	private:
		float m_Timer = 0.0f;
		uint32 m_CurrentKeyIndex = 0;

		float m_FPS = 15.0f;
		DArray<KeyFrame> m_KeyFrames; // Frames of the flipbook (animation)
	};

}
