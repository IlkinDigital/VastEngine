#include "vastpch.h"
#include "Renderer2D.h"

#include "Math/MathOperations.h"

#include "VertexArray.h"
#include "Shader/Shader.h"
#include "RendererCommand.h"

namespace Vast {

	struct SkyboxVertex
	{
		Vector3 Position;
	};

	struct QuadVertex
	{
		Vector3 Position;
		Vector4 Color;
		Vector2 TextureCoords;
		float TextureIndex = 0.0f;
	};

	struct LineVertex
	{
		Vector3 Position;
		Vector4 Color;
	};

	struct RendererData
	{
		static const uint32 MaxTextureSlots = 32;

		Ref<Shader> SkyboxShader;
		Ref<VertexBuffer> SkyboxVertexBuffer;
		Ref<VertexArray> SkyboxVertexArray;

		SkyboxVertex SkyboxVertices[8];
		Vector3 SkyboxVertexPositions[8];

		Ref<VertexBuffer> LineVertexBuffer;
		Ref<VertexArray> LineVertexArray;
		Ref<Shader> LineShader;

		LineVertex LineVertices[2];

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;

		QuadVertex QuadVertices[4];
		Vector4 QuadVertexPositions[4];

		Ref<Texture2D> WhiteTexture;

		SArray<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32 TextureSlotIndex = 1; // 0 => white texture

	};

	static RendererData s_Data;

	void Renderer2D::Init()
	{
		OPTICK_EVENT();

		/**
		* Syboxes
		*/
		{
			uint32 indices[]
			{
				1, 2, 6,
				6, 5, 1,

				0, 4, 7,
				7, 3, 0,

				4, 5, 6,
				6, 7, 4,
				
				0, 3, 2,
				2, 1, 0,
				
				0, 1, 5,
				5, 4, 0,
				
				3, 7, 6,
				6, 2, 3
			};

			s_Data.SkyboxVertexPositions[0] = { -1.0f, -1.0f,  1.0f };
			s_Data.SkyboxVertexPositions[1] = {  1.0f, -1.0f,  1.0f };
			s_Data.SkyboxVertexPositions[2] = {  1.0f, -1.0f, -1.0f };
			s_Data.SkyboxVertexPositions[3] = { -1.0f, -1.0f, -1.0f };
			s_Data.SkyboxVertexPositions[4] = { -1.0f,  1.0f,  1.0f };
			s_Data.SkyboxVertexPositions[5] = {  1.0f,  1.0f,  1.0f };
			s_Data.SkyboxVertexPositions[6] = {  1.0f,  1.0f, -1.0f };
			s_Data.SkyboxVertexPositions[7] = { -1.0f,  1.0f, -1.0f };

			s_Data.SkyboxVertexArray = VertexArray::Create();
			s_Data.SkyboxVertexBuffer = VertexBuffer::Create(8 * sizeof(SkyboxVertex));

			s_Data.SkyboxVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Pos" }
			});

			s_Data.SkyboxVertexArray->AddVertexBuffer(s_Data.SkyboxVertexBuffer);
			s_Data.SkyboxVertexArray->SetIndexBuffer(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));
		}

		/**
		* Quads
		*/
		{
			OPTICK_EVENT("QuadInit");

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
		}

		/**
		* Lines
		*/
		{
			OPTICK_EVENT("LineInit");

			s_Data.LineVertexArray = VertexArray::Create();
			s_Data.LineVertexBuffer = VertexBuffer::Create(2 * sizeof(LineVertex));
			s_Data.LineVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Pos"},
				{ ShaderDataType::Float4, "a_Color" }
			});

			s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		
		}
		
		int32 samplers[s_Data.MaxTextureSlots]{};

		for (int32 i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		uint32 whitePixel = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.WhiteTexture->SetData(&whitePixel, sizeof(whitePixel));

		s_Data.QuadShader = Shader::Create("Resources/Shaders/Renderer2D-Quad.glsl");
		s_Data.LineShader = Shader::Create("Resources/Shaders/Renderer2D-Line.glsl");
		s_Data.SkyboxShader = Shader::Create("Resources/Shaders/Skybox.glsl");
	}

	void Renderer2D::BeginScene(const Camera& camera, const Mat4& transform)
	{
		OPTICK_EVENT();

		Mat4 viewProj = camera.GetProjection() * Math::Inverse(transform);
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->UploadMat4("u_ViewProjection", viewProj);
		s_Data.LineShader->Bind();
		s_Data.LineShader->UploadMat4("u_ViewProjection", viewProj);
		s_Data.SkyboxShader->Bind();
		s_Data.SkyboxShader->UploadMat4("u_ViewProjection", viewProj);
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		OPTICK_EVENT();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->UploadMat4("u_ViewProjection", camera.GetViewProjection());

		s_Data.LineShader->Bind();
		s_Data.LineShader->UploadMat4("u_ViewProjection", camera.GetViewProjection());

		s_Data.SkyboxShader->Bind();
		Mat4 sbView = camera.GetViewMatrix();
		Mat4 sbProj = camera.GetProjection();
		sbView[3][0] = 0.0f;
		sbView[3][1] = 0.0f;
		sbView[3][2] = 0.0f;
		s_Data.SkyboxShader->UploadMat4("u_ViewProjection", sbProj * sbView);
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawSkybox(const Ref<Cubemap>& cubemap)
	{
		RendererCommand::SetDepthMask(false);

		for (int i = 0; i < 8; i++)
		{
			s_Data.SkyboxVertices[i].Position = s_Data.SkyboxVertexPositions[i];
		}

		s_Data.SkyboxVertexBuffer->SetVertexData(s_Data.SkyboxVertices, 8 * sizeof(SkyboxVertex));

		cubemap->Bind();

		s_Data.SkyboxShader->Bind();
		s_Data.SkyboxVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.SkyboxVertexArray->GetIndexBuffer()->GetIndexCount());
		
		RendererCommand::SetDepthMask(true);
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

		s_Data.QuadShader->Bind();
		s_Data.QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVertexArray->GetIndexBuffer()->GetIndexCount());
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture)
	{
		DrawQuad(transform, texture, {{ {0.0f, 0.0f}, {1.0f, 1.0f} }});
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture, SArray<Vector2, 2> uvCoords)
	{
		OPTICK_EVENT();

		texture->Bind();

		Vector4 color(1.0f); // white

		s_Data.QuadVertices[0].Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertices[0].Color = color;
		s_Data.QuadVertices[0].TextureCoords = uvCoords[0];

		s_Data.QuadVertices[1].Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertices[1].Color = color;
		s_Data.QuadVertices[1].TextureCoords = { uvCoords[1].x, uvCoords[0].y };

		s_Data.QuadVertices[2].Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertices[2].Color = color;
		s_Data.QuadVertices[2].TextureCoords = uvCoords[1];

		s_Data.QuadVertices[3].Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertices[3].Color = color;
		s_Data.QuadVertices[3].TextureCoords = { uvCoords[0].x, uvCoords[1].y };

		s_Data.QuadVertexBuffer->SetVertexData(s_Data.QuadVertices, 4 * sizeof(QuadVertex));

		s_Data.QuadShader->Bind();
		s_Data.QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data.QuadVertexArray->GetIndexBuffer()->GetIndexCount());
	}

	void Renderer2D::DrawLine(const Vector3& pt1, const Vector3& pt2, const Vector4& color)
	{
		OPTICK_EVENT();

		s_Data.LineVertices[0].Position = pt1;
		s_Data.LineVertices[0].Color = color;

		s_Data.LineVertices[1].Position = pt2;
		s_Data.LineVertices[1].Color = color;

		s_Data.LineVertexBuffer->SetVertexData(s_Data.LineVertices, 2 * sizeof(LineVertex));
		
		s_Data.LineShader->Bind();
		s_Data.LineVertexArray->Bind();
		RendererCommand::DrawLines(2);
	}


}