#pragma once

#include "XamlObject.h"
#include <unordered_map>

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			class XamlRuntimeObject : public XamlObject
			{
				std::unordered_map<XamlAttachedProperty*, void*>* _attachedPropertyValues = new std::unordered_map<XamlAttachedProperty*, void*>();
			public:
				using XamlObject::XamlObject;

				void set_value(XamlAttachedProperty* prop, void* value)
				{
					(*_attachedPropertyValues)[prop] = value;
				}

				void* get_value(XamlAttachedProperty* prop)
				{
					std::unordered_map<XamlAttachedProperty*, void*>::const_iterator found = _attachedPropertyValues->find(prop);
					if (found != _attachedPropertyValues->end())
					{
						return found->second;
					}
					return 0;
				}

			};
		}
	}
}