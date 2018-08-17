#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			struct Thickness
			{
				double top = 0;
				double left = 0;
				double bottom = 0;
				double right = 0;
			};

			double Parse_ThicknessSectionDouble(char* text, int sectionStart, int sectionEnd)
			{
				int sectionLength = sectionEnd - sectionStart + 1;
				char* parseText = (char*)malloc(sectionLength + 1);
				memcpy(parseText, text + sectionStart, sectionLength);
				parseText[sectionLength] = 0;
				return atof(parseText);
				delete parseText;
			}


			bool Parse_Thickness(char* text, Thickness* value)
			{
				double parsedValues[4] = { 0, 0, 0, 0 };
				int sectionIndex = 0;
				int currentSectionStart = -1;
				bool inCurrentSection = false;
				bool finishedCurrentSection = false;
				int i = -1;
				while (text[++i] != 0)
				{
					switch (text[i])
					{
					case ' ':
						if (inCurrentSection)
						{
							finishedCurrentSection = true;
							parsedValues[sectionIndex] = Parse_ThicknessSectionDouble(text, currentSectionStart, i - 1);
						}
						break;
					case ',':
						if (!finishedCurrentSection)
						{
							if (!inCurrentSection)
							{
								return false;
							}
							parsedValues[sectionIndex] = Parse_ThicknessSectionDouble(text, currentSectionStart, i - 1);
						}
						sectionIndex += 1;
						currentSectionStart = -1;
						inCurrentSection = false;
						finishedCurrentSection = false;
						break;
					case '.':
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						if (!inCurrentSection)
						{
							inCurrentSection = true;
							currentSectionStart = i;
						}
						break;
					}
				}
				if (!finishedCurrentSection)
				{
					if (!inCurrentSection)
					{
						return false;
					}
					parsedValues[sectionIndex] = Parse_ThicknessSectionDouble(text, currentSectionStart, i - 1);
				}

				if (sectionIndex == 0)
				{
					value->left = parsedValues[0];
					value->right = parsedValues[0];
					value->top = parsedValues[0];
					value->bottom = parsedValues[0];
				}
				else if (sectionIndex == 1)
				{
					value->left = parsedValues[0];
					value->right = parsedValues[0];
					value->top = parsedValues[1];
					value->bottom = parsedValues[1];
				}
				else if (sectionIndex == 3)
				{
					value->left = parsedValues[0];
					value->top = parsedValues[1];
					value->right = parsedValues[2];
					value->bottom = parsedValues[3];
				}
				else
				{
					return false;
				}

				return true;
			}
		}
	}
}