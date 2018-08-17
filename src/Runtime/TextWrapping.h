#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			enum class TextWrapping
			{
				None,
				Wrap
			};

			bool parse_TextWrapping(char* text, TextWrapping** value)
			{
				if (strcmp(text, "None") == 0)
				{
					*value = (TextWrapping*)malloc(sizeof(TextWrapping));
					**value = TextWrapping::None;
					return true;
				}
				if (strcmp(text, "Wrap") == 0)
				{
					*value = (TextWrapping*)malloc(sizeof(TextWrapping));
					**value = TextWrapping::Wrap;
					return true;
				}
				return false;
			}
		}
	}
}