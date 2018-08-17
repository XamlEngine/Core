// Parser.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "XamlTokenizer.h"
#include "XamlNodeVisitor.h"

namespace CoreVar
{
	namespace Xaml
	{
		namespace Parsing
		{

			class XamlParser
			{
			protected:
				void parseStartElement(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					if (!tokenizer->moveNext())
					{
						throw;
					}
					switch (tokenizer->currentTokenType())
					{
					case XamlTokenType::LessThan:
						switch (tokenizer->peekCharTokenType(1))
						{
						case XamlTokenType::Text:
							parseElementName(tokenizer, nodeVisitor);
							break;
						default:
							nodeVisitor->visitUnknown(tokenizer->text(), tokenizer->currentTokenBegin(), tokenizer->currentTokenEnd());
							break;
						}
						break;
					default:
						break;
					}
					
					while (tokenizer->moveThroughWhitespace())
					{
						switch (tokenizer->peekCharTokenType(1))
						{
						case XamlTokenType::Text:
							parseAttributeName(tokenizer, nodeVisitor);
							break;
						default:
							// TODO: Handle this
							break;
						}
						switch (tokenizer->peekCharTokenType(1))
						{
						case XamlTokenType::Equals:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							parseAttributeValue(tokenizer, nodeVisitor);
							break;
						default:
							// TODO: Handle this
							break;
						}
					}
					int closeStartBegin;
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::GreaterThan:
						parseEndElement(tokenizer, nodeVisitor);
						break;
					case XamlTokenType::ForwardSlash:
						switch (tokenizer->peekCharTokenType(2))
						{
						case XamlTokenType::GreaterThan:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							closeStartBegin = tokenizer->currentTokenBegin();
							if (!tokenizer->moveNext())
							{
								throw;
							}
							nodeVisitor->visitCloseStartElement(tokenizer->text(), closeStartBegin, tokenizer->currentTokenEnd());
							break;
						default:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							nodeVisitor->visitUnknown(tokenizer->text(), tokenizer->currentTokenBegin(), tokenizer->currentTokenEnd());
							break;
						}
						break;
					default:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nodeVisitor->visitUnknown(tokenizer->text(), tokenizer->currentTokenBegin(), tokenizer->currentTokenEnd());
						break;
					}
				}

				void parseElementName(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					if (tokenizer->peekCharTokenType(1) != XamlTokenType::Text)
					{
						throw;
					}

					int begin = tokenizer->currentTokenBegin();

					if (!tokenizer->moveNext())
					{
						throw;
					}
					int end = tokenizer->currentTokenEnd();
					XamlTokenType nextTokenType = tokenizer->peekCharTokenType(1);
					bool closeElement = false;
					switch (nextTokenType)
					{
					case XamlTokenType::Whitespace:
					case XamlTokenType::GreaterThan:
						break;
					case XamlTokenType::Colon:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nextTokenType = tokenizer->peekCharTokenType(1);
						switch (nextTokenType)
						{
						case XamlTokenType::Text:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							nextTokenType = tokenizer->peekCharTokenType(1);
							switch (nextTokenType)
							{
							case XamlTokenType::Period:
								if (!tokenizer->moveNext())
								{
									throw;
								}
								nextTokenType = tokenizer->peekCharTokenType(1);
								switch (nextTokenType)
								{
								case XamlTokenType::Text:
									if (!tokenizer->moveNext())
									{
										throw;
									}
									break;
								default:
									end = tokenizer->currentTokenEnd();
									nodeVisitor->visitUnknown(tokenizer->text(), begin, end);
									return;
								}
								break;
							default:
								// TODO: Handle this
								break;
							}
							break;
						default:
							// TODO: Handle this
							break;
						}
						break;
					case XamlTokenType::Period:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nextTokenType = tokenizer->peekCharTokenType(1);
						switch (nextTokenType)
						{
						case XamlTokenType::Text:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							break;
						default:
							end = tokenizer->currentTokenEnd();
							nodeVisitor->visitUnknown(tokenizer->text(), begin, end);
							return;
						}
						break;
					default:
						end = tokenizer->currentTokenEnd();
						nodeVisitor->visitUnknown(tokenizer->text(), begin, end);
						return;
					}
					end = tokenizer->currentTokenEnd();

					nodeVisitor->visitBeginStartElement(tokenizer->text(), begin, end);
				}

				void parseAttributeName(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					if (tokenizer->peekCharTokenType(1) != XamlTokenType::Text)
					{
						throw;
					}

					if (!tokenizer->moveNext())
					{
						throw;
					}

					int begin = tokenizer->currentTokenBegin();
					int end = tokenizer->currentTokenEnd();
					XamlTokenType nextTokenType = tokenizer->peekCharTokenType(1);
					switch (nextTokenType)
					{
					case XamlTokenType::Colon:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nextTokenType = tokenizer->peekCharTokenType(1);
						switch (nextTokenType)
						{
						case XamlTokenType::Text:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							nextTokenType = tokenizer->peekCharTokenType(1);
							switch (nextTokenType)
							{
							case XamlTokenType::Period:
								if (!tokenizer->moveNext())
								{
									throw;
								}
								nextTokenType = tokenizer->peekCharTokenType(1);
								switch (nextTokenType)
								{
								case XamlTokenType::Text:
									if (!tokenizer->moveNext())
									{
										throw;
									}
									break;
								default:
									end = tokenizer->peekCharTokenType(1);
									nodeVisitor->visitUnknown(tokenizer->text(), begin, end);
									return;
								}
								break;
							case XamlTokenType::Equals:
								break;
							default:
								// TODO: Handle this
								break;
							}
						default:
							// TODO: Handle this
							break;
						}
						break;
					case XamlTokenType::Period:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nextTokenType = tokenizer->peekCharTokenType(1);
						switch (nextTokenType)
						{
						case XamlTokenType::Text:
							if (!tokenizer->moveNext())
							{
								throw;
							}
							break;
						default:
							end = tokenizer->currentTokenEnd();
							nodeVisitor->visitUnknown(tokenizer->text(), begin, end);
							return;
						}
						break;
					case XamlTokenType::Equals:
						break;
					default:
						end = tokenizer->currentTokenEnd();
						nodeVisitor->visitUnknown(tokenizer->text(), begin, end);
						return;
					}
					end = tokenizer->currentTokenEnd();

					nodeVisitor->visitAttributeName(tokenizer->text(), begin, end);
				}

				void parseAttributeValue(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					XamlTokenType validatorTokenType = tokenizer->peekCharTokenType(1);
					if ((validatorTokenType != XamlTokenType::Quote && validatorTokenType != XamlTokenType::Apostrophe) || !tokenizer->moveNext())
					{
						throw;
					}
					int begin = tokenizer->currentTokenBegin();
					while (tokenizer->moveNext() && tokenizer->currentTokenType() != validatorTokenType && tokenizer->currentTokenType() != XamlTokenType::EndOfFile);
					nodeVisitor->visitAttributeValue(tokenizer->text(), begin, tokenizer->currentTokenEnd());
				}

				char* get_str(char* text, int begin, int end)
				{
					int length = end - begin + 1;
					char* result = (char*)malloc(length + 1);
					result[length] = 0;
					memcpy(result, text + begin, length);
					return result;
				}

				void parseClosingElement(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					if (!tokenizer->moveNext())
					{
						throw;
					}
					int begin = tokenizer->currentTokenBegin();
					if (!tokenizer->moveNext())
					{
						throw;
					}
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::Text:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					default:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nodeVisitor->visitUnknown(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					}
					bool hasColon = false;
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::Colon:
						hasColon = true;
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					case XamlTokenType::Period:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					case XamlTokenType::GreaterThan:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nodeVisitor->visitClosingElement(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					default:
						// TODO: Handle this
						break;
					}
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::Text:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					default:
						nodeVisitor->visitUnknown(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					}
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::GreaterThan:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nodeVisitor->visitClosingElement(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					case XamlTokenType::Period:
						if (!hasColon)
						{
							nodeVisitor->visitUnknown(tokenizer->text(), begin, tokenizer->currentTokenEnd());
							return;
						}
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					default:
						nodeVisitor->visitUnknown(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					}
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::Text:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					default:
						nodeVisitor->visitUnknown(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					}
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::GreaterThan:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nodeVisitor->visitClosingElement(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					default:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						nodeVisitor->visitUnknown(tokenizer->text(), begin, tokenizer->currentTokenEnd());
						return;
					}
				}

				void parseEndElement(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					if (tokenizer->peekCharTokenType(1) != XamlTokenType::GreaterThan)
					{
						throw;
					}
					if (!tokenizer->moveNext())
					{
						throw;
					}
					int begin = tokenizer->currentTokenBegin();
					int end = tokenizer->currentTokenEnd();
					nodeVisitor->visitEndStartElement(tokenizer->text(), begin, end);
				}
					
				void parseContent(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					int begin = tokenizer->currentTokenEnd() + 1;
					bool hasContent = false;
					while (tokenizer->moveNext())
					{
						switch (tokenizer->peekCharTokenType(1))
						{
						case XamlTokenType::LessThan:
							switch (tokenizer->peekCharTokenType(2))
							{
							case XamlTokenType::Text:
							case XamlTokenType::ForwardSlash:
								nodeVisitor->visitContent(tokenizer->text(), begin, tokenizer->currentTokenEnd());
								return;
							case XamlTokenType::Exclamation:
								if (tokenizer->peekCharTokenType(3) == XamlTokenType::Minus && tokenizer->peekCharTokenType(4) == XamlTokenType::Minus)
								{
									nodeVisitor->visitContent(tokenizer->text(), begin, tokenizer->currentTokenEnd());
									return;
								}
								break;
							default:
								break;
							}
							break;
						default:
							break;
						}
					}
					nodeVisitor->visitContent(tokenizer->text(), begin, tokenizer->currentTokenEnd());
				}

				void parseComment(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{

				}

				void parseAny(XamlTokenizer* tokenizer, XamlNodeVisitor* nodeVisitor)
				{
					switch (tokenizer->peekCharTokenType(1))
					{
					case XamlTokenType::LessThan:
						switch (tokenizer->peekCharTokenType(2))
						{
						case XamlTokenType::Text:
							parseStartElement(tokenizer, nodeVisitor);
							break;
						case XamlTokenType::ForwardSlash:
							parseClosingElement(tokenizer, nodeVisitor);
							break;
						case XamlTokenType::Exclamation:
							if (tokenizer->peekCharTokenType(3) == XamlTokenType::Minus && tokenizer->peekCharTokenType(4) == XamlTokenType::Minus)
							{
								parseComment(tokenizer, nodeVisitor);
							}
							break;
						default:
							parseContent(tokenizer, nodeVisitor);
							break;
						}
						break;
					case XamlTokenType::Whitespace:
					case XamlTokenType::NewLine:
						if (!tokenizer->moveNext())
						{
							throw;
						}
						break;
					case XamlTokenType::EndOfFile:
						tokenizer->moveNext();
						
						break;
					default:
						parseContent(tokenizer, nodeVisitor);
						break;
					}
				}

			public:
				
				void parse(char* text, XamlNodeVisitor* nodeVisitor)
				{
					XamlTokenizer* tokenizer = new XamlTokenizer(text);
					nodeVisitor->visitBOF(text);
					while (!tokenizer->isEOF())
					{
						parseAny(tokenizer, nodeVisitor);
					}
					nodeVisitor->visitEOF(text);
				}

			};
		}
	}
}