#pragma once

/*

D3Dクラスやゲームシーン管理クラスの
初期化～アップデートなどを行う

*/

#include"Window.h"
#include"Scene.h"

class App {
public:

	void Init();

	void Update();

private:

	Window window;
	Scene scene;

};