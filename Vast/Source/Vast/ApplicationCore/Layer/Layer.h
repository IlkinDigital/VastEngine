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

		virtual void OnEvent(Event& event) {}

		// When created
		virtual void OnAttach() {}
		// When destroyed
		virtual void OnDetach() {}
		// In the run loop
		virtual void OnUpdate() {}
		// When UI is being drawn
 		virtual void OnGUIRender() {}

		const String& GetName() const { return m_Name; }
	protected:
		String m_Name;
	};

}