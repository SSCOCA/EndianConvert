//词法解析器实现
#include "lexer.h"


//正则表达式对应的匹配结果
//C++11正则表达式库函数，第一个返回的是匹配成功的代码
enum MacthRet
{
	MATCH_FLOAT = 2,
	MATCH_INT,
	MATCH_ID,
	MATCH_2OP,
	MATCH_STRING,
	MATCH_COMMENT_BEGIN = 8,
	MATCH_SYMBOL
};

Lexer::Lexer(std::istream* reader) 
:m_reader(reader), m_lineNo(0), m_commentBegin(false)
{
	//先后顺序代表匹配优先级
	const std::string patStr = "\\s*("					//空白字符
		"//.*|/\\*.*\\*/|"								//单行注释
		"([0-9]+\\.[0-9]*)|"							//浮点数，浮点数放在整数之前是为了防止浮点数被整数匹配小数点前面的部分
		"(0x[0-9]+|0X[0-9]+|[0-9]+)|"					//整数
		"([A-Z_a-z][A-Z_a-z0-9]*)|"						//标识
		"(==|>=|<=|\\|\\||&&|!=|::|\\+\\+|\\-\\-)|"		//双字节运算符
		"(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")|"			//字符串	
		"(/\\*.*)|"										//多行注释开头
		"(.))";											//非字母元素
	
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
		std::cerr << "解析器输入流为空指针" << std::endl;
		exit(1);
		return false;
	}

	std::string lineStr;
	while (lineStr.empty() && !m_reader->eof())
	{
		//如果是个空行，跳过继续读，直到读到非空行为止
		getline(*m_reader, lineStr);
		m_lineNo++;
	}
	
	//如果字符串仍是空，说明读取完毕，返回结束符
	if (lineStr.empty())
	{
		return false;
	}

	//解析完成的一行
	return parser(lineStr);
}

bool Lexer::parser(std::string &lineStr)
{
	//开始对字符串进行从前往后处理
	unsigned int begin = 0;
	unsigned int end = lineStr.length();
	while (begin != end)
	{
		//查找结果
		std::smatch ret;
		
		//如果开始了多行注释，则查找相应的"*/"，没有找到前，该行直接抛弃，返回一个TokenEol
		if (m_commentBegin)
		{
			//查找能与*/对应的行
			bool found = std::regex_search(lineStr, ret, std::regex("[^(\\*/)]*\\*/"));
			if (!found)
			{
				break;
			}
			else
			{
				//如果找到，则退出多行注释，并获取*/之后的字符串起始位置
				begin += (ret.position() + ret.length());
				m_commentBegin = false;
				continue;
			}
		}

		std::string str(lineStr, begin, end);
		bool found = std::regex_search(str, ret, *m_regex);
		if (!found || ret.position() != 0)
		{
			errMsg("词法处理器无法处理词法:" + str);
			return false;
		}

		for (unsigned int i = 2; i < ret.size(); i++)
		{
			//判断是否和多行注释"/*"匹配
			if (ret[MATCH_COMMENT_BEGIN].matched)
			{
				if (!m_commentBegin)
				{
					m_commentBegin = true;
					continue;
				}
			}
			
			if (ret[i].matched)
			{
				createToken(ret[i].str(), i, begin + ret.length());
				break;
			}

		}

		//计算待匹配的字符串起始位置
		begin += (ret.position() + ret.length());
	}

	//添加一个换行符
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
		m_tokens.push_back(SpToken(new TokenID(m_lineNo, colNum, str)));
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

