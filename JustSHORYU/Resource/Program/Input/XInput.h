#pragma once

/*

Windows標準のXInputを使用した
XInput入力状態管理クラス

*/

#include<Windows.h>
#include<Xinput.h>
#include<vector>
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
	
	//配列用インデックス変換関数
	PadInputIndex	GetIndex(PadInput) const;
	PadInput		GetPadInput(PadInputIndex index) const;

	void UpdateButton();

	DWORD padIndex;
	bool isConnect;

	XINPUT_STATE xInputState;

	std::vector<InputState> buttonStateCurrent;
	std::vector<InputState> buttonStatePrev;

	std::vector<float> analogValue;

};
