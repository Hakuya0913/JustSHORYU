#pragma once

/*

キーボード・マウスの入力を管理
InterfaceInputを継承した
RawInputアダプタークラス

*/

#include<Windows.h>
#include<DirectXMath.h>
#include<SimpleMath.h>
#include<vector>
#include"InterfaceInput.h"
#include"ConstantInput.h"

class KeyMouseInput : public InterfaceInput {
public:

	KeyMouseInput();

	void Update() override;

	//ゲッター
	InputState GetKeyState(USHORT vk) const { return keyState[vk]; }
	InputState GetMouseButtonState(UINT number) const { return mouseButtonState[number]; }

	DirectX::SimpleMath::Vector2 GetMousePos() const { return mousePos; }
	DirectX::SimpleMath::Vector2 GetMouseDelta() const { return mouseDelta; }
	int GetWheelDelta() const { return wheelDelta; }


	//キーマウ入力受け取り系
	void RegisterRID(HWND hwnd);
	void SetLParam(LPARAM lParam);

private:

	//キーボード入力
	std::vector<InputState> keyState;
	std::vector<InputState> keyStatePrev;

	//マウスのボタン入力
	std::vector<InputState> mouseButtonState;
	std::vector<InputState> mouseButtonStatePrev;

	//マウスの移動
	DirectX::SimpleMath::Vector2 mousePos		= DirectX::SimpleMath::Vector2::Zero;
	DirectX::SimpleMath::Vector2 mousePosPrev	= DirectX::SimpleMath::Vector2::Zero;
	DirectX::SimpleMath::Vector2 mouseDelta		= DirectX::SimpleMath::Vector2::Zero;
	DirectX::SimpleMath::Vector2 mouseDeltaPrev = DirectX::SimpleMath::Vector2::Zero;

	//マウスホイール
	int wheelDelta		= 0;
	int wheelDeltaPrev	= 0;

	//入力更新
	void SetKeyboard(USHORT vk, bool isPress);
	void SetMouseButton(int number, bool isPress);
	void SetMouseMove(float dx, float dy);
	void SetMouseWheel(int delta);

};