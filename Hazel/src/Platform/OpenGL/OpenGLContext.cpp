#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window Handle is Null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		HZ_CORE_INFO("  Info:  ");
		HZ_CORE_INFO("  Renderer: {0} ", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Vender: {0} ", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Version: {0} ", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}


}