#pragma once
#pragma once
#include "Game/GameConfig.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <iostream>
#include <fstream>
#include <string>



//--------------Config File----------------------------------------------------\\


extern bool g_WindowMode;
extern bool g_Fullscreen;
extern int g_WindowHeight;
extern int g_WindowWidth;
extern int g_FeildOfView;
extern std::string g_AppName;
extern std::string g_ImageName;


//------------------------------------------------------------------------------------
inline void ListConfigs(char* configTaken)
{
	DebuggerPrintf(configTaken);
}

//---------------------Setter-------------------------------------------------------

inline void setWindowHeight(std::string  configValue)
{
	g_WindowHeight = atoi(configValue.c_str());
	ListConfigs(" Window Height ");
}

inline void setWindowWidth(std::string  configValue)
{
	g_WindowWidth = atoi(configValue.c_str());
	ListConfigs(" Window Width ");
}

inline void setFeildOfView(std::string  configValue)
{
    //g_FeildOfView = atoi(configValue.c_str());
	ListConfigs(" FoV ");
}

inline void setGameName(std::string  configValue)
{
	g_AppName = configValue;
	ListConfigs(" GameName ");
}

inline void setImageName(std::string  configValue)
{
	g_ImageName = configValue;
	ListConfigs(" ImageName ");
}


inline void setWindowedOrBoderless(bool windowedOrBoderless)
{
	if (windowedOrBoderless)
	{
		g_WindowMode = true;
		ListConfigs(" Windowed ");
	}
	if (!windowedOrBoderless)
	{
		g_WindowMode = false;
		ListConfigs(" Boderless ");
	}
}

inline void setFullscreen(bool fullScreen)
{
	if (fullScreen)
	{
		g_Fullscreen = true;
		ListConfigs(" FullScreen ");
	}
	if (!fullScreen)
	{
		g_Fullscreen = false;
		ListConfigs(" Not FullScreen ");
	}
}

//-------------------------------Getters-----------------------------------------------------------------------\\


inline bool GetConfigInt(std::string configOption)
{
	std::string options;
	std::string equals;
	std::string configValue;
	std::ifstream myfile("data/Config.txt");

	if (!myfile.is_open())
		return false;

	while (myfile >> options >> equals >> configValue)
	{
		if (options == configOption && configOption == "Window_Width")
		{
			setWindowWidth(configValue);
			return true;
		}
		if (options == configOption &&configOption == "Window_Height")
		{
			setWindowHeight(configValue);
			return true;
		}
		if (options == configOption &&configOption == "FeildOfView")
		{
			setFeildOfView(configValue);
			return true;
		}
	}
	return false;
}


inline bool GetConfigFloat(std::string configOption)
{
	std::string options;
	std::string equals;
	std::string configValue;
	std::ifstream myfile("data/Config.txt");

	if (!myfile.is_open())
		return false;

	while (myfile >> options >> equals >> configValue)
	{
			//Nothing in Float Value Yet
	}
	return false;
}


inline bool GetConfigString(std::string configOption)
{
	std::string options;
	std::string equals;
	std::string configValue;
	std::ifstream myfile("data/Config.txt");

	if (!myfile.is_open())
		return false;

	while (myfile >> options >> equals >> configValue)
	{
		if (options == configOption && configOption == "App_Name")
		{
			setGameName(configValue);
			return true;
		}
		if (options == configOption && configOption == "Image_Name")
		{
			setImageName(configValue);
			return true;
		}
	}
	return false;
}

inline bool GetConfigBool(std::string configOption)
{
	std::string options;
	std::string equals;
	std::string configValue;
	std::ifstream myfile("data/Config.txt");

	if (!myfile.is_open())
		return false;

	while (myfile >> options >> equals >> configValue)
	{
		if (options == configOption && configOption == "Windowed")
		{
			if (configValue == "true")
				setWindowedOrBoderless(true);
			if (configValue == "false")
				setWindowedOrBoderless(false);
			return true;
		}

		if (options == configOption && configOption == "Fullscreen")
		{
			if (configValue == "true")
				setFullscreen(true);
			if (configValue == "false")
				setFullscreen(false);
			return true;
		}
	}
	return false;
}

//------------------------------------Change this in Engine------------------------------------------------------------------\\

inline  void setWindowModeInEngine(bool& WindowedorNot)
{
	bool didItPass = GetConfigBool("Windowed");
	if (didItPass == false)
		WindowedorNot = DEFAULT_WINDOWED_MODE_ON;

	WindowedorNot = g_WindowMode;
}

inline void setFullscreeninEngine(bool& fullScreenOrNot)
{
	bool didItPass = GetConfigBool("Fullscreen");
	if (didItPass == false)
		fullScreenOrNot = DEFAULT_FULLSCREEN_ON;

	fullScreenOrNot = g_Fullscreen;
}

inline void setWindowHeightAndWidthInEngine(int& windowWidth, int& windowHeight)
{
	bool didItPassHeight = GetConfigInt("Window_Height");
	bool didItPassWidth = GetConfigInt("Window_Width");

	if (didItPassWidth == false && didItPassHeight == false)
	{
		windowWidth = DEFAULT_WINDOW_WIDTH;
		windowHeight = DEFAULT_WINDOW_HEIGHT;
		return;
	}

	windowWidth = g_WindowWidth;
	windowHeight = g_WindowHeight;
}


inline void setFeildOfViewInEngine(int& Fov)
{
	bool didItPassFoV = GetConfigInt("FeildOfView");
	std::string options;
	std::string equals;
	std::string configValue;
	std::ifstream myfile("data/Config.txt");

	if (!myfile.is_open())
		return;

	while (myfile >> options >> equals >> configValue)
	{
		if (options == "FeildOfView")
		{
			Fov = atoi(configValue.c_str());
			ListConfigs(" FoV ");
			return;
		}
	}
	//return false;


	if (didItPassFoV == false)
	{
		Fov = DEFAULT_FOV;
		return;
	}

	//Fov = g_FeildOfView;
}

inline void setWindowNameInEngine(char*& appName)
{
	bool didItPass = GetConfigString("App_Name");

	if (didItPass == false)
	{
		appName = "No Name Given";
		return;
	}

	//TODO Free Memory
	appName = (char *)malloc(g_AppName.size() + 1);
	memcpy(appName, g_AppName.c_str(), g_AppName.size() + 1);
}

inline void setBackgroundImageInEngine(char*& imageName)
{
	bool didItPass = GetConfigString("Image_Name");

	if (didItPass == false)
	{
		imageName = "No Name Given";
		return;
	}

	//TODO Free Memory
	imageName = (char *)malloc(g_ImageName.size() + 1);
	memcpy(imageName, g_ImageName.c_str(), g_ImageName.size() + 1);
}


