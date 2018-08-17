#pragma once

#include <unordered_set>

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			class XamlType
			{
				std::string* _name;
				std::string* _fullname;
				std::function<void*(XamlType*)>* _instanceCreator;
				std::function<void*(XamlType*, char*)>* _attributeParser;
				std::function<void*(XamlType*, char*)>* _contentParser;
				
			public:

				XamlType(std::string* name, std::string* fullname, std::function<void*(XamlType*)>* instanceCreator = 0, std::function<void*(XamlType*, char*)>* attributeParser = 0, std::function<void*(XamlType*, char*)>* contentParser = 0)
				{
					_name = name;
					_fullname = fullname;
					_instanceCreator = instanceCreator;
					_attributeParser = attributeParser;
					_contentParser = contentParser;
				}

				bool get_attributeParseable()
				{
					return _attributeParser != 0;
				}

				bool get_contentParseable()
				{
					return _contentParser != 0;
				}

				std::string* get_name()
				{
					return _name;
				}

				std::string* get_fullname()
				{
					return _fullname;
				}

				void* createInstance()
				{
					if (_instanceCreator == 0)
					{
						return 0;
					}
					return (*_instanceCreator)(this);
				}

				void* parseAttributeText(char* text)
				{
					if (_attributeParser == 0)
					{
						return 0;
					}
					return (*_attributeParser)(this, text);
				}

				void* parseContentText(char* text)
				{
					if (_contentParser == 0)
					{
						return 0;
					}
					return (*_contentParser)(this, text);
				}

			};

		}
	}
}