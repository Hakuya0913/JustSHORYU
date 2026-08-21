#pragma once
#include<Windows.h>

class Vector2_LONG {
public:

	LONG x;
	LONG y;

	Vector2_LONG(LONG x = 0, LONG y = 0);

	static Vector2_LONG Zero() { return Vector2_LONG(0, 0); }

private:



};

class Vector2_SHORT {
public:

	SHORT x;
	SHORT y;

	Vector2_SHORT(SHORT x = 0, SHORT y = 0);

	static Vector2_SHORT Zero() { return Vector2_SHORT(0, 0); }

private:



};