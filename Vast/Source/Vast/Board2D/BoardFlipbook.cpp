#include "vastpch.h"
#include "BoardFlipbook.h"

namespace Vast::Board2D {

	Flipbook::Flipbook()
	{
	}

	void Flipbook::PushKeyFrame(const KeyFrame& key)
	{
		m_KeyFrames.emplace_back(key);
	}

	void Flipbook::Update(Timestep ts)
	{
		m_Timer += ts;

		if (m_Timer >= 1.0f / m_FPS)
		{
			m_CurrentKeyIndex++;
			
			if (m_CurrentKeyIndex >= m_KeyFrames.size())
			{
				m_CurrentKeyIndex %= m_KeyFrames.size();
			}

			m_Timer = 0.0f;
		}
	}

}
