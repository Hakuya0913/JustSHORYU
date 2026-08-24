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
	InputState GetKeyState(USHORT vk)			const { return keyStateCurrent[vk]; }
	InputState GetMouseButtonState(UINT number) const { return mouseButtonStateCurrent[number]; }

	DirectX::SimpleMath::Vector2 GetMousePos()	 const { return mousePosCurrent; }
	Vector2_LONG GetMouseDelta() const { return mouseDeltaCurrent; }
	Vector2_SHORT GetWheelDelta() const { return wheelDeltaCurrent; }


	//キーマウ入力受け取り系
	void RegisterRID(HWND hwnd);
	void SetLParam(LPARAM lParam);

private:

	//キーボード入力
	std::vector<bool>		isKeyPress;	//生値で押されているか
	std::vector<InputState> keyStateCurrent;
	std::vector<InputState> keyStatePrev;

	//マウスのボタン入力
	std::vector<bool>		isMouseButtonPress;	//生値で押されているか
	std::vector<InputState> mouseButtonStateCurrent;
	std::vector<InputState> mouseButtonStatePrev;

	//マウス移動
	Vector2_LONG mouseDeltaCurrent;
	Vector2_LONG mouseDeltaPrev;

	//マウス位置
	DirectX::SimpleMath::Vector2 mousePosCurrent;
	DirectX::SimpleMath::Vector2 mousePosPrev;
	
	//マウスホイール
	Vector2_SHORT wheelDeltaCurrent;
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