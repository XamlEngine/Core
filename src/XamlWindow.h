#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		class XamlWindow
		{
		protected:
			virtual void onShow() = 0;

		public:
			void show()
			{
				onShow();
			}
		};
	}
}