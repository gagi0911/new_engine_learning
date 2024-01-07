#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <chrono>



Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CamController(1280.0f / 720.0f) 
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Hazel::Texture2D::Create("C:/Users/hl/Desktop/game_engine/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	// Update
	m_CamController.OnUpdate(ts);
	

	// Rendering
	{
		HZ_PROFILE_SCOPE("Renderer prep SetColor");
		Hazel::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Hazel::RenderCommand::Clear();

	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CamController.GetCamera());

		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ -0.5f, 0.0f }, { 0.5f, 0.5f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f}, { 5.0f, 5.0f }, m_Texture, 5, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		Hazel::Renderer2D::EndScene();

	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CamController.OnEvent(e);
}
