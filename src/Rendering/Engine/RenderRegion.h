#pragma once

#include "RenderSurfaceContext.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Engine
			{
				class RenderRegion
				{
				public:
					double x = 0;
					double y = 0;
					double width = 0;
					double height = 0;
					bool visible = true;
					RenderRegion* next = 0;
					bool initialized = false;

					virtual void initialize(RenderSurfaceContext* context) = 0;

					virtual void terminate() = 0;

					virtual void updateLayout(RenderSurfaceContext* context) = 0;

					virtual void render(RenderSurfaceContext* context) = 0;

				};
			}
		}
	}
}