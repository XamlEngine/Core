#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Logging
		{
			class Logger
			{

			public:
				LogLevel level;

				virtual void log(LogLevel level, char* message) = 0;

			};
		}
	}
}