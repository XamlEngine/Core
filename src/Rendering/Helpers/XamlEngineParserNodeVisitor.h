#pragma once

#include "../../Parsing/XamlNodeVisitor.h"
#include "../XamlRenderEngine.h"
#include "XamlElementTracker.h"
#include <unordered_map>
#include "../../Runtime/XamlType.h"
#include "../../Runtime/IList.h"

using namespace CoreVar::Xaml::Parsing;
using namespace CoreVar::Xaml::Rendering;
using namespace CoreVar::Xaml::UI;

namespace CoreVar
{
	namespace Xaml
	{
		namespace Rendering
		{
			namespace Helpers
			{
				class XamlEngineParserNodeVisitor : public XamlNodeVisitor
				{
					XamlRenderEngine* _engine;
					std::string* _defaultNamespace;
					XamlElementTracker* _elementTracker = 0;
					std::unordered_map<std::string, std::string>* _namespaces = new std::unordered_map<std::string, std::string>();
					XamlObject* _result = 0;

					std::string* getFullTypeName(std::string* text)
					{
						
						size_t colonIndex = text->find(':', 1);
						if (colonIndex == std::string::npos)
						{
							if (_elementTracker == NULL)
							{
								return new std::string(*_defaultNamespace + "." + *text);
							}
							return new std::string(*_elementTracker->defaultNamespace + "." + *text);
						}
						else
						{
							std::string namespaceKey = text->substr(0, colonIndex - 1);
							std::unordered_map<std::string, std::string>::const_iterator namespaceFound = _namespaces->find(namespaceKey);

							if (namespaceFound == _namespaces->end())
							{
								// TODO: Add meaningful parser error to output
								return 0;
							}

							std::string objectName = text->substr(colonIndex + 1);
							return new std::string(namespaceFound->second + "." + objectName);
						}
					}

					bool tryGetProperty(std::string* elementName, std::string** typeName, std::string** name)
					{
						int startIndex = 0;
						std::size_t colonFound = elementName->find(std::string(":"));
						if (colonFound != std::string::npos)
						{
							startIndex = colonFound;
						}
						std::size_t dotFound = elementName->find(std::string("."), startIndex);
						if (dotFound == std::string::npos)
						{
							return false;
						}
						int elementNameSize = elementName->size();
						int propertyNameSize = elementNameSize - dotFound;
						
						char* propertyNameText = (char*)malloc(propertyNameSize + 1);
						memcpy(propertyNameText, elementName->c_str() + dotFound + 1, propertyNameSize);
						propertyNameText[propertyNameSize] = 0;

						char* typeNameText = (char*)malloc(dotFound + 1);
						memcpy(typeNameText, elementName->c_str(), dotFound);
						typeNameText[dotFound] = 0;

						*typeName = getFullTypeName(new std::string(typeNameText));
						*name = new std::string(propertyNameText);

						return true;
					}

					void visitBeginStartElement(char* text, int begin, int end)
					{
						char* beginStartElementName = getBeginStartElementName(text, begin, end);

						
						std::string* elementName = new std::string(beginStartElementName);
						std::string* propertyNameString;
						std::string* fullTypeName;
						if (tryGetProperty(elementName, &fullTypeName, &propertyNameString))
						{
							XamlType* declaringType = _engine->get_typeResolver()->get_type(fullTypeName);

							if (declaringType == 0)
							{
								// TODO: Handle this failure gracefully

								return;
							}

							if (declaringType == _elementTracker->element->get_type())
							{
								XamlProperty* prop = _engine->get_typeResolver()->get_property(declaringType, propertyNameString);
								if (prop == NULL)
								{
									// TODO: Handle this failure gracefully
									return;
								}

								_elementTracker->currentProperty = prop;
							}
							else
							{
								XamlAttachedProperty* attachedProperty = _engine->get_typeResolver()->get_attachedProperty(declaringType, propertyNameString);

								if (attachedProperty == NULL)
								{
									// TODO: Handle this failure gracefully
									return;
								}

								_elementTracker->currentAttachedProperty = attachedProperty;
							}

							return;
						}
						
						fullTypeName = getFullTypeName(elementName);

						XamlType* elementType = _engine->get_typeResolver()->get_type(fullTypeName);

						if (elementType == 0)
						{
							// TODO: Handle this failure gracefully
							return;
						}

						XamlObject* element = (XamlObject*)elementType->createInstance();

						if (element == 0)
						{
							// TODO: Handle this failure gracefully
							return;
						}

						XamlElementTracker* newElementTracker = new XamlElementTracker();
						newElementTracker->element = element;
						newElementTracker->parent = _elementTracker;
						newElementTracker->defaultNamespace = _defaultNamespace;
						newElementTracker->elementName = beginStartElementName;

						_elementTracker = newElementTracker;
					}

					void visitEndStartElement(char* text, int begin, int end)
					{

					}

					void closeElement(char* text, int begin, int end)
					{
						if (_elementTracker->parent == 0)
						{
							_result = _elementTracker->element;
						}
						if (_elementTracker->parent != 0)
						{
							XamlType* parentElementType = _elementTracker->parent->element->get_type();

							if (_elementTracker->parent->currentProperty != 0)
							{
								XamlProperty* currentProperty = _elementTracker->parent->currentProperty;
								XamlType* listInterfaceType = _engine->get_typeResolver()->get_type(new std::string("System.IList"));

								if (_engine->get_services()->typeResolver->get_implementsInterface(currentProperty->propertyType(), listInterfaceType))
								{
									((IListBase*)currentProperty->get_value(_elementTracker->parent->element))->add_item((XamlObject*)_elementTracker->element);
								}
								else
								{
									currentProperty->set_value(_elementTracker->parent->element, _elementTracker->element);
								}
							}
							else if (_elementTracker->parent->currentAttachedProperty != 0)
							{
								XamlAttachedProperty* currentAttachedProperty = _elementTracker->parent->currentAttachedProperty;
								XamlType* listInterfaceType = _engine->get_typeResolver()->get_type(new std::string("System.IList"));

								if (_engine->get_services()->typeResolver->get_implementsInterface(currentAttachedProperty->propertyType(), listInterfaceType))
								{
									//printf("add item %s to list %s\n", ((XamlObject*)_elementTracker->element)->get_type()->get_name()->c_str(), ((XamlRuntimeObject*)_elementTracker->parent->element)->get_value(currentAttachedProperty))->get_type()->get_name()->c_str());
									((IListBase*)((XamlRuntimeObject*)_elementTracker->parent->element)->get_value(currentAttachedProperty))->add_item((XamlObject*)_elementTracker->element);
								}
								else
								{
									((XamlRuntimeObject*)_elementTracker->parent->element)->set_value(currentAttachedProperty, _elementTracker->element);
								}
							}
							else
							{
								XamlProperty* contentProperty = _engine->get_typeResolver()->get_contentProperty(parentElementType);

								XamlType* listInterfaceType = _engine->get_typeResolver()->get_type(new std::string("System.IList"));
								if (contentProperty != NULL)
								{
									if (_engine->get_services()->typeResolver->get_implementsInterface(contentProperty->propertyType(), listInterfaceType))
									{
										((IListBase*)contentProperty->get_value(_elementTracker->parent->element))->add_item((XamlObject*)_elementTracker->element);
									}
									else
									{
										contentProperty->set_value(_elementTracker->parent->element, _elementTracker->element);
									}
								}
							}
						}
						_elementTracker = _elementTracker->parent;
						_defaultNamespace = _elementTracker->defaultNamespace;
					}

					void visitCloseStartElement(char* text, int begin, int end)
					{
						closeElement(text, begin, end);
					}

					void visitClosingElement(char* text, int begin, int end)
					{
						char* closingElementName = getClosingElementName(text, begin, end);

						std::string* elementName = new std::string(getClosingElementName(text, begin, end));
						std::string* propertyNameString;
						std::string* fullTypeName;
						if (tryGetProperty(elementName, &fullTypeName, &propertyNameString))
						{
							XamlType* declaringType = _engine->get_typeResolver()->get_type(fullTypeName);
							
							if (declaringType == 0)
							{
								// TODO: Handle this failure gracefully
								return;
							}

							if (declaringType == _elementTracker->element->get_type() || _engine->get_typeResolver()->get_isSubtypeOf(_elementTracker->element->get_type(), declaringType))
							{
								XamlProperty* prop = _engine->get_typeResolver()->get_property(declaringType, propertyNameString);
								if (prop != _elementTracker->currentProperty)
								{
									// TODO: Handle this failure gracefully
									return;
								}

								_elementTracker->currentProperty = NULL;
							}
							else
							{
								XamlAttachedProperty* attachedProperty = _engine->get_typeResolver()->get_attachedProperty(declaringType, propertyNameString);

								if (attachedProperty != _elementTracker->currentAttachedProperty)
								{
									// TODO: Handle this failure gracefully
									return;
								}

								_elementTracker->currentAttachedProperty = NULL;
							}

							return;
						}
						else if (strcmp(closingElementName, _elementTracker->elementName) != 0)
						{
							// TODO: Handle this error gracefully
							printf("closing element name '%s' doesn't match starting element name '%s'\n", closingElementName, _elementTracker->elementName);
							return;
						}
						
						closeElement(text, begin, end);
					}

					void visitAttributeName(char* text, int begin, int end)
					{
						std::string* attributeName = new std::string(getAttributeName(text, begin, end));

						int indexOfColon = attributeName->find(':', 1);
						int typeNameStart = 0;
						if (indexOfColon > 0)
						{
							typeNameStart = indexOfColon + 1;
						}
						int indexOfPeriod = attributeName->find('.', typeNameStart + 1);
						if (indexOfPeriod > 0)
						{
							std::string abbreviatedName = attributeName->substr(0, indexOfPeriod);
							std::string attachedPropertyName = attributeName->substr(indexOfPeriod + 1);
							std::string* typeName = getFullTypeName(&abbreviatedName);

							XamlType* ownerType = _engine->get_typeResolver()->get_type(typeName);

							XamlAttachedProperty* attachedProp = _engine->get_typeResolver()->get_attachedProperty(ownerType, &attachedPropertyName);

							_elementTracker->currentAttachedProperty = attachedProp;
						}
						else
						{
							if (indexOfColon > 0)
							{
								// TODO: Handle this error gracefully
								return;
							}
							XamlProperty* prop = _engine->get_typeResolver()->get_property(_elementTracker->element->get_type(), attributeName);

							_elementTracker->currentProperty = prop;
						}
					}

					void visitAttributeValue(char* text, int begin, int end)
					{
						if (_elementTracker->currentProperty != 0)
						{
							if (!_elementTracker->currentProperty->propertyType()->get_attributeParseable())
							{
								// TODO: Handle error gracefully
								return;
							}
							char* attributeValueText = getAttributeValue(text, begin, end);

							void* value = _elementTracker->currentProperty->propertyType()->parseAttributeText(attributeValueText);
							_elementTracker->currentProperty->set_value(_elementTracker->element, value);
							
							_elementTracker->currentProperty = 0;
						}
						else if (_elementTracker->currentAttachedProperty != 0)
						{
							if (!_elementTracker->currentAttachedProperty->propertyType()->get_attributeParseable())
							{
								// TODO: Handle error gracefully
								return;
							}
							char* attributeValueText = getAttributeValue(text, begin, end);
							
							void* value = _elementTracker->currentAttachedProperty->propertyType()->parseAttributeText(attributeValueText);
							((XamlRuntimeObject*)_elementTracker->element)->set_value(_elementTracker->currentAttachedProperty, value);

							_elementTracker->currentAttachedProperty = 0;
						}
						else
						{
							// TODO: Handle error gracefully
							
						}
					}

					void visitUnknown(char* text, int begin, int end)
					{
						printf("Unknown\n");
					}

					void visitContent(char* text, int begin, int end)
					{
						printf("Content\n");
					}

					void visitBOF(char* text)
					{
						printf("Begin of file\n");
					}

					void visitEOF(char* text)
					{
						printf("End of file\n");
					}

				public:
					XamlEngineParserNodeVisitor(XamlRenderEngine* engine, std::string* defaultNamespace)
					{
						_engine = engine;
						_defaultNamespace = defaultNamespace;
					}

					XamlObject* get_result()
					{
						return _result;
					}

				};
			}
		}
	}
}