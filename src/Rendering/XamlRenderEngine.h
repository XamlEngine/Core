#pragma once

#include "Engine/RenderSurface.h"
#include "../Runtime/XamlType.h"
#include "../Runtime/XamlObject.h"
#include "../Runtime/IXamlResolver.h"
#include "../Runtime/TypeResolver.h"
#include "../Services/XamlObjectFactory.h"
#include "../UI/UIElement.h"
#include "XamlServices.h"
#include "Engine/RenderEvents.h"
#include <vector>

using namespace CoreVar::Xaml::Rendering::Engine;
using namespace CoreVar::Xaml::Runtime;
using namespace CoreVar::Xaml::UI;
using namespace CoreVar::Xaml::Services;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{

			class XamlRenderEngine : public RenderEvents
			{
				RenderSurface* _surface;
				XamlObjectFactory* _objectFactory;
				bool _initialized;
				UIElement* _rootVisual = 0;
				std::vector<IXamlResolver*>* _resolvers = new std::vector<IXamlResolver*>();
				TypeResolver* _typeResolver;
				XamlServices* _services;
				VisualTree* _visualTree;

				void updateLayout(XamlRenderContext* context)
				{
					if (_rootVisual == 0)
					{
						return;
					}

					_services->currentContext = context;

					Size surfaceSize;
					surfaceSize.width = _surface->get_width() / context->renderSurfaceContext->devicePixelRatio;
					surfaceSize.height = _surface->get_height() / context->renderSurfaceContext->devicePixelRatio;

					_rootVisual->measure(surfaceSize);

					Size* finalSize = &_rootVisual->desiredSize;

					Rect surfaceRect;
					surfaceRect.x = 0;
					surfaceRect.y = 0;
					surfaceRect.width = finalSize->width;
					surfaceRect.height = finalSize->height;

					_rootVisual->arrange(surfaceRect);

					_services->currentContext = 0;
				}

			public:

				XamlRenderEngine(RenderSurface* surface, XamlObjectFactory* objectFactory)
				{
					_surface = surface;
					_objectFactory = objectFactory;
					_typeResolver = new TypeResolver(_resolvers);
					_services = new XamlServices();
					_visualTree = new VisualTree();

					_services->surface = _surface;
					_services->objectFactory = _objectFactory;
					_services->typeResolver = _typeResolver;
					_services->visualTree = _visualTree;
					_services->commonTypes = new CommonTypes(_typeResolver);
					_services->fontManager = _objectFactory->createFontManager();

					_services->fontManager->invalidated = new std::function<void()>([=]() -> void
					{
						if (_rootVisual)
						{
							_rootVisual->invalidateVisual();
						}
					});

					_services->fontManager->initialize();

					_surface->set_events(this);
				}

				~XamlRenderEngine()
				{
					delete _visualTree;
					delete _typeResolver;
					delete _resolvers;
					delete _services;
				}

				XamlRenderContext* create_context()
				{
					RenderSurfaceContext* renderSurfaceContext = _surface->createContext();

					renderSurfaceContext->width = _surface->get_width() / _surface->get_devicePixelRatio();
					renderSurfaceContext->height = _surface->get_height() / _surface->get_devicePixelRatio();
					renderSurfaceContext->devicePixelRatio = _surface->get_devicePixelRatio();

					XamlRenderContext* context = new XamlRenderContext();
					context->renderSurfaceContext = renderSurfaceContext;

					return context;
				}

				void surfaceSizeChanged()
				{
					if (_rootVisual == 0)
					{
						return;
					}

					if (_rootVisual != 0)
					{
						_rootVisual->invalidateVisual();

						XamlRenderContext* context = create_context();

						updateLayout(context);
						
						render(context);

						delete context;
					}
				}

				RenderSurface* get_surface()
				{
					return _surface;
				}

				XamlServices* get_services()
				{
					return _services;
				}

				TypeResolver* get_typeResolver()
				{
					return _typeResolver;
				}

				void add_resolver(IXamlResolver* resolver)
				{
					resolver->set_services(_services);
					_resolvers->push_back(resolver);
				}

				void set_rootVisual(UIElement* value)
				{
					_rootVisual = value;

					if (_rootVisual != 0)
					{
						_rootVisual->invalidateVisual();
					}
				}

				void render(XamlRenderContext* context)
				{
					_services->currentContext = context;

					_surface->render(context->renderSurfaceContext);

					if (_rootVisual != 0)
					{
						if (!_rootVisual->isVisualValid())
						{
							updateLayout(context);
						}
						_rootVisual->renderElement(context);
					}

					_surface->finalizeRender(context->renderSurfaceContext);

					_services->currentContext = 0;
				}

			};

		}
	}
}