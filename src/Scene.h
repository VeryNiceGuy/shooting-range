#pragma once

enum SceneBackground
{
	MAIN_MENU_BACKGROUND,
	GAME_BACKGROUND
};

class Scene
{
	public:

		Scene();
		~Scene();

		void setBackground(SceneBackground background);
		void draw();

	private:

		Render::Texture *_mainMenuBackground;
		Render::Texture *_gameBackground;
		SceneBackground _background;
};