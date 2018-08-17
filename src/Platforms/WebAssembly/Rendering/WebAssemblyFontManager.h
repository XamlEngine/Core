#pragma once

#include "../../../Rendering/FontManager.h"
#include "WebAssemblyFontFamily.h"

#include <unordered_map>

#include <emscripten.h>
#include <functional>


#include <ft2build.h>
#include FT_FREETYPE_H

void __download_font_complete(void* arg, void* data, int dataSize);

void __download_font_error(void* arg);

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace WebAssembly
			{

				struct LoadFontRequest
				{
					void* fontManager;
					std::string* fontName;
					std::string* url;
				};

				class WebAssemblyFontManager : public FontManager
				{
					FT_Library _library;
					std::unordered_map<std::string, FontFamily*> _fonts = std::unordered_map<std::string, FontFamily*>();

				public:

					void initialize()
					{
						int error = FT_Init_FreeType(&_library);
						if (error)
						{
							// TODO: Handle this error appropriately
							printf("error initializing freetype\n");
						}
					}

					void __load_font_complete(std::string* url, std::string* name, void* data, int dataSize)
					{
						WebAssemblyFontFamily* font = new WebAssemblyFontFamily();
						
						int error = FT_New_Memory_Face(_library, (const FT_Byte*)data, (FT_Long)dataSize, (FT_Long)0, &font->face);

						if (error)
						{
							// TODO: Handle this error appropriately
							printf("Error loading font face\n");
							return;
						}
						_fonts.insert({ *name, font });

						(*invalidated)();
					}

					void begin_load_font(std::string* url, std::string* name)
					{
						LoadFontRequest* request = new LoadFontRequest();
						request->fontManager = this;
						request->fontName = name;
						request->url = url;

						emscripten_async_wget_data(url->c_str(), request, __download_font_complete, __download_font_error);
					}

					FontFamily* get_font(std::string* name)
					{
						std::unordered_map<std::string, FontFamily*>::const_iterator found = _fonts.find(*name);
						if (found != _fonts.end())
						{
							return found->second;
						}
						return NULL;
					}
				};

			}
		}
	}
}