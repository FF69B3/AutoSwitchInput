#pragma once

#include "StyleResources.g.h"

namespace winrt::AutoSwitchInput::implementation
{

	MIDL_INTERFACE("A240AC45-8961-4B2F-9259-7F9C8740BFFC") IStyleResourceDictionary : IUnknown
	{
	};

	struct StyleResourceDictionary : winrt::Microsoft::UI::Xaml::ResourceDictionaryT<StyleResourceDictionary, IStyleResourceDictionary>
	{
	};

    struct StyleResources : StyleResourcesT<StyleResources>
    {
        StyleResources() = default;

		static winrt::Microsoft::UI::Xaml::DependencyProperty ResourcesProperty()
		{
			return m_ResourcesProperty;
		}

		static winrt::Microsoft::UI::Xaml::ResourceDictionary GetResources(const winrt::Microsoft::UI::Xaml::DependencyObject& obj)
		{
			if (obj)
			{
				return obj.GetValue(m_ResourcesProperty).as<winrt::Microsoft::UI::Xaml::ResourceDictionary>();
			}
			else
			{
				return nullptr;
			}
		}

		static void SetResources(const winrt::Microsoft::UI::Xaml::DependencyObject& obj, const winrt::Microsoft::UI::Xaml::ResourceDictionary& value)
		{
			if (obj)
			{
				obj.SetValue(m_ResourcesProperty, value);
			}
		}

	private:

		static void OnResourcesChanged(const winrt::Microsoft::UI::Xaml::DependencyObject& d, const winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs& e);
		static winrt::Microsoft::UI::Xaml::ResourceDictionary CloneResourceDictionary(const  winrt::Microsoft::UI::Xaml::ResourceDictionary& resource, const winrt::Microsoft::UI::Xaml::ResourceDictionary& destination);

		static winrt::Microsoft::UI::Xaml::DependencyProperty m_ResourcesProperty;

    };
}

namespace winrt::AutoSwitchInput::factory_implementation
{
    struct StyleResources : StyleResourcesT<StyleResources, implementation::StyleResources>
    {
    };
}
