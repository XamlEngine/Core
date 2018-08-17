#pragma once


#include "RenderRegion.h"

#include "RoundedRectangleRenderRegion.h"
#include "RenderEvents.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Engine
			{

				class RenderSurface
				{

				protected:
					virtual void set_width(double value) = 0;
					virtual void set_height(double value) = 0;
					virtual void set_devicePixelRatio(double value) = 0;
					RenderRegion* firstRegion = 0;
					RenderEvents* events = 0;

					virtual RenderSurfaceContext* onCreateContext() = 0;

				public:
					virtual double get_width() = 0;
					virtual double get_height() = 0;
					virtual double get_devicePixelRatio() = 0;

					void set_events(RenderEvents* value)
					{
						events = value;
					}

					virtual void initialize() = 0;

					virtual void terminate() = 0;

					virtual void render(RenderSurfaceContext* context) = 0;

					virtual void finalizeRender(RenderSurfaceContext* context) = 0;

					RenderRegion* get_firstRegion()
					{
						return firstRegion;
					}

					void set_firstRegion(RenderRegion* value)
					{
						firstRegion = value;
					}

					RenderSurfaceContext* createContext()
					{
						RenderSurfaceContext* context = onCreateContext();
						context->width = get_width();
						context->height = get_height();
						context->devicePixelRatio = get_devicePixelRatio();
						return context;
					}

				};

			}
		}
	}
}