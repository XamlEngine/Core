#pragma once

#include "../../../Services/XamlObjectFactory.h"
#include "../Rendering/Engine/WebGLRoundedRectangleRenderRegion.h"
#include "../Rendering/Engine/WebGLRenderSurface.h"
#include "../Rendering/WebAssemblyFontManager.h"

using namespace CoreVar::Xaml::Rendering;
using namespace CoreVar::Xaml::Rendering::WebAssembly;
using namespace CoreVar::Xaml::Rendering::Engine;
using namespace CoreVar::Xaml::Rendering::Engine::WebAssembly;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Services
		{
			namespace WebAssembly
			{
				class WebAssemblyXamlObjectFactory : public XamlObjectFactory
				{

					RoundedRectangleRenderRegion* createRoundedRectangle()
					{
						return new WebGLRoundedRectangleRenderRegion();
					}

					RenderSurface* createRenderSurface()
					{
						return new WebGLRenderSurface();
					}

					FontManager* createFontManager()
					{
						return new WebAssemblyFontManager();
					}

				};
			}
		}
	}
}