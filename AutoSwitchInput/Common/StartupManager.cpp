#include "pch.h"
#include "StartupManager.h"
#include <windows.h>
#include <shlwapi.h>
#include <stdexcept>

#pragma comment(lib, "shlwapi.lib")

namespace StartupManager
{
	std::wstring GetExecutablePath()
	{
		wchar_t buffer[MAX_PATH] = { 0 };
		DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

		if (length == 0 || length == MAX_PATH)
		{
			throw std::runtime_error("Failed to get executable path");
		}

		return std::wstring(buffer);
	}

	bool IsStartupEnabled(const std::wstring& appName)
	{
		HKEY hKey = nullptr;
		LPCWSTR regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

		// 打开注册表项
		if (RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			return false;
		}

		// 检查值是否存在
		DWORD type = 0;
		DWORD dataSize = 0;

		LONG result = RegQueryValueExW(
			hKey,
			appName.c_str(),
			nullptr,
			&type,
			nullptr,
			&dataSize
		);

		RegCloseKey(hKey);

		return (result == ERROR_SUCCESS && type == REG_SZ);
	}

	bool EnableStartup(const std::wstring& appName)
	{
		std::wstring exePath = GetExecutablePath();
		return SetStartupWithPath(appName, exePath);
	}

	bool SetStartupWithPath(const std::wstring& appName, const std::wstring& appPath)
	{
		HKEY hKey = nullptr;
		LPCWSTR regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

		// 打开注册表项
		LONG result = RegOpenKeyExW(
			HKEY_CURRENT_USER,
			regPath,
			0,
			KEY_WRITE,
			&hKey
		);

		if (result != ERROR_SUCCESS)
		{
			return false;
		}

		// 设置注册表值
		result = RegSetValueExW(
			hKey,
			appName.c_str(),
			0,
			REG_SZ,
			reinterpret_cast<const BYTE*>(appPath.c_str()),
			static_cast<DWORD>((appPath.length() + 1) * sizeof(wchar_t))
		);

		RegCloseKey(hKey);

		return (result == ERROR_SUCCESS);
	}

	bool DisableStartup(const std::wstring& appName)
	{
		HKEY hKey = nullptr;
		LPCWSTR regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

		// 打开注册表项
		LONG result = RegOpenKeyExW(
			HKEY_CURRENT_USER,
			regPath,
			0,
			KEY_WRITE,
			&hKey
		);

		if (result != ERROR_SUCCESS)
		{
			return false;
		}

		// 删除注册表值
		result = RegDeleteValueW(hKey, appName.c_str());

		RegCloseKey(hKey);

		// 如果值不存在，也视为成功
		return (result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND);
	}

	bool ToggleStartup(const std::wstring& appName)
	{
		if (IsStartupEnabled(appName))
		{
			return DisableStartup(appName);
		}
		else
		{
			return EnableStartup(appName);
		}
	}
}