#pragma once

#include "XamlObject.h"
#include <vector>
#include "IList.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{
			template<class T>
			class List : public IList<T>, public XamlObject
			{
			protected:
				std::vector<T> items = std::vector<T>();

				virtual void adding_item(T item, int index)
				{

				}

				virtual void added_item(T item, int index)
				{

				}

				virtual void removing_item(T item, int index)
				{

				}

				virtual void removed_item(T item, int index)
				{

				}

				virtual void clearing_items()
				{

				}

				virtual void cleared_items()
				{

				}

				virtual void replacing_item(T oldItem, T newItem, int index)
				{

				}

				virtual void replaced_item(T oldItem, T newItem, int index)
				{

				}

			public:
				using XamlObject::XamlObject;

				void add_item(T item)
				{
					int index = items.size();
					adding_item(item, index);
					items.push_back(item);
					added_item(item, index);
				}

				void insert_item(T item, int index)
				{
					adding_item(item, index);
					items.insert(items.begin() + index, item);
					added_item(item, index);
				}

				T get_item(int index)
				{
					return items.at(index);
				}

				bool remove_item(T item)
				{
					auto found = std::find(items.begin(), items.end(), item);
					if (found == items.end())
					{
						return false;
					}
					int index = found - items.begin();
					removing_item(item, index);
					items.erase(found);
					removed_item(item, index);
					return true;
				}

				void remove_item_at(int index)
				{
					T item = items.at(index);
					removing_item(item, index);
					items.erase(items.begin() + index);
					removed_item(item, index);
				}

				void replace_item(T newItem, int index)
				{
					T currentItem = items.at(index);
					replacing_item(currentItem, newItem, index);
					items[index] = newItem;
					replaced_item(currentItem, newItem, index);
				}

				void clear()
				{
					clearing_items();
					items.clear();
					cleared_items();
				}

				int count()
				{
					return items.size();
				}

				virtual void add_item(void* item)
				{
					add_item((T)(XamlObject*)item);
				}

				virtual void insert_item(void* item, int index)
				{
					insert_item((T)(XamlObject*)item, index);
				}

				virtual void* get_itemBase(int index)
				{
					return (XamlObject*)get_item(index);
				}

				virtual bool remove_item(void* item)
				{
					return remove_item((T)(XamlObject*)item);
				}

			};
		}
	}
}