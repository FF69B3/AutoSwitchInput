#pragma once
#include <windows.h>
#include <imm.h>
#pragma comment(lib, "imm32.lib")

#define IMC_GETCONVERSIONMODE 0x0001
#define IMC_SETCONVERSIONMODE 0x0002

#define IME_CMODE_ALPHANUMERIC  0x0000
#define IME_CMODE_NATIVE        0x0001
#define IME_CMODE_KATAKANA      0x0002
#define IME_CMODE_LANGUAGE      0x0003
#define IME_CMODE_FULLSHAPE     0x0008
#define IME_CMODE_ROMAN         0x0010
#define IME_CMODE_CHARCODE      0x0020
#define IME_CMODE_HANJACONVERT  0x0040
#define IME_CMODE_SOFTKBD       0x0080
#define IME_CMODE_NOCONVERSION  0x0100
#define IME_CMODE_EUDC          0x0200
#define IME_CMODE_SYMBOL        0x0400
#define IME_CMODE_FIXED         0x0800


enum class UserInputType : BYTE
{
	Default = 0,
	English,
	Chinese
};


struct UserInputHook
{
	inline static HWINEVENTHOOK g_hook = nullptr;
	inline static UserInputType InputType = UserInputType::English;

	static void CALLBACK EventHookCallback(
		[[maybe_unused]] HWINEVENTHOOK hWinEventHook,
		[[maybe_unused]] DWORD event,
		[[maybe_unused]] HWND hwnd,
		[[maybe_unused]] LONG idObject,
		[[maybe_unused]] LONG idChild,
		[[maybe_unused]] DWORD dwEventThread,
		[[maybe_unused]] DWORD dwmsEventTime
	);

	static bool Initialize();

	static void Clear();
};