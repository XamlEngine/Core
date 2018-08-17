#pragma once

#include "../Runtime/TypeResolver.h"

using namespace CoreVar::Xaml::Runtime;

#define CommonXamlType(accessor, name) \
XamlType* _##accessor = NULL; \
XamlType* accessor() \
{ \
	if (_##accessor == NULL) \
	{ \
		_##accessor = _typeResolver->get_type(new std::string(#name)); \
	} \
	return _##accessor; \
} \

#define CommonXamlAttachedProperty(type, name) \
XamlAttachedProperty* _##type##_##name = NULL; \
XamlAttachedProperty* type##_##name() \
{ \
	if (_##type##_##name == NULL) \
	{ \
		_##type##_##name = _typeResolver->get_attachedProperty(type(), new std::string(#name)); \
	} \
	return _##type##_##name; \
} \

#define CommonXamlProperty(type, name) \
XamlProperty* _##type##_##name = NULL; \
XamlProperty* type##_##name() \
{ \
	if (_##type##_##name == NULL) \
	{ \
		_##type##_##name = _typeResolver->get_property(type(), new std::string(#name)); \
	} \
	return _##type##_##name; \
} \


namespace CoreVar
{
	namespace Xaml
	{
		namespace Services
		{

			class CommonTypes
			{
				TypeResolver* _typeResolver;

			public:
				CommonTypes(TypeResolver* typeResolver)
				{
					_typeResolver = typeResolver;
				}

				CommonXamlType(Xaml_UI_Grid, CoreVar.Xaml.UI.Grid)

				CommonXamlAttachedProperty(Xaml_UI_Grid, Row)

				CommonXamlAttachedProperty(Xaml_UI_Grid, RowSpan)

				CommonXamlAttachedProperty(Xaml_UI_Grid, Column)

				CommonXamlAttachedProperty(Xaml_UI_Grid, ColumnSpan)

				CommonXamlType(Xaml_UI_RowDefinitionCollection, CoreVar.Xaml.UI.RowDefinitionCollection)

				CommonXamlType(Xaml_UI_ColumnDefinitionCollection, CoreVar.Xaml.UI.ColumnDefinitionCollection)

				CommonXamlType(Xaml_UI_Brush, CoreVar.Xaml.UI.Brush)

				CommonXamlType(Xaml_UI_SolidColorBrush, CoreVar.Xaml.UI.SolidColorBrush)

				CommonXamlType(Xaml_UI_FrameworkElement, CoreVar.Xaml.UI.FrameworkElement)

			};

		}
	}
}