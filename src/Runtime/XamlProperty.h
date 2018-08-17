#pragma once

#include "XamlType.h"
#include "XamlObject.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			class XamlProperty
			{
				XamlType* _declaringType;
				char* _name;
				XamlType* _propertyType;
				std::function<void*(XamlObject*)>* _getter;
				std::function<void(XamlObject*, void*)>* _setter;

			public:
				XamlProperty(XamlType* declaringType, char* name, XamlType* propertyType, std::function<void*(XamlObject*)>* valueGetter = 0, std::function<void(XamlObject*, void*)>* valueSetter = 0)
				{
					_declaringType = declaringType;
					_name = name;
					_propertyType = propertyType;
					_getter = valueGetter;
					_setter = valueSetter;
				}

				XamlType* declaringType()
				{
					return _declaringType;
				}

				char* name()
				{
					return _name;
				}

				XamlType* propertyType()
				{
					return _propertyType;
				}

				void* get_value(XamlObject* object)
				{
					return (*_getter)(object);
				}

				void set_value(XamlObject* object, void* value)
				{
					(*_setter)(object, value);
				}

			};
		}
	}
}