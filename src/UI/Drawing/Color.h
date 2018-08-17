#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{
			namespace Drawing
			{

				struct Color
				{
					uint8_t alpha;
					uint8_t red;
					uint8_t green;
					uint8_t blue;
				};

				bool charHexValue(char hexChar, uint8_t* value)
				{
					switch (hexChar)
					{
					case '0':
						*value = 0;
						break;
					case '1':
						*value = 1;
						break;
					case '2':
						*value = 2;
						break;
					case '3':
						*value = 3;
						break;
					case '4':
						*value = 4;
						break;
					case '5':
						*value = 5;
						break;
					case '6':
						*value = 6;
						break;
					case '7':
						*value = 7;
						break;
					case '8':
						*value = 8;
						break;
					case '9':
						*value = 9;
						break;
					case 'A':
					case 'a':
						*value = 10;
						break;
					case 'B':
					case 'b':
						*value = 11;
						break;
					case 'C':
					case 'c':
						*value = 12;
						break;
					case 'D':
					case 'd':
						*value = 13;
						break;
					case 'E':
					case 'e':
						*value = 14;
						break;
					case 'F':
					case 'f':
						*value = 15;
						break;
					default:
						return false;
					}
					return true;
				}

				bool colorFromHexString(char* text, Color* color)
				{
					if (text[0] != '#')
					{
						return false;
					}
					Color result;
					for (int i = 1; i < 9; i += 2)
					{
						char hexChar1 = text[i];
						if (hexChar1 == 0)
						{
							return false;
						}
						char hexChar2 = text[i + 1];
						if (hexChar2 == 0)
						{
							return false;
						}
						uint8_t charHexValue1;
						if (!charHexValue(hexChar1, &charHexValue1))
						{
							return false;
						}
						uint8_t charHexValue2;
						if (!charHexValue(hexChar2, &charHexValue2))
						{
							return false;
						}
						
						uint8_t colorValue = charHexValue1 * 16 + charHexValue2;
						switch (i)
						{
						case 1:
							result.alpha = colorValue;
							break;
						case 3:
							result.red = colorValue;
							break;
						case 5:
							result.green = colorValue;
							break;
						case 7:
							result.blue = colorValue;
							break;
						}
					}
					if (text[9] != 0)
					{
						return false;
					}
					*color = result;
					printf("a=%d r=%d g=%d b=%d\n", result.alpha, result.red, result.green, result.blue);
					return true;
				}

				bool colorFromName(char* text, Color* color)
				{
					return false;
				}


			}
		}
	}
}