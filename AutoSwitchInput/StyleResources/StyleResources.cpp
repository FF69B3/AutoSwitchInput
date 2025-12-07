#include "pch.h"
#include "StyleResources.h"
#if __has_include("StyleResources.g.cpp")
#include "StyleResources.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <ranges>

namespace winrt::AutoSwitchInput::implementation
{
	winrt::Microsoft::UI::Xaml::DependencyProperty StyleResources::m_ResourcesProperty =
		winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
			L"Resources",
			xaml_typename<winrt::Microsoft::UI::Xaml::ResourceDictionary>(),
			xaml_typename<AutoSwitchInput::StyleResources>(),
			winrt::Microsoft::UI::Xaml::PropertyMetadata(nullptr, OnResourcesChanged));

	void StyleResources::OnResourcesChanged(const winrt::Microsoft::UI::Xaml::DependencyObject& d, const winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs& e)
	{
		if (const auto source = d.try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>())
		{
			winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::ResourceDictionary> mergedDicts(nullptr);
			if (const auto resources = source.Resources())
			{
				mergedDicts = resources.MergedDictionaries();
			}

			if (mergedDicts)
			{
				const auto begin = mergedDicts.begin();
				const auto end = mergedDicts.end();
				const auto it = std::ranges::find_if(begin, end, [](const winrt::Microsoft::UI::Xaml::ResourceDictionary& resDict)
				{
					return resDict.try_as<IStyleResourceDictionary>() != nullptr;
				});

				if (it != end)
				{
					mergedDicts.RemoveAt(static_cast<uint32_t>(it - begin));
				}

				if (const auto newVal = e.NewValue().try_as<winrt::Microsoft::UI::Xaml::ResourceDictionary>())
				{
					const auto clonedRes = winrt::make<StyleResourceDictionary>();
					CloneResourceDictionary(newVal, clonedRes);
					mergedDicts.Append(clonedRes);
				}
			}
		}
	}

	winrt::Microsoft::UI::Xaml::ResourceDictionary StyleResources::CloneResourceDictionary(const winrt::Microsoft::UI::Xaml::ResourceDictionary& resource, const winrt::Microsoft::UI::Xaml::ResourceDictionary& destination)
	{
		if (!resource)
		{
			return nullptr;
		}

		if (const auto source = resource.Source())
		{
			destination.Source(source);
		}
		else
		{
			if (const auto themeDicts = resource.ThemeDictionaries())
			{
				for (const auto theme : themeDicts)
				{
					if (const auto themeResource = theme.Value().try_as<winrt::Microsoft::UI::Xaml::ResourceDictionary>())
					{
						winrt::Microsoft::UI::Xaml::ResourceDictionary themeDict;
						CloneResourceDictionary(themeResource, themeDict);
						destination.ThemeDictionaries().Insert(theme.Key(), themeDict);
					}
					else
					{
						destination.ThemeDictionaries().Insert(theme.Key(), theme.Value());
					}
				}
			}

			if (const auto mergeDicts = resource.MergedDictionaries())
			{
				for (const auto mergedResource : mergeDicts)
				{
					winrt::Microsoft::UI::Xaml::ResourceDictionary mergedDict;
					CloneResourceDictionary(mergedResource, mergedDict);
					destination.MergedDictionaries().Append(mergedDict);
				}
			}

			for (const auto item : resource)
			{
				destination.Insert(item.Key(), item.Value());
			}
		}

		return destination;
	}

}
