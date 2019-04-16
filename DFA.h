#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<cctype>

using namespace std;

enum Type//��������
{
    Reserved_Word,
    Special_Symbol,
    Identifier,
    Integer,
    Float,
    Note,
    Error//�������ͣ��޷�ʶ��
};

class DFA
{
public:
    int Snum;//״̬��
    vector<Type> name;//ÿ��״̬��Ӧ�����ͣ��ǽ���̬ΪError
    set<char> otherP;//�������ַ��������

    DFA();
    int Trans(int state,char c);//state״̬��ͨ��cת��������һ״̬��-1��ʾ�޷�ת��
    Type getType(int state);//��ȡĳ��״̬����
};

#endif // DFA_H_INCLUDED
