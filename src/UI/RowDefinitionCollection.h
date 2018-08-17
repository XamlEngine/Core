#pragma once

#include "DefinitionCollectionBase.h"
#include "RowDefinition.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace UI
		{

			class RowDefinitionCollection : public DefinitionCollectionBase<RowDefinition*>
			{
			public:
				RowDefinitionCollection(XamlType* type, std::function<void()> invalidated) : DefinitionCollectionBase<RowDefinition*>(type, invalidated)
				{

				}
			};

		}
	}
}