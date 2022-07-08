#include "vastpch.h"
#include "BoardFlipbook.h"

namespace Vast::Board2D {

	Flipbook::Flipbook()
	{
	}

	Flipbook::Flipbook(const Flipbook& other)
		: m_Timer(0.0f), m_CurrentKeyIndex(0),
		  m_FPS(other.m_FPS), m_KeyFrames(DArray<Board2D::KeyFrame>(other.m_KeyFrames.begin(), other.m_KeyFrames.end()))
	{
	}

	void Flipbook::PushKeyFrame(const KeyFrame& key)
	{
		m_KeyFrames.emplace_back(key);
	}

	void Flipbook::InsertKeyFrame(uint32 index, const KeyFrame& key)
	{
		if (index > m_KeyFrames.size())
			return;

		m_KeyFrames.insert(m_KeyFrames.begin() + index, key);
	}

	void Flipbook::RemoveKeyFrame(uint32 index)
	{
		if (index >= m_KeyFrames.size())
			return;

		if (m_KeyFrames.size() - 1)
			m_CurrentKeyIndex /= m_KeyFrames.size() - 1;
		else
			m_CurrentKeyIndex = 0;
		m_KeyFrames.erase(m_KeyFrames.begin() + index);
	}

	void Flipbook::Clear()
	{
		m_KeyFrames.clear();
	}

	void Flipbook::Update(Timestep ts)
	{
		OPTICK_EVENT();

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
