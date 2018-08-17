#pragma once

#include "Panel.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class StackPanel : public Panel
			{
			public:
				StackPanel(XamlType* type, XamlServices* services) : Panel(type, services)
				{

				}
			};

		}
	}
}