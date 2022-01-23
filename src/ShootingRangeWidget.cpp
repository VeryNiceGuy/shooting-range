#include "stdafx.h"
#include "ShootingRangeWidget.h"
#include "ShootingRangeApplication.h"
#include <ctime>

void PlayBackgroundMusic(void* userData)
{
	switch(utils::random<unsigned char>(3))
	{
		case 0:
			MM::manager.SetCallback(MM::manager.PlaySample("ThanksBro"),
									PlayBackgroundMusic,
									0);
		break;

		case 1:
			MM::manager.SetCallback(MM::manager.PlaySample("IfYouNeverSayGoodbye"),
									PlayBackgroundMusic,
									0);
		break;

		case 2:
			MM::manager.SetCallback(MM::manager.PlaySample("XFilesTheme"),
									PlayBackgroundMusic,
									0);
		break;
	}
}

ShootingRangeWidget::ShootingRangeWidget(const std::string& name): Widget(name),
																	_mouseOverStartButton(false),
																	_mouseOverOkButton(false),
																	_targetCount(0),
																	_speed(0.0f),
																	_time(0.0f),
																	_gameStartTime(0.0f),
																	_gameOver(false)
{
	Init();
}

ShootingRangeWidget::ShootingRangeWidget(const string& name,
										 Xml::TiXmlElement* xmlElement): Widget(name),
																		_mouseOverStartButton(false),
																		_mouseOverOkButton(false),
																		_targetCount(0),
																		_speed(0.0f),
																		_time(0.0f),
																		_gameStartTime(0.0f),
																		_gameOver(false)
{
	Init();
}

void ShootingRangeWidget::Init()
{
	_startButton = Core::resourceManager.getTexture("StartButton");
	_startButtonOver = Core::resourceManager.getTexture("StartButtonOver");

	_okButton = Core::resourceManager.getTexture("OkButton");
	_okButtonOver = Core::resourceManager.getTexture("OkButtonOver");
	_okButtonWindow = Core::resourceManager.getTexture("OkWindow");

	ShootingRangeApplication* app = static_cast<ShootingRangeApplication*>(Core::appInstance);

	_targetCount = app->_targetCount;
	_speed = app->_speed;
	_time = app->_time;

	changeGameState(MAIN_MENU_STATE);
	PlayBackgroundMusic(0);
}

void ShootingRangeWidget::Draw() 
{
	_scene.draw();

	if(_gameState == MAIN_MENU_STATE)
	{
		Render::device.PushMatrix();

			_mouseOverStartButton ? _startButtonOver->Bind() : _startButton->Bind();

			Render::device.MatrixTranslate(math::Vector3(700.0f, 100.0f, 0.0f));
			Render::device.MatrixTranslate(math::Vector3(-128.0f, -64.0f, 0.0f));
			Render::DrawRect(FRect(_startButton->getBitmapRect()), FRect(0,1,0,1));

		Render::device.PopMatrix();
		return;
	}

	_angryCannon.draw();

	if(_gameOver)
	{
		_okButtonWindow->Bind();
		Render::device.PushMatrix();

			Render::device.MatrixTranslate(math::Vector3(512.0f, 384, 0.0f));
			Render::device.MatrixTranslate(math::Vector3(-256.0f, -128.0f, 0.0f));
			Render::DrawRect(FRect(_okButtonWindow->getBitmapRect()), FRect(0,1,0,1));

		Render::device.PopMatrix();

		_mouseOverOkButton ? _okButtonOver->Bind() : _okButton->Bind();
		Render::device.PushMatrix();

			Render::device.MatrixTranslate(math::Vector3(512.0f, 310.0f, 0.0f));
			Render::device.MatrixTranslate(math::Vector3(-64.0f, -32.0f, 0.0f));
			Render::DrawRect(FRect(_okButton->getBitmapRect()), FRect(0,1,0,1));

		Render::device.PopMatrix();

		if (!Render::isFontLoaded("default"))
			return;
			
		Render::BindFont("default");
		Render::PrintString(512,
							340,
							utils::lexical_cast(_angryCannon.getEnemyCount() -
												_angryCannon.getAliveEnemyCount()),
							5.0f,
							CenterAlign);
		return;
	}

	if(!Render::isFontLoaded("default"))
		return;

	Render::BindFont("default");
	Render::PrintString(1006,
						732 - 3 * 18,
						std::string("Time: ") +
						utils::lexical_cast(math::round(((_time * 1000.0f) - 
											(getCurrentTime() - _gameStartTime)) / 
											1000.0f)) +
											std::string("s"),
						1.0f,
						RightAlign);
}

void ShootingRangeWidget::Update(float dt)
{
	if(_gameState == MAIN_MENU_STATE)
		return;

	if(_gameOver)
		return;

	if(getCurrentTime() - _gameStartTime >= (_time * 1000.0f))
	{
		_angryCannon.getAliveEnemyCount();
		_gameOver = true;
	}

	_angryCannon.update();
}

bool ShootingRangeWidget::MouseDown(const IPoint &mouse_pos)
{
	if(_gameState == MAIN_MENU_STATE)
	{
		if(_mouseOverStartButton)
			changeGameState(GAME_STATE);

		return false;
	}

	if(_gameOver)
	{
		if(_mouseOverOkButton)
			changeGameState(MAIN_MENU_STATE);

		return false;
	}

	if (Core::mainInput.GetMouseLeftButton())
		_angryCannon.fire();

	return false;
}

void ShootingRangeWidget::MouseMove(const IPoint &mouse_pos)
{
	if(_gameState == MAIN_MENU_STATE)
	{
		const float startButtonOriginX = 700.0f;
		const float startButtonOriginY = 100.0f;
		const float startButtonWidth = 256.0f;
		const float startButtonHeight = 128.0f;

		if(mouse_pos.x >= (startButtonOriginX - (startButtonWidth / 2.0f)) &&
			mouse_pos.x <= (startButtonOriginX + (startButtonWidth / 2.0f)) &&
			mouse_pos.y >= (startButtonOriginY - (startButtonHeight / 2.0f)) &&
			mouse_pos.y <= (startButtonOriginY + (startButtonHeight / 2.0f)))
			_mouseOverStartButton = true;
		else 
			_mouseOverStartButton = false;

		return;
	}

	if(_gameOver)
	{
		const float okButtonOriginX = 512.0f;
		const float okButtonOriginY = 310.0f;
		const float okButtonWidth = 128.0f;
		const float okButtonHeight = 64.0f;

		if(mouse_pos.x >= (okButtonOriginX - (okButtonWidth / 2.0f)) &&
			mouse_pos.x <= (okButtonOriginX + (okButtonWidth / 2.0f)) &&
			mouse_pos.y >= (okButtonOriginY - (okButtonHeight / 2.0f)) &&
			mouse_pos.y <= (okButtonOriginY + (okButtonHeight / 2.0f)))
			_mouseOverOkButton = true;
		else 
			_mouseOverOkButton = false;

		return;
	}

	_angryCannon.updateDirection(static_cast<float>(mouse_pos.x),
									static_cast<float>(mouse_pos.y));
}

void ShootingRangeWidget::MouseUp(const IPoint &mouse_pos){}

void ShootingRangeWidget::changeGameState(GameState gameState)
{
	_gameState = gameState;

	if(_gameState == MAIN_MENU_STATE)
	{
		_scene.setBackground(MAIN_MENU_BACKGROUND);
		return;
	}
	
	_gameOver = false;
	_angryCannon.createEnemies(_targetCount);
	_angryCannon.getBall().setVelocity(_speed);
	_gameStartTime = getCurrentTime();
	_scene.setBackground(GAME_BACKGROUND);
}

void ShootingRangeWidget::AcceptMessage(const Message& message)
{
	if (message.getPublisher() == "KeyPress") 
	{
		if (utils::lexical_cast<int>(message.getData()) < 0)
			return;
	}
}

float ShootingRangeWidget::getCurrentTime() const
{
	return static_cast<float>(clock()) /
			(static_cast<float>(CLOCKS_PER_SEC) / 1000.0f);
}
