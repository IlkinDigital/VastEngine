#pragma once

#include "Clock/Timestep.h"
#include "AssetManager/AssetTypes/BoardSpriteAsset.h"

namespace Vast::Board2D {

	struct KeyFrame
	{
		Ref<BoardSpriteAsset> Sprite;
	};

	class Flipbook
	{
	public:
		Flipbook();
		Flipbook(const Flipbook& other);

		void Update(Timestep ts);

		float GetFPS() const { return m_FPS; }
		void SetFPS(float fps) { m_FPS = fps; }

		bool IsValid() const { return m_KeyFrames.size(); }

		const Ref<Sprite>& GetCurrentFrame() const { return m_KeyFrames[m_CurrentKeyIndex].Sprite->GetSprite(); }

		const DArray<KeyFrame>& GetKeyFrames() const { return m_KeyFrames; }

		void PushKeyFrame(const KeyFrame& key);
		void InsertKeyFrame(uint32 index, const KeyFrame& key);
		void RemoveKeyFrame(uint32 index);

		void Clear();
	private:
		float m_Timer = 0.0f;
		uint32 m_CurrentKeyIndex = 0;

		float m_FPS = 15.0f;
		DArray<KeyFrame> m_KeyFrames; // Frames of the flipbook (animation)
	};

}
