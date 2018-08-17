#pragma once

#include "../../../Rendering/FontFamily.h"

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace WebAssembly
			{

				class WebAssemblyFontFamily : public FontFamily
				{
				public:
					FT_Face face;
				};

			}
		}
	}
}