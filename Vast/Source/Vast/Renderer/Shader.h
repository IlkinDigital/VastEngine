#pragma once

#include "Renderer/RendererCore.h"

namespace Vast {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const String& GetName() const = 0;

		static Ref<Shader> Create(const String& name, const String& vertSource, const String& fragSource);
	};

}

