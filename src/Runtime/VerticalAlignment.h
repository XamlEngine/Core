#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{


			enum class VerticalAlignment
			{
				Stretch,
				Center,
				Top,
				Bottom
			};

			bool parse_VerticalAlignment(char* text, VerticalAlignment** value)
			{
				if (strcmp(text, "Stretch") == 0)
				{
					*value = (VerticalAlignment*)malloc(sizeof(VerticalAlignment));
					**value = VerticalAlignment::Stretch;
					return true;
				}
				if (strcmp(text, "Center") == 0)
				{
					*value = (VerticalAlignment*)malloc(sizeof(VerticalAlignment));
					**value = VerticalAlignment::Center;
					return true;
				}
				if (strcmp(text, "Top") == 0)
				{
					*value = (VerticalAlignment*)malloc(sizeof(VerticalAlignment));
					**value = VerticalAlignment::Top;
					return true;
				}
				if (strcmp(text, "Bottom") == 0)
				{
					*value = (VerticalAlignment*)malloc(sizeof(VerticalAlignment));
					**value = VerticalAlignment::Bottom;
					return true;
				}
				return false;
			}

		}
	}
}