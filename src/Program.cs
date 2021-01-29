using System;

namespace Compiler
{
	internal static class Program
	{
		private static void Main(string[] args)
		{
			while (true)
			{
				Console.Write("> ");
				string text = Console.ReadLine();

				if (text == "#exit")
				{
					break;
				}
				else if (text == "#cls")
				{
					Console.Clear();
					continue;
				}

				Lexer lexer = new Lexer(text);
				while (true)
				{
					Token token = lexer.NextToken();
					if (token.Kind != TokenKind.SingleLineComment &&
						token.Kind != TokenKind.MultilineComment &&
						token.Kind != TokenKind.Whitespace)
					{
						Console.WriteLine($"<{token.Kind}> = '{token.Text}'");
					}
					if (token.Kind == TokenKind.EndOfFile)
						break;
				}
			}
		}
	}
}
