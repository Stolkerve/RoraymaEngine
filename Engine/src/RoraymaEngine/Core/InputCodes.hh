#pragma once


enum class KeysCode
{
	// From glfw3.h
	D0 = 48, /* 0 */
	D1 = 49, /* 1 */
	D2 = 50, /* 2 */
	D3 = 51, /* 3 */
	D4 = 52, /* 4 */
	D5 = 53, /* 5 */
	D6 = 54, /* 6 */
	D7 = 55, /* 7 */
	D8 = 56, /* 8 */
	D9 = 57, /* 9 */


	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	Space = 32,
	Escape		= 256,
	Enter		= 257,
	Tab			= 258,
	Backspace	= 259,
	Right		= 262,
	Left		= 263,
	Down		= 264,
	Up			= 265,
	PrintScreen = 283,
	LeftShift = 340,
	LeftControl = 341,
	LeftAlt = 342,
	LeftSuper = 343,
	RightShift = 344,
	RightControl = 345,
	RightAlt = 346,
	RightSuper = 347,

	F1  = 290,
	F2  = 291,
	F3  = 292,
	F4  = 293,
	F5  = 294,
	F6  = 295,
	F7  = 296,
	F8  = 297,
	F9  = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
};

enum class WinKeys
{
	Space = 0x20,
	Escape = 0x1B,
	Enter = 0x0D,
	Tab = 0x09,
	Backspace = 0x08,
	Right = 0x27,
	Left = 0x25,
	Down = 0x28,
	Up = 0x26,
	PrintScreen = 283,

	LeftShift = 0xA0,
	RightShift = 0xA1,
	LeftControl = 0xA2,
	RightControl = 0xA3,
	LeftAlt = 0xA4,
	RightAlt = 0xA5,

	D0 = 0x30, /* 0 */
	D1 = 0x31, /* 1 */
	D2 = 0x32, /* 2 */
	D3 = 0x33, /* 3 */
	D4 = 0x34, /* 4 */
	D5 = 0x35, /* 5 */
	D6 = 0x36, /* 6 */
	D7 = 0x37, /* 7 */
	D8 = 0x38, /* 8 */
	D9 = 0x39, /* 9 */

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4a,
	K = 0x4b,
	L = 0x4c,
	M = 0x4d,
	N = 0x4e,
	O = 0x4f,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5f,

	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,
};


enum class WinButton
{
	ButtonLeft = 0x01,
	ButtonRight = 0x02,
	ButtonMiddle = 0x04,
};

enum class MouseCode
{
	ButtonLeft = 0,
	ButtonRight = 1,
	ButtonMiddle = 2,
};
