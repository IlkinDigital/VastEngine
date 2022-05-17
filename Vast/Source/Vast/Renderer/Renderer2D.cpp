#include "vastpch.h"
#include "Renderer2D.h"

#include "Math/MathOperations.h"

#include "VertexArray.h"
#include "Shader/Shader.h"
#include "RendererCommand.h"

namespace Vast {

	struct QuadVertex
	{
		Vector3 Position;
		Vector4 Color;
	};

	struct RendererData
	{
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<VertexArray> QuadVertexArray;

		Ref<Shader> QuadShader;

		QuadVertex QuadVertices[4];

		Vector4 QuadVertexPositions[4];
	};

	static RendererData s_Data;

	void Renderer2D::Init()
	{
		uint32 indices[]
		{
			0, 1, 2,
			0, 2, 3
		};

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(4 * sizeof(QuadVertex));
		
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" }
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, 6));

		s_Data.QuadShader = Shader::Create("Assets/Shaders/Sprite2D.glsl");
		s_Data.QuadShader->Bind();
	}

	void Renderer2D::BeginScene(const Camera& camera, const Mat4& transform)
	{
		s_Data.QuadShader->Bind();
		Mat4 viewProj = camera.GetProjection() * transform;

		s_Data.QuadShader->UploadUniformMat4("u_ViewProjection", viewProj);
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Vector4& color)
	{
		s_Data.QuadVertices[0].Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertices[0].Color = color;

		s_Data.QuadVertices[1].Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertices[1].Color = color;

		s_Data.QuadVertices[2].Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertices[2].Color = color;

		s_Data.QuadVertices[3].Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertices[3].Color = color;

		s_Data.QuadVertexBuffer->SetVertexData(s_Data.QuadVertices, 4 * sizeof(QuadVertex));

		s_Data.QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVertexArray->GetIndexBuffer()->GetIndexCount());
	}

}