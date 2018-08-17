#pragma once

#include "ContentControl.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class UserControl : public ContentControl
			{
			public:
				UserControl(XamlType* type, XamlServices* services) : ContentControl(type, services)
				{

				}
			};

		}
	}
}