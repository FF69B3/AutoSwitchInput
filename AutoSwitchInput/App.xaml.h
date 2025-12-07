#pragma once

#include "App.xaml.g.h"

namespace winrt::AutoSwitchInput::implementation
{
    struct App : AppT<App>
    {
        App();

        winrt::Windows::Foundation::IAsyncAction OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
