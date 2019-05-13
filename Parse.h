#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include<iostream>
#include<cctype>
#include<algorithm>
#include<vector>
#include<LA.h>

using namespace std;

//语法分析



class TreeNode
{
public:
    string Kind;//节点类别
    Token tk;//产生此节点的Token
    vector<int> chs;//孩子节点的位置
    TreeNode(){}
    TreeNode(string _Kind,Token _tk):Kind(_Kind),tk(_tk){}
};

class Parse
{
public:
    LA lex;
    vector<Token> tks;//所有的Token
    int TokenNum;//Token总数
    int TokenPos;//当前处理的Token位置
    vector<TreeNode> Tree;//语法树
    int TreePos;//当前将插入语法树的节点位置

    Parse(string fileName);
    bool match(TokenType tt,string val);//判断当前Token是否匹配，匹配则TokenPos自增
    bool match(TokenType tt);
    void backup(int &TokenBak,int &TreeBak);//备份Token和Tree的位置
    void goBack(int TokenBak,int TreeBak);//当一条路走不通时，回退状态，即清空垃圾节点，使得不影响其他路的搜索
    void addChild(TreeNode &fa,TreeNode &ch);//向树中插入某个节点的子节点，TreePos自动增加
    void printTree(int pos,int dep);//将语法树打印出来，参数为当前位置和在树上的深度
    void printSpace(int dep);

    //非终结符对应的函数，返回当前路径是否可走，pos为生成的子节点位置
    bool declaration_list();//初始调用
    bool declaration(int &pos);
    bool var_declaration(int &pos);//变量定义
    bool fun_declaration(int &pos);//函数定义
    bool compound_stmt(int &pos);//函数体
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
    bool expression(int &pos);//表达式
    bool var(int &pos);
    bool simple_expression(int &pos);
    bool additive_expression(int &pos);//加法表达式
    bool relop(int &pos);//关系运算符
    bool addop(int &pos);//加法运算符（+、-）
    bool mulop(int &pos);//乘法运算符（*、/）
    bool term(int &pos);//关系运算符
    bool factor(int &pos);//加法运算符（+、-）
    bool call(int &pos);//函数调用
    bool arg_list(int &pos);//参数列表
    /*



    bool declaration_list(int &pos);
    bool declaration_list(int &pos);
    bool declaration_list(int &pos);*/
};

#endif // PARSE_H_INCLUDED
