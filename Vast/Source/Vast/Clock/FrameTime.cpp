#include "vastpch.h"
#include "FrameTime.h"

namespace Vast {

	void FrameTime::Update(Timestep ts)
	{
		m_Wait += ts.Milliseconds();

		if (m_Delay <= m_Wait)
		{
			m_FrameTime = ts.Milliseconds();
			m_Wait = 0.0f;
		}
	}

}
