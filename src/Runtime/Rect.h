#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			struct Rect
			{
				double x;
				double y;
				double width;
				double height;

				double get_left()
				{
					return x;
				}

				double get_right()
				{
					return x + width;
				}

				double get_top()
				{
					return y;
				}

				double get_bottom()
				{
					return y + height;
				}
				
			};

		}
	}
}