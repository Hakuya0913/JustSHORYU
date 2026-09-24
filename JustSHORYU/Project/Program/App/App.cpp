#include"App.h"
#include"../Core/GraphicsDevice.h"

App::App()
{

	//特に処理なし

}

void App::Init() {

	window.Init();
	GraphicsDevice::GetInstance().Init(window.GetHWND());

	//デバッグ用の要素初期化呼び出し等
	{

		bool isCorrect;

		using namespace DirectX;
		using namespace DirectX::SimpleMath;

		debugRenderer.Init();
		isCorrect = modelRenderer.Init(GraphicsDevice::GetInstance().GetDevice(), camera);

		if (isCorrect == false)
		{

			return;

		}

		//カメラの要素セット
		camera.SetPosition(Vector3(0.0f, 25.0f, -150.0f));
		camera.SetLookAt(Vector3(0.0f, 50.0f, 0.0f));
		camera.SetPerspective(
			XMConvertToRadians(60.0f),
			static_cast<float>(ConstVal::Window::ScreenW) / static_cast<float>(ConstVal::Window::ScreenH),
			0.1f,
			1000.0f
		);

		//モデルロード
		model.Load("Model/effole/effole/effole.fbx");//ファイルパスを要設定

		//modelの読み込みチェック


		//モデルは原点に配置するのでTransfromいじらない

		//RenderComponentに紐付け
		renderComponent.SetTransform(transform);
		renderComponent.SetModel(model);

		modelRenderer.CreateModelResource(model);

	}

}

void App::Update() {

	MSG message{};
	auto gfxDevice = GraphicsDevice::GetInstance();

	while (message.message != WM_QUIT) {

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {

			TranslateMessage(&message);
			DispatchMessage(&message);

		}
		else {

			auto gfxDevice = GraphicsDevice::GetInstance();

			//各オブジェクトの更新処理など
			gfxDevice.BeginFrame();


			auto& input = InputManager::GetInstance();
			input.Update();

			//デバッグ用コード
			{

				if (input.GetXInput().GetDigitalState(PadInputDigital::A) == InputState::Hold) {

					debugRenderer.DrawTriangle();

				}

				//Camera
				camera.Update();

				modelRenderer.Render(gfxDevice.GetCmdList(), renderComponent);

			}

			gfxDevice.EndFrame();

		}

	}

}
