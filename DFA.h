#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<cctype>

using namespace std;

enum Type//所有类型
{
    Reserved_Word,
    Special_Symbol,
    Identifier,
    Integer,
    Float,
    Note,
    Error//错误类型，无法识别
};

class DFA
{
public:
    int Snum;//状态数
    vector<Type> name;//每个状态对应的类型，非接收态为Error
    set<char> otherP;//其他单字符特殊符号

    DFA();
    int Trans(int state,char c);//state状态下通过c转换到的下一状态，-1表示无法转换
    Type getType(int state);//获取某个状态类型
};

#endif // DFA_H_INCLUDED
