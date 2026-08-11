#include"App.h"
#include"GraphicsDevice.h"

void App::Init() {

	window.Init();
	GraphicsDevice::GetInstance().Init(window.GetHWND());

	scene.Init();

}

void App::Update() {

	MSG message{};

	while (message.message != WM_QUIT) {

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE == TRUE)) {

			TranslateMessage(&message);
			DispatchMessage(&message);

		}
		else {

			//各オブジェクトの更新処理など
			GraphicsDevice::GetInstance().BeginFrame();

			scene.Update();

			scene.Draw();

			GraphicsDevice::GetInstance().EndFrame();

		}

	}

}