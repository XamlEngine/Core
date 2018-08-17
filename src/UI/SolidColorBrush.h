#pragma once

#include "Brush.h"
#include "Drawing/Color.h"
#include "../Runtime/Types.h"

using namespace CoreVar::Xaml::UI::Drawing;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class SolidColorBrush : public Brush
			{
				Color _color;

			public:
				SolidColorBrush(XamlType* type) : Brush(type)
				{

				}

				Color* get_color()
				{
					return &_color;
				}

				void set_color(Color value)
				{
					_color = value;
				}

			};

			bool Parse_SolidColorBrush(XamlType* solidColorBrushType, char* text, SolidColorBrush** parsedResult)
			{
				Color color;
				if (!colorFromHexString(text, &color) &&
					!colorFromName(text, &color))
				{
					return 0;
				}
				SolidColorBrush* solidColorBrush = new SolidColorBrush(solidColorBrushType);
				solidColorBrush->set_color(color);
				*parsedResult = solidColorBrush;
				return true;
			}


		}
	}
}