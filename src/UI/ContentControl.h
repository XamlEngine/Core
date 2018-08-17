#pragma once

#include "Control.h"
#include "../Runtime/Types.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class ContentControl : public Control
			{
				bool _initialized = false;
				XamlObject* _content = 0;
				XamlType* _uiElementType = 0;

				XamlType* get_uiElementType()
				{
					if (services == 0)
					{
						return 0;
					}
					if (_uiElementType != 0)
					{
						return _uiElementType;
					}
					return services->typeResolver->get_type(new std::string("CoreVar.Xaml.UI.UIElement"));
				}

			protected:

				Size measureOverride(Size availableSize)
				{
					XamlObject* content = get_content();
					XamlType* uiElementType = get_uiElementType();
					XamlType* contentType = content->get_type();
					if (this->services->typeResolver->get_isSubtypeOf(contentType, uiElementType))
					{
						((UIElement*)content)->measure(availableSize);
					}
					return availableSize;
				}

				Size arrangeOverride(Size finalSize)
				{
					XamlObject* content = get_content();
					XamlType* uiElementType = get_uiElementType();
					XamlType* contentType = content->get_type();
					if (services->typeResolver->get_isSubtypeOf(contentType, uiElementType))
					{
						Rect childRect;
						childRect.x = 0;
						childRect.y = 0;
						childRect.width = finalSize.width;
						childRect.height = finalSize.height;
						((UIElement*)content)->arrange(childRect);
					}
					return finalSize;
				}

			public:
				ContentControl(XamlType* type, XamlServices* services) : Control(type, services)
				{

				}

				XamlObject* get_content()
				{
					return _content;
				}

				void set_content(XamlObject* content)
				{
					XamlType* uiElementType = services->typeResolver->get_type(new std::string("CoreVar.Xaml.UI.UIElement"));
					if (_content != 0 && services->typeResolver->get_isSubtypeOf(_content->get_type(), uiElementType))
					{
						services->visualTree->remove_child((XamlRuntimeObject*)this, (XamlRuntimeObject*)_content);
					}
					_content = content;
					if (_content != 0 && services->typeResolver->get_isSubtypeOf(_content->get_type(), uiElementType))
					{
						services->visualTree->add_child((XamlRuntimeObject*)this, (XamlRuntimeObject*)_content);
					}
					invalidateVisual();
				}

			};

		}
	}
}