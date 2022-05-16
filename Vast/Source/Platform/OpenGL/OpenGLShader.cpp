#include "vastpch.h"
#include "OpenGLShader.h"

#include "Math/MathUtils.h"

#include <glad/glad.h>

namespace Vast {

	OpenGLShader::OpenGLShader(const String& name, const String& vertexSource, const String& fragSource)
		: m_Name(name), m_Program(0)
	{
		Compile(vertexSource, fragSource);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_Program);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat4(const String& name, const Mat4& data) const
	{
		GLuint location = glGetUniformLocation(m_Program, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, Math::ValuePointer(data));
	}

	void OpenGLShader::Compile(const String& vertexSource, const String& fragSource)
	{
		// Vertex shader compilation
		GLuint vtxToken = glCreateShader(GL_VERTEX_SHADER);
		{
			auto vtxStr = vertexSource.c_str();
			glShaderSource(vtxToken, 1, &vtxStr, nullptr);
			glCompileShader(vtxToken);

			// TODO: Consider stripping compilation check for distribution
			GLint isCompiled = 0;
			glGetShaderiv(vtxToken, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 250;
				glGetShaderiv(vtxToken, GL_INFO_LOG_LENGTH, &maxLength);

				String errorLog(maxLength, 0);
				glGetShaderInfoLog(vtxToken, maxLength, &maxLength, &errorLog[0]);

				glDeleteShader(vtxToken);

				VAST_CORE_FATAL("'{0}' Vertex shader compilation FAILED: {1}", m_Name, errorLog);
				VAST_CORE_ASSERT(false, "Shader compilation failed");
				return;
			}
		}

		// Fragment shader compilation
		GLuint fragToken = glCreateShader(GL_FRAGMENT_SHADER);
		{
			auto fragStr = fragSource.c_str();
			glShaderSource(fragToken, 1, &fragStr, nullptr);
			glCompileShader(fragToken);

			// TODO: Consider stripping compilation check for distribution
			GLint isCompiled = 0;
			glGetShaderiv(fragToken, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 250;
				glGetShaderiv(fragToken, GL_INFO_LOG_LENGTH, &maxLength);

				String errorLog(maxLength, 0);
				glGetShaderInfoLog(fragToken, maxLength, &maxLength, &errorLog[0]);

				glDeleteShader(vtxToken);
				glDeleteShader(fragToken);

				VAST_CORE_FATAL("'{0}' Fragment shader compilation FAILED: {1}", m_Name, errorLog);
				VAST_CORE_ASSERT(false, "Shader compilation failed");
				return;
			}
		}

		GLuint program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vtxToken);
		glAttachShader(program, fragToken);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			String infoLog;
			infoLog.reserve(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vtxToken);
			glDeleteShader(fragToken);

			VAST_CORE_ASSERT(false, "{0} Shader link FAILED: {1}", m_Name, infoLog);

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vtxToken);
		glDetachShader(program, fragToken);

		m_Program = program;
	}

}