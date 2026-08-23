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
#include"../Math/StructureMath.h"

class KeyMouseInput : public InterfaceInput {
public:

	KeyMouseInput();

	void Update() override;

	//ゲッター
	InputState GetKeyState(USHORT vk)			const { return keyState[vk]; }
	InputState GetMouseButtonState(UINT number) const { return mouseButtonState[number]; }

	DirectX::SimpleMath::Vector2 GetMousePos()	 const { return mousePos; }
	Vector2_LONG GetMouseDelta() const { return mouseDelta; }
	Vector2_SHORT GetWheelDelta() const { return wheelDelta; }


	//キーマウ入力受け取り系
	void RegisterRID(HWND hwnd);
	void SetLParam(LPARAM lParam);

private:

	//キーボード入力
	std::vector<bool>		isKeyPress;	//生値で押されているか
	std::vector<InputState> keyState;
	std::vector<InputState> keyStatePrev;

	//マウスのボタン入力
	std::vector<bool>		isMouseButtonPress;	//生値で押されているか
	std::vector<InputState> mouseButtonState;
	std::vector<InputState> mouseButtonStatePrev;

	//マウス移動
	Vector2_LONG mouseDelta;
	Vector2_LONG mouseDeltaPrev;

	//マウス位置
	DirectX::SimpleMath::Vector2 mousePos;
	DirectX::SimpleMath::Vector2 mousePosPrev;
	
	//マウスホイール
	Vector2_SHORT wheelDelta;
	Vector2_SHORT wheelDeltaPrev;

	//生入力の状態更新
	void SetKeyboard(USHORT vk, bool isPress);
	void SetMouseButton(USHORT number, bool isPress);
	void SetMouseMove(LONG dx, LONG dy);
	void SetMouseWheel(bool isVertical, SHORT delta);

	//InputState更新
	void UpdateKeyboard();
	void UpdateMouseButton();

};