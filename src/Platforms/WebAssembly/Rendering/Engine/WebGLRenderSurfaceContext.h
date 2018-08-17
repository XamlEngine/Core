#import "../../../../Rendering/Engine/RenderSurfaceContext.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Engine
			{
				namespace WebAssembly
				{

					class WebGLRenderSurfaceContext : public RenderSurfaceContext
					{
					public:
						SDL_Surface * screen;
					};

				}
			}
		}
	}
}