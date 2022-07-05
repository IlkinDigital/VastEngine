#pragma once

#include "ApplicationCore/Layer/Layer.h"
#include "Core/UUID.h"

#include <imgui.h>

namespace Vast {

	class Panel
	{
	public:
		Panel() = default;
		Panel(const String& name)
			: m_Name((String&&)(name + "##" + std::to_string(s_ID))) 
		{
			s_ID++;
		}

		const String& GetName() const { return m_Name; }

		virtual void Open() { m_IsOpen = true; }
		virtual void Close() { m_IsOpen = false; }

		virtual void DrawPanel() = 0;

		virtual void OnGUIRender()
		{
			if (m_IsOpen)
				DrawPanel();
		}
	protected:
		String m_Name = "Panel";
		bool m_IsOpen = false;
	private:
		inline static uint16 s_ID = 0;
	};

	/**
	* Panel that acts more like a window/layer
	*/
	class Subwindow : public Layer
	{
	public:
		Subwindow() = default;
		Subwindow(const String& name)
			: Layer(name) {}

		UUID GetUUID() const { return m_UUID; }

		void Open() { OnAttach(); m_IsOpen = true; }
		void Close() { OnDetach(); m_IsOpen = false; }

		bool IsOpen() const { return m_IsOpen; }

		virtual void DrawPanel() = 0;

		virtual void OnGUIRender() override
		{
			if (m_IsOpen)
			{
				DrawPanel();
			}
		}
	protected:
		bool m_IsOpen = false;
	private:
		UUID m_UUID;
	};

}