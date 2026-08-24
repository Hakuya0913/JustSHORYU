#pragma once

/*

Windows標準のXInputを使用した
XInput入力状態管理クラス

*/

#include<Windows.h>
#include<Xinput.h>
#include"ConstantInput.h"
#include"InterfaceInput.h"

class XInput : public InterfaceInput {
public:

	XInput(DWORD index);

	void Update() override;

	bool IsConnect() const { return isConnect; }

	InputState GetDegitalState(PadInput inputType) const;
	AnalogStrength GetAnalogStrength(PadInput inputType) const;
	float GetAnalogValue(PadInput inputType) const;

private:

	DWORD padIndex;
	bool isConnect;

	XINPUT_STATE stateCurrent;
	XINPUT_STATE statePrev;

};
