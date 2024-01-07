#pragma once

#include "RenderCommand.h"
#include "OrthoCamera.h"
#include "Shader.h"

namespace Hazel {


	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(OrthoCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetRendererAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjection;
		};

		static SceneData* m_SceneData;
	};

}
