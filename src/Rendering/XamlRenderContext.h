#pragma once

#include "Engine/RenderSurface.h"
#include "Engine/RenderSurfaceContext.h"
#include "../Services/XamlObjectFactory.h"
#include "../Runtime/TypeResolver.h"

using namespace CoreVar::Xaml::Rendering::Engine;
using namespace CoreVar::Xaml::Services;
using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{

			class XamlRenderContext
			{
			public:

				~XamlRenderContext()
				{
					delete renderSurfaceContext;
				}

				RenderSurfaceContext* renderSurfaceContext = 0;

			};

		}
	}
}