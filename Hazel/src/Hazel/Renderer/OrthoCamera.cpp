#include "hzpch.h"
#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
		: m_Projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_View(1.0f)
	{
		m_ViewProjection = m_Projection * m_View;
	}

	void OrthoCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjection = m_Projection * m_View;
	}

	void OrthoCamera::ReCalculate()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_View = glm::inverse(transform);
		m_ViewProjection = m_Projection * m_View;
	}
}