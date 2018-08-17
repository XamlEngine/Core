#pragma once

#include "IXamlResolver.h"
#include "..\Rendering\XamlServices.h"
#include "XamlType.h"
#include "XamlProperty.h"
#include "XamlAttachedProperty.h"
#include "Types.h"
#include <unordered_map>
#include <functional>

using namespace CoreVar::Xaml::Rendering;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			class XamlResolver : public IXamlResolver
			{
				XamlServices* _services;

				XamlTypeDictionary* _typeCreators;
				XamlPropertyDictionary _propertyCreators = XamlPropertyDictionary();
				XamlAttachedPropertyDictionary _attachedPropertyCreators = XamlAttachedPropertyDictionary();
				XamlContentPropertyNameDictionary _contentPropertyNames = XamlContentPropertyNameDictionary();
				XamlImplementedInterfacesDictionary _implementedInterfaces = XamlImplementedInterfacesDictionary();
				XamlParentTypesDictionary _parentTypes = XamlParentTypesDictionary();


			protected:

				XamlPropertyDictionary * propertyCreators()
				{
					return &_propertyCreators;
				}

				XamlAttachedPropertyDictionary* attachedPropertyCreators()
				{
					return &_attachedPropertyCreators;
				}

				XamlContentPropertyNameDictionary* contentPropertyNames()
				{
					return &_contentPropertyNames;
				}

				XamlServices* get_services()
				{
					return _services;
				}

				XamlImplementedInterfacesDictionary* implementedInterfaces()
				{
					return &_implementedInterfaces;
				}

				XamlParentTypesDictionary* parentTypes()
				{
					return &_parentTypes;
				}

				virtual XamlTypeDictionary* onGetTypeCreators() = 0;

			public:

				void set_services(void* services)
				{
					_services = (XamlServices*)services;
				}

				XamlType * resolveType(std::string* typeName)
				{
					if (_typeCreators == NULL)
					{
						_typeCreators = onGetTypeCreators();
					}

					std::unordered_map<std::string, std::function<XamlType*()>>::const_iterator found = _typeCreators->find(*typeName);
					if (found == _typeCreators->end())
					{
						return 0;
					}
					return found->second();
				}

				XamlProperty* resolveProperty(XamlType* type, std::string* propertyName)
				{
					PropertyKeyTuple key = (PropertyKeyTuple)std::make_tuple(type, *propertyName);
					XamlPropertyDictionary::const_iterator propertyFound = _propertyCreators.find(key);
					if (propertyFound != _propertyCreators.end())
					{
						return propertyFound->second();
					}
					return 0;
				}

				XamlAttachedProperty* resolveAttachedProperty(XamlType* type, std::string* propertyName)
				{
					PropertyKeyTuple key = (PropertyKeyTuple)std::make_tuple(type, *propertyName);
					XamlAttachedPropertyDictionary::const_iterator propertyFound = _attachedPropertyCreators.find(key);
					if (propertyFound != _attachedPropertyCreators.end())
					{
						return propertyFound->second();
					}
					return 0;
				}

				XamlProperty* resolveContentProperty(XamlType* type)
				{
					XamlContentPropertyNameDictionary::const_iterator found = _contentPropertyNames.find(type);
					if (found != _contentPropertyNames.end())
					{
						XamlProperty* contentProperty = resolveProperty(type, found->second);
						if (contentProperty != NULL)
						{
							return contentProperty;
						}
					}
					return NULL;
				}

				XamlType* resolveParentType(XamlType* type)
				{
					XamlParentTypesDictionary::const_iterator foundParent = _parentTypes.find(type);
					if (foundParent != _parentTypes.end())
					{
						return (*foundParent->second)();
					}
					return NULL;
				}

				std::unordered_set<XamlType*>* resolveInterfaces(XamlType* type)
				{
					XamlImplementedInterfacesDictionary::const_iterator foundInterfaces = _implementedInterfaces.find(type);
					if (foundInterfaces != _implementedInterfaces.end())
					{
						return (*foundInterfaces->second)();
					}
					return NULL;
				}

			};

		}
	}
}