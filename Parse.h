#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include<iostream>
#include<cctype>
#include<algorithm>
#include<vector>
#include<LA.h>

using namespace std;

//�﷨����



class TreeNode
{
public:
    string Kind;//�ڵ����
    Token tk;//�����˽ڵ��Token
    vector<int> chs;//���ӽڵ��λ��
    TreeNode(){}
    TreeNode(string _Kind,Token _tk):Kind(_Kind),tk(_tk){}
};

class Parse
{
public:
    LA lex;
    vector<Token> tks;//���е�Token
    int TokenNum;//Token����
    int TokenPos;//��ǰ�����Tokenλ��
    vector<TreeNode> Tree;//�﷨��
    int TreePos;//��ǰ�������﷨���Ľڵ�λ��

    Parse(string fileName);
    bool match(TokenType tt,string val);//�жϵ�ǰToken�Ƿ�ƥ�䣬ƥ����TokenPos����
    bool match(TokenType tt);
    void backup(int &TokenBak,int &TreeBak);//����Token��Tree��λ��
    void goBack(int TokenBak,int TreeBak);//��һ��·�߲�ͨʱ������״̬������������ڵ㣬ʹ�ò�Ӱ������·������
    void addChild(TreeNode &fa,TreeNode &ch);//�����в���ĳ���ڵ���ӽڵ㣬TreePos�Զ�����
    void printTree(int pos,int dep);//���﷨����ӡ����������Ϊ��ǰλ�ú������ϵ����
    void printSpace(int dep);

    //���ս����Ӧ�ĺ��������ص�ǰ·���Ƿ���ߣ�posΪ���ɵ��ӽڵ�λ��
    bool declaration_list();//��ʼ����
    bool declaration(int &pos);
    bool var_declaration(int &pos);//��������
    bool fun_declaration(int &pos);//��������
    bool compound_stmt(int &pos);//������
    bool params(int &pos);
    bool param_list(int &pos);
    bool param(int &pos);
    bool local_declarations(int &pos);
    bool statement_list(int &pos);
    bool statement(int &pos);
    bool expression_stmt(int &pos);
    bool selection_stmt(int &pos);
    bool iteration_stmt(int &pos);
    bool return_stmt(int &pos);
    bool expression(int &pos);//���ʽ
    bool var(int &pos);
    bool simple_expression(int &pos);
    bool additive_expression(int &pos);//�ӷ����ʽ
    bool relop(int &pos);//��ϵ�����
    bool addop(int &pos);//�ӷ��������+��-��
    bool mulop(int &pos);//�˷��������*��/��
    bool term(int &pos);//��ϵ�����
    bool factor(int &pos);//�ӷ��������+��-��
    bool call(int &pos);//��������
    bool arg_list(int &pos);//�����б�
    /*



    bool declaration_list(int &pos);
    bool declaration_list(int &pos);
    bool declaration_list(int &pos);*/
};

#endif // PARSE_H_INCLUDED
