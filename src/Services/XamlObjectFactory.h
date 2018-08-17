#pragma once

#include "../Rendering/Engine/RoundedRectangleRenderRegion.h"
#include "../Rendering/Engine/RenderSurface.h"
#include "../Rendering/FontManager.h"

using namespace CoreVar::Xaml::Rendering;
using namespace CoreVar::Xaml::Rendering::Engine;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Services
		{

			class XamlObjectFactory
			{
			public:
				virtual RoundedRectangleRenderRegion* createRoundedRectangle() = 0;

				virtual RenderSurface* createRenderSurface() = 0;

				virtual FontManager* createFontManager() = 0;

			};

		}
	}
}