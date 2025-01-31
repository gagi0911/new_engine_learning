#pragma once

#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

// REMOVE!
typedef unsigned int GLenum;

namespace Hazel {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetInt(int value, const std::string& name) override;
		virtual void SetMat4(const glm::mat4& value, const std::string& name) override;
		virtual void SetFloat4(const glm::vec4& value, const std::string& name) override;
		virtual void SetFloat3(const glm::vec3& value, const std::string& name) override;
		virtual void SetFloat(float value, const std::string& name) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(int value, const std::string& name);

		void UploadUniformFloat4(const glm::vec4& values, const std::string& name);
		void UploadUniformFloat3(const glm::vec3& values, const std::string& name);
		void UploadUniformFloat2(const glm::vec2& values, const std::string& name);
		void UploadUniformFloat(float value, const std::string& name);

		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name);
		void UploadUniformMat3(const glm::mat3& matrix, const std::string& name);
	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}