#pragma once

#include "XamlType.h"
#include "XamlObject.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			class XamlAttachedProperty
			{
				XamlType* _type;
				char* _name;
				XamlType* _elementType;
				XamlType* _propertyType;

			public:
				XamlAttachedProperty(XamlType* type, char* name, XamlType* elementType, XamlType* propertyType)
				{
					_type = type;
					_name = name;
					_elementType = elementType;
					_propertyType = propertyType;
				}

				XamlType* type()
				{
					return _type;
				}

				char* name()
				{
					return _name;
				}

				XamlType* elementType()
				{
					return _elementType;
				}

				XamlType* propertyType()
				{
					return _propertyType;
				}

			};

		}
	}
}