namespace Compiler
{
	public sealed class Token
	{
		public Token(TokenKind kind, string text, int position)
		{
			Kind = kind;
			Text = text;
			Position = position;
		}

		public TokenKind Kind { get; }
		public string Text { get; }
		public int Position { get; }
	}
}
