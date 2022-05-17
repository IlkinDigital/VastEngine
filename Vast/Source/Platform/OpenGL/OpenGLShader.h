#pragma once

#include "Renderer/Shader/Shader.h"

namespace Vast {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const String& name, const String& vertexSource, const String& fragSource);
		OpenGLShader(const String& filepath);
		~OpenGLShader() override;

		const String& GetName() const override { return m_Name; }
	
		void Bind() const override;
		void Unbind() const override;
	
		void UploadMat4(const String& name, const Mat4& data) const override;
		void UploadInt(const String& name, int data) const override;
		void UploadIntArray(const String& name, int* data, uint32 count) const override;
	private:
		void Compile(const String& vertexSource, const String& fragSource);
	private:
		String m_Name;

		RendererID m_Program;
	};
}
