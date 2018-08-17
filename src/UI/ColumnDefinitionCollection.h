#pragma once

#include "DefinitionCollectionBase.h"
#include "ColumnDefinition.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class ColumnDefinitionCollection : public DefinitionCollectionBase<ColumnDefinition*>
			{
			public:
				ColumnDefinitionCollection(XamlType* type, std::function<void()> invalidated) : DefinitionCollectionBase<ColumnDefinition*>(type, invalidated)
				{

				}
			};

		}
	}
}