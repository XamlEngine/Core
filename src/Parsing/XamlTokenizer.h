#pragma once

#include "XamlTokenType.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Parsing
		{
			class XamlTokenizer
			{
			private:
				char* _text;
				int _currentCharPosition = -1;
				int _currentTokenBegin = -1;
				int _currentTokenEnd = -1;
				XamlTokenType _currentTokenType = XamlTokenType::BeginOfFile;

			protected:
				char currentChar()
				{
					if (_currentCharPosition < 0)
					{
						throw;
					}

					return *(_text + _currentCharPosition);
				}

				char peekNext()
				{
					if (_currentCharPosition < -1)
					{
						throw;
					}

					if (currentTokenType() == XamlTokenType::EndOfFile)
					{
						return XamlTokenType::EndOfFile;
					}
					return *(_text + _currentCharPosition + 1);
				}

				XamlTokenType charToTokenType(char character)
				{
					switch (character)
					{
					case '\0':
						return XamlTokenType::EndOfFile;
					case ' ':
					case '\t':
						return XamlTokenType::Whitespace;
					case '<':
						return XamlTokenType::LessThan;
					case '>':
						return XamlTokenType::GreaterThan;
					case '/':
						return XamlTokenType::ForwardSlash;
					case '\\':
						return XamlTokenType::BackSlash;
					case '\n':
					case '\r':
						return XamlTokenType::NewLine;
					case '&':
						return XamlTokenType::Amperstand;
					case '~':
						return XamlTokenType::Tilda;
					case '`':
						return XamlTokenType::GraveAccent;
					case '!':
						return XamlTokenType::Exclamation;
					case '@':
						return XamlTokenType::Asperand;
					case '#':
						return XamlTokenType::Pound;
					case '$':
						return XamlTokenType::DollarSign;
					case '%':
						return XamlTokenType::Percent;
					case '^':
						return XamlTokenType::Power;
					case '*':
						return XamlTokenType::Asterisk;
					case '(':
						return XamlTokenType::LeftParenthesis;
					case ')':
						return XamlTokenType::RightParenthesis;
					case '_':
						return XamlTokenType::Underscore;
					case '-':
						return XamlTokenType::Minus;
					case '+':
						return  XamlTokenType::Plus;
					case '=':
						return XamlTokenType::Equals;
					case ',':
						return XamlTokenType::Comma;
					case '.':
						return XamlTokenType::Period;
					case '?':
						return XamlTokenType::QuestionMark;
					case ':':
						return XamlTokenType::Colon;
					case ';':
						return XamlTokenType::Semicolon;
					case '\'':
						return XamlTokenType::Apostrophe;
					case '"':
						return XamlTokenType::Quote;
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'g':
					case 'h':
					case 'i':
					case 'j':
					case 'k':
					case 'l':
					case 'm':
					case 'n':
					case 'o':
					case 'p':
					case 'q':
					case 'r':
					case 's':
					case 't':
					case 'u':
					case 'v':
					case 'w':
					case 'x':
					case 'y':
					case 'z':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
					case 'G':
					case 'H':
					case 'I':
					case 'J':
					case 'K':
					case 'L':
					case 'M':
					case 'N':
					case 'O':
					case 'P':
					case 'Q':
					case 'R':
					case 'S':
					case 'T':
					case 'U':
					case 'V':
					case 'W':
					case 'X':
					case 'Y':
					case 'Z':
						return XamlTokenType::Text;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						return XamlTokenType::Number;
					default:
						return XamlTokenType::Unknown;
					}
				}

				XamlTokenType currentCharToken()
				{
					if (_currentCharPosition < 0)
					{
						return XamlTokenType::BeginOfFile;
					}
					return charToTokenType(currentChar());
				}

				XamlTokenType peekNextCharTokenType()
				{
					if (_currentCharPosition < -1)
					{
						throw;
					}

					return charToTokenType(peekNext());
				}

			public:
				XamlTokenizer(char* text)
				{
					_text = text;
				}
				
				bool moveNext()
				{
					if (_currentTokenType == XamlTokenType::EndOfFile)
					{
						return false;
					}
					_currentCharPosition++;
					if (_currentTokenType == XamlTokenType::EndOfFile)
					{
						return false;
					}

					_currentTokenBegin = _currentCharPosition;
					_currentTokenEnd = _currentCharPosition;

					_currentTokenType = currentCharToken();
					XamlTokenType nextTokenType = peekNextCharTokenType();
					switch (_currentTokenType)
					{
					case XamlTokenType::Whitespace:
						while (nextTokenType == XamlTokenType::Whitespace)
						{
							_currentCharPosition++;
							_currentTokenEnd = _currentCharPosition;
							nextTokenType = peekNextCharTokenType();
						}
						break;
					case XamlTokenType::Text:
						while (nextTokenType == XamlTokenType::Text)
						{
							_currentCharPosition++;
							_currentTokenEnd = _currentCharPosition;
							nextTokenType = peekNextCharTokenType();
						}
						break;
					case XamlTokenType::Number:
						while (nextTokenType == XamlTokenType::Number)
						{
							_currentCharPosition++;
							_currentTokenEnd = _currentCharPosition;
							nextTokenType = peekNextCharTokenType();
						}
					default:
						// TODO: Handle this
						break;
					}

					return true;
				}

				bool moveThroughWhitespace()
				{
					XamlTokenType nextTokenType = peekNextCharTokenType();
					if (nextTokenType != XamlTokenType::Whitespace && nextTokenType != XamlTokenType::NewLine)
					{
						return false;
					}
					while (nextTokenType == XamlTokenType::Whitespace || nextTokenType == XamlTokenType::NewLine)
					{
						if (!moveNext())
						{
							return false;
						}
						nextTokenType = peekNextCharTokenType();
					}
					return true;
				}


				int currentCharPosition()
				{
					return _currentCharPosition;
				}

				XamlTokenType currentTokenType()
				{
					return _currentTokenType;
				}

				XamlTokenType peekCharTokenType(int offset)
				{
					int position = _currentCharPosition + offset;
					if (position < 0)
					{
						return XamlTokenType::BeginOfFile;
					}
					else if (offset > 0 && peekCharTokenType(offset - 1) == XamlTokenType::EndOfFile)
					{
						return XamlTokenType::EndOfFile;
					}
					return charToTokenType(*(_text + position));
				}

				int currentTokenBegin()
				{
					return _currentTokenBegin;
				}

				int currentTokenEnd()
				{
					return _currentTokenEnd;
				}

				char* text()
				{
					return _text;
				}

				bool isEOF()
				{
					return currentCharToken() == XamlTokenType::EndOfFile;
				}

				bool isBOF()
				{
					return _currentCharPosition < 0;
				}

			};
		}
	}
}