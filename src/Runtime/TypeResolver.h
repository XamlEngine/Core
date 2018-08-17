#pragma once

#include "BaseTypes.h"
#include "IXamlResolver.h"
#include "XamlType.h"
#include "XamlProperty.h"
#include "XamlAttachedProperty.h"
#include "XamlRuntimeObject.h"
#include <unordered_map>
#include <functional>

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			class TypeResolver
			{
				XamlTypeCacheDictionary _types = XamlTypeCacheDictionary();
				XamlPropertyCacheDictionary _properties = XamlPropertyCacheDictionary();
				XamlAttachedPropertyCacheDictionary _attachedProperties = XamlAttachedPropertyCacheDictionary();
				XamlContentPropertyCacheDictionary _contentProperties = XamlContentPropertyCacheDictionary();
				XamlImplementedInterfacesCacheDictionary _implementedInterfaces = XamlImplementedInterfacesCacheDictionary();
				XamlParentTypesCacheDictionary _parentTypes = XamlParentTypesCacheDictionary();
				std::vector<IXamlResolver*>* _resolvers;

			public:
				TypeResolver(std::vector<IXamlResolver*>* resolvers)
				{
					_resolvers = resolvers;
				}

				XamlType* get_type(std::string* typeName)
				{
					XamlTypeCacheDictionary::const_iterator foundType = _types.find(*typeName);
					if (foundType != _types.end())
					{
						return foundType->second;
					}
					for (int i = 0; i < _resolvers->size(); i++)
					{
						IXamlResolver* resolver = _resolvers->at(i);
						XamlType* type = resolver->resolveType(typeName);
						if (type != 0)
						{
							_types.insert({ *typeName, type });
							return type;
						}
					}
					return 0;
				}

				XamlType* get_parent(XamlType* type)
				{
					XamlParentTypesCacheDictionary::const_iterator foundParent = _parentTypes.find(type);
					if (foundParent != _parentTypes.end())
					{
						return foundParent->second;
					}
					for (int i = 0; i < _resolvers->size(); i++)
					{
						IXamlResolver* resolver = _resolvers->at(i);
						
					}
					return NULL;
				}

				XamlProperty* get_property(XamlType* type, std::string* name)
				{
					XamlType* checkType = type;
					do
					{
						PropertyKeyTuple propertyKey = (PropertyKeyTuple)std::make_tuple(checkType, *name);
						XamlPropertyCacheDictionary::const_iterator foundProperty = _properties.find(propertyKey);
						if (foundProperty != _properties.end())
						{
							if (checkType != type)
							{
								_properties.insert({ (PropertyKeyTuple)std::make_tuple(type, *name), foundProperty->second });
							}
							return foundProperty->second;
						}
						for (int i = 0; i < _resolvers->size(); i++)
						{
							IXamlResolver* resolver = _resolvers->at(i);
							XamlProperty* prop = resolver->resolveProperty(checkType, name);
							if (prop != 0)
							{
								_properties.insert({ (PropertyKeyTuple)std::make_tuple(prop->declaringType(), *name), prop });
								if (prop->declaringType() != type)
								{
									_properties.insert({ (PropertyKeyTuple)std::make_tuple(type, *name), prop });
								}
								return prop;
							}
						}
						checkType = get_parentType(checkType);
					} while (checkType != NULL);
					return NULL;
				}

				XamlType* get_parentType(XamlType* type)
				{
					XamlParentTypesCacheDictionary::const_iterator foundType = _parentTypes.find(type);
					if (foundType != _parentTypes.end())
					{
						return foundType->second;
					}
					for (int i = 0; i < _resolvers->size(); i++)
					{
						IXamlResolver* resolver = _resolvers->at(i);
						XamlType* parentType = resolver->resolveParentType(type);
						if (parentType != NULL)
						{
							return parentType;
						}
					}
					return NULL;
				}

				bool get_isSubtypeOf(XamlType* type, XamlType* compareType)
				{
					XamlType* thisType = type;
					while ((thisType = get_parentType(thisType)) != NULL)
					{
						if (thisType == compareType)
						{
							return true;
						}
					}
					return false;
				}

				XamlAttachedProperty* get_attachedProperty(XamlType* type, std::string* name)
				{
					XamlType* checkType = type;
					do
					{
						PropertyKeyTuple propertyKey = (PropertyKeyTuple)std::make_tuple(checkType, *name);
						XamlAttachedPropertyCacheDictionary::const_iterator foundProperty = _attachedProperties.find(propertyKey);
						if (foundProperty != _attachedProperties.end())
						{
							if (type != checkType)
							{
								_attachedProperties.insert({ (PropertyKeyTuple)std::make_tuple(type, *name), foundProperty->second });
							}
							return foundProperty->second;
						}
						for (int i = 0; i < _resolvers->size(); i++)
						{
							IXamlResolver* resolver = _resolvers->at(i);
							XamlAttachedProperty* prop = resolver->resolveAttachedProperty(checkType, name);
							if (prop != 0)
							{
								_attachedProperties.insert({ (PropertyKeyTuple)std::make_tuple(prop->type(), *name), prop });
								if (prop->type() != type)
								{
									_attachedProperties.insert({ (PropertyKeyTuple)std::make_tuple(type, *name), prop });
								}
								return prop;
							}
						}
						checkType = get_parentType(checkType);
					} while (checkType != NULL);\
					return 0;
				}

				XamlProperty* get_contentProperty(XamlType* type)
				{
					XamlType* checkType = type;
					do
					{
						XamlContentPropertyCacheDictionary::const_iterator foundProperty = _contentProperties.find(checkType);
						if (foundProperty != _contentProperties.end())
						{
							if (checkType != type)
							{
								_contentProperties.insert({ type, foundProperty->second });
							}
							return foundProperty->second;
						}
						for (int i = 0; i < _resolvers->size(); i++)
						{
							IXamlResolver* resolver = _resolvers->at(i);
							XamlProperty* prop = resolver->resolveContentProperty(checkType);
							if (prop != 0)
							{
								_contentProperties.insert({ prop->declaringType(), prop });
								if (prop->declaringType() != type)
								{
									_contentProperties.insert({ type, prop });
								}
								return prop;
							}
						}
						checkType = get_parentType(checkType);
					} while (checkType != NULL);
					return 0;
				}

				std::unordered_set<XamlType*>* getImplementedInterfaces(XamlType* type)
				{
					XamlImplementedInterfacesCacheDictionary::const_iterator foundImplementedInterfaces = _implementedInterfaces.find(type);
					if (foundImplementedInterfaces != _implementedInterfaces.end())
					{
						return foundImplementedInterfaces->second;
					}

					XamlType* parentType = get_parentType(type);
					std::unordered_set<XamlType*>* parentInterfaces = parentType == NULL ? NULL : getImplementedInterfaces(parentType);

					for (int i = 0; i < _resolvers->size(); i++)
					{
						IXamlResolver* resolver = _resolvers->at(i);
						std::unordered_set<XamlType*>* implementedInterfaces = resolver->resolveInterfaces(type);
						if (implementedInterfaces != NULL)
						{
							if (parentInterfaces != NULL)
							{
								implementedInterfaces->insert(parentInterfaces->begin(), parentInterfaces->end());
							}
							return implementedInterfaces;
						}
						else if (parentInterfaces != NULL)
						{
							return parentInterfaces;
						}
					}
					return NULL;
				}

				bool get_implementsInterface(XamlType* type, XamlType* compareType)
				{
					std::unordered_set<XamlType*>* implementedInterfaces = getImplementedInterfaces(type);

					std::unordered_set<XamlType*>::const_iterator foundInterface = implementedInterfaces->find(compareType);

					return foundInterface != implementedInterfaces->end();
				}

			};

		}
	}
}