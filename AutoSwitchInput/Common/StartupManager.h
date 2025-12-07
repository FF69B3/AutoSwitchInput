#pragma once
#include <string>

namespace StartupManager
{
	bool EnableStartup(const std::wstring& appName = L"MyApplication");
	bool DisableStartup(const std::wstring& appName = L"MyApplication");
	bool ToggleStartup(const std::wstring& appName = L"MyApplication");
	bool IsStartupEnabled(const std::wstring& appName = L"MyApplication");
	std::wstring GetExecutablePath();
	bool SetStartupWithPath(const std::wstring& appName, const std::wstring& appPath);
}