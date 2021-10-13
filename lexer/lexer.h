/*************************************************
File: lexer.h
Describe:�ʷ�������
Author: 10127
Date: ���� 2020
Version:0.01
Other:
****************************************************/
#ifndef lexer_h__
#define lexer_h__
#include "token.h"
#include <list>
#include <regex>
#include <memory>
#include <iostream>

typedef std::shared_ptr<Token> SpToken;
typedef std::list<SpToken> Tokens;

//�ʷ�������
class Lexer
{
public:
	Lexer(std::istream* reader);
	~Lexer();

	/*Name: read
	 *Func: ��ȡһ��Token
	 *Input:bool isPeek-�Ƿ�Ԥ��ȡ
	 *output:Token����ָ�����  */
	SpToken read(bool isPeek = false);

	/*Name: read
	 *Func: Ԥ��ȡһ��Token
	 *Input:��
	 *output:Token����ָ�����  */
	SpToken peek();

private:
	//��ȡһ�в�����
	bool parserNewLine();

	//�����ַ���
	bool parser(std::string &str);

	//����Token
	void createToken(const std::string& str, int matchNo, int colNum);

	//���������Ϣ
	void errMsg(const std::string& msg);

private:
	std::istream* m_reader;		//IO��ȡ����
	std::regex* m_regex;		//������ʽ
	bool m_commentBegin;		//����ע�Ϳ�ʼ��־
	int m_lineNo;				//�к�
	Tokens m_tokens;			//token����
};


//������ʽ���ͣ�
//	����:	(0x[0-9]+)|(0X[0-9]+)|([0-9]+)
//	������:	([0-9]+.[0-9]*)
//	�ַ���:	"(\\"|\\\\|\\n|[^\"])*\"
//	ע��:	(//.*)|(/\*(.|\n)*\*/)
//			��һ��ע��//.*			//����һ��ע��
//			�ڶ���ע��/\*(.|\n)*\*/	/*����һ��ע��*/
//	��ʶ��:	[A-Z_a-z][A-Z_a-z0-9]*
//	�������:[\+\-\*%/=&!\(\);\.,]|==|>=|<=|\|\||&&|!=
//	�հ��ַ�:\s
//
//	������������ʶ������[A-Z_a-z0-9.]*

//	�ж�һ���������ַ����Ƿ���ָ�����ͣ�������ʽ���£�Ҫ��ȫƥ�䣬����ʹ����^$�޶���
//

#endif // lexer_h__
