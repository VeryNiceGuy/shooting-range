#include "stdafx.h"
#include "Scene.h"

Scene::Scene(): _background(MAIN_MENU_BACKGROUND)
{
	_mainMenuBackground = Core::resourceManager.getTexture("MainMenuBackground");
	_gameBackground = Core::resourceManager.getTexture("GameBackground");
}

Scene::~Scene(){}

void Scene::setBackground(SceneBackground background)
{
	_background = background;
}

void Scene::draw()
{
	Render::device.PushMatrix();

	_background == MAIN_MENU_BACKGROUND?
	_mainMenuBackground->Bind(),
	Render::DrawRect(FRect(_mainMenuBackground->getBitmapRect()), FRect(0,1,0,1)):

	_gameBackground->Bind(),
	Render::DrawRect(FRect(_gameBackground->getBitmapRect()), FRect(0,1,0,1));

	Render::device.PopMatrix();
}
