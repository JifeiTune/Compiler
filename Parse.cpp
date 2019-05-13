#include<Parse.h>

Token voidToken=Token(Void,-1,-1);

Parse::Parse(string fileName)
{
    cout<<"Start to do Lexical Analysis...\n";
    lex=LA("in.txt");
    Token tk;
    TokenType tp;
    try
    {
        while(1)
        {
            tk=lex.getNextToken();
            //tk.printToken();
            tp=tk.type;
            if(tp!=Note)
            {
                tks.push_back(tk);
            }
        }
    }
    catch(Empty em)
    {
        TokenNum=tks.size();
        tks.push_back(Token(Error,-1,-1));//末尾放一个Error Token，以方便编程
        cout<<"Done! No errors\n";
    }
    catch(WrongT wr)//错误Token
    {
        cout<<"Error! line ";
        cout<<wr.line<<": "<<wr.pre<<'\n';
    }
}

bool Parse::match(TokenType tt,string val)
{
    Token a=tks[TokenPos];
    if(a.type==tt)
    {
        if(tt==Reserved_Word&&val==RW[a.p] || tt==Special_Symbol&&val==SS[a.p])
        {
            ++TokenPos;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

bool Parse::match(TokenType tt)
{
    Token a=tks[TokenPos];
    if(a.type==tt)
    {
        ++TokenPos;
        return 1;
    }
    return 0;
}

void Parse::backup(int &TokenBak,int &TreeBak)
{
    TokenBak=TokenPos,TreeBak=TreePos;
}

void Parse::goBack(int TokenBak,int TreeBak)
{
    TokenPos=TokenBak,TreePos=TreeBak;
    while(tks.size()>TokenPos)
    {
        tks.pop_back();
    }
    while(Tree.size()>TreePos)
    {
        Tree.pop_back();
    }
}

void Parse::printSpace(int dep)
{
    for(int i=1;i<=dep;i++)
    {
        cout<<"|   ";
    }
    cout<<'\n';
}

void Parse::addChild(TreeNode &fa,TreeNode &ch)
{
    Tree.push_back(ch);//子节点先加入
    fa.chs.push_back(TreePos);
    ++TreePos;//加了节点要增
}

void Parse::printTree(int pos,int dep)
{
    int i,len=Tree[pos].chs.size();
    for(i=1;i<dep;i++)
    {
        cout<<"|   ";
    }
    if(dep>0)
    {
        cout<<"|---";
    }

    if(Tree[pos].tk.type==Void)
    {
        cout<<Tree[pos].Kind<<'\n';
    }
    else
    {
        cout<<Tree[pos].Kind<<": ";
        Tree[pos].tk.printVal();
        cout<<'\n';
    }
    for(i=0;i<len;i++)
    {
        int temp=Tree[pos].chs[i];
        printTree(temp,dep+1);//打印子树
        if(Tree[temp].chs.size()!=0&&i!=len-1)
        {
            printSpace(dep+1);
        }
    }
}

bool Parse::declaration_list()//初始调用
{
    Tree.push_back(TreeNode("Program",voidToken));//插入虚拟节点
    TokenPos=0,TreePos=1 ;
    int pos,nowToken;
    while(TokenPos<TokenNum)//在读完前不断尝试
    {
        nowToken=TokenPos;
        if(declaration(pos))
        {
            Tree[0].chs.push_back(pos);//插入子节点
        }
        else
        {
            cout<<"Error, the follow codes don't obey the grammar\n";
            int i,ed=min(TokenNum,nowToken+5);//输出接下来的出错的语句
            cout<<"line"<<tks[nowToken].line<<": ";
            for(i=nowToken;i<ed;i++)
            {
                tks[i].printVal();
                cout<<' ';
            }
            cout<<"...\n";
            return 0;
        }
    }
    return 0;
}

bool Parse::declaration(int &pos)
{
    int TokenBak,TreeBak;//备份位置
    backup(TokenBak,TreeBak);
    if(var_declaration(pos))//一般变量声明出现概率更大，放前面
    {
        return 1;
    }
    else
    {
        goBack(TokenBak,TreeBak);
    }
    if(fun_declaration(pos))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool Parse::var_declaration(int &pos)
{
    Token n1=tks[TokenPos],n2,n3;
    if(match(Reserved_Word,"void")||match(Reserved_Word,"int"))
    {
        n2=tks[TokenPos];
        if(match(Identifier))
        {
            if(match(Special_Symbol,";"))//普通变量声明
            {
                TreeNode t1=TreeNode("Var declaration",voidToken),t2=TreeNode("Type Specifier",n1),t3=TreeNode("Identifier",n2);
                addChild(t1,t2);
                addChild(t1,t3);
                Tree.push_back(t1);
                pos=TreePos;
                ++TreePos;
                return 1;
            }
            else if(match(Special_Symbol,"["))//数组声明
            {
                n3=tks[TokenPos];
                if(match(Integer)&&match(Special_Symbol,"]")&&match(Special_Symbol,";"))//数组声明
                {
                    TreeNode t1=TreeNode("Array declaration",voidToken),t2=TreeNode("Type Specifier",n1),t3=TreeNode("Identifier",n2),t4=TreeNode("Integer",n3);
                    addChild(t1,t2);
                    addChild(t1,t3);
                    addChild(t1,t4);
                    Tree.push_back(t1);
                    pos=TreePos;
                    ++TreePos;
                    return 1;
                }
            }
        }
    }
    return 0;
}

bool Parse::fun_declaration(int &pos)
{
    Token n1=tks[TokenPos],n2,n3;
    int Ppos,Cpos,TokenBak,TreeBak;
    backup(TokenBak,TreeBak);
    if(match(Reserved_Word,"void")||match(Reserved_Word,"int"))
    {
        n2=tks[TokenPos];
        if(match(Identifier)&&match(Special_Symbol,"("))
        {
            if(params(Ppos)&&match(Special_Symbol,")"))//识别到函数
            {
                if(compound_stmt(Cpos))
                {
                    TreeNode t1=TreeNode("Fun declaration",voidToken);//函数节点
                    TreeNode t2=TreeNode("Type Specifier",n1);
                    TreeNode t3=TreeNode("Identifier",n2);
                    addChild(t1,t2);
                    addChild(t1,t3);
                    t1.chs.push_back(Ppos);//参数列表节点
                    t1.chs.push_back(Cpos);//函数体节点
                    Tree.push_back(t1);
                    pos=TreePos;
                    ++TreePos;
                    return 1;
                }

            }
        }
    }
    goBack(TokenBak,TreeBak);
    return 0;
}

bool Parse::params(int &pos)
{
    int TokenBak,TreeBak;//备份位置
    backup(TokenBak,TreeBak);
    if(param_list(pos))
    {
        return 1;//无需创建节点
    }
    goBack(TokenBak,TreeBak);
    if(match(Reserved_Word,"void"))//参数仅为void
    {
        TreeNode t=TreeNode("Void Param",voidToken);
        pos=TreePos;
        Tree.push_back(t);
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::param_list(int &pos)
{
    int Ppos,TokenBak,TreeBak;//备份位置;
    backup(TokenBak,TreeBak);
    if(param(Ppos))
    {
        TreeNode t1=TreeNode("Param List",voidToken);
        t1.chs.push_back(Ppos);//第一个参数放入
        while(match(Special_Symbol,","))//还有参数
        {
            if(param(Ppos))
            {
                t1.chs.push_back(Ppos);
            }
            else//有一个失败则错误
            {
                return 0;
            }
        }
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::param(int &pos)
{
    Token n1=tks[TokenPos],n2,n3;
    if(match(Reserved_Word,"void")||match(Reserved_Word,"int"))
    {
        n2=tks[TokenPos];
        if(match(Identifier))
        {
            TreeNode t2=TreeNode("Type Specifier",n1);
            TreeNode t3=TreeNode("Identifier",n2);
            TreeNode t1;
            if(match(Special_Symbol,"[")&&match(Special_Symbol,"]"))//数组参数
            {
                t1=TreeNode("Array Param",voidToken);
            }
            else//普通参数
            {
                t1=TreeNode("Normal Param",voidToken);
            }
            addChild(t1,t2);
            addChild(t1,t3);
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
    }
    return 0;
}

bool Parse::compound_stmt(int &pos)
{
    TreeNode t1=TreeNode("Compound Statement",voidToken);
    int LD,SL,TokenBak,TreeBak;
    backup(TokenBak,TreeBak);
    if(match(Special_Symbol,"{"))
    {
        int _TokenBak,_TreeBak;
        backup(_TokenBak,_TreeBak);
        while(var_declaration(LD))
        {
            t1.chs.push_back(LD);
            backup(_TokenBak,_TreeBak);//总是备份最后一个对的
        }
        goBack(_TokenBak,_TreeBak);//上面可能产生垃圾态，需恢复
        while(statement(SL))
        {
            if(SL!=-1)//可能为空
            {
                t1.chs.push_back(SL);
            }
            backup(_TokenBak,_TreeBak);//总是备份最后一个对的
        }
        goBack(_TokenBak,_TreeBak);//上面可能产生垃圾态，需恢复
        if(match(Special_Symbol,"}"))//完全识别成功
        {
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
    }
    goBack(TokenBak,TreeBak);
    return 0;
}

bool Parse::local_declarations(int &pos)//抽象语法树中暂且省略该节点
{

}

bool Parse::statement_list(int &pos)//抽象语法树中暂且省略该节点
{

}

bool Parse::statement(int &pos)
{
    int TokenBak,TreeBak;//备份位置
    backup(TokenBak,TreeBak);
    if(expression_stmt(pos))
    {
        return 1;
    }
    goBack(TokenBak,TreeBak);
    if(compound_stmt(pos))
    {
        return 1;
    }
    goBack(TokenBak,TreeBak);
    if(selection_stmt(pos))
    {
        return 1;
    }
    goBack(TokenBak,TreeBak);
    if(iteration_stmt(pos))
    {
        return 1;
    }
    goBack(TokenBak,TreeBak);
    if(return_stmt(pos))
    {
        return 1;
    }
    return 0;
}

bool Parse::expression_stmt(int &pos)//抽象语法树中暂且省略该节点
{
    if(match(Special_Symbol,";"))
    {
        pos=-1;//空语句
        return 1;
    }

    if(expression(pos)&&match(Special_Symbol,";"))
    {
        return 1;
    }
    return 0;
}

bool Parse::selection_stmt(int &pos)
{
    int Epos,Spos1,Spos2,TokenBak,TreeBak;//备份位置
    if(match(Reserved_Word,"if")&&match(Special_Symbol,"(")&&expression(Epos)&&match(Special_Symbol,")")&&statement(Spos1))
    {
        TreeNode t1=TreeNode("If Statement",voidToken);
        t1.chs.push_back(Epos);
        t1.chs.push_back(Spos1);
        backup(TokenBak,TreeBak);
        if(match(Reserved_Word,"else")&&statement(Spos2))//最长匹配，看看有没有else
        {
            t1.chs.push_back(Spos2);
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
        goBack(TokenBak,TreeBak);
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::iteration_stmt(int &pos)
{
    int Epos,Spos;//备份位置
    if(match(Reserved_Word,"while")&&match(Special_Symbol,"(")&&expression(Epos)&&match(Special_Symbol,")")&&statement(Spos))
    {
        TreeNode t1=TreeNode("While Statement",voidToken);
        t1.chs.push_back(Epos);
        t1.chs.push_back(Spos);
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::return_stmt(int &pos)
{
    int Epos;
    if(match(Reserved_Word,"return"))
    {
        TreeNode t1=TreeNode("Return Statement",voidToken);
        if(match(Special_Symbol,";"))//返回空
        {
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
        if(expression(Epos)&&match(Special_Symbol,";"))
        {
            t1.chs.push_back(Epos);
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
    }
    return 0;

}

bool Parse::expression(int &pos)
{
    int Vpos,Epos,TokenBak,TreeBak;
    backup(TokenBak,TreeBak);
    if(var(Vpos)&&match(Special_Symbol,"=")&&expression(Epos))//赋值语句
    {
        TreeNode t1=TreeNode("Assignment",voidToken);
        t1.chs.push_back(Vpos);
        t1.chs.push_back(Epos);
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    goBack(TokenBak,TreeBak);

    if(simple_expression(pos))
    {
        return 1;
    }
    return 0;
}

bool Parse::var(int &pos)
{
    int TokenBak,TreeBak;//备份位置
    Token n1=tks[TokenPos];
    if(match(Identifier))
    {
        TreeNode t1,t2;
        backup(TokenBak,TreeBak);
        int Epos;
        if(match(Special_Symbol,"[")&&expression(Epos)&&match(Special_Symbol,"]"))//下标运算
        {
            t1=TreeNode("Array Element",voidToken);
            t2=TreeNode("Identifier",n1);
            addChild(t1,t2);
            t1.chs.push_back(Epos);
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
        goBack(TokenBak,TreeBak);//仅为普通变量
        t1=TreeNode("Identifier",n1);
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::simple_expression(int &pos)
{
    int Lpos,Rpos,TokenBak,TreeBak;
    if(additive_expression(Lpos))
    {
        backup(TokenBak,TreeBak);
        if(relop(pos)&&additive_expression(Rpos))//有关系运算
        {
            TreeNode &mid=Tree[pos];
            mid.chs.push_back(Lpos);
            mid.chs.push_back(Rpos);
            return 1;
        }
        goBack(TokenBak,TreeBak);//只有加法表达式
        pos=Lpos;
        return 1;
    }
    return 0;
}

bool Parse::additive_expression(int &pos)
{
    int now,Lpos,Rpos;
    Token n1;
    if(term(Lpos))//加减运算符把term连接起来，注意先生成的是后生成的孩子
    {
        while(addop(now))//读到了运算符就必须读到右值
        {
            if(term(Rpos))//完成一个
            {
                Tree[now].chs.push_back(Lpos);
                Tree[now].chs.push_back(Rpos);
                Lpos=now;//变成下一次的左值
            }
            else
            {
                return 0;
            }
        }
        pos=Lpos;
        return 1;
    }
    return 0;
}

bool Parse::relop(int &pos)
{
    Token n1=tks[TokenPos];
    if(match(Special_Symbol,"<=")||match(Special_Symbol,"<")||match(Special_Symbol,">=")||match(Special_Symbol,">")||match(Special_Symbol,"==")||match(Special_Symbol,"!="))
    {
        Tree.push_back(TreeNode("Operator",n1));
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::addop(int &pos)
{
    Token n1=tks[TokenPos];
    if(match(Special_Symbol,"+")||match(Special_Symbol,"-"))
    {
        Tree.push_back(TreeNode("Operator",n1));
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::mulop(int &pos)
{
    Token n1=tks[TokenPos];
    if(match(Special_Symbol,"*")||match(Special_Symbol,"/"))
    {
        Tree.push_back(TreeNode("Operator",n1));
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}

bool Parse::term(int &pos)
{
    int now,Lpos,Rpos;
    Token n1;
    if(factor(Lpos))//乘除运算符把factor连接起来，注意先生成的是后生成的孩子
    {
        while(mulop(now))//读到了运算符就必须读到右值
        {
            if(factor(Rpos))//完成一个
            {
                Tree[now].chs.push_back(Lpos);
                Tree[now].chs.push_back(Rpos);
                Lpos=now;//变成下一次的左值
            }
            else
            {
                return 0;
            }
        }
        pos=Lpos;
        return 1;
    }
    return 0;
}

bool Parse::factor(int &pos)
{
    int TokenBak,TreeBak;
    Token n1=tks[TokenPos];
    backup(TokenBak,TreeBak);
    if(call(pos))//call和var有左公因子，把更长的call放前面
    {
        return 1;
    }
    goBack(TokenBak,TreeBak);
    if(match(Integer))
    {
        Tree.push_back(TreeNode("Integer",n1));
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    backup(TokenBak,TreeBak);
    if(match(Special_Symbol,"(")&&expression(pos)&&match(Special_Symbol,")"))//带括号的表达式
    {
        return 1;
    }
    goBack(TokenBak,TreeBak);
    if(var(pos))
    {
        return 1;
    }
    return 0;
}

bool Parse::call(int &pos)
{
    Token n1=tks[TokenPos];
    int Apos,TokenBak,TreeBak;
    if(match(Identifier)&&match(Special_Symbol,"("))
    {
        if(arg_list(Apos)&&match(Special_Symbol,")"))
        {
            TreeNode t1=TreeNode("Call",voidToken);
            TreeNode t2=TreeNode("Identifier",n1);
            addChild(t1,t2);
            t1.chs.push_back(Apos);
            Tree.push_back(t1);
            pos=TreePos;
            ++TreePos;
            return 1;
        }
    }
    return 0;
}

bool Parse::arg_list(int &pos)
{
    TreeNode t1=TreeNode("Arg List",voidToken);
    if(match(Special_Symbol,")"))//参数为空
    {
        --TokenPos;//恢复位置
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    int Epos,TokenBak,TreeBak;//备份位置;
    backup(TokenBak,TreeBak);
    if(expression(Epos))
    {
        t1.chs.push_back(Epos);//第一个参数放入
        while(match(Special_Symbol,","))//还有参数
        {
            if(expression(Epos))
            {
                t1.chs.push_back(Epos);
            }
            else//有一个失败则错误，回退
            {
                return 0;
            }
        }
        Tree.push_back(t1);
        pos=TreePos;
        ++TreePos;
        return 1;
    }
    return 0;
}
