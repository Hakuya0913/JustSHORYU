#include"App.h"
#include"../Core/GraphicsDevice.h"

void App::Init() {

	window.Init();
	GraphicsDevice::GetInstance().Init(window.GetHWND());

	debugRenderer.Init();

}

void App::Update() {

	MSG message{};
	auto gfxDevice = GraphicsDevice::GetInstance();

	while (message.message != WM_QUIT) {

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE == TRUE)) {

			TranslateMessage(&message);
			DispatchMessage(&message);

		}
		else {

			//各オブジェクトの更新処理など
			GraphicsDevice::GetInstance().BeginFrame();


			auto& input = InputManager::GetInstance();
			
			input.Update();

			if (input.GetXInput().GetDigitalState(PadInputDigital::A) == InputState::Hold) {

				debugRenderer.DrawTriangle();

			}

			GraphicsDevice::GetInstance().EndFrame();

		}

	}

}