#pragma once

namespace Vast {

	class Timestep
	{
	public:
		Timestep(float seconds)
			: m_Seconds(seconds) {}

		float Milliseconds() const { return m_Seconds * 1000.0f; }
		float Seconds()		 const { return m_Seconds; }

		operator float() const { return m_Seconds; }
	private:
		float m_Seconds;
	};

}
