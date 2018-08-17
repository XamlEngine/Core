#include <cstdio>
#include "main.h"

using namespace CoreVar::Xaml;
using namespace CoreVar::Xaml::OpenGL;

void main(int c_size, char** c_args)
{
	XamlWindow* window = new OpenGLXamlWindow();

	window->show();

	delete window;
}