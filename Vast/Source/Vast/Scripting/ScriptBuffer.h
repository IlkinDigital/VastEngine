#pragma once

#include "Scene/Components/NativeScriptComponent.h"

namespace Vast {

	class ScriptBuffer
	{
	public:
		ScriptBuffer();

		static ScriptBuffer& Get() { return *s_Instance; }

		void SetBuffer(const DArray<NativeScriptComponent>& buffer) { m_Buffer = buffer; }
		const DArray<NativeScriptComponent>& GetBuffer() { return m_Buffer; }

		void ClearBuffer() { m_Buffer.clear(); }

		Scope<NativeScriptComponent> FindByName(const String& name);
	private:
		DArray<NativeScriptComponent> m_Buffer;

		static Scope<ScriptBuffer> s_Instance;
	};

}
