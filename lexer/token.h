/*************************************************
*File: token.h
*Desc: 词法基本元素定义
*Author: SSCOCA
*Date: 2020/02/28
*Version:
*Other:
****************************************************/
#ifndef token_h__
#define token_h__
#include <string>

//Token基类
class Token
{
public:
	enum TokenType
	{
		TOKEN_NA = 0,		//NA
		TOKEN_INT,			//整型
		TOKEN_DOUBLE,		//浮点数
		TOKEN_STRING,		//字符串
		TOKEN_IDENTIFIER,	//标识符
		TOKEN_EOF,			//结束符
		TOKEN_EOL,			//换行符
	};

	//构造函数
	Token(int lineNo, int colNo, TokenType type):m_lineNo(lineNo), m_colNo(colNo),m_type(type){}
	~Token(){};

	/*Name:type
	 *Func:返回Token类型
	 *Input:无
	 *output:Token类型  */
	TokenType type(){ return m_type; }

	/*Name: lineNo
	 *Func: 返回Token所在行号
	 *Input:无
	 *output:Token所在行号  */
	int lineNo(){ return m_lineNo; }

	/*Name: colNo
	 *Func: 返回Token所在列号
	 *Input:无
	 *output:Token所在列号  */
	unsigned int colNo(){ return m_colNo; }

	/*Name:toString
	 *Func:返回Token对应字符串
	 *Input:无
	 *output:Token对应的字符串 */
	virtual std::string toString() = 0;

private:
	TokenType m_type;	//Token类型
	int m_lineNo;		//Token行号
	int m_colNo;		//Token列号
};

//整数Token
class TokenInt: public Token
{
public:
	TokenInt(int lineNo, int colNo, int val) :Token(lineNo, colNo, TOKEN_INT), m_val(val){}
	~TokenInt(){}

	//返回Token对应的值
	int value(){ return m_val; }

	//转换为字符串
	virtual std::string toString(){ return std::to_string(m_val); }

private:
	int m_val;
};

//浮点数Token
class TokenDouble : public Token
{
public:
	TokenDouble(int lineNo, int colNo, double val):Token(lineNo, colNo, TOKEN_DOUBLE), m_val(val){}
	~TokenDouble(){}

	//返回Token对应的值
	double value(){ return m_val; }

	//转换为字符串
	virtual std::string toString(){ return std::to_string(m_val); }

private:
	double m_val;
};

class TokenString : public Token
{
public:
	TokenString(int lineNo, int colNo, const std::string& str):Token(lineNo, colNo, TOKEN_IDENTIFIER), m_str(str){}
	~TokenString(){}

	//返回Token对应的值
	std::string value(){ return m_str; }

	//转换为字符串
	virtual std::string toString(){ return m_str; }

private:
	std::string m_str;
};


class TokenID : public Token
{
public:
	TokenID(int lineNo, int colNo, const std::string& id):Token(lineNo, colNo, TOKEN_STRING), m_id(id){}

	~TokenID(){}

	//返回Token对应的值
	std::string value(){ return m_id; }

	//转换为字符串
	virtual std::string toString(){ return m_id; }

private:
	std::string m_id;
};

//行结束符Token
class TokenEol : public Token
{
public:
	TokenEol(int lineNo, int colNo) :Token(lineNo, colNo, TOKEN_EOL){}
	~TokenEol(){}

	//转换为字符串
	virtual std::string toString(){ return "\n"; }
};

//文件结束符Token
class TokenEof : public Token
{
public:
	TokenEof() :Token(0, 0, TOKEN_EOF){}
	~TokenEof(){}

	//转换为字符串
	virtual std::string toString(){ return NULL; }
};


#endif // token_h__
