#include "vastpch.h"
#include "BoardFlipbook.h"

namespace Vast::Board2D {

	static bool SortFunc(KeyFrame& k1, KeyFrame& k2)
	{
		return k1.ActivationFraction < k2.ActivationFraction;
	}

	Flipbook::Flipbook()
	{
	}

	void Flipbook::PushKeyFrame(const Ref<Texture2D>& texture, float timePos)
	{
		KeyFrame kf{ texture, timePos };
		m_KeyFrames.emplace_back(kf);
		std::sort(m_KeyFrames.begin(), m_KeyFrames.end(), SortFunc);
	}

	void Flipbook::Update(Timestep ts)
	{
		m_Timer += ts;
		if (m_Timer >= m_Timeline)
			m_Timer -= m_Timeline;


		if (m_Timer >= m_KeyFrames[m_CurrentKeyIndex].ActivationFraction * m_Timeline)
		{
			m_CurrentKeyIndex++;
			
			if (m_CurrentKeyIndex >= m_KeyFrames.size())
			{
				m_CurrentKeyIndex %= m_KeyFrames.size();
				m_Timer = 0.0f;
			}
		}
	}

}
