#pragma once

#include <string>
#include <functional>
#include <tuple>
#include "BaseTypes.h"

#define SYSTEM_OBJECT System.Object
#define SYSTEM_DOUBLE System.Double
#define SYSTEM_STRING System.String

#define XAMLUI_CANVAS CoreVar.Xaml.UI.Canvas
#define XAMLUI_CONTENTCONTROL CoreVar.Xaml.UI.ContentControl
#define XAMLUI_CONTROL CoreVar.Xaml.UI.Control
#define XAMLUI_FRAMEWORKELEMENT CoreVar.Xaml.UI.FrameworkElement
#define XAMLUI_GRID CoreVar.Xaml.UI.Grid
#define XAMLUI_PANEL CoreVar.Xaml.UI.Panel
#define XAMLUI_RECTANGLE CoreVar.Xaml.UI.Rectangle
#define XAMLUI_SHAPE CoreVar.Xaml.UI.Shape
#define XAMLUI_STACKPANEL CoreVar.Xaml.UI.StackPanel
#define XAMLUI_TEXTBLOCK CoreVar.Xaml.UI.TextBlock
#define XAMLUI_UIELEMENT CoreVar.Xaml.UI.UIElement
#define XAMLUI_USERCONTROL CoreVar.Xaml.UI.UserControl
#define XAMLUI_IMAGE CoreVar.Xaml.UI.Image
#define XAMLUI_BRUSH CoreVar.Xaml.UI.Brush
#define XAMLUI_SOLIDCOLORBRUSH CoreVar.Xaml.UI.SolidColorBrush
#define XAMLUI_BITMAPSOURCE CoreVar.Xaml.UI.Imaging.BitmapSource
#define XAMLUI_IMAGESOURCE CoreVar.Xaml.UI.ImageSource

#define XAMLUIDRAWING_COLOR CoreVar.Xaml.UI.Drawing.Color

#define XAML_TypeName(type) #type


#define Declare_XamlStandardTypeOptions(type, fullName) \
{ std::string(#fullName), [=]() -> XamlType* \
	{ \
		std::function<XamlType*()>* getParentTypeFunction = 0; \
		std::string* typeName = new std::string(#type); \
		std::string* fullTypeName = new std::string(#fullName); \
		std::function<void*(XamlType*, char*)>* attributeParserFunction = NULL; \
		std::function<void*(XamlType*, char*)>* contentParserFunction = NULL; \
		XamlPropertyDictionary* propertyDictionary = NULL; \
		XamlAttachedPropertyDictionary* attachedPropertyDictionary = NULL; \
		std::function<std::unordered_set<XamlType*>*()>* implementedInterfacesSet = NULL; \
		std::string* contentPropertyName = NULL;

#define Declare_XamlUIElementType(type, fullName) \
		Declare_XamlStandardTypeOptions(type, fullName) \
		std::function<type*(XamlObject*)> declaringTypeCast = [=](XamlObject* original) -> type* { return (type*)original; }; \
		std::function<void*(XamlType*)>* constructorFunction = new std::function<void*(XamlType*)>([=](XamlType* typeRef) -> void* \
		{ \
			type* result = new type(typeRef, this->get_services()); \
			return (XamlObject*)result; \
		});

#define Declare_XamlObjectType(type, fullName) \
		Declare_XamlStandardTypeOptions(type, fullName) \
		std::function<type*(XamlObject*)> declaringTypeCast = [=](XamlObject* original) -> type* { return (type*)original; }; \
		std::function<void*(XamlType*)>* constructorFunction = new std::function<void*(XamlType*)>([=](XamlType* typeRef) -> void* \
		{ \
			return (XamlObject*)new type(typeRef); \
		});

#define Declare_XamlPrimitiveType(type, fullName) \
		Declare_XamlStandardTypeOptions(type, fullName) \
		std::function<void*(XamlType*)>* constructorFunction = 0;

#define Declare_XamlAbstractType(type, fullName) \
		Declare_XamlStandardTypeOptions(type, fullName) \
		std::function<type*(XamlObject*)> declaringTypeCast = [=](XamlObject* original) -> type* { return (type*)original; }; \
		std::function<void*(XamlType*)>* constructorFunction = 0;

#define Declare_XamlCustomAbstractType(typeName, customType, fullName) \
		Declare_XamlStandardTypeOptions(typeName, fullName) \
		std::function<customType*(XamlObject*)> declaringTypeCast = [=](XamlObject* original) -> customType* { return (customType*)original; }; \
		std::function<void*(XamlType*)>* constructorFunction = 0;


#define Create_XamlObjectParseFunction(parseFunction) \
		attributeParserFunction = new std::function<void*(XamlType*, char*)>([=](XamlType* typeRef, char* value) -> void* \
		parseFunction); \
		contentParserFunction = new std::function<void*(XamlType*, char*)>([=](XamlType* typeRef, char* value) -> void* \
		parseFunction);

#define Define_XamlParentType(parentName) \
		getParentTypeFunction = new std::function<XamlType*()>([=]() -> XamlType* { \
			return this->get_services()->typeResolver->get_type(new std::string(#parentName)); \
		});

#define Create_XamlContentProperty(name) \
		contentPropertyName = new std::string(#name);

#define Begin_XamlType \
		XamlType* declaringTypeRef = new XamlType(typeName, fullTypeName, constructorFunction, attributeParserFunction, contentParserFunction);

#define End_XamlType \
		if (propertyDictionary != NULL) \
		{ \
			this->propertyCreators()->insert(propertyDictionary->begin(), propertyDictionary->end()); \
		} \
		if (attachedPropertyDictionary != NULL) \
		{ \
			this->attachedPropertyCreators()->insert(attachedPropertyDictionary->begin(), attachedPropertyDictionary->end()); \
		} \
		if (contentPropertyName != NULL) \
		{ \
			this->contentPropertyNames()->insert({ declaringTypeRef, contentPropertyName }); \
		} \
		if (getParentTypeFunction != NULL) \
		{ \
			this->parentTypes()->insert({ declaringTypeRef, getParentTypeFunction }); \
		} \
		if (implementedInterfacesSet != NULL) \
		{ \
			this->implementedInterfaces()->insert({ declaringTypeRef, implementedInterfacesSet }); \
		} \
		return declaringTypeRef; \
	} \
}

#define Begin_XamlProperties \
	propertyDictionary = new XamlPropertyDictionary({

#define End_XamlProperties \
	});

#define Begin_XamlAttachedProperties \
	attachedPropertyDictionary = new XamlAttachedPropertyDictionary({

#define End_XamlAttachedProperties \
	});

#define Begin_XamlImplementedInterfaces \
	implementedInterfacesSet = new std::function<std::unordered_set<XamlType*>*()>([=]() -> std::unordered_set<XamlType*>* \
	{ \
		return new std::unordered_set<XamlType*>({

#define End_XamlImplementedInterfaces \
		}); \
	});

#define New_XamlObjectValueProperty(name, propertyTypeName, actualType, getter, setter) \
{ std::make_tuple(declaringTypeRef, std::string(#name)), [=]() -> XamlProperty* \
	{ \
		XamlType* propertyType = ((XamlServices*)this->get_services())->typeResolver->get_type(new std::string(#propertyTypeName)); \
		return new XamlProperty(declaringTypeRef, (char*)#name, propertyType, \
		new std::function<void*(XamlObject*)>([=](XamlObject* object) -> void* { \
			return (void*)(declaringTypeCast(object)->getter()); \
		}), \
		new std::function<void(XamlObject*, void*)>([=](XamlObject* object, void* value) -> void { \
			declaringTypeCast(object)->setter(*(actualType*)value); \
		})); \
	} \
}

#define New_XamlObjectReferenceProperty(name, propertyTypeName, actualType, getter, setter) \
{ std::make_tuple(declaringTypeRef, std::string(#name)), [=]() -> XamlProperty* \
	{ \
		XamlType* propertyType = this->get_services()->typeResolver->get_type(new std::string(#propertyTypeName)); \
		return new XamlProperty(declaringTypeRef, (char*)#name, propertyType, \
		new std::function<void*(XamlObject*)>([=](XamlObject* object) -> void* { \
			return (void*)(declaringTypeCast(object)->getter()); \
		}), \
		new std::function<void(XamlObject*, void*)>([=](XamlObject* object, void* value) -> void { \
			declaringTypeCast(object)->setter((actualType*)(XamlObject*)value); \
		})); \
	} \
}

#define New_XamlReadOnlyObjectReferenceProperty(name, propertyTypeName, actualType, getter) \
{ std::make_tuple(declaringTypeRef, std::string(#name)), [=]() -> XamlProperty* \
	{ \
		XamlType* propertyType = this->get_services()->typeResolver->get_type(new std::string(#propertyTypeName)); \
		return new XamlProperty(declaringTypeRef, (char*)#name, propertyType, \
		new std::function<void*(XamlObject*)>([=](XamlObject* object) -> void* { \
			return (void*)(declaringTypeCast(object)->getter()); \
		})); \
	} \
}

#define New_XamlRuntimeObjectAttachedValueProperty(name, propertyTypeName, elementTypeName) \
{ std::make_tuple(declaringTypeRef, std::string(#name)), [=]() -> XamlAttachedProperty* \
	{ \
		XamlType* propertyType = this->get_services()->typeResolver->get_type(new std::string(#propertyTypeName)); \
		XamlType* elementType = this->get_services()->typeResolver->get_type(new std::string(#elementTypeName)); \
		return new XamlAttachedProperty(declaringTypeRef, (char*)#name, elementType, propertyType); \
	} \
}

#define New_XamlRuntimeObjectAttachedReferenceProperty(name, propertyTypeName, elementTypeName) \
{ std::make_tuple(declaringTypeRef, std::string(#name)), [=]() -> XamlAttachedProperty* \
	{ \
		XamlType* propertyType = engineResolver(new std::string(#propertyTypeName)); \
		XamlType* elementType = engineResolver(new std::string(#elementTypeName)); \
		return new XamlAttachedProperty(declaringTypeRef, (char*)#name, elementType, propertyType); \
	} \
}

#define Define_XamlImplementedInterface(fullName) \
	this->get_services()->typeResolver->get_type(new std::string(#fullName))