#pragma once

#include <cstdio>
#include "../../XamlWindow.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace OpenGL
		{

			class OpenGLXamlWindow : public XamlWindow
			{
			protected:
				void onShow()
				{
					printf("Show OpenGL Window\n");
				}
			};

		}
	}
}