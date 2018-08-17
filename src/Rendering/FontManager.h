#pragma once

#include "FontFamily.h"
#include <string>
#include <functional>

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{

			class FontManager
			{
			public:
				virtual void initialize() = 0;

				virtual FontFamily* get_font(std::string* name) = 0;

				std::function<void()>* invalidated = 0;
			};

		}
	}
}