#pragma once

#include "ShootingRangeWidget.h"

extern bool needLoadScreen;

class ShootingRangeApplication : public Core::Application
{
	public:

		ShootingRangeApplication(HINSTANCE hInstance,
					int nCmdShow,
					bool fullscreen,
					int targetCount,
					float speed,
					float time): Application(hInstance,
											nCmdShow,
											fullscreen)
		{
			GAME_CONTENT_WIDTH = 1024;
			GAME_CONTENT_HEIGHT = 768;

			_targetCount = targetCount;
			_speed = speed;
			_time = time;
		}
		
		void RegisterTypes()
		{
			Application::RegisterTypes();
			REGISTER_WIDGET_XML(ShootingRangeWidget, "ShootingRangeWidget");
		}
		
		void LoadResources()
		{
			Core::LuaDoFile("start.lua");
		}

		void DrawFps()
		{
			if (!Render::isFontLoaded("default"))
				return;
			
			Render::BindFont("default");
			
			int dy = Render::getFontHeight();
			int x = Render::device.Width() - dy;
			int y = Render::device.Height() - 2 * dy;
			
			Render::PrintString(x, y, std::string("FPS: ") + utils::lexical_cast(currentFps), 1.0f, RightAlign);
			Render::PrintString(x, y - 1 * dy, std::string("Vm: ") + utils::lexical_cast(Render::device.GetVideoMemUsage()) + std::string("MB"), 1.0f, RightAlign);
			Render::PrintString(x, y - 2 * dy, std::string("Am: ") + utils::lexical_cast(MM::manager.GetMemUsage()) + std::string("KB"), 1.0f, RightAlign);
		}

		int _targetCount;
		float _speed;
		float _time;
};
