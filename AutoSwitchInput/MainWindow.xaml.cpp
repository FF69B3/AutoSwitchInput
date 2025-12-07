#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <Microsoft.UI.Xaml.Window.h>
#include <winrt/Microsoft.UI.Interop.h>

#include <windowsx.h>
#include "Common/NotifyIcon.h"
#include "Common/UserInputHook.h"
#include "Common/StartupManager.h"
#include "Common/DPICalcHelper.h"

#define StartupName L"AutoSwitchInputApp"

namespace winrt::AutoSwitchInput::implementation
{

	static WNDPROC oldWndProc = nullptr;

	static MainWindow* pTrayMenuWindow{ nullptr };

	MainWindow::MainWindow()
	{
		pTrayMenuWindow = this;
	}

	void MainWindow::OnLoaded([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{

		//ExtendsContentIntoTitleBar(true);

		hWnd = GetWindowHwnd();
		m_AppWindow = AppWindow();

		auto presenter = Microsoft::UI::Windowing::OverlappedPresenter::CreateForContextMenu();
		presenter.IsAlwaysOnTop(true);

		m_AppWindow.SetPresenter(presenter);
		m_AppWindow.MoveAndResize(winrt::Windows::Graphics::RectInt32{ -1, -1, 1, 1 });
		m_AppWindow.IsShownInSwitchers(false);


		SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);

		oldWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MainWindow::TrayMenuWindowProc));

		if (UserInputHook::Initialize()) NotifyIcon::CreateTrayIcon(hWnd);


		StartupItem().IsChecked(StartupManager::IsStartupEnabled(StartupName));

	}

	void MainWindow::OnClosed([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::Microsoft::UI::Xaml::WindowEventArgs const& args)
	{
		pTrayMenuWindow = nullptr;

		NotifyIcon::RemoveTrayIcon();
		UserInputHook::Clear();

	}

	void MainWindow::OnExit([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{
		this->Close();
	}

	void MainWindow::ShowTrayMenuFlyout(POINT const& p)
	{
		SetForegroundWindow(hWnd);

		float scalingFactor = DPICalcHelper::GetScalingFactor(hWnd);


		//winrt::Windows::Foundation::Size menuSize = { 120 * scalingFactor, 100 * scalingFactor };

		float menuWidthInPixels = 120.f * scalingFactor;

		float centerX = p.x - menuWidthInPixels / 2;

		TrayMenuFlyout().ShowAt(this->Content(), {centerX, static_cast<float>(p.y)});

	}

	void MainWindow::OnInputTypeChange(winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{

		if (auto SelectedItem = sender.try_as<winrt::Microsoft::UI::Xaml::Controls::RadioMenuFlyoutItem>()) {
		
			auto Text = SelectedItem.Text();

			if (Text == L"英文输入法") {
				UserInputHook::InputType = UserInputType::English;
		
			}
			else if (Text == L"默认输入法") {
				UserInputHook::InputType = UserInputType::Default;
			}


		}

	}

	void MainWindow::OnStartupClick(winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
	{

		if (auto SelectedItem = sender.try_as<winrt::Microsoft::UI::Xaml::Controls::ToggleMenuFlyoutItem>()) {


			if (SelectedItem.IsChecked()) {
			
				StartupManager::EnableStartup(StartupName);
			}
			else {
			
				StartupManager::DisableStartup(StartupName);
			}

		}

	}

	HWND MainWindow::GetWindowHwnd()
	{

		HWND hwnd = nullptr;

		if (hwnd == nullptr)
		{
			winrt::Microsoft::UI::Xaml::Window window = *this;
			window.as<IWindowNative>()->get_WindowHandle(&hwnd);
		}
		return hwnd;
	}

	LRESULT CALLBACK MainWindow::TrayMenuWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_TRAYMOUSEMESSAGE: {

				switch (LOWORD((int)lParam))
				{
					case WM_RBUTTONDOWN: {


						POINT pt;
						GetCursorPos(&pt);
						if (pTrayMenuWindow) pTrayMenuWindow->ShowTrayMenuFlyout({ pt.x, pt.y });
						break;
					}

				}
			}

			default: break;
		}

		return CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
	}


}

