#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Parsing
		{

			class XamlNodeVisitor
			{
			protected:
				char* getSubstr(char* text, int begin, int end)
				{
					int length = end - begin + 1;
					char* result = new char[length + 1];
					result[length] = 0;
					memcpy(result, text + begin, length);
					return result;
				}


			protected:
				char* getBeginStartElementName(char* text, int begin, int end)
				{
					begin = begin + 1;
					return getSubstr(text, begin, end);
				}

				char* getAttributeName(char* text, int begin, int end)
				{
					return getSubstr(text, begin, end);
				}

				char* getClosingElementName(char* text, int begin, int end)
				{
					begin += 2;
					end -= 1;
					return getSubstr(text, begin, end);
				}

				char* getAttributeValue(char* text, int begin, int end)
				{
					begin += 1;
					end -= 1;
					return getSubstr(text, begin, end);
				}


				char* getContent(char* text, int begin, int end)
				{
					return getSubstr(text, begin, end);
				}

			public:

				virtual void visitBeginStartElement(char* text, int begin, int end) = 0;

				virtual void visitEndStartElement(char* text, int begin, int end) = 0;

				virtual void visitCloseStartElement(char* text, int begin, int end) = 0;

				virtual void visitClosingElement(char* text, int begin, int end) = 0;

				virtual void visitAttributeName(char* text, int begin, int end) = 0;

				virtual void visitAttributeValue(char* text, int begin, int end) = 0;

				virtual void visitUnknown(char* text, int begin, int end) = 0;

				virtual void visitContent(char* text, int begin, int end) = 0;

				virtual void visitBOF(char* text) = 0;

				virtual void visitEOF(char* text) = 0;

			};

		}
	}
}