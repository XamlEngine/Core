#pragma once

#include "FrameworkElement.h"
#include "UIElementCollection.h"
#include <vector>

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class Panel : public FrameworkElement
			{
				UIElementCollection* _children = NULL;
				
			protected:
				using FrameworkElement::FrameworkElement;

				Panel(XamlType* type, XamlServices* services) : FrameworkElement(type, services)
				{
					_children = new UIElementCollection(services->typeResolver->get_type(new std::string("CoreVar.Xaml.UI.UIElementCollection")), services, this);
				}

			public:
				UIElementCollection * get_children()
				{
					return _children;
				}
			};

		}
	}
}