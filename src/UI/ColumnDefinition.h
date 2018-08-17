#pragma once

#include "../Runtime/XamlRuntimeObject.h"
#include "../Runtime/GridLength.h"
#include <vector>

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class ColumnDefinition : public DefinitionBase
			{
			private:
				XamlServices * _services;
				GridLength _width = GridLength();

			public:
				ColumnDefinition(XamlType* type, XamlServices* services) : DefinitionBase(type, services)
				{
					
				}

				GridLength get_width()
				{
					return _width;
				}

				void set_width(GridLength value)
				{
					_width = value;
				}

				GridLength* get_width_pointer()
				{
					return &_width;
				}

			};

		}
	}
}