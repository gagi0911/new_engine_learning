#include "hzpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		HZ_PROFILE_FUNCTION();
		
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		HZ_PROFILE_FUNCTION();

		std::string Source = ReadFile(path);
		auto shaderSources = PreProcess(Source);
		Compile(shaderSources);
		
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}



	OpenGLShader::OpenGLShader(const std::string& name, const std::string & vertexSrc, const std::string & fragmentSrc)
		:m_Name(name)
	{
		HZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);

	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		HZ_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			HZ_CORE_ERROR("Could not oepn file '{0}'", path);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		HZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		/*
		std::string shaders_str; // The shader file's contents as a string
		std::regex r("#type (.*)(?:\n|\r)((?:(?!#type )(?:.|\n|\r))*)");
		while (!shaders_str.empty()) {
			std::smatch match;
			std::regex_search(shaders_str, match, r);
			std::string shader_type = match[1];
			std::string shader_source = match[2];

			// Do whatever with the type and source

			shaders_str = match.suffix();
		}*/

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& ShaderSources)
	{
		HZ_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(ShaderSources.size() <= 2, "no more than 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int shaderindex = 0;
		for (auto& kv : ShaderSources)
		{
			GLenum type = kv.first;
			const GLchar* source = kv.second.c_str();

			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ERROR("{0}", &infoLog[0]);
				HZ_CORE_ASSERT(false, "shader compilation failed");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[shaderindex++] = shader;
		}

		
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failed");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDeleteShader(id);

		m_RendererID = program;
	}


	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(int value, const std::string& name)
	{
		UploadUniformInt(value, name);
	}

	void OpenGLShader::SetMat4(const glm::mat4& value, const std::string& name)
	{
		UploadUniformMat4(value, name);
	}

	void OpenGLShader::SetFloat4(const glm::vec4& value, const std::string& name)
	{
		UploadUniformFloat4(value , name);
	}

	void OpenGLShader::SetFloat3(const glm::vec3& value, const std::string& name)
	{
		UploadUniformFloat3(value, name);
	}

	void OpenGLShader::SetFloat(float value, const std::string& name)
	{
		UploadUniformFloat(value, name);
	}

	void OpenGLShader::UploadUniformInt(int value, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat4(const glm::vec4 & values, const std::string & name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformFloat3(const glm::vec3 & values, const std::string & name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat2(const glm::vec2 & values, const std::string & name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat(float value, const std::string & name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformMat4(const glm::mat4& matrix, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat3(const glm::mat3& matrix, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}