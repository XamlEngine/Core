#pragma once

#include <limits>
#include <cmath>


namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{
			namespace Internals
			{

				template <class T>
				struct DefinitionState
				{
					T definition;
					double size = std::numeric_limits<double>::quiet_NaN();
				};

			}
		}
	}
}