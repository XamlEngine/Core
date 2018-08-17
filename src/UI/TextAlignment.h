#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{
			enum class TextAlignment
			{
				Left,
				Center,
				Right,
				Justify
			};

			bool parse_TextAlignment(char* text, TextAlignment** value)
			{
				if (strcmp(text, "Left") == 0)
				{
					*value = (TextAlignment*)malloc(sizeof(TextAlignment));
					**value = TextAlignment::Left;
					return true;
				}
				if (strcmp(text, "Center") == 0)
				{
					*value = (TextAlignment*)malloc(sizeof(TextAlignment));
					**value = TextAlignment::Center;

					return true;
				}
				if (strcmp(text, "Right") == 0)
				{
					*value = (TextAlignment*)malloc(sizeof(TextAlignment));
					**value = TextAlignment::Right;
					return true;
				}
				if (strcmp(text, "Justify") == 0)
				{
					*value = (TextAlignment*)malloc(sizeof(TextAlignment));
					**value = TextAlignment::Justify;
					return true;
				}
				return false;
			}
		}
	}
}