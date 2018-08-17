#pragma once

#include "XamlType.h"
#include "XamlProperty.h"
#include "XamlAttachedProperty.h"
#include <unordered_set>

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			class IXamlResolver
			{

			public:
				virtual void set_services(void* services) = 0;

				virtual XamlType * resolveType(std::string* typeName) = 0;

				virtual XamlProperty* resolveProperty(XamlType* type, std::string* propertyName) = 0;

				virtual XamlAttachedProperty* resolveAttachedProperty(XamlType* type, std::string* propertyName) = 0;

				virtual XamlProperty* resolveContentProperty(XamlType* type) = 0;

				virtual XamlType* resolveParentType(XamlType* type) = 0;

				virtual std::unordered_set<XamlType*>* resolveInterfaces(XamlType* type) = 0;

			};
		}
	}
}