#pragma once

namespace CoreVar
{
	namespace Xaml
	{
		namespace Parsing
		{

			enum XamlTokenType
			{
				Whitespace,
				NewLine,
				LessThan,
				GreaterThan,
				ForwardSlash,
				BackSlash,
				Text,
				Number,
				Amperstand,
				Pound,
				Colon,
				Semicolon,
				Equals,
				LeftCurlyBrace,
				RightCurlyBrace,
				Quote,
				Apostrophe,
				Exclamation,
				Asperand,
				DollarSign,
				Percent,
				Power,
				Asterisk,
				LeftParenthesis,
				RightParenthesis,
				QuestionMark,
				Comma,
				Period,
				Tilda,
				GraveAccent,
				Underscore,
				Minus,
				Plus,
				Pipe,
				Unknown,
				BeginOfFile,
				EndOfFile
			};
		}
	}
}