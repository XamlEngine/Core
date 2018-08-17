#pragma once

#include "../Runtime/List.h"
#include <functional>

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{
			template <class T>
			class DefinitionCollectionBase : public List<T>
			{
			private:
				std::function<void()> _invalidated;

			protected:
				
				virtual void added_item(T item, int index)
				{
					_invalidated();
				}
				
				virtual void removed_item(T item, int index)
				{
					_invalidated();
				}

				virtual void cleared_items()
				{
					_invalidated();
				}

				virtual void replaced_item(T oldItem, T newItem, int index)
				{
					_invalidated();
				}
				
				DefinitionCollectionBase(XamlType* type, std::function<void()> invalidated) : List<T>(type)
				{
					_invalidated = invalidated;
				}

			};

		}
	}
}