#pragma once


#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_PROFILE_BEGIN_SEESION("start up", "HAZEL_PROFILE_startup.json");
	auto app = Hazel::CreateApplication();
	HZ_PROFILE_END_SEESION();

	HZ_PROFILE_BEGIN_SEESION("start up", "HAZEL_PROFILE_Runtime.json");
	app->Run();
	HZ_PROFILE_END_SEESION();
	
	HZ_PROFILE_BEGIN_SEESION("start up", "HAZEL_PROFILE_Shutdown.json");
	delete app;
	HZ_PROFILE_END_SEESION();
}

#else
#error Hazel only supports Windows!
#endif // HZ_PLATFORM_WINDOWS
