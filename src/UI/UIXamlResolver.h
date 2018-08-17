#pragma once

#include "../Runtime/XamlResolver.h"
#include "Canvas.h"
#include "StackPanel.h"
#include "TextBlock.h"
#include "Rectangle.h"
#include "Grid.h"
#include "../Runtime/Types.h"
#include "Drawing/Color.h"
#include "SolidColorBrush.h"
#include "../Rendering/XamlServices.h"
#include "Image.h"
#include "ImageSource.h"
#include "UserControl.h"

using namespace CoreVar::Xaml::Runtime;
using namespace CoreVar::Xaml::Rendering;
using namespace CoreVar::Xaml::UI::Drawing;

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class UIXamlResolver : public XamlResolver
			{

			protected:

				XamlTypeDictionary* onGetTypeCreators()
				{
					return new XamlTypeDictionary(
						{
							Declare_XamlUIElementType(Canvas, XAMLUI_CANVAS)
								Define_XamlParentType(CoreVar.Xaml.UI.Panel)
								Begin_XamlType
									Begin_XamlAttachedProperties
										New_XamlRuntimeObjectAttachedValueProperty(Left, System.Double, CoreVar.Xaml.UI.UIElement),
										New_XamlRuntimeObjectAttachedValueProperty(Top, System.Double, CoreVar.Xaml.UI.UIElement)
									End_XamlAttachedProperties
								End_XamlType,

							Declare_XamlUIElementType(ContentControl, XAMLUI_CONTENTCONTROL)
								Define_XamlParentType(CoreVar.Xaml.UI.Control)
								Create_XamlContentProperty(Content)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectReferenceProperty(Content, System.Object, XamlObject, get_content, set_content)
									End_XamlProperties
								End_XamlType,

							Declare_XamlAbstractType(Control, XAMLUI_CONTROL)
								Define_XamlParentType(CoreVar.Xaml.UI.FrameworkElement)
								Begin_XamlType
								End_XamlType,

							Declare_XamlAbstractType(FrameworkElement, XAMLUI_FRAMEWORKELEMENT)
								Define_XamlParentType(CoreVar.Xaml.UI.UIElement)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectValueProperty(Width, System.Double, double, get_width_pointer, set_width),
										New_XamlObjectValueProperty(Height, System.Double, double, get_height_pointer, set_height),
										New_XamlObjectValueProperty(VerticalAlignment, System.VerticalAlignment, VerticalAlignment, get_verticalAlignment_pointer, set_verticalAlignment),
										New_XamlObjectValueProperty(HorizontalAlignment, System.HorizontalAlignment, HorizontalAlignment, get_horizontalAlignment_pointer, set_horizontalAlignment)
									End_XamlProperties
								End_XamlType,

							Declare_XamlUIElementType(Grid, XAMLUI_GRID)
								Define_XamlParentType(CoreVar.Xaml.UI.Panel)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlReadOnlyObjectReferenceProperty(RowDefinitions, CoreVar.Xaml.UI.RowDefinitionCollection, RowDefinitionCollection, get_rowDefinitions),
										New_XamlReadOnlyObjectReferenceProperty(ColumnDefinitions, CoreVar.Xaml.UI.ColumnDefinitionCollection, ColumnDefinitionCollection, get_columnDefinitions)
									End_XamlProperties
									Begin_XamlAttachedProperties
										New_XamlRuntimeObjectAttachedValueProperty(Row, System.Int32, CoreVar.Xaml.UI.UIElement),
										New_XamlRuntimeObjectAttachedValueProperty(RowSpan, System.Int32, CoreVar.Xaml.UI.UIElement),
										New_XamlRuntimeObjectAttachedValueProperty(Column, System.Int32, CoreVar.Xaml.UI.UIElement),
										New_XamlRuntimeObjectAttachedValueProperty(ColumnSpan, System.Int32, CoreVar.Xaml.UI.UIElement)
									End_XamlAttachedProperties
								End_XamlType,

							Declare_XamlAbstractType(Panel, XAMLUI_PANEL)
								Define_XamlParentType(CoreVar.Xaml.UI.FrameworkElement)
								Create_XamlContentProperty(Children)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlReadOnlyObjectReferenceProperty(Children, CoreVar.Xaml.UI.UIElementCollection, UIElementCollection, get_children)
									End_XamlProperties
								End_XamlType,

							Declare_XamlUIElementType(Rectangle, XAMLUI_RECTANGLE)
								Define_XamlParentType(CoreVar.Xaml.UI.Shape)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectReferenceProperty(Fill, CoreVar.Xaml.UI.Brush, Brush, get_fill, set_fill),
										New_XamlObjectValueProperty(CornerRadius, System.CornerRadius, CornerRadius, get_cornerRadius_pointer, set_cornerRadius)
									End_XamlProperties
								End_XamlType,

							Declare_XamlAbstractType(Shape, XAMLUI_SHAPE)
								Define_XamlParentType(CoreVar.Xaml.UI.FrameworkElement)
								Begin_XamlType
								End_XamlType,

							Declare_XamlUIElementType(StackPanel, XAMLUI_STACKPANEL)
								Define_XamlParentType(CoreVar.Xaml.UI.Panel)
								Begin_XamlType
								End_XamlType,

							Declare_XamlUIElementType(TextBlock, XAMLUI_TEXTBLOCK)
								Define_XamlParentType(CoreVar.Xaml.UI.FrameworkElement)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectReferenceProperty(Text, System.String, std::string, get_text, set_text),
										New_XamlObjectValueProperty(FontSize, System.Double, double, get_fontSize_pointer, set_fontSize),
										New_XamlObjectReferenceProperty(Foreground, CoreVar.Xaml.UI.Brush, Brush, get_foreground, set_foreground),
										New_XamlObjectValueProperty(TextAlignment, CoreVar.Xaml.UI.TextAlignment, TextAlignment, get_textAlignment_pointer, set_textAlignment),
										New_XamlObjectValueProperty(TextWrapping, System.TextWrapping, TextWrapping, get_textWrapping_pointer, set_textWrapping)
									End_XamlProperties
								End_XamlType,

							Declare_XamlAbstractType(UIElement, XAMLUI_UIELEMENT)
								Define_XamlParentType(System.XamlRuntimeObject)
								Begin_XamlType
								End_XamlType,

							Declare_XamlUIElementType(UserControl, XAMLUI_USERCONTROL)
								Define_XamlParentType(CoreVar.Xaml.UI.ContentControl)
								Begin_XamlType
								End_XamlType,

							Declare_XamlUIElementType(Image, XAMLUI_IMAGE)
								Define_XamlParentType(CoreVar.Xaml.UI.FrameworkElement)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectReferenceProperty(Source, CoreVar.Xaml.UI.Imaging.BitmapSource, BitmapSource, get_source, set_source)
									End_XamlProperties
								End_XamlType,

							Declare_XamlAbstractType(Brush, XAMLUI_BRUSH)
								Define_XamlParentType(System.Object)
								Create_XamlObjectParseFunction(
									{
										SolidColorBrush* result;
										if (Parse_SolidColorBrush(((XamlServices*)this->get_services())->typeResolver->get_type(new std::string("CoreVar.Xaml.UI.SolidColorBrush")), value, &result))
										{
											return (XamlObject*)result;
										}
										return 0;
									})
								Begin_XamlType
								End_XamlType,

							Declare_XamlObjectType(SolidColorBrush, XAMLUI_SOLIDCOLORBRUSH)
								Define_XamlParentType(CoreVar.Xaml.UI.Brush)
								Create_XamlObjectParseFunction(
									{
										SolidColorBrush* result;
										if (Parse_SolidColorBrush(typeRef, value, &result))
										{
											return (XamlObject*)result;
										}
										return 0;
									})
								Begin_XamlType
								End_XamlType,

							Declare_XamlAbstractType(BitmapSource, XAMLUI_BITMAPSOURCE)
								Define_XamlParentType(System.Object)
								Create_XamlObjectParseFunction(
									{
										XamlType* imageSourceType = ((XamlServices*)this->get_services())->typeResolver->get_type(new std::string("CoreVar.Xaml.UI.ImageSource"));
										ImageSource* result;
										if (Parse_ImageSource(imageSourceType, value, &result))
										{
											return (XamlObject*)result;
										}
										return 0;
									})
								Begin_XamlType
								End_XamlType,

							Declare_XamlObjectType(ImageSource, XAMLUI_IMAGESOURCE)
								Define_XamlParentType(CoreVar.Xaml.UI.Imaging.BitmapSource)
								Create_XamlObjectParseFunction(
									{
										ImageSource* result;
										if (Parse_ImageSource(typeRef, value, &result))
										{
											return (XamlObject*)result;
										}
										return 0;
									})
								Begin_XamlType
								End_XamlType,

							Declare_XamlPrimitiveType(RowDefinitionCollection, CoreVar.Xaml.UI.RowDefinitionCollection)
								Define_XamlParentType(CoreVar.Xaml.UI.DefinitionCollectionBase)
								Begin_XamlType
								End_XamlType,

							Declare_XamlPrimitiveType(ColumnDefinitionCollection, CoreVar.Xaml.UI.ColumnDefinitionCollection)
								Define_XamlParentType(CoreVar.Xaml.UI.DefinitionCollectionBase)
								Begin_XamlType
								End_XamlType,

							Declare_XamlAbstractType(DefinitionBase, CoreVar.Xaml.UI.DefinitionBase)
								Define_XamlParentType(System.FrameworkContentElement)
								Begin_XamlType
								End_XamlType,

							Declare_XamlUIElementType(RowDefinition, CoreVar.Xaml.UI.RowDefinition)
								Define_XamlParentType(CoreVar.Xaml.UI.DefinitionBase)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectValueProperty(Height, System.GridLength, GridLength, get_height_pointer, set_height)
									End_XamlProperties
								End_XamlType,

							Declare_XamlUIElementType(ColumnDefinition, CoreVar.Xaml.UI.ColumnDefinition)
								Define_XamlParentType(CoreVar.Xaml.UI.DefinitionBase)
								Begin_XamlType
									Begin_XamlProperties
										New_XamlObjectValueProperty(Width, System.GridLength, GridLength, get_width_pointer, set_width)
									End_XamlProperties
								End_XamlType,

							Declare_XamlPrimitiveType(DefinitionCollectionBase, CoreVar.Xaml.UI.DefinitionCollectionBase)
								Define_XamlParentType(System.List)
								Begin_XamlType
								End_XamlType,

							Declare_XamlPrimitiveType(UIElementCollection, CoreVar.Xaml.UI.UIElementCollection)
								Define_XamlParentType(System.List)
							Begin_XamlType
							End_XamlType,

							Declare_XamlPrimitiveType(TextAlignment, CoreVar.Xaml.UI.TextAlignment)
								Create_XamlObjectParseFunction({
									TextAlignment* parsedValue;
									if (!parse_TextAlignment(value, &parsedValue))
									{
										parsedValue = (TextAlignment*)malloc(sizeof(TextAlignment));
										*parsedValue = TextAlignment::Left;
									}
									return parsedValue;
								})
							Begin_XamlType
							End_XamlType
						});
				}

				public:
					UIXamlResolver()
					{

					}

			};
		}
	}
}