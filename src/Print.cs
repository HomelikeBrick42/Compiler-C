using System;

namespace Compiler
{
	internal static class Print
	{
		private static int GetLine(string source, int position)
		{
			string[] lines = source.Split("\n");
			int charPosition = 0;
			for (int i = 0; i < lines.Length; i++)
			{
				if (charPosition + lines[i].Length >= position)
				{
					return i + 1;
				}
				charPosition += lines[i].Length;
			}

			return lines.Length;
		}

		private static string GetLine(string[] lines, int index)
		{
			if (index >= lines.Length || index < 0)
				return "";
			return lines[index];
		}

		public static void TokenError(string source, Token token, string error)
		{
			int line = GetLine(source, token.Position);
			string[] lines = source.Split("\n");
			int position = 0;
			for (int i = 0; i < line - 1; i++)
			{
				position += lines[i].Length;
			}

			string line1 = GetLine(lines, line - 3);
			string line2 = GetLine(lines, line - 2);
			string line3 = GetLine(lines, line - 1);
			int offset = (token.Text.Length / 2) + (token.Text.Length == 0 ? 0 : 1);
			string arrow = new string('-', Math.Max(token.Position - position, 0) + offset) + "^";
			Console.WriteLine($"\n{error}\nLine: {line}:{token.Position - position}\n> {line1}\n> {line2}\n> {line3}\n{arrow}\n");
		}
	}
}
