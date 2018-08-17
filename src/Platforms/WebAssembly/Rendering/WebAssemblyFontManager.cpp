#include "WebAssemblyFontManager.h"

using namespace CoreVar::Xaml::Rendering::WebAssembly;

void __download_font_complete(void* arg, void* data, int dataSize)
{
	LoadFontRequest* request = (LoadFontRequest*)arg;;
	((WebAssemblyFontManager*)request->fontManager)->__load_font_complete(request->url, request->fontName, data, dataSize);
}

void __download_font_error(void* arg)
{
	printf("error downloading font\n");
}
