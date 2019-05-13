#ifndef LA_H_INCLUDED
#define LA_H_INCLUDED
#include<DFA.h>
#include<vector>
#include<string>
#include<map>
#include<cstdlib>
using namespace std;

/*
词法分析
*/
extern vector<string> RW;//保留字表
extern vector<string> SS;//特殊符号表
extern vector<string> ID;//标识符表
extern vector<int> Integers;//整数表
extern vector<double> Floats;//浮点数表

//每种类型的值在各种符号表中的下标，判断存在与去重，避免线性查找
extern map<string,int> RWmp;
extern map<string,int> SSmp;
extern map<string,int> IDmp;
extern map<int,int> Imp;
extern map<double,int> Fmp;


class Token//词法单元
{
public:
    TokenType type;
    int p;//指向对应符号表的指针
    int line;//所在行号

    Token(){}
    Token(TokenType _type,int _p,int _line):type(_type),p(_p),line(_line){}
    bool operator==(const Token &r) const
    {
        if(type==r.type&&p==r.p)
        {
            return 1;
        }
        return 0;
    }
    void printToken();//打印Token类型和内容
    void printType();//打印Token类型
    void printVal();//打印Token内容
};

class Empty//读完异常
{

};

class WrongT//错误Token异常
{
public:
    string pre;//出错前读到的
    int line;//错误所在行号

    WrongT(string _pre,int _line):pre(_pre),line(_line){}
};



class LA
{
public:
    DFA dfa;
    string fileName;
    char buf[1000001];//缓冲区
    int len;//源码长度
    int p;//当前指针位置
    int line;//当前所在行

    LA();
    LA(string fileName);
    Token getNextToken();//获取下一个token，若不存在，或者识别错误都会抛出异常
};


#endif // LA_H_INCLUDED
