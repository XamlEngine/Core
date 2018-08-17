#pragma once

#include "../Rendering/XamlServices.h"
#include "../Runtime/Media/Visual.h"

#include "../Runtime/Rect.h"
#include "../Runtime/Size.h"

using namespace CoreVar::Xaml::Rendering;
using namespace CoreVar::Xaml::Runtime;
using namespace CoreVar::Xaml::Runtime::Media;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class UIElement : public Visual
			{
			private:
				bool _isVisualValid = false;
			protected:
				XamlServices * services = 0;

				UIElement(XamlType* type, XamlServices* services) : Visual(type)
				{
					this->services = services;
				}

				XamlRenderContext* get_renderContext()
				{
					if (services == 0)
					{
						return 0;
					}
					return services->currentContext;
				}

				virtual Size measureCore(Size availableSize)
				{
					return availableSize;
				}

				virtual void arrangeCore(Rect finalRect)
				{

				}

				virtual void render(XamlRenderContext* context)
				{

				}

				
			public:

				Size desiredSize;

				bool isVisualValid()
				{
					return _isVisualValid;
				}

				void invalidateVisual()
				{
					_isVisualValid = false;
					XamlType* uiElementType = services->typeResolver->get_type(new std::string("CoreVar.Xaml.UI.UIElement"));
					XamlRuntimeObject* parent = services->visualTree->get_parent(this);
					if (parent != NULL && this->services->typeResolver->get_isSubtypeOf(parent->get_type(), uiElementType))
					{
						((UIElement*)parent)->invalidateVisual();
					}
				}

				void measure(Size availableSize)
				{
					desiredSize = measureCore(availableSize);
				}

				void arrange(Rect finalRect)
				{
					arrangeCore(finalRect);
					
					_isVisualValid = true;
				}

				void renderElement(XamlRenderContext* context)
				{
					render(context);

					int childCount = services->visualTree->get_childCount(this);
					for (int i = 0; i < childCount; i++)
					{
						UIElement* child = (UIElement*)services->visualTree->get_child(this, i);
						child->renderElement(context);
					}
				}

			};

		}
	}
}