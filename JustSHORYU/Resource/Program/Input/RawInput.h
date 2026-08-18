#pragma once

/*

RawInput(キーボード、マウス)入力を管理
InterfaceInputを継承した
RawInputアダプタークラス

*/

#include<Windows.h>
#include<DirectXMath.h>
#include<SimpleMath.h>
#include<vector>
#include"InterfaceInput.h"
#include"ConstantInput.h"

class RawInput : public InterfaceInput {
public:

	RawInput();

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

	//生入力保存
	RAWINPUTDEVICE rawInputDevice[KeyMouseConst::RowInputDeviceCount];

	//キーボード入力
	std::vector<InputState> keyState;
	std::vector<InputState> keyStatePrev;

	//マウスのボタン入力
	std::vector<InputState> mouseButtonState;
	std::vector<InputState> mouseButtonStatePrev;

	//マウスの移動
	DirectX::SimpleMath::Vector2 mousePos;
	DirectX::SimpleMath::Vector2 mousePosPrev;
	DirectX::SimpleMath::Vector2 mouseDelta;
	DirectX::SimpleMath::Vector2 mouseDeltaPrev;

	//マウスホイール
	int wheelDelta		= 0;
	int wheelDeltaPrev	= 0;

	//入力更新
	void SetInput();
	void SetKeyboard(USHORT vk, bool isPress);
	void SetMouseButton(int number, bool isPress);
	void SetMouseMove(float dx, float dy);
	void SetMouseWheel(int delta);

};