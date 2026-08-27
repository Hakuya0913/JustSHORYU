#pragma once

/*

Windows標準のXInputを使用した
XInput入力状態管理クラス

*/

#include<Windows.h>
#include<Xinput.h>
#include<vector>
#include<DirectXMath.h>
#include<SimpleMath.h>
#include"ConstantInput.h"
#include"InterfaceInput.h"

class XInput : public InterfaceInput {
public:

	XInput(DWORD index);

	void Update() override;

	//Getter
	bool IsConnect() const { return isConnect; }

	InputState		GetDegitalState(	PadInputDigital inputType) const;
	AnalogStrength	GetAnalogStrength(	PadInputAnalog  inputType) const;
	float			GetAnalogValue(		PadInputAnalog  inputType) const;
	float			GetAnalogDelta(		PadInputAnalog  inputType) const;

private:

	//PadInputを意味を持つ数字で返す
	WORD	ConvertWORD(		PadInputDigital padInput)	const;	//XINPUT_GAMEPAD系のビットフラグに変換
	float	ConvertAnalogValue(	PadInputAnalog	padInput)	const;	//アナログ入力の正規化値を返す

	//状態の更新
	void UpdateDegitalInput();
	void UpdateAnalogInput();

	//デッドゾーン適用
	DirectX::SimpleMath::Vector2 ApplyDeadzoneRadial(DirectX::SimpleMath::Vector2 vector) const;
	float						 ApplyDeadzone(float value) const;

	DWORD padIndex;
	bool isConnect;

	//XInputの状態を保存
	XINPUT_STATE xInputState;

	//ボタン入力の状態を保存
	std::array<InputState, DigitalList.size()> buttonStateCurrent;
	std::array<InputState, DigitalList.size()> buttonStatePrev;

	//アナログ入力の正規化値を保存
	std::array<float, AnalogList.size()> analogValueCurrent;
	std::array<float, AnalogList.size()> analogValuePrev;
	std::array<float, AnalogList.size()> analogValueDelta;

};
