using System;

namespace Compiler
{
	public sealed class Lexer
	{
		private readonly string m_Text;
		private int m_Position = 0;

		private char Current => Peek(0);
		private char Peek(int offset)
		{
			int index = m_Position + offset;
			if (index >= m_Text.Length)
				return '\0';
			return m_Text[index];
		}

		public Lexer(string text)
		{
			m_Text = text;
		}

		public Token NextToken()
		{
			int start = m_Position;

			switch (Current)
			{
				case ' ': case '\n': case '\r':
				case '\t': case '\v': case '\f':
				{
					int length = 0;

					while (true)
					{
						switch (Current)
						{
							case ' ': case '\n': case '\r':
							case '\t': case '\v': case '\f':
								length++;
								m_Position++;
								continue;
							default:
								break;
						}
						break;
					}

					return new Token(TokenKind.Whitespace, m_Text.Substring(start, length), start);
				}

				case '0':
				{
					int length = 0;

					length++;
					m_Position++;

					switch (Current)
					{
						case 'b': case 'B':
						{
							length++;
							m_Position++;

							while (true)
							{
								switch (Current)
								{
									case '0': case '1':
										length++;
										m_Position++;
										continue;
									default:
										break;
								}
								break;
							}

							return new Token(TokenKind.Binary, m_Text.Substring(start, length), start);
						}

						case 'x': case 'X':
						{
							length++;
							m_Position++;

							while (true)
							{
								switch (Current)
								{
									case '0': case '1': case '2': case '3': case '4':
									case '5': case '6': case '7': case '8': case '9':
									case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
									case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
										length++;
										m_Position++;
										continue;
									default:
										break;
								}
								break;
							}

							return new Token(TokenKind.Hexadecimal, m_Text.Substring(start, length), start);
						}

						default:
							return new Token(TokenKind.Int, m_Text.Substring(start, length), start);
					}
				}

				case '1': case '2': case '3': case '4': case '5':
				case '6': case '7': case '8': case '9': case '.':
				{
					int length = 0;
					bool isFloat = false;

					while (true)
					{
						switch (Current)
						{
							case '.':
								if (isFloat)
									break;
								isFloat = true;
								length++;
								m_Position++;
								continue;
							case '0': case '1': case '2': case '3': case '4':
							case '5': case '6': case '7': case '8': case '9':
								length++;
								m_Position++;
								continue;
							default:
								break;
						}
						break;
					}

					return new Token(isFloat ? TokenKind.Float : TokenKind.Int, m_Text.Substring(start, length), start);
				}

				case 'a': case 'b': case 'c': case 'd': case 'e':
				case 'f': case 'g': case 'h': case 'i': case 'j':
				case 'k': case 'l': case 'm': case 'n': case 'o':
				case 'p': case 'q': case 'r': case 's': case 't':
				case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
				case 'A': case 'B': case 'C': case 'D': case 'E':
				case 'F': case 'G': case 'H': case 'I': case 'J':
				case 'K': case 'L': case 'M': case 'N': case 'O':
				case 'P': case 'Q': case 'R': case 'S': case 'T': 
				case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
				case '_':
				{
					int length = 0;

					while (true)
					{
						switch (Current)
						{
							case 'a': case 'b': case 'c': case 'd': case 'e':
							case 'f': case 'g': case 'h': case 'i': case 'j':
							case 'k': case 'l': case 'm': case 'n': case 'o':
							case 'p': case 'q': case 'r': case 's': case 't':
							case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
							case 'A': case 'B': case 'C': case 'D': case 'E':
							case 'F': case 'G': case 'H': case 'I': case 'J':
							case 'K': case 'L': case 'M': case 'N': case 'O':
							case 'P': case 'Q': case 'R': case 'S': case 'T': 
							case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
							case '0': case '1': case '2': case '3': case '4':
							case '5': case '6': case '7': case '8': case '9':
							case '_':
								length++;
								m_Position++;
								continue;
							default:
								break;
						}
						break;
					}

					return new Token(TokenKind.Identifier, m_Text.Substring(start, length), start);
				}

				case '\'':
				{
					if (Peek(0) == '\'' &&
						Peek(2) == '\'')
					{
						m_Position += 3;
						return new Token(TokenKind.Char, m_Text.Substring(start + 1, 1), start);
					}

					if (Peek(0) == '\'' &&
						Peek(1) == '\\' &&
						Peek(3) == '\'')
					{
						m_Position += 4;
						return new Token(TokenKind.Char, m_Text.Substring(start + 1, 2), start);
					}

					Token error = new Token(TokenKind.Bad, m_Text.Substring(start, 2), start);
					Print.TokenError(m_Text, error, "[Lexer]: Invalid character literal");
					return error;
				}

				case '"':
				{
					int length = 0;
					m_Position++;

					while (Current != '"' && Current != '\0')
					{
						if (Peek(0) == '\\' &&
							Peek(1) == '"')
						{
							length++;
							m_Position++;
						}

						length++;
						m_Position++;
					}

					if (Current == '\0')
					{
						Token error = new Token(TokenKind.Bad, m_Text.Substring(start, length), start);
						Print.TokenError(m_Text, error, "[Lexer]: Unclosed string literal");
						return error;
					}
					else
					{
						m_Position++;
					}

					return new Token(TokenKind.String, m_Text.Substring(start + 1, length), start);
				}
				
				case '/':
				{
					m_Position++;

					switch (Current)
					{
						case '/':
						{
							int length = 0;
							m_Position++;

							while (Current != '\0' && Current != '\n' && Current != '\v' && Current != '\r')
							{
								length++;
								m_Position++;
							}

							return new Token(TokenKind.SingleLineComment, m_Text.Substring(start + 2, length), start);
						}

						case '*':
						{
							int length = 0;
							UInt64 depth = 1;
							m_Position++;

							while (Current != '\0' && depth > 0)
							{
								if (Peek(0) == '/' &&
									Peek(1) == '*')
								{
									depth++;
									length++;
									m_Position++;
								}

								if (Peek(0) == '*' &&
									Peek(1) == '/')
								{
									depth--;
									length++;
									m_Position++;
								}

								length++;
								m_Position++;
							}

							if (depth != 0)
							{
								Token error = new Token(TokenKind.Bad, m_Text.Substring(start, 2), start);
								Print.TokenError(m_Text, error, "[Lexer]: Unclosed multiline comment");
								return error;
							}
							else
							{
								length -= 2;
							}

							return new Token(TokenKind.MultilineComment, m_Text.Substring(start + 2, length), start);
						}

						case '=':
						{
							m_Position++;
							return new Token(TokenKind.ForwardSlashEquals, m_Text.Substring(start, 1), start);
						}

						default:
							return new Token(TokenKind.ForwardSlash, m_Text.Substring(start, 1), start);
					}
				}

				case ':':
				{
					m_Position++;

					switch (Current)
					{
						case ':':
						{
							m_Position++;
							return new Token(TokenKind.ColonColon, m_Text.Substring(start, 2), start);
						}

						case '=':
						{
							m_Position++;
							return new Token(TokenKind.ColonEquals, m_Text.Substring(start, 2), start);
						}

						default:
							return new Token(TokenKind.Colon, m_Text.Substring(start, 1), start);
					}
				}

				case '+':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.PlusEquals, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Plus, m_Text.Substring(start, 1), start);
				}

				case '-':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.MinusEquals, m_Text.Substring(start, 2), start);
					}

					if (Current == '>')
					{
						m_Position++;
						return new Token(TokenKind.RightArrow, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Minus, m_Text.Substring(start, 1), start);
				}

				case '*':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.AsteriskEquals, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Asterisk, m_Text.Substring(start, 1), start);
				}

				case '=':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.EqualsEquals, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Equals, m_Text.Substring(start, 1), start);
				}

				case '!':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.ExclamationMarkEquals, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.ExclamationMark, m_Text.Substring(start, 1), start);
				}

				case '%':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.PercentEquals, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Percent, m_Text.Substring(start, 1), start);
				}

				case '^':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.CaretEquals, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Caret, m_Text.Substring(start, 1), start);
				}

				case '<':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.LessThanEquals, m_Text.Substring(start, 2), start);
					}

					if (Current == '<')
					{
						m_Position++;

						if (Current == '=')
						{
							m_Position++;
							return new Token(TokenKind.LessThanLessThanEquals, m_Text.Substring(start, 3), start);
						}

						return new Token(TokenKind.LessThanLessThan, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.LessThan, m_Text.Substring(start, 1), start);
				}

				case '>':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.GreaterThanEquals, m_Text.Substring(start, 2), start);
					}

					if (Current == '>')
					{
						m_Position++;

						if (Current == '=')
						{
							m_Position++;
							return new Token(TokenKind.GreaterThanGreaterThanEquals, m_Text.Substring(start, 3), start);
						}

						return new Token(TokenKind.GreaterThanGreaterThan, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.GreaterThan, m_Text.Substring(start, 1), start);
				}

				case '&':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.AmpersandEquals, m_Text.Substring(start, 2), start);
					}

					if (Current == '&')
					{
						m_Position++;
						return new Token(TokenKind.AmpersandAmpersand, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Ampersand, m_Text.Substring(start, 1), start);
				}

				case '|':
				{
					m_Position++;

					if (Current == '=')
					{
						m_Position++;
						return new Token(TokenKind.PipeEquals, m_Text.Substring(start, 2), start);
					}

					if (Current == '|')
					{
						m_Position++;
						return new Token(TokenKind.PipePipe, m_Text.Substring(start, 2), start);
					}

					return new Token(TokenKind.Pipe, m_Text.Substring(start, 1), start);
				}

				case '\0': return new Token(TokenKind.EndOfFile,          m_Text.Substring(m_Position++, 0), start);
				case '(':  return new Token(TokenKind.OpenParentheses,    m_Text.Substring(m_Position++, 1), start);
				case ')':  return new Token(TokenKind.CloseParentheses,   m_Text.Substring(m_Position++, 1), start);
				case '[':  return new Token(TokenKind.OpenSquareBracket,  m_Text.Substring(m_Position++, 1), start);
				case ']':  return new Token(TokenKind.CloseSquareBracket, m_Text.Substring(m_Position++, 1), start);
				case '{':  return new Token(TokenKind.OpenBracket,        m_Text.Substring(m_Position++, 1), start);
				case '}':  return new Token(TokenKind.CloseBracket,       m_Text.Substring(m_Position++, 1), start);
				case ';':  return new Token(TokenKind.Semicolon,          m_Text.Substring(m_Position++, 1), start);
				case ',':  return new Token(TokenKind.Comma,              m_Text.Substring(m_Position++, 1), start);
				case '?':  return new Token(TokenKind.QuestionMark,       m_Text.Substring(m_Position++, 1), start);
				case '`':  return new Token(TokenKind.Grave,              m_Text.Substring(m_Position++, 1), start);
				default:   return new Token(TokenKind.Bad,                m_Text.Substring(m_Position++, 1), start);
			}
		}
	}
}
