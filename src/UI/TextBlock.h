#pragma once

#include "FrameworkElement.h"
#include "SolidColorBrush.h"
#include "TextAlignment.h"
#include "../Runtime/TextWrapping.h"

using namespace CoreVar::Xaml::Rendering;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class TextBlock : public FrameworkElement
			{
				RoundedRectangleRenderRegion* _region = NULL;
				std::string* _text = NULL;
				bool initialize = false;
				double _fontSize = 12;
				Brush* _foreground = NULL;
				TextAlignment _textAlignment = TextAlignment::Left;
				TextWrapping _textWrapping = TextWrapping::None;

				void updateForeground()
				{
					if (_region == NULL)
					{
						return;
					}

					if (_foreground == NULL)
					{
						_region->solidColorFill = NULL;
					}

					XamlType* solidColorBrushType = services->commonTypes->Xaml_UI_SolidColorBrush();
					XamlType* foregroundType = _foreground->get_type();

					if (solidColorBrushType == foregroundType)
					{
						_region->solidColorFill = ((SolidColorBrush*)_foreground)->get_color();
					}
				}

			protected:

				Size measureOverride(Size availableSize)
				{
					XamlRenderContext* context = get_renderContext();

					if (initialize)
					{
						_region->initialize(context->renderSurfaceContext);
					}

					Size result = Size();

					result.width = 0;
					result.height = 0;

					if (_region != NULL)
					{
						if (_region->font == NULL)
						{
							_region->font = services->fontManager->get_font(new std::string("Arial"));
						}

						XamlRenderContext* renderContext = get_renderContext();
						double dpiRatio = renderContext->renderSurfaceContext->devicePixelRatio;

						_region->fontSize = _fontSize;

						Size measureSize = availableSize;
						measureSize.width *= dpiRatio;
						measureSize.height *= dpiRatio;

						result =_region->measureText(context->renderSurfaceContext, measureSize);

						result.width = result.width / dpiRatio;
						result.height = result.height / dpiRatio;
						
						printf("measure result %f %f\n", result.width, result.height);
					}

					return result;
				}

				void arrangeCore(Rect finalRect)
				{
					if (_region == NULL)
					{
						return;
					}

					XamlRenderContext* renderContext = get_renderContext();
					double dpiRatio = renderContext->renderSurfaceContext->devicePixelRatio;

					printf("final size %f %f desired size %f %f\n", finalRect.width, finalRect.height, desiredSize.width, desiredSize.height);

					_region->x = finalRect.x * dpiRatio;
					_region->y = finalRect.y * dpiRatio;
					_region->width = fmax(finalRect.width, desiredSize.width) * dpiRatio;
					_region->height = fmax(finalRect.height, desiredSize.height) * dpiRatio;

					_region->updateLayout(renderContext->renderSurfaceContext);
				}

				void render(XamlRenderContext* context)
				{
					if (_region == NULL)
					{
						return;
					}

					_region->render(context->renderSurfaceContext);
				}

			public:
				TextBlock(XamlType* type, XamlServices* services) : FrameworkElement(type, services)
				{
					
				}

				std::string* get_text()
				{
					return _text;
				}

				void set_text(std::string* text)
				{
					_text = text;
					if (_text != NULL)
					{
						if (_region == NULL)
						{
							_region = services->objectFactory->createRoundedRectangle();
							initialize = true;
						}
						_region->textToRender = _text->c_str();
					}
					else
					{
						if (_region != NULL)
						{
							_region->textToRender = NULL;
						}
					}
				}

				double get_fontSize()
				{
					return _fontSize;
				}

				double* get_fontSize_pointer()
				{
					return &_fontSize;
				}

				void set_fontSize(double value)
				{
					if (_fontSize == value)
					{
						return;
					}
					_fontSize = value;
					invalidateVisual();
				}

				Brush* get_foreground()
				{
					return _foreground;
				}

				void set_foreground(Brush* value)
				{
					if (_foreground == value)
					{
						return;
					}
					_foreground = value;

					updateForeground();
				}

				TextAlignment get_textAlignment()
				{
					return _textAlignment;
				}

				TextAlignment* get_textAlignment_pointer()
				{
					return &_textAlignment;
				}

				void set_textAlignment(TextAlignment value)
				{
					if (value == _textAlignment)
					{
						return;
					}
					_region->textAlignment = _textAlignment = value;
					invalidateVisual();
				}

				TextWrapping get_textWrapping()
				{
					return _textWrapping;
				}

				TextWrapping* get_textWrapping_pointer()
				{
					return &_textWrapping;
				}

				void set_textWrapping(TextWrapping value)
				{
					if (_textWrapping == value)
					{
						return;
					}
					_region->textWrapping = _textWrapping = value;
					invalidateVisual();
				}

			};

		}
	}
}