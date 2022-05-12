#pragma once

#include "CoreMinimal.h"
#include "Vast/Events/Event.h"

namespace Vast {

	class Layer
	{
	public:
		Layer(const String& name)
			: m_Name(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		const String& GetName() const { return m_Name; }
	protected:
		String m_Name;
	};

}