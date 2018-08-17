#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			struct CornerRadius
			{
				double topLeft = 0;
				double topRight = 0;
				double bottomRight = 0;
				double bottomLeft = 0;
			};

			double Parse_CornerRadiusSectionDouble(char* text, int sectionStart, int sectionEnd)
			{
				int sectionLength = sectionEnd - sectionStart + 1;
				char* parseText = (char*)malloc(sectionLength + 1);
				memcpy(parseText, text + sectionStart, sectionLength);
				parseText[sectionLength] = 0;
				return atof(parseText);
				delete parseText;
			}

			bool Parse_CornerRadius(char* text, CornerRadius* value)
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
							parsedValues[sectionIndex] = Parse_CornerRadiusSectionDouble(text, currentSectionStart, i - 1);
						}
						break;
					case ',':
						if (!finishedCurrentSection)
						{
							if (!inCurrentSection)
							{
								return false;
							}
							parsedValues[sectionIndex] = Parse_CornerRadiusSectionDouble(text, currentSectionStart, i - 1);
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
					parsedValues[sectionIndex] = Parse_CornerRadiusSectionDouble(text, currentSectionStart, i - 1);
				}

				if (sectionIndex == 0)
				{
					value->topLeft = parsedValues[0];
					value->topRight = parsedValues[0];
					value->bottomRight = parsedValues[0];
					value->bottomLeft = parsedValues[0];
				}
				else if (sectionIndex == 3)
				{
					value->topLeft = parsedValues[0];
					value->topRight = parsedValues[1];
					value->bottomRight = parsedValues[2];
					value->bottomLeft = parsedValues[3];
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
