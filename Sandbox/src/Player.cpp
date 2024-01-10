#include "Player.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

Player::Player()
{
}

void Player::LoadAssets()
{
	m_ShipTexture = Hazel::Texture2D::Create("assets/textures/Ship.png");
}

void Player::OnUpdate(Hazel::Timestep ts)
{
	m_Time += ts;

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_1))
	{
		m_Velocity.y += m_EnginePower;
		// ?
		if (m_Velocity.y < 0.0f)
			m_Velocity.y += m_EnginePower * 2.0f;
		
	}
	else
	{
		m_Velocity.y -= m_Gravity;
	}

	m_Velocity.y = glm::clamp(m_Velocity.y, -20.0f, 20.0f);
	m_Position += m_Velocity * (float)ts;
}

void Player::OnRender()
{
	Hazel::Renderer2D::DrawRotatedQuad({ m_Position.x, m_Position.y, 0.5 }, { 1.0f, 1.3f }, glm::radians(GetRotation()), m_ShipTexture);
}

void Player::OnImGuiRender()
{
	ImGui::DragFloat("Engine Power", &m_EnginePower, 0.1f);
	ImGui::DragFloat("Gravity", &m_Gravity, 0.1f);
}

void Player::Reset()
{
	m_Position = { -10.0f, 0.0f };
	m_Velocity = { 5.0f, 0.0f };
}

