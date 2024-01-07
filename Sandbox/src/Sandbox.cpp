#include <Hazel.h>
#include <Hazel/Core/Entrypoint.h>

#include "imgui/imgui.h"
#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CamController(1280.0f/720.0f)
	{
		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
		// m_VertexBuffer->Bind();


		Hazel::BufferLayout layout = {
			{Hazel::ShaderDataType::Float3, "a_Position"},
			{Hazel::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };

		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_sqVertexArray = Hazel::VertexArray::Create();



		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main ()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			void main ()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Hazel::Shader::Create("triangle", vertexSrc, fragmentSrc);

		float sq_vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB = Hazel::VertexBuffer::Create(sq_vertices, sizeof(sq_vertices));

		Hazel::BufferLayout sq_layout = {
			{Hazel::ShaderDataType::Float3, "a_Position"}, 
			{Hazel::ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(sq_layout);
		m_sqVertexArray->AddVertexBuffer(squareVB);

		uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(sq_indices, sizeof(sq_indices) / sizeof(uint32_t)));
		m_sqVertexArray->SetIndexBuffer(squareIB);

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main ()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main ()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader2 = Hazel::Shader::Create("Square", vertexSrc2, fragmentSrc2);

		auto TextureShader = m_ShaderLib.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("C:/Users/hl/Desktop/game_engine/Checkerboard.png");
		m_Logo = Hazel::Texture2D::Create("C:/Users/hl/Desktop/game_engine/ChernoLogo.png");


		std::dynamic_pointer_cast<Hazel::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(TextureShader)->UploadUniformInt(0, "u_Texture");

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SqColor));
		ImGui::End();
	}

	void OnUpdate(Hazel::Timestep timestep) override
	{
		// Update
		m_CamController.OnUpdate(timestep);

		// Rendering
		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CamController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_Shader2)->UploadUniformFloat3(m_SqColor, "u_Color");

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				glm::vec3 pos(i*0.2f, j*0.2f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_sqVertexArray, m_Shader2, transform);
			}
		}

		auto TextureShader = m_ShaderLib.Get("Texture");

		m_Texture->Bind();
		Hazel::Renderer::Submit(m_sqVertexArray, TextureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Logo->Bind();
		Hazel::Renderer::Submit(m_sqVertexArray, TextureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// triangle
		/*glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePos);

		Hazel::Renderer::Submit(m_sqVertexArray, m_Shader2, transform);*/
		// Hazel::Renderer::Submit(m_VertexArray, m_Shader);

		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override
	{
		m_CamController.OnEvent(event);

		
	}


private:
	Hazel::ShaderLibrary m_ShaderLib;
	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_Shader2; // flatcolorshader
	Hazel::Ref<Hazel::VertexArray> m_sqVertexArray;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_Logo;

	Hazel::OrthoCamController m_CamController;
	glm::vec3 m_CamPos;
	

	glm::vec3 m_SqColor = { 0.2f, 0.3f, 0.8f };


};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}
};


Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}