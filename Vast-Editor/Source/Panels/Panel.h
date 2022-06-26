#pragma once

namespace Vast {

	class Panel
	{
	public:
		virtual void Open() { m_IsOpen = true; }
		virtual void Close() { m_IsOpen = false; }

		virtual void DrawPanel() = 0;

		void OnGUIRender()
		{
			if (m_IsOpen)
				DrawPanel();
		}
	private:
		bool m_IsOpen = false;
	};

}