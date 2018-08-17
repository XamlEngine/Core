#pragma once

#include "DefinitionBase.h"
#include "../Runtime/GridLength.h"
#include <vector>

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class RowDefinition : public DefinitionBase
			{
			private:
				GridLength _height = GridLength();

			public:
				RowDefinition(XamlType* type, XamlServices* services) : DefinitionBase(type, services)
				{
					
				}

				GridLength get_height()
				{
					return _height;
				}

				void set_height(GridLength value)
				{
					_height = value;
				}

				GridLength* get_height_pointer()
				{
					return &_height;
				}

			};

		}
	}
}