#pragma once

#include "../../Runtime/XamlObject.h"
#include <SDL/SDL_surface.h>

using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{
			namespace Imaging
			{
				class BitmapSource : public XamlObject
				{
				public:
					using XamlObject::XamlObject;

					virtual void begin_load(void* arg, void(*loadComplete)(void*, SDL_Surface*), void(*errorCallback)(void*)) = 0;

					virtual void cancel_load() = 0;

				};
			}
		}
	}
}