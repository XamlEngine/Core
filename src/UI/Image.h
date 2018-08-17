#pragma once

#include "FrameworkElement.h"
#include "ImageSource.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class Image : public FrameworkElement
			{
				RoundedRectangleRenderRegion* _region = 0;
				BitmapSource* _source = 0;
				SDL_Surface* _textureSurface = 0;
				double _imageWidth = 0;
				double _imageHeight = 0;

			protected:
				Size measureOverride(Size availableSize)
				{
					Size result = Size();
					double widthRatio = 1;
					double heightRatio = 1;
					if (!isnan(get_height()) && !isinf(get_height()))
					{
						heightRatio = get_height() / _imageHeight;
					}
					if (!isnan(get_width()) && !isinf(get_width()))
					{
						result.width = get_width();
						if (_imageWidth != 0 && get_width() != 0)
						{
							widthRatio = get_width() / _imageWidth;
						}
					}
					else
					{
						switch (get_horizontalAlignment())
						{
						case HorizontalAlignment::Stretch:
							result.width = isinf(availableSize.width) ? heightRatio * _imageWidth : availableSize.width;
							break;
						default:
							result.width = heightRatio * _imageWidth;
							break;
						}
					}
					if (!isnan(get_height()) && !isinf(get_height()))
					{
						result.height = get_height();
					}
					else
					{
						switch (get_verticalAlignment())
						{
						case VerticalAlignment::Stretch:
							result.height = isinf(availableSize.height) ? widthRatio * _imageHeight : availableSize.height;
							break;
						default:
							result.height = widthRatio * _imageHeight;
						}
					}

					return result;
				}

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
					}

					_region->textureSurface = _textureSurface;

					_region->updateLayout(context->renderSurfaceContext);
				}

				void render(XamlRenderContext* context)
				{
					_region->render(context->renderSurfaceContext);
				}

			public:

				Image(XamlType* type, XamlServices* services) : FrameworkElement(type, services)
				{

				}

				BitmapSource* get_source()
				{
					return _source;
				}

				void _load_image_complete(SDL_Surface* textureSurface)
				{
					_textureSurface = textureSurface;

					_imageWidth = textureSurface->w;
					_imageHeight = textureSurface->h;

					invalidateVisual();
				}

				void _load_image_error()
				{
					printf("error loading image\n");
				}

				void set_source(BitmapSource* value)
				{
					_source = value;
					if (_source != 0)
					{
						_source->begin_load(this, 
							[](void* arg, SDL_Surface* surface) -> void
						{
							((Image*)arg)->_load_image_complete(surface);
						},
							[](void* arg) -> void
						{
							((Image*)arg)->_load_image_error();
						});
					}
				}

			};

		}
	}
}