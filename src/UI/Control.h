#pragma once

#include "FrameworkElement.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class Control : public FrameworkElement
			{
			protected:
				using FrameworkElement::FrameworkElement;
			};

		}
	}
}