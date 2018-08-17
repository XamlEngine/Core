#pragma once

#include "Panel.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class Canvas : public Panel
			{

				Size measureOverride(Size availableSize)
				{
					int childCount = get_children()->count();
					for (int i = 0; i < childCount; i++)
					{
						UIElement* uiElement = get_children()->get_item(i);
						uiElement->measure(availableSize);
					}
					return availableSize;
				}

				Size arrangeOverride(Size finalSize)
				{
					int childCount = get_children()->count();
					for (int i = 0; i < childCount; i++)
					{
						UIElement* uiElement = get_children()->get_item(i);

						XamlType* canvasElementType = get_type();
						XamlAttachedProperty* topProperty = services->typeResolver->get_attachedProperty(canvasElementType, new std::string("Top"));
						XamlAttachedProperty* leftProperty = services->typeResolver->get_attachedProperty(canvasElementType, new std::string("Left"));
						void* topValue = uiElement->get_value(topProperty);
						void* leftValue = uiElement->get_value(leftProperty);

						Rect childRect;
						childRect.x = *(double*)leftValue;
						childRect.y = *(double*)topValue;
						childRect.width = uiElement->desiredSize.width;
						childRect.height = uiElement->desiredSize.height;
						uiElement->arrange(childRect);
					}
					return finalSize;
				}

			public:
				Canvas(XamlType* type, XamlServices* services) : Panel(type, services)
				{

				}
			};

		}
	}
}