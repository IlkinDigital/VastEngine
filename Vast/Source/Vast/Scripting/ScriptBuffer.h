#pragma once

#include "Scene/Components/NativeScriptComponent.h"

namespace Vast {

	class ScriptBuffer
	{
	public:
		ScriptBuffer() = default;

		void SetBuffer(const DArray<NativeScriptComponent>& buffer) { m_Buffer = buffer; }
		const DArray<NativeScriptComponent>& GetBuffer() const { return m_Buffer; }

		void ClearBuffer() { m_Buffer.clear(); }

		const NativeScriptComponent& FindByName(const String& name) const;
	private:
		DArray<NativeScriptComponent> m_Buffer;
	};

}
