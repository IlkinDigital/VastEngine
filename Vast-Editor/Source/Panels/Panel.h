#pragma once

#include "ApplicationCore/Layer/Layer.h"
#include "Core/UUID.h"

#include "Math/Math.h"

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
		Subwindow()
			: Subwindow("Subwindow") {}
		Subwindow(const String& name)
			: Layer(name + "##" + std::to_string(s_ID))
		{
			s_ID++;
		}

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
		Vector2 m_DefaultSize = { 1080.0f, 720.0f };
		bool m_IsOpen = false;
	private:
		UUID m_UUID;
		inline static uint16 s_ID = 0;
	};

}