#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hazel {

	struct Renderer2DStorage
	{
		Ref <VertexArray> twoDvertexarray;
		Ref <Shader> TextureShader;
		Ref <Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_2Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_2Data = new Renderer2DStorage();

		s_2Data->twoDvertexarray = VertexArray::Create();

		float sq_vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
 			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(sq_vertices, sizeof(sq_vertices));

		BufferLayout sq_layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(sq_layout);
		s_2Data->twoDvertexarray->AddVertexBuffer(squareVB);

		uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(sq_indices, sizeof(sq_indices) / sizeof(uint32_t)));
		s_2Data->twoDvertexarray->SetIndexBuffer(squareIB);

		s_2Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTextureData = 0xffffffff;
		s_2Data->WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));

		//s_2Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_2Data->TextureShader = Shader::Create("assets/shaders/Texture_demo.glsl");
		s_2Data->TextureShader->Bind();
		s_2Data->TextureShader->SetInt(0, "u_Texture");
	}

	void Renderer2D::ShutDown()
	{
		HZ_PROFILE_FUNCTION();

		delete s_2Data;
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		s_2Data->TextureShader->Bind();
		s_2Data->TextureShader->SetMat4(camera.GetViewProjection(), "u_ViewProjection");
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		s_2Data->TextureShader->Bind();

		//set color
		s_2Data->TextureShader->SetFloat4(color, "u_Color");
		s_2Data->TextureShader->SetFloat(1.0f, "u_TilingFactor");
		// set white texture
		s_2Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2Data->TextureShader->SetMat4(transform, "u_Transform");

		s_2Data->twoDvertexarray->Bind();
		RenderCommand::DrawIndexed(s_2Data->twoDvertexarray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintcolor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, tintcolor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintcolor)
	{
		HZ_PROFILE_FUNCTION();

		s_2Data->TextureShader->SetFloat4(tintcolor, "u_Color");
		s_2Data->TextureShader->SetFloat(tiling, "u_TilingFactor");
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2Data->TextureShader->SetMat4(transform, "u_Transform");


		s_2Data->twoDvertexarray->Bind();
		RenderCommand::DrawIndexed(s_2Data->twoDvertexarray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float Rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, Rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float Rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		s_2Data->TextureShader->Bind();

		//set color
		s_2Data->TextureShader->SetFloat4(color, "u_Color");
		s_2Data->TextureShader->SetFloat(1.0f, "u_TilingFactor");
		// set white texture
		s_2Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), Rotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2Data->TextureShader->SetMat4(transform, "u_Transform");

		s_2Data->twoDvertexarray->Bind();
		RenderCommand::DrawIndexed(s_2Data->twoDvertexarray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float Rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintcolor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, Rotation, texture, tiling, tintcolor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float Rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& tintcolor)
	{
		HZ_PROFILE_FUNCTION();

		s_2Data->TextureShader->SetFloat4(tintcolor, "u_Color");
		s_2Data->TextureShader->SetFloat(tiling, "u_TilingFactor");
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), Rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2Data->TextureShader->SetMat4(transform, "u_Transform");


		s_2Data->twoDvertexarray->Bind();
		RenderCommand::DrawIndexed(s_2Data->twoDvertexarray);
	}

}