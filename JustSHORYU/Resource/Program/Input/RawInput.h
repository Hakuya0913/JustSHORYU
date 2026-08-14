#pragma once

/*

RawInput(キーボード、マウス)入力を管理
InterfaceInputを継承した
RawInputアダプタークラス

*/

#include<Windows.h>
#include"InterfaceInput.h"

class RawInput : public InterfaceInput {
public:

	RawInput();

	void Update() override;

	bool IsPress(PhysicalKey keyCode) const override;
	float GetAxis(PhysicalAxis keyCode) const override;

	//キーマウ入力受け取り関数
	void SetKeyboard(USHORT vk, bool isPress);
	void SetMouseButton(int button, bool isPress);
	void SetMouseMove(float dx, float dy);

private:

	bool keyState[KeyMouseConst::KeyMax]{};
	bool mouseButton[KeyMouseConst::MouseButtonMax]{};
	float mouseX = 0.0f;
	float mouseY = 0.0f;

};