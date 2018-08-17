#pragma once

#include "Imaging/BitmapSource.h"
#include "../IO/FileManagement.h"

using namespace CoreVar::Xaml::UI::Imaging;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{


			class ImageSource : public BitmapSource
			{

			public:
				using BitmapSource::BitmapSource;

				void begin_load(void* arg, void (*loadComplete)(void*, SDL_Surface*), void (*errorCallback)(void*))
				{
					load_image_file(url.c_str(), arg, loadComplete, errorCallback);
				}

				void cancel_load()
				{

				}

				std::string url;

			};

			bool Parse_ImageSource(XamlType* imageSourceType, char* text, ImageSource** parsedResult)
			{
				ImageSource* imageSource = new ImageSource(imageSourceType);
				imageSource->url = std::string(text);
				*parsedResult = imageSource;
				return true;
			}
		}
	}
}