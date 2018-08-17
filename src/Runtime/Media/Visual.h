#pragma once

#include "../XamlRuntimeObject.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			namespace Media
			{

				class Visual : public XamlRuntimeObject
				{
				protected:
					using XamlRuntimeObject::XamlRuntimeObject;
				};

			}
		}
	}
}