#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include <winrt/Microsoft.Windows.AppLifecycle.h>

namespace winrt::AutoSwitchInput::implementation
{
    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    winrt::Windows::Foundation::IAsyncAction App::OnLaunched([[maybe_unused]] winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs const& e)
    {

		auto AppInstance{ Microsoft::Windows::AppLifecycle::AppInstance::FindOrRegisterForKey(L"AutoSwitchInputApp") };

		if (!AppInstance.IsCurrent())
		{
			auto activatedEventArgs{ winrt::Microsoft::Windows::AppLifecycle::AppInstance::GetCurrent().GetActivatedEventArgs() };
			co_await AppInstance.RedirectActivationToAsync(activatedEventArgs);
			::ExitProcess(0);
			co_return;
		}


        window = make<MainWindow>();
        window.AppWindow().MoveAndResize(winrt::Windows::Graphics::RectInt32{ -1, -1, 1, 1 });;
        window.Activate();
    }
}
