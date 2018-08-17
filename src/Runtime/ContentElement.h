#pragma once

#include "XamlRuntimeObject.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Runtime
		{

			class ContentElement : public XamlRuntimeObject
			{
			private:

			
			protected:
				XamlServices * services;

				ContentElement(XamlType* type, XamlServices* services) : XamlRuntimeObject(type)
				{
					this->services = services;
				}

			public:
			

			};

		}
	}
}