#pragma once

/*

InputBindAnalog1DÇÃ2Dî≈

*/

#include"InputBindAnalog1D.h"

class InputBindAnalog2D {
public:

	InputBindAnalog2D();

	//äeé≤ÇÃGetter
			InputBindAnalog1D&	GetX()			{ return x; }
	const	InputBindAnalog1D&	GetX() const	{ return x; }

			InputBindAnalog1D&	GetY()			{ return y; }
	const	InputBindAnalog1D&	GetY() const	{ return y; }

private:

	InputBindAnalog1D x;
	InputBindAnalog1D y;

};