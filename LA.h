#ifndef LA_H_INCLUDED
#define LA_H_INCLUDED
#include<DFA.h>
#include<vector>
#include<string>
#include<map>
#include<cstdlib>
using namespace std;

class Token//�ʷ���Ԫ
{
public:
    Type type;
    int p;//ָ���Ӧ���ű��ָ��
    int line;//�����к�

    Token(){}
    Token(Type _type,int _p,int _line):type(_type),p(_p),line(_line){}
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
    vector<string> RW;//�����ֱ�
    vector<string> SS;//������ű�
    vector<string> ID;//��ʶ����
    vector<int> Integers;//������
    vector<double> Floats;//��������

    //ÿ�����͵�ֵ�ڸ��ַ��ű��е��±꣬�жϴ�����ȥ�أ��������Բ���
    map<string,int> RWmp;
    map<string,int> SSmp;
    map<string,int> IDmp;
    map<int,int> Imp;
    map<double,int> Fmp;


    DFA dfa;
    string fileName;
    char buf[1000001];//������
    int len;//Դ�볤��
    int p;//��ǰָ��λ��
    int line;//��ǰ������

    LA(string fileName);
    Token getNextToken();//��ȡ��һ��token���������ڣ�����ʶ����󶼻��׳��쳣
};


#endif // LA_H_INCLUDED
