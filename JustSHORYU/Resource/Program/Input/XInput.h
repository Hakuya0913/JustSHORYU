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

	void Update() override;

	bool IsPress(PhysicalKey keyCode) const override;
	float GetAxis(PhysicalAxis keyCode) const override;

private:

	XINPUT_STATE state{};

};