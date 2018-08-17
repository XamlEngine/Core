#pragma once

#include "XamlObject.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			class XamlObjectReference
			{
				void* _value = 0;
				XamlType* _valueType = 0;
			public:
				using XamlObject::XamlObject;

				void set_value(void* value, XamlType* valueType);
				{
					_value = value;
					_valueType = valueType;
				}

				void* get_value(void* value)
				{
					return _value;
				}

				XamlType* get_valueType()
				{
					return _valueType;
				}

			};

		}
	}
}