#include "pch.h"
#include "UserInputHook.h"
#include <thread>
#include <chrono>

void CALLBACK UserInputHook::EventHookCallback(
	[[maybe_unused]] HWINEVENTHOOK hWinEventHook,
	[[maybe_unused]] DWORD event,
	[[maybe_unused]] HWND hwnd,
	[[maybe_unused]] LONG idObject,
	[[maybe_unused]] LONG idChild,
	[[maybe_unused]] DWORD dwEventThread,
	[[maybe_unused]] DWORD dwmsEventTime
) {
	DWORD processId = 0;
	DWORD threadId = GetWindowThreadProcessId(hwnd, &processId);

	if (threadId == 0 || hwnd != GetForegroundWindow())	return;

	ImmAssociateContext(hwnd, ImmGetContext(hwnd));

	HWND IMEWnd = ImmGetDefaultIMEWnd(hwnd);

	if (IMEWnd != NULL && IsWindow(IMEWnd)) {

		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		switch (InputType)
		{
			case UserInputType::Default:
				SendMessage(IMEWnd, WM_IME_CONTROL, IMC_SETCONVERSIONMODE, IME_CMODE_NATIVE);
				break;
			case UserInputType::English:
				SendMessage(IMEWnd, WM_IME_CONTROL, IMC_SETCONVERSIONMODE, IME_CMODE_ALPHANUMERIC);
				break;
			//case UserInputType::Chinese:
			//	SendMessage(IMEWnd, WM_IME_CONTROL, IMC_SETCONVERSIONMODE, IME_CMODE_CHINESE);
			//	break;
			default:
				break;
		}

		//DWORD TargetMode = 0;
		//bool IsNeedMonitoring = false;

		//switch (InputType)
		//{
		//	case UserInputType::English:
		//		TargetMode = IME_CMODE_ALPHANUMERIC;
		//		IsNeedMonitoring = true;
		//		break;
		//	//case UserInputType::Chinese:
		//	//	TargetMode = IME_CMODE_CHINESE;
		//	//	IsNeedMonitoring = true;
		//	//	break;
		//	default:
		//		TargetMode = IME_CMODE_NATIVE;
		//		IsNeedMonitoring = false;
		//		break;
		//}

		//if (IsNeedMonitoring) {

		//	auto t = std::thread([imeHwnd, TargetMode]() {

		//		auto start_time = std::chrono::steady_clock::now();
		//		auto timeout = std::chrono::seconds(2);

		//		while (true) {

		//			auto now = std::chrono::steady_clock::now();
		//			if (now - start_time >= timeout) break;

		//			LRESULT CurrentMode = SendMessage(imeHwnd, WM_IME_CONTROL, IMC_GETCONVERSIONMODE, NULL);

		//			if ((CurrentMode & TargetMode)) {
		//			
		//				break;
		//			}

		//			if (CurrentMode != 0 && !(CurrentMode & TargetMode)) {
		//				SendMessage(imeHwnd, WM_IME_CONTROL, IMC_SETCONVERSIONMODE, TargetMode);
		//				break;
		//			}

		//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//		}
		//	});

		//	t.detach();

		//}
	}

}

bool UserInputHook::Initialize() {


	g_hook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND,
		EVENT_SYSTEM_FOREGROUND,
		NULL,
		EventHookCallback,
		0,
		0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	);

	//如果前景窗口钩子失败，尝试焦点对象钩子
	//if (!g_hook) {
	//	g_hook = SetWinEventHook(
	//		EVENT_OBJECT_FOCUS,
	//		EVENT_OBJECT_FOCUS,
	//		NULL,
	//		EventHookCallback,
	//		0,
	//		0,
	//		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	//	);
	//}

	return g_hook != NULL;
}

void UserInputHook::Clear() {

	if (g_hook) {
		UnhookWinEvent(g_hook);
		g_hook = NULL;
	}
}