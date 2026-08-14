#pragma once

/*

アダプタ層
異なる入力形態をまとめる
まとめる入力は
"ConstantInput.h"に記載

*/

#include"ConstantInput.h"

class InterfaceInput {
public:

	~InterfaceInput() = default;

	virtual void Update() = 0;
	virtual bool IsPress(PhysicalKey keyCode) const = 0;
	virtual float GetAxis(PhysicalAxis keyCode) const = 0;

private:



};
