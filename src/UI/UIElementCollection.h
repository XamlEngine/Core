#pragma once

#include "../Runtime/List.h"
#include "UIElement.h"

using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class UIElementCollection : public List<UIElement*>
			{
				XamlServices* _services;
				UIElement* _parent = 0;

			protected:
				void adding_item(UIElement* item, int index)
				{
					_services->visualTree->add_child(_parent, item, index);
				}

				void removed_item(UIElement* item, int index)
				{
					_services->visualTree->remove_child(_parent, item);
				}

				void cleared_items()
				{
					int itemCount = _services->visualTree->get_childCount(_parent);
					for (int i = 0; i < itemCount; i++)
					{
						_services->visualTree->remove_child(_parent, _services->visualTree->get_child(_parent, i));
					}
				}

				void replacing_item(UIElement* oldItem, UIElement* newItem, int index)
				{
					_services->visualTree->remove_child(_parent, oldItem);
					_services->visualTree->add_child(_parent, newItem, index);
				}

			public:
				UIElementCollection(XamlType* type, XamlServices* services, UIElement* parent) : List<UIElement*>(type)
				{
					_services = services;
					_parent = parent;
				}

			};

		}
	}
}