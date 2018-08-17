#pragma once

#include "UIElement.h"
#include "../Runtime/Size.h"
#include "../Common/Numeric.h"
#include "../Runtime/HorizontalAlignment.h"
#include "../Runtime/VerticalAlignment.h"

using namespace CoreVar::Xaml::Runtime;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class FrameworkElement : public UIElement
			{
			private:
				double _width = Double_NaN;
				double _height = Double_NaN;
				HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Stretch;
				VerticalAlignment _verticalAlignment = VerticalAlignment::Stretch;

			protected:
				using UIElement::UIElement;

				virtual Size measureOverride(Size availableSize)
				{
					Size result = Size();
					if (!isnan(_width) && !isinf(_width))
					{
						result.width = _width;
					}
					else
					{
						switch (_horizontalAlignment)
						{
						case HorizontalAlignment::Stretch:
							result.width = isinf(availableSize.width) ? 0 : availableSize.width;
							break;
						default:
							result.width = 0;
							break;
						}
					}
					if (!isnan(_height) && !isinf(_height))
					{
						result.height = _height;
					}
					else
					{
						switch (_verticalAlignment)
						{
						case VerticalAlignment::Stretch:
							result.height = isinf(availableSize.height) ? 0 : availableSize.height;
							break;
						default:
							result.height = 0;
						}
					}
					return result;
				}

				virtual Size arrangeOverride(Size finalSize)
				{
					return finalSize;
				}

				virtual Size measureCore(Size availableSize)
				{
					return measureOverride(availableSize);
				}

				virtual void arrangeCore(Rect finalRect)
				{
					Size size;
					size.width = finalRect.width;
					size.height = finalRect.height;
					arrangeOverride(size);
				}

			public:
				double get_width()
				{
					return _width;
				}

				double* get_width_pointer()
				{
					return &_width;
				}

				void set_width(double value)
				{
					_width = value;
					invalidateVisual();
				}

				double get_height()
				{
					return _height;
				}

				double* get_height_pointer()
				{
					return &_height;
				}

				void set_height(double value)
				{
					_height = value;
					invalidateVisual();
				}

				HorizontalAlignment get_horizontalAlignment()
				{
					return _horizontalAlignment;
				}

				HorizontalAlignment* get_horizontalAlignment_pointer()
				{
					return &_horizontalAlignment;
				}

				void set_horizontalAlignment(HorizontalAlignment value)
				{
					_horizontalAlignment = value;
					invalidateVisual();
				}

				VerticalAlignment get_verticalAlignment()
				{
					return _verticalAlignment;
				}

				VerticalAlignment* get_verticalAlignment_pointer()
				{
					return &_verticalAlignment;
				}

				void set_verticalAlignment(VerticalAlignment value)
				{
					_verticalAlignment = value;
					invalidateVisual();
				}

			};

		}
	}
}