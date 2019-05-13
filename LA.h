#ifndef LA_H_INCLUDED
#define LA_H_INCLUDED
#include<DFA.h>
#include<vector>
#include<string>
#include<map>
#include<cstdlib>
using namespace std;

/*
�ʷ�����
*/
extern vector<string> RW;//�����ֱ�
extern vector<string> SS;//������ű�
extern vector<string> ID;//��ʶ����
extern vector<int> Integers;//������
extern vector<double> Floats;//��������

//ÿ�����͵�ֵ�ڸ��ַ��ű��е��±꣬�жϴ�����ȥ�أ��������Բ���
extern map<string,int> RWmp;
extern map<string,int> SSmp;
extern map<string,int> IDmp;
extern map<int,int> Imp;
extern map<double,int> Fmp;


class Token//�ʷ���Ԫ
{
public:
    TokenType type;
    int p;//ָ���Ӧ���ű��ָ��
    int line;//�����к�

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
    void printToken();//��ӡToken���ͺ�����
    void printType();//��ӡToken����
    void printVal();//��ӡToken����
};

class Empty//�����쳣
{

};

class WrongT//����Token�쳣
{
public:
    string pre;//����ǰ������
    int line;//���������к�

    WrongT(string _pre,int _line):pre(_pre),line(_line){}
};



class LA
{
public:
    DFA dfa;
    string fileName;
    char buf[1000001];//������
    int len;//Դ�볤��
    int p;//��ǰָ��λ��
    int line;//��ǰ������

    LA();
    LA(string fileName);
    Token getNextToken();//��ȡ��һ��token���������ڣ�����ʶ����󶼻��׳��쳣
};


#endif // LA_H_INCLUDED
