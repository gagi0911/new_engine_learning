#pragma once

#include <glm/glm.hpp>

namespace Hazel {

	class OrthoCamera 
	{
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		void SetPosition(const glm::vec3& position) { 
			m_Position = position;
			ReCalculate();
		}

		void SetRotation(float rotation) {
			m_Rotation = rotation;
			ReCalculate();
		}

		inline const glm::mat4& GetProjection()const { return m_Projection; }
		inline const glm::mat4& GetView()const { return m_View; }
		inline const glm::mat4& GetViewProjection() const { return m_ViewProjection; }

	private:
		void ReCalculate();

	private:
		glm::mat4 m_View;
		glm::mat4 m_Projection;
		glm::mat4 m_ViewProjection;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}
