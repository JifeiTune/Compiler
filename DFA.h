#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<cctype>
#include<Globle.h>

using namespace std;

enum TokenType//����Token����
{
    Reserved_Word,
    Special_Symbol,
    Identifier,
    Integer,
    Float,
    Note,
    Error,//�������ͣ��޷�ʶ��
    Void//�գ���ֵ�������﷨��
};

class DFA
{
public:
    int Snum;//״̬��
    vector<TokenType> name;//ÿ��״̬��Ӧ�����ͣ��ǽ���̬ΪError
    set<char> otherP;//�������ַ��������

    DFA();
    int Trans(int state,char c);//state״̬��ͨ��cת��������һ״̬��-1��ʾ�޷�ת��
    TokenType getType(int state);//��ȡĳ��״̬����
};

#endif // DFA_H_INCLUDED
