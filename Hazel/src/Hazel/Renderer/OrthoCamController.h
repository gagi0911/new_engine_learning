#pragma once

#include "Hazel/Renderer/OrthoCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class OrthoCamController
	{
	public:
		OrthoCamController(float aspectRatio, bool rotation = false);

		const OrthoCamera& GetCamera() const { return m_Cam; }
		OrthoCamera& GetCamera() { return m_Cam; }
		

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetZoomLevel(float zoomlevel) { m_ZoomLevel = zoomlevel; }
		float GetZoomLevel() const { return m_ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthoCamera m_Cam;

		bool m_Rotation = false;
		glm::vec3 m_CamPos = { 0.0f, 0.0f, 0.0f };
		float m_CamRotation = 0.0f;

		float m_CamSpeed = 2.0f;
		float m_CamRotSpeed = 30.0f;
	};

}