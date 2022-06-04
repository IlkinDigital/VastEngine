#pragma once

#include "Scene/Components/NativeScriptComponent.h"

namespace Vast {

	class ScriptBuffer
	{
	public:
		ScriptBuffer() = default;

		void Set(const DArray<NativeScriptComponent>& buffer) { m_Buffer = buffer; }
		const DArray<NativeScriptComponent>& GetBuffer() const { return m_Buffer; }

		NativeScriptComponent& GetByName(const String& name);
	private:
		DArray<NativeScriptComponent> m_Buffer;
	};

}
