#pragma once

#ifdef _WIN32
	#define RYM_PLATFORM_WINDOWS
	#ifdef _WIN64
	#else
		#error "Only support x64 bits Windows"
	#endif // WIN64

#elif __linux__
	#error "Linux is a actualy plataform target, but, there is some wire isues with imgui docking in the distro I use, Linux Lite. Also, I need implement some plataflorm specific inputs and I not have any idea of how use X11. Sorry :p."
	#define RYM_PLATFORM_LINUX
#elif __ANDROID__
	#error "Later my friend :D"
	#define RYM_PLATFORM_ANDROID
#elif __APPLE__
	#error "Sorry, but I am a very poor Venezuelans, so I doesnt have a over expensive computer like a mac, even a computer can run Visual Studio 2019 with out lag... Fuck you Chavismo, fuck you Communins"
	#define RYM_PLATFORM_MAC
#endif



