#pragma once

#include "Shape.h"
#include "../Rendering/Engine/RoundedRectangleRenderRegion.h"
#include "../Runtime/CornerRadius.h"
#include "../Runtime/Thickness.h"
#include "SolidColorBrush.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class Rectangle : public Shape
			{
				RoundedRectangleRenderRegion* _region = 0;
				Brush* _fill = 0;
				CornerRadius _cornerRadius = CornerRadius();
				Thickness _strokeThickness = Thickness();

				void updateFill()
				{
					if (_region == NULL)
					{
						return;
					}

					XamlType* solidColorBrushType = services->commonTypes->Xaml_UI_SolidColorBrush();

					if (_fill == NULL)
					{
						_region->solidColorFill = NULL;
						return;
					}
					
					XamlType* fillType = _fill->get_type();

					if (fillType == solidColorBrushType)
					{
						_region->solidColorFill = ((SolidColorBrush*)_fill)->get_color();
					}
				}

			protected:

				void arrangeCore(Rect finalRect)
				{
					bool initialize = false;
					bool updateLayout = false;
					if (_region == 0)
					{
						_region = services->objectFactory->createRoundedRectangle();
						initialize = true;
					}

					XamlRenderContext* renderContext = get_renderContext();
					double dpiRatio = renderContext->renderSurfaceContext->devicePixelRatio;

					_region->x = finalRect.x * dpiRatio;
					_region->y = finalRect.y * dpiRatio;
					_region->width = finalRect.width * dpiRatio;
					_region->height = finalRect.height * dpiRatio;

					XamlRenderContext* context = get_renderContext();

					if (initialize)
					{
						_region->initialize(context->renderSurfaceContext);

						RenderSurface* renderSurface = services->surface;

						renderSurface->set_firstRegion(_region);
					}

					updateFill();

					_region->updateLayout(context->renderSurfaceContext);
				}

				void render(XamlRenderContext* context)
				{
					_region->render(context->renderSurfaceContext);
				}

			public:
				Rectangle(XamlType* type, XamlServices* services) : Shape(type, services)
				{

				}

				Brush* get_fill()
				{
					return _fill;
				}

				void set_fill(Brush* value)
				{
					if (_fill == value)
					{
						return;
					}
					_fill = value;

					updateFill();
				}

				CornerRadius get_cornerRadius()
				{
					return _cornerRadius;
				}

				CornerRadius* get_cornerRadius_pointer()
				{
					return &_cornerRadius;
				}

				void set_cornerRadius(CornerRadius value)
				{
					_cornerRadius = value;

					invalidateVisual();
				}

				Thickness get_strokeThickness()
				{
					return _strokeThickness;
				}

				Thickness* get_strokeThickness_pointer()
				{
					return &_strokeThickness;
				}

				void set_strokeThickness(Thickness value)
				{
					_strokeThickness = value;

					invalidateVisual();
				}

			};

		}
	}
}