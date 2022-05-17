#pragma once

#include "Renderer/RendererCore.h"

#include "Math/Math.h"

namespace Vast {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const String& GetName() const = 0;

		virtual void UploadMat4(const String& name, const Mat4& data) const = 0;
		virtual void UploadInt(const String& name, int data) const = 0;
		virtual void UploadIntArray(const String& name, int* data, uint32 count) const = 0;

		static Ref<Shader> Create(const String& name, const String& vertSource, const String& fragSource);
		static Ref<Shader> Create(const String& filepath);
	};

}

