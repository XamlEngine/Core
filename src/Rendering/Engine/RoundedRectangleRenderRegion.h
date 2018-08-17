#pragma once

#include "RenderRegion.h"
#include "../../UI/Drawing/Color.h"
#include "../../Runtime/TextWrapping.h"
#include "../../UI/TextAlignment.h"
#include <SDL/SDL_surface.h>

#include "../FontFamily.h"

#include "../../Runtime/Size.h"

using namespace CoreVar::Xaml::UI;
using namespace CoreVar::Xaml::UI::Drawing;
using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Engine
			{

				class RoundedRectangleRenderRegion : public RenderRegion
				{
				
				public:
					double topLeftCornerRadius = 0;
					double topRightCornerRadius = 0;
					double bottomLeftCornerRadius = 0;
					double bottomRightCornerRadius = 0;

					double topStrokeThickness = 0;
					double leftStrokeThickness = 0;
					double bottomStrokeThickness = 0;
					double rightStrokeThickness = 0;

					bool renderFill = false;
					bool renderStroke = false;

					Color* solidColorFill = 0;
					Color* solidColorStroke = 0;

					SDL_Surface* textureSurface = 0;
					SDL_Surface* strokeTextureSurface = 0;

					const char* textToRender = 0;

					FontFamily* font = 0;

					double fontSize = 12;

					TextAlignment textAlignment = TextAlignment::Left;
					TextWrapping textWrapping = TextWrapping::None;

					virtual Size measureText(RenderSurfaceContext* context, Size availableSize) = 0;

				};

			}
		}
	}
}