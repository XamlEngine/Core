#include <SDL\SDL_surface.h>

namespace CoreVar
{
	namespace Xaml
	{
		namespace IO
		{

			struct LoadImageOperation
			{
				const char* relativePath;
				void* arg;
				void (*imageLoadCallback)(void*, SDL_Surface*);
				void (*errorCallback)(void*);
				char* data = 0;
				int dataSize = -1;
			};

		}
	}
}