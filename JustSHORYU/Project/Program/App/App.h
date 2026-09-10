#pragma once

/*

D3Dクラスやゲームシーン管理クラスの
初期化～アップデートなどを行う

*/

#include"../Core/Window.h"

//デバッグ用
#include"../Renderer/DebugRenderer.h"
#include"../Input/InputManager.h"

class App {
public:

	void Init();

	void Update();

private:

	Window window;

	//デバッグ用
	DebugRenderer debugRenderer;

};