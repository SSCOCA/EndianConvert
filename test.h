//�ʷ�������ʵ��
#include "lexer.h"

/*************************************************
File: lexer.h
Describe:�ʷ�������
Author: 10127
Date: ���� 2020
Version:0.01
Other:
//*******"abc"***********************\************

/* */

#:

*********
*/abc

//������ʽ��Ӧ��ƥ����
//C++11������ʽ�⺯������һ�����ص���ƥ��ɹ��Ĵ���
enum MacthRet
{
	MATCH_COMMENT_END = 2,
	MATCH_FLOAT,
	MATCH_INT,
	MATCH_ID,
	MATCH_2OP,
	MATCH_STRING,
	MATCH_COMMENT_BEGIN = 9,
	MATCH_SYMBOL = 10
};

Lexer::Lexer(std::istream* reader) 
:m_reader(reader), m_lineNo(0), m_commentBegin(false)
{
	//�Ⱥ�˳�����ƥ�����ȼ�
	const std::string patStr = "\\s*("					//�հ��ַ�
		"//.*|/\\*.*\\*/|"								//����ע��
		"([^\"]*\\*/)|"									//����ע�ͽ���
		"([0-9]+\\.[0-9]*)|"							//����������������������֮ǰ��Ϊ�˷�ֹ������������ƥ��С����ǰ��Ĳ���
		"(0x[0-9]+|0X[0-9]+|[0-9]+)|"					//����
		"([A-Z_a-z][A-Z_a-z0-9]*)|"						//��ʶ
		"(==|>=|<=|\\|\\||&&|!=|::|\\+\\+|\\-\\-)|"		//˫�ֽ������
		"(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")|"			//�ַ���	
		"(/\\*.*)|"										//����ע�Ϳ�ͷ
		"(.))";											//����ĸԪ��
	
	m_regex = new std::regex(patStr);
}

Lexer::~Lexer()
{
	if (m_regex!=NULL)
	{
		delete m_regex;
		m_regex = NULL;
	}
}

SpToken Lexer::read(bool isPeek)
{
	if (m_tokens.empty())
	{
		parserNewLine();
	}

	if (m_tokens.empty())
	{
		return SpToken(new TokenEof);
	}

	SpToken token = m_tokens.front();
	if (!isPeek)
	{
		m_tokens.pop_front();
	}
	return token;
}

SpToken Lexer::peek()
{
	return read(true);
}


bool Lexer::parserNewLine()
{
	if (m_reader == NULL)
	{
		std::cerr << "������������Ϊ��ָ��" << std::endl;
		exit(1);
		return false;
	}

	std::string lineStr;
	while (lineStr.empty() && !m_reader->eof())
	{
		//����Ǹ����У�������������ֱ�������ǿ���Ϊֹ
		getline(*m_reader, lineStr);
		m_lineNo++;
	}
	
	//����ַ������ǿգ�˵����ȡ��ϣ����ؽ�����
	if (lineStr.empty())
	{
		return false;
	}

	//������ɵ�һ��
	return parser(lineStr);
}

bool Lexer::parser(std::string &lineStr)
{
	unsigned int begin = 0;
	unsigned int end = lineStr.length();
	while (begin != end)
	{
		std::smatch ret;
		std::string str(lineStr, begin, end);
		bool found = std::regex_search(str, ret, *m_regex);
		if (!found || ret.position() != 0)
		{
			errMsg("�ʷ��������޷�����ʷ�:" + str);
			return false;
		}

		for (unsigned int i = 2; i < ret.size(); i++)
		{
			if (ret[MATCH_COMMENT_BEGIN].matched)
			{
				if (!m_commentBegin)
				{
					m_commentBegin = true;
				}
				else
				{
					errMsg("�ʷ�����������ע�ʹ��󣬶��ʹ��/*" );
				}
			}

			if (ret[MATCH_COMMENT_END].matched)
			{
				if (m_commentBegin)
				{
					m_commentBegin = false;
				}
				else
				{
					errMsg("�ʷ�����������ע�ʹ��󣬴���ʹ��*/");
				}
			}

			if (m_commentBegin)
			{
				parserNewLine();
				return true;
			}

			if (ret[i].matched)
			{
				createToken(ret[i].str(), i, begin + ret.length());
				break;
			}

		}

		//�����ƥ����ַ�����ʼλ��
		begin += (ret.position() + ret.length());
	}

	//���һ�����з�
	m_tokens.push_back(SpToken(new TokenEol(m_lineNo, begin)));
	return true;
}


void Lexer::createToken(const std::string& str, int matchNo, int colNum)
{
	double val = 0;
	switch (matchNo)
	{
	case MATCH_FLOAT:
		val = atof(str.c_str());
		m_tokens.push_back(SpToken(new TokenDouble(m_lineNo, colNum, val)));
		break;

	case MATCH_INT:
		if (str[1] == 'x' || str[1] == 'X')
		{
			for (unsigned int i = 2; i < str.length(); i++)
			{
				char c = str[i];
				if (tolower(c) > '9')
				{
					val = 16 * val + (10 + tolower(c) - 'a');
				}
				else
				{
					val = 16 * val + (tolower(c) - '0');
				}
			}
		}
		else
		{
			val = atof(str.c_str());
		}
		m_tokens.push_back(SpToken(new TokenInt(m_lineNo, colNum, (int)val)));
		break;
	
	case MATCH_ID:
		m_tokens.push_back(SpToken(new TokenID(m_lineNo, colNum, str)));
		break;
	
	case MATCH_2OP:
		m_tokens.push_back(SpToken(new TokenID(m_lineNo, colNum, str)));
		break;

	case MATCH_STRING:
		m_tokens.push_back(SpToken(new TokenString(m_lineNo, colNum, str)));
		break;

	case MATCH_SYMBOL:
		m_tokens.push_back(SpToken(new TokenSymbol(m_lineNo, colNum, str)));
		break;

	default:
		break;
	}
}

void Lexer::errMsg(const std::string& msg)
{
	std::cerr << "line" << m_lineNo << ":" << msg << std::endl;
	exit(1);
}

#include <fstream>
void main()
{
	std::cout << "�������:" << std::endl;

	std::ifstream in;
	in.open("test.txt");
	if (!in.is_open())
	{
		return;
	}
	Lexer lex(&in);// &std::cin);//
	SpToken token = lex.read();
	while (token->type() != Token::TOKEN_EOF)
	{
		std::cout<< token->toString() <<" ";
		token = lex.read();
	}
	getchar();
}