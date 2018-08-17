
#include "../../../IO/FileManagement.h"
#include "LoadImageOperation.h"
#include <emscripten/emscripten.h>
#include <SDL/SDL_surface.h>
#include <SDL/SDL_image.h>
#include <cstring>
#include <cstdlib>

using namespace CoreVar::Xaml::IO;

void __load_image_run_plugins_complete_callback(void* arg, const char* filename)
{
	LoadImageOperation* operation = (LoadImageOperation*)arg;
	SDL_Surface* texture = IMG_Load(filename);
	operation->imageLoadCallback(operation->arg, texture);
}

void __load_image_run_plugins_error_callback(void* arg)
{
	printf("run plugins error\n");
}

void __load_image_file_load_callback(void* arg, void* data, int dataSize)
{
	LoadImageOperation* imageOperation = (LoadImageOperation*)arg;

	const char* relativePath = imageOperation->relativePath;
	int filenameLen = strlen(relativePath);
	int lastPeriodIndex = -1;
	for (int i = filenameLen - 1; i >= 0; i--)
	{
		if (relativePath[i] == '.')
		{
			lastPeriodIndex = i;
			break;
		}
	}
	int suffixLength = filenameLen - lastPeriodIndex;
	char* suffix = (char*)malloc(suffixLength + 1);
	suffix[suffixLength] = 0;
	memcpy(suffix, relativePath + lastPeriodIndex + 1, suffixLength);

	emscripten_run_preload_plugins_data((char*)data, dataSize, (const char*)suffix, (void*)imageOperation, __load_image_run_plugins_complete_callback, __load_image_run_plugins_error_callback);

	delete suffix;
}

void __load_image_file_error_callback(void* arg)
{
	printf("load file error\n");
}

void load_image_file(const char* relativePath, void* arg, void (*imageLoadCallback)(void*, SDL_Surface*), void (*errorCallback)(void*))
{
	LoadImageOperation* operation = new LoadImageOperation();
	operation->relativePath = relativePath;
	operation->arg = arg;
	operation->imageLoadCallback = imageLoadCallback;
	operation->errorCallback = errorCallback;

	emscripten_async_wget_data(relativePath, (void*)operation, __load_image_file_load_callback, __load_image_file_error_callback);
}