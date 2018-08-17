#pragma once

#include "XamlType.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			class XamlObject
			{
				XamlType* _type;

			public:

				XamlObject(XamlType* type)
				{
					_type = type;
				}

				XamlType * get_type()
				{
					return _type;
				}

			};

		}
	}
}