/*************************************************
*File: token.h
*Desc: �ʷ�����Ԫ�ض���
*Author: SSCOCA
*Date: 2020/02/28
*Version:
*Other:
****************************************************/
#ifndef token_h__
#define token_h__
#include <string>

//Token����
class Token
{
public:
	enum TokenType
	{
		TOKEN_NA = 0,		//NA
		TOKEN_INT,			//����
		TOKEN_DOUBLE,		//������
		TOKEN_STRING,		//�ַ���
		TOKEN_IDENTIFIER,	//��ʶ��
		TOKEN_EOF,			//������
		TOKEN_EOL,			//���з�
	};

	//���캯��
	Token(int lineNo, int colNo, TokenType type):m_lineNo(lineNo), m_colNo(colNo),m_type(type){}
	~Token(){};

	/*Name:type
	 *Func:����Token����
	 *Input:��
	 *output:Token����  */
	TokenType type(){ return m_type; }

	/*Name: lineNo
	 *Func: ����Token�����к�
	 *Input:��
	 *output:Token�����к�  */
	int lineNo(){ return m_lineNo; }

	/*Name: colNo
	 *Func: ����Token�����к�
	 *Input:��
	 *output:Token�����к�  */
	unsigned int colNo(){ return m_colNo; }

	/*Name:toString
	 *Func:����Token��Ӧ�ַ���
	 *Input:��
	 *output:Token��Ӧ���ַ��� */
	virtual std::string toString() = 0;

private:
	TokenType m_type;	//Token����
	int m_lineNo;		//Token�к�
	int m_colNo;		//Token�к�
};

//����Token
class TokenInt: public Token
{
public:
	TokenInt(int lineNo, int colNo, int val) :Token(lineNo, colNo, TOKEN_INT), m_val(val){}
	~TokenInt(){}

	//����Token��Ӧ��ֵ
	int value(){ return m_val; }

	//ת��Ϊ�ַ���
	virtual std::string toString(){ return std::to_string(m_val); }

private:
	int m_val;
};

//������Token
class TokenDouble : public Token
{
public:
	TokenDouble(int lineNo, int colNo, double val):Token(lineNo, colNo, TOKEN_DOUBLE), m_val(val){}
	~TokenDouble(){}

	//����Token��Ӧ��ֵ
	double value(){ return m_val; }

	//ת��Ϊ�ַ���
	virtual std::string toString(){ return std::to_string(m_val); }

private:
	double m_val;
};

class TokenString : public Token
{
public:
	TokenString(int lineNo, int colNo, const std::string& str):Token(lineNo, colNo, TOKEN_IDENTIFIER), m_str(str){}
	~TokenString(){}

	//����Token��Ӧ��ֵ
	std::string value(){ return m_str; }

	//ת��Ϊ�ַ���
	virtual std::string toString(){ return m_str; }

private:
	std::string m_str;
};


class TokenID : public Token
{
public:
	TokenID(int lineNo, int colNo, const std::string& id):Token(lineNo, colNo, TOKEN_STRING), m_id(id){}

	~TokenID(){}

	//����Token��Ӧ��ֵ
	std::string value(){ return m_id; }

	//ת��Ϊ�ַ���
	virtual std::string toString(){ return m_id; }

private:
	std::string m_id;
};

//�н�����Token
class TokenEol : public Token
{
public:
	TokenEol(int lineNo, int colNo) :Token(lineNo, colNo, TOKEN_EOL){}
	~TokenEol(){}

	//ת��Ϊ�ַ���
	virtual std::string toString(){ return "\n"; }
};

//�ļ�������Token
class TokenEof : public Token
{
public:
	TokenEof() :Token(0, 0, TOKEN_EOF){}
	~TokenEof(){}

	//ת��Ϊ�ַ���
	virtual std::string toString(){ return NULL; }
};


#endif // token_h__
