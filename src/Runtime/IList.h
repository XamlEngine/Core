#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			class IListBase
			{
			public:
				virtual void add_item(void* item) = 0;

				virtual void insert_item(void* item, int index) = 0;

				virtual void* get_itemBase(int index) = 0;

				virtual bool remove_item(void* item) = 0;

				virtual void remove_item_at(int index) = 0;

				virtual void clear() = 0;

				virtual int count() = 0;
			};

			template<class T>
			class IList : public IListBase
			{
				
			public:

				virtual void add_item(T item) = 0;
				
				virtual void insert_item(T item, int index) = 0;
				
				virtual T get_item(int index) = 0;
				
				virtual bool remove_item(T item) = 0;
				

			};
		}
	}
}