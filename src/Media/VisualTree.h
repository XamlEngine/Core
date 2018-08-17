#pragma once

#include "VisualTreeElement.h"
#include <unordered_map>

namespace CoreVar
{
	namespace Xaml
	{
		namespace Media
		{
			class VisualTree
			{
				std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>* _elements = new std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>();
				
			public:

				XamlRuntimeObject * get_parent(XamlRuntimeObject* child)
				{
					std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>::const_iterator found = _elements->find(child);
					if (found == _elements->end())
					{
						// TODO: Handle this error gracefully.
					}
					else
					{
						VisualTreeElement* element = found->second;
						if (element->parent != 0)
						{
							return element->parent->element;
						}
					}
					return 0;
				}

				void add_child(XamlRuntimeObject* parent, XamlRuntimeObject* child, int index = -1)
				{
					if (parent != 0)
					{
						VisualTreeElement* parentElement;
						std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>::const_iterator foundParent = _elements->find(parent);
						if (foundParent == _elements->end())
						{
							parentElement = new VisualTreeElement();
							parentElement->element = parent;
							parentElement->children = new std::vector<VisualTreeElement*>();
							parentElement->parent = 0;
							_elements->insert({ parent, parentElement });
						}
						else
						{
							parentElement = foundParent->second;
						}

						VisualTreeElement* childElement;
						std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>::const_iterator childFound = _elements->find(child);
						if (childFound == _elements->end())
						{
							childElement = new VisualTreeElement();
							childElement->element = child;
							childElement->children = new std::vector<VisualTreeElement*>();
							childElement->parent = parentElement;
							_elements->insert({ child, childElement });
						}
						else
						{
							childElement = childFound->second;
							childElement->parent = parentElement;
						}
						if (index < 0)
						{
							parentElement->children->push_back(childElement);
						}
						else
						{
							parentElement->children->insert(parentElement->children->begin() + index, childElement);
						}
					}
					else
					{
						VisualTreeElement* childElement = new VisualTreeElement();
						childElement->element = child;
						childElement->children = new std::vector<VisualTreeElement*>();
						_elements->insert({ child, childElement });
					}
				}

				void remove_child(XamlRuntimeObject* parent, XamlRuntimeObject* child)
				{
					std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>::const_iterator found = _elements->find(child);
					if (found == _elements->end())
					{
						// TODO: Handle this error gracefully.
					}
					else
					{
						VisualTreeElement* element = found->second;
						VisualTreeElement* parentElement = element->parent;
						if (parentElement != 0)
						{
							std::vector<VisualTreeElement*>::iterator parentChildrenIterator = std::find(parentElement->children->begin(), parentElement->children->end(), element);
							if (parentChildrenIterator == parentElement->children->end())
							{
								// TODO: Handle this error gracefully
							}
							else
							{
								parentElement->children->erase(parentChildrenIterator);
							}
						}
						delete element;
					}
				}

				int get_childCount(XamlRuntimeObject* parent)
				{
					std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>::const_iterator found = _elements->find(parent);
					if (found == _elements->end())
					{
						// TODO: Handle this error gracefully.
					}
					else
					{
						VisualTreeElement* parentElement = found->second;
						return parentElement->children->size();
					}
					return -1;
				}

				XamlRuntimeObject* get_child(XamlRuntimeObject* parent, int index)
				{
					std::unordered_map<XamlRuntimeObject*, VisualTreeElement*>::const_iterator found = _elements->find(parent);
					if (found == _elements->end())
					{
						// TODO: Handle this error gracefully.
					}
					else
					{
						VisualTreeElement* parentElement = found->second;
						return parentElement->children->at(index)->element;
					}
					return 0;
				}

			};
		}
	}
}