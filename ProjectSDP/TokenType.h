#ifndef TOKENTYPE
#define TOKENTYPE

//using enum class because it is considered a good practice and doesnt get converted to int implicitly
enum class TokenType
{
	//primitive
	INT,
	FLOAT,
	STRING,

	//bool types
	EQ,
	NEQ,
	LESS,
	MORE,
	OR,
	AND,
	NOT,

	LPAREN,
	RPAREN,

	//arithmetic types
	PLUS,
	MINUS,
	MUL,
	DIV,

	//go to other cell
	SQ_LPAREN,
	SQ_RPAREN,
	X_AXIS,
	Y_AXIS,

	//if statement
	IF,
	COMMA,

	SUM,
	COUNT,
	COLON, //special type used for range

	//if something is of this type during tokenizing the input, then there are forbidden symbols in the input
	OTHER,
	//tokens with this tag contain the text of a cell that can't be tokenized
	ERROR,

	//every tokenized input should have this at the end
	END
};

#endif // !TOKENTYPE

