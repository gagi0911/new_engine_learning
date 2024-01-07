#include "hzpch.h"
#include "Hazel/Renderer/OrthoCamController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	OrthoCamController::OrthoCamController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio) ,m_Cam(-m_AspectRatio*m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthoCamController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_A)) m_CamPos.x -= m_CamSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_D)) m_CamPos.x += m_CamSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_W)) m_CamPos.y += m_CamSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_S)) m_CamPos.y -= m_CamSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_E)) m_CamRotation -= m_CamRotSpeed * ts;
			else if (Input::IsKeyPressed(HZ_KEY_Q)) m_CamRotation += m_CamRotSpeed * ts;
			m_Cam.SetRotation(m_CamRotation);
		}
		
		m_Cam.SetPosition(m_CamPos);

		m_CamSpeed = m_ZoomLevel;
	}

	void OrthoCamController::OnEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthoCamController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthoCamController::OnWindowResized));
	}

	bool OrthoCamController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Cam.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthoCamController::OnWindowResized(WindowResizeEvent & e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Cam.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}