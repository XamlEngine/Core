#pragma once

#include "GridUnitType.h"
#include <limits>
#include <cmath>
#include "../Common/Numeric.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			struct GridLength
			{

				GridUnitType gridUnitType = GridUnitType::Star;
				double value = 1;

			};

			bool Parse_GridLength(char* text, GridLength* value)
			{
				int textLength = strlen(text);
				if (textLength <= 0)
				{
					return false;
				}
				GridLength result = GridLength();
				if (strcmp(text, "Auto") == 0)
				{
					result.gridUnitType = GridUnitType::Auto;
				}
				else
				{
					if (text[textLength - 1] == '*')
					{
						result.gridUnitType = GridUnitType::Star;
						if (textLength > 1)
						{
							int intValueLength = textLength - 1;
							char* intValue = (char*)malloc(textLength);
							memcpy(intValue, text, intValueLength);
							intValue[intValueLength] = 0;
							result.value = atoi(intValue);
							delete intValue;
						}
					}
					else
					{
						result.gridUnitType = GridUnitType::Pixel;
						result.value = atof(text);
					}
				}
				*value = result;
				return true;
			}

		}
	}
}