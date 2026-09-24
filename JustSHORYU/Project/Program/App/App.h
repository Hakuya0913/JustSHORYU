#pragma once

/*

D3Dクラスやゲームシーン管理クラスの
初期化～アップデートなどを行う

*/

#include"../Core/Window.h"

//デバッグ用
#include"../Renderer/DebugRenderer.h"
#include"../Input/InputManager.h"
#include"..\Camera\Camera.h"
#include"..\Renderer\/ModelRenderer.h"
#include"../Component/ModelComponent.h"
#include"../Component/RenderComponent.h"
#include"../Component/TransformComponent.h"


class App {
public:

	App();
	~App() = default;

	void Init();

	void Update();

private:

	Window window;

	//デバッグ用
	DebugRenderer debugRenderer;
	Camera camera;
	TransformComponent transform;
	ModelComponent model;
	RenderComponent renderComponent;
	ModelRenderer modelRenderer;

};