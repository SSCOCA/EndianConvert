#include "./lexer/lexer.h"
#include <fstream>

void main()
{
	std::cout << "ÊäÈë´úÂë:" << std::endl;
	std::ifstream in;
	in.open("test.h");
	if (!in.is_open())
	{
		return;
	}

	Lexer lex(&in);// &std::cin);//
	SpToken token = lex.read();
	while (token->type() != Token::TOKEN_EOF)
	{
		std::cout << "$" << token->toString() << " ";
		token = lex.read();
	}
	getchar();
}