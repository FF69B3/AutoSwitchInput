#pragma once
#include "MainWindow.g.h"
#include <winrt/Microsoft.UI.Windowing.h>

#include <windows.h>

namespace winrt::AutoSwitchInput::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

		void OnLoaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnClosed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);
        static LRESULT CALLBACK TrayMenuWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        void ShowTrayMenuFlyout(POINT const& p);
        HWND GetWindowHwnd();

	public:
		void OnInputTypeChange(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnStartupClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnExit(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

    private:
        HWND hWnd{ nullptr };
        winrt::Microsoft::UI::Windowing::AppWindow m_AppWindow{nullptr};
    };
}

namespace winrt::AutoSwitchInput::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
