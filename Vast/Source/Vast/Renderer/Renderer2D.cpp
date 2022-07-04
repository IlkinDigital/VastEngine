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
		Vector2 TextureCoords;
		float TextureIndex = 0.0f;
	};

	struct RendererData
	{
		static const uint32 MaxTextureSlots = 32;

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<VertexArray> QuadVertexArray;

		Ref<Shader> QuadShader;

		QuadVertex QuadVertices[4];

		Ref<Texture2D> WhiteTexture;

		SArray<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32 TextureSlotIndex = 1; // 0 => white texture

		Vector4 QuadVertexPositions[4];
	};

	static RendererData s_Data;

	void Renderer2D::Init()
	{
		OPTICK_EVENT();

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
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TextureCoords" },
			{ ShaderDataType::Float, "a_TextureIndex" }
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, 6));

		int32 samplers[s_Data.MaxTextureSlots]{};

		for (int32 i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		uint32 whitePixel = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.WhiteTexture->SetData(&whitePixel, sizeof(whitePixel));

		s_Data.QuadShader = Shader::Create("Assets/Shaders/Sprite2D.glsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->UploadIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
	}

	void Renderer2D::BeginScene(const Camera& camera, const Mat4& transform)
	{
		OPTICK_EVENT();

		s_Data.QuadShader->Bind();
		Mat4 viewProj = camera.GetProjection() * Math::Inverse(transform);

		s_Data.QuadShader->UploadMat4("u_ViewProjection", viewProj);
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		OPTICK_EVENT();

		s_Data.QuadShader->Bind();

		s_Data.QuadShader->UploadMat4("u_ViewProjection", camera.GetViewProjection());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Vector4& color)
	{
		OPTICK_EVENT();

		s_Data.WhiteTexture->Bind();

		s_Data.QuadVertices[0].Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertices[0].Color = color;
		s_Data.QuadVertices[0].TextureCoords = { 0.0f, 0.0f };

		s_Data.QuadVertices[1].Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertices[1].Color = color;
		s_Data.QuadVertices[1].TextureCoords = { 1.0f, 0.0f };

		s_Data.QuadVertices[2].Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertices[2].Color = color;
		s_Data.QuadVertices[2].TextureCoords = { 1.0f, 1.0f };

		s_Data.QuadVertices[3].Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertices[3].Color = color;
		s_Data.QuadVertices[3].TextureCoords = { 0.0f, 1.0f };

		s_Data.QuadVertexBuffer->SetVertexData(s_Data.QuadVertices, 4 * sizeof(QuadVertex));

		s_Data.QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVertexArray->GetIndexBuffer()->GetIndexCount());
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture)
	{
		OPTICK_EVENT();

		texture->Bind();

		Vector4 color(1.0f); // white

		s_Data.QuadVertices[0].Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertices[0].Color = color;
		s_Data.QuadVertices[0].TextureCoords = { 0.0f, 0.0f };

		s_Data.QuadVertices[1].Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertices[1].Color = color;
		s_Data.QuadVertices[1].TextureCoords = { 1.0f, 0.0f };

		s_Data.QuadVertices[2].Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertices[2].Color = color;
		s_Data.QuadVertices[2].TextureCoords = { 1.0f, 1.0f };

		s_Data.QuadVertices[3].Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertices[3].Color = color;
		s_Data.QuadVertices[3].TextureCoords = { 0.0f, 1.0f };

		s_Data.QuadVertexBuffer->SetVertexData(s_Data.QuadVertices, 4 * sizeof(QuadVertex));

		s_Data.QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVertexArray->GetIndexBuffer()->GetIndexCount());
	}


}