#pragma once

#include "Types.h"
#include "XamlResolver.h"
#include "XamlType.h"
#include <unordered_map>
#include "GridLength.h"
#include "CornerRadius.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			class SystemXamlResolver : public XamlResolver
			{
				
				XamlTypeDictionary* onGetTypeCreators()
				{
					return new XamlTypeDictionary({
						Declare_XamlPrimitiveType(String, SYSTEM_STRING)
							Create_XamlObjectParseFunction(
							{
								return new std::string(value);
							})
							Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(Double, SYSTEM_DOUBLE)
							Create_XamlObjectParseFunction(
							{
								double* parsedValue = (double*)malloc(sizeof(double));
								*parsedValue = atof(value);
								return parsedValue;
							})
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(Int32, System.Int32)
							Create_XamlObjectParseFunction(
							{
								int* parsedValue = (int*)malloc(sizeof(int));
								*parsedValue = atoi(value);
								return parsedValue;
							})
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(VerticalAlignment, System.VerticalAlignment)
							Create_XamlObjectParseFunction(
							{
								VerticalAlignment* parsedValue;
								if (!parse_VerticalAlignment(value, &parsedValue))
								{
									parsedValue = (VerticalAlignment*)malloc(sizeof(VerticalAlignment));
									*parsedValue = VerticalAlignment::Stretch;
								}
								return parsedValue;
							})
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(HorizontalAlignment, System.HorizontalAlignment)
							Create_XamlObjectParseFunction({
								HorizontalAlignment* parsedValue;
								if (!parse_HorizontalAlignment(value, &parsedValue))
								{
									parsedValue = (HorizontalAlignment*)malloc(sizeof(HorizontalAlignment));
									*parsedValue = HorizontalAlignment::Stretch;
								}
								return parsedValue;
							})
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(TextWrapping, System.TextWrapping)
							Create_XamlObjectParseFunction({
								TextWrapping* parsedValue;
								if (!parse_TextWrapping(value, &parsedValue))
								{
									parsedValue = (TextWrapping*)malloc(sizeof(TextWrapping));
									*parsedValue = TextWrapping::None;
								}
								return parsedValue;
							})
						Begin_XamlType
						End_XamlType,

						Declare_XamlAbstractType(XamlRuntimeObject, System.XamlRuntimeObject)
							Define_XamlParentType(System.Object)
						Begin_XamlType
						End_XamlType,

						Declare_XamlAbstractType(ContentElement, System.ContentElement)
							Define_XamlParentType(System.XamlRuntimeObject)
						Begin_XamlType
						End_XamlType,

						Declare_XamlAbstractType(FrameworkContentElement, System.FrameworkContentElement)
							Define_XamlParentType(System.ContentElement)
						Begin_XamlType
						End_XamlType,

						Declare_XamlCustomAbstractType(Object, XamlObject, System.Object)
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(IList, System.IList)
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(List, System.List)
						Begin_XamlType
							Begin_XamlImplementedInterfaces
								Define_XamlImplementedInterface(System.IList)
							End_XamlImplementedInterfaces
						End_XamlType,

						Declare_XamlPrimitiveType(GridLength, System.GridLength)
							Create_XamlObjectParseFunction(
								{
									GridLength* result = new GridLength();
									Parse_GridLength((char*)value, result);
									return result;
								})
						Begin_XamlType
						End_XamlType,

						Declare_XamlPrimitiveType(CornerRadius, System.CornerRadius)
							Create_XamlObjectParseFunction(
								{
									CornerRadius* result = new CornerRadius();
									Parse_CornerRadius((char*)value, result);
									return result;
								})
						Begin_XamlType
						End_XamlType
					});
				}

			public:
				SystemXamlResolver()
				{

				}

			};

		}
	}
}