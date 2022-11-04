#pragma once

#include "ApplicationCore/Layer/Layer.h"
#include "GUI/Panels/SubwindowManager.h"

namespace Vast {
	
	class DebugLayer : public Layer
	{
	public:
		DebugLayer(const String& name)
			: Layer(name)
		{
			s_Instance = this;
		}

		virtual void OnUpdate(Timestep ts) override
		{
			m_SM.OnUpdate(ts);
		}

		virtual void OnGUIRender() override
		{
			m_SM.OnGUIRender();
		}

		static void AddDebugSW(const Ref<Subwindow>& sw)
		{
			s_Instance->m_SM.PushSubwindow(sw, sw->GetUUID());
		}

	private:
		SubwindowManager m_SM;

		inline static DebugLayer* s_Instance = nullptr;
	};
}