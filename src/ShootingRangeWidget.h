#pragma once

#include "AngryCannon.h"
#include "Scene.h"

void PlayBackgroundMusic(void* userData);

enum GameState
{
	MAIN_MENU_STATE,
	GAME_STATE
};

class ShootingRangeWidget : public GUI::Widget
{
	public:

		ShootingRangeWidget(const std::string& name);
		ShootingRangeWidget(const std::string& name,
							Xml::TiXmlElement* xmlElement);

		void Draw();
		void Update(float dt);
		
		void AcceptMessage(const Message& message);
		
		bool MouseDown(const IPoint& mouse_pos);
		void MouseMove(const IPoint& mouse_pos);
		void MouseUp(const IPoint& mouse_pos);

		void changeGameState(GameState gameState);

	private:

		void Init();
		float getCurrentTime() const;

		Scene _scene;
		AngryCannon _angryCannon;
		GameState _gameState;

		Render::Texture* _startButton;
		Render::Texture* _startButtonOver;

		Render::Texture* _okButton;
		Render::Texture* _okButtonOver;
		Render::Texture* _okButtonWindow;

		bool _mouseOverStartButton;
		bool _mouseOverOkButton;

		int _targetCount;
		float _speed;
		float _time;

		float _gameStartTime;
		bool _gameOver;
};
