#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			enum class HorizontalAlignment
			{
				Stretch,
				Center,
				Left,
				Right
			};

			bool parse_HorizontalAlignment(char* text, HorizontalAlignment** value)
			{
				if (strcmp(text, "Stretch") == 0)
				{
					*value = (HorizontalAlignment*)malloc(sizeof(HorizontalAlignment));
					**value = HorizontalAlignment::Stretch;
					return true;
				}
				if (strcmp(text, "Center") == 0)
				{
					*value = (HorizontalAlignment*)malloc(sizeof(HorizontalAlignment));
					**value = HorizontalAlignment::Center;
					return true;
				}
				if (strcmp(text, "Left") == 0)
				{
					*value = (HorizontalAlignment*)malloc(sizeof(HorizontalAlignment));
					**value = HorizontalAlignment::Left;
					return true;
				}
				if (strcmp(text, "Right") == 0)
				{
					*value = (HorizontalAlignment*)malloc(sizeof(HorizontalAlignment));
					**value = HorizontalAlignment::Right;
					return true;
				}
				return false;
			}

		}
	}
}
