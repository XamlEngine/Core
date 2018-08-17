#pragma once

#include "XamlRenderContext.h"
#include "../Media/VisualTree.h"
#include "../Services/CommonTypes.h"
#include "FontManager.h"

using namespace CoreVar::Xaml::Media;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{

			class XamlServices
			{
			public:
				XamlRenderContext * currentContext;

				RenderSurface* surface = 0;

				XamlObjectFactory* objectFactory = 0;

				TypeResolver* typeResolver = 0;

				VisualTree* visualTree = 0;

				CommonTypes* commonTypes = 0;

				FontManager* fontManager = 0;

			};

		}
	}
}