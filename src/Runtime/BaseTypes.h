#pragma once

#include <unordered_map>
#include <unordered_set>
#include "XamlProperty.h"
#include "XamlAttachedProperty.h"

using namespace CoreVar::Xaml::Runtime;

typedef std::tuple<XamlType*, std::string> PropertyKeyTuple;

struct PropertyKey_Hash : public std::unary_function<PropertyKeyTuple, std::size_t>
{
	std::size_t operator()(const PropertyKeyTuple& key) const
	{
		return (std::size_t)std::get<0>(key) ^ std::hash<std::string>{}(std::get<1>(key));
	}
};

struct PropertyKey_Equal : public std::binary_function<PropertyKeyTuple, PropertyKeyTuple, bool>
{
	bool operator()(const PropertyKeyTuple& k0, const PropertyKeyTuple& k1) const
	{
		return (std::get<0>(k0) == std::get<0>(k1) &&
			std::get<1>(k0) == std::get<1>(k1));
	}
};

typedef std::unordered_map<std::string, std::function<XamlType*()>> XamlTypeDictionary;
typedef std::unordered_map<PropertyKeyTuple, std::function<XamlProperty*()>, PropertyKey_Hash, PropertyKey_Equal> XamlPropertyDictionary;
typedef std::unordered_map<PropertyKeyTuple, std::function<XamlAttachedProperty*()>, PropertyKey_Hash, PropertyKey_Equal> XamlAttachedPropertyDictionary;
typedef std::unordered_map<XamlType*, std::string*> XamlContentPropertyNameDictionary;
typedef std::unordered_map<XamlType*, std::function<std::unordered_set<XamlType*>*()>*> XamlImplementedInterfacesDictionary;
typedef std::unordered_map<XamlType*, std::function<XamlType*()>*> XamlParentTypesDictionary;

typedef std::unordered_map<std::string, XamlType*> XamlTypeCacheDictionary;
typedef std::unordered_map<PropertyKeyTuple, XamlProperty*, PropertyKey_Hash, PropertyKey_Equal> XamlPropertyCacheDictionary;
typedef std::unordered_map<PropertyKeyTuple, XamlAttachedProperty*, PropertyKey_Hash, PropertyKey_Equal> XamlAttachedPropertyCacheDictionary;
typedef std::unordered_map<XamlType*, XamlProperty*> XamlContentPropertyCacheDictionary;
typedef std::unordered_map<XamlType*, std::unordered_set<XamlType*>*> XamlImplementedInterfacesCacheDictionary;
typedef std::unordered_map<XamlType*, XamlType*> XamlParentTypesCacheDictionary;