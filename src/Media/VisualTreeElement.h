#pragma once

#include "../Runtime/XamlRuntimeObject.h"
#include <vector>

using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Media
		{

			struct VisualTreeElement
			{
				XamlRuntimeObject* element;
				VisualTreeElement* parent;
				std::vector<VisualTreeElement*>* children = new std::vector<VisualTreeElement*>();

				~VisualTreeElement()
				{

					delete children;
				}

			};

		}
	}
}