#pragma once

#include <vector>
#include "../../Runtime/XamlObject.h"

using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Helpers
			{

				struct XamlElementTracker
				{
					XamlElementTracker* parent = 0;

					XamlObject* element = 0;

					XamlProperty* currentProperty = 0;

					XamlAttachedProperty* currentAttachedProperty = 0;

					std::string* defaultNamespace = 0;

					char* elementName = 0;

				};

			}
		}
	}
}