#include "stdafx.h"
#include "ShootingRangeApplication.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	bool fullscreen = false;
	Core::log.Init("log.htm", true);

	File::c_file input("input.txt");

	if (!input.is_open())
		return -1;

	int targetCount;
	int speed;
	int time;

	std::string inputStr;
	std::string name;
	std::string value;

	inputStr = input.getline();
	utils::ReadNvp(inputStr, name, value);
	targetCount = utils::lexical_cast<int>(value);

	inputStr = input.getline();
	utils::ReadNvp(inputStr, name, value);
	speed = utils::lexical_cast<int>(value);

	inputStr = input.getline();
	utils::ReadNvp(inputStr, name, value);
	time = utils::lexical_cast<int>(value);

	File::c_file settings("settings.ini");
	if (!settings.is_open())
		return -1;
	
	std::string str = settings.getline();
	while (!str.empty())
	{
		std::string name, value;
		if (utils::ReadNvp(str, name, value))
		{
			if (name == "path")
			{
				File::cd(value);
			}
			else if (name == "fullscreen")
			{
				fullscreen = utils::lexical_cast<bool>(value);
			}
		}
		
		str = settings.getline();
	}

	const char* className = "SHOOTING_RANGE_CLASS";
	const char* gameName = "ShootingRange";

	HWND hWnd = ::FindWindow(className,
							gameName);

	if (hWnd == NULL)
	{
		if (CheckDXVersion())
		{
			ShootingRangeApplication application(hInstance,
												nCmdShow,
												fullscreen,
												targetCount,
												static_cast<float>(speed),
												static_cast<float>(time));

			application.enableCustomCursor = false;
			application.showFps = true;
			application.maxFps = 0;
			application.SETTINGS_REG_KEY = "Software\\Playrix Entertainment\\ShootingRange";
			application.APPLICATION_NAME = gameName;
			application.WINDOW_CLASS_NAME = className;
			application.Init(true);
			application.Start();
			application.ShutDown();
		}
	}
	else
	{
		::SetForegroundWindow(hWnd);
		::SetFocus(hWnd);
		::UpdateWindow(hWnd);
	}
	
	return 0;
}
