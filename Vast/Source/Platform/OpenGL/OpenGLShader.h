#pragma once

#include "Renderer/Shader/Shader.h"

namespace Vast {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const String& name, const String& vertexSource, const String& fragSource);
		~OpenGLShader() override;

		const String& GetName() const override { return m_Name; }
	
		void Bind() const override;
		void Unbind() const override;
	
		void UploadUniformMat4(const String& name, const Mat4& data) const override;
	private:
		void Compile(const String& vertexSource, const String& fragSource);
	private:
		String m_Name;

		RendererID m_Program;
	};
}
