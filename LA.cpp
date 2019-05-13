#include<LA.h>

vector<string> RW;//保留字表
vector<string> SS;//特殊符号表
vector<string> ID;//标识符表
vector<int> Integers;//整数表
vector<double> Floats;//浮点数表

//每种类型的值在各种符号表中的下标，判断存在与去重，避免线性查找
map<string,int> RWmp;
map<string,int> SSmp;
map<string,int> IDmp;
map<int,int> Imp;
map<double,int> Fmp;



void Token::printType()
{
    switch(type)
    {
    case Identifier:
        {
            cout<<"Identifier";
            break;
        }
    case Reserved_Word:
        {
            cout<<"Reserved_Word";
            break;
        }
    case Special_Symbol:
        {
            cout<<"Special_Symbol";
            break;
        }
    case Integer:
        {
            cout<<"Integer";
            break;
        }
    case Float:
        {
            cout<<"Float";
            break;
        }
    case Note://实际使用时应该忽略
        {
            cout<<"Note";
        }
    }
}

void Token::printVal()
{
    switch(type)
    {
    case Identifier:
        {
            cout<<ID[p];
            break;
        }
    case Reserved_Word:
        {
            cout<<RW[p];
            break;
        }
    case Special_Symbol:
        {
            cout<<SS[p];
            break;
        }
    case Integer:
        {
            cout<<Integers[p];
            break;
        }
    case Float:
        {
            cout<<Floats[p];
            break;
        }
    }
}

void Token::printToken()
{
    cout<<"line"<<line<<' ';
    printType();
    cout<<": ";
    printVal();
    cout<<'\n';
}

LA::LA()
{

}

LA::LA(string fileName)
{
    this->fileName=fileName;

    freopen("DFA.txt","r",stdin);
    int Rnum,Snum,i;
    string s;
    char c;
    cin>>Rnum;
    for(i=0;i<Rnum;i++)//读入保留字
    {
        cin>>s;
        RW.push_back(s);
        RWmp[s]=i;
    }
    cin>>Snum;
    for(i=0;i<Snum;i++)//读入特殊符号
    {
        cin>>s;
        SS.push_back(s);
        SSmp[s]=i;
    }

    cin>>Snum;
    for(i=0;i<Snum;i++)//读入特殊符号
    {
        cin>>c;
        dfa.otherP.insert(c);
    }

    freopen((this->fileName).c_str(),"r",stdin);
    p=0;
    while((c=getchar())!=EOF)
    {
        buf[p++]=c;
    }
    buf[p]='\0';
    len=p;
    p=0;
    line=1;
    freopen("CON","r",stdin);//恢复重定向
    cin.clear();
	cin.sync();
}

Token LA::getNextToken()//允许预读到\0
{
    while(p<=len&&isspace(buf[p]))
    {
        if(buf[p]=='\n')
        {
            ++line;//更新当前行号
        }
        ++p;//忽略开头的空白符
    }
    if(p>=len)
    {
        Empty ans;
        throw ans;
    }
    int state=0,to;
    TokenType tp;
    string ans="";
    int pos;
    while(p<=len)
    {
        to=dfa.Trans(state,buf[p]);
        if(to==-1)//回退
        {
            tp=dfa.getType(state);
            switch(tp)
            {
            case Identifier://统一当标识符处理，但可能是保留字
                {
                    if(RWmp.find(ans)!=RWmp.end())//是保留字
                    {
                        return Token(Reserved_Word,RWmp[ans],line);
                    }
                    else
                    {
                        if(IDmp.find(ans)!=IDmp.end())//出现过的标识符
                        {
                            return Token(Identifier,IDmp[ans],line);
                        }
                        else//新增标识符
                        {
                            ID.push_back(ans);//插入表中
                            pos=ID.size()-1;
                            IDmp[ans]=pos;//更新映射
                            return Token(Identifier,pos,line);
                        }
                    }
                }
            case Integer:
                {
                    int val=atoi(ans.c_str());
                    if(Imp.find(val)!=Imp.end())
                    {
                        return Token(Integer,Imp[val],line);
                    }
                    else
                    {
                        Integers.push_back(val);
                        pos=Integers.size()-1;
                        Imp[val]=pos;
                        return Token(Integer,pos,line);
                    }
                }
            case Float:
                {
                    double val=atof(ans.c_str());
                    if(Fmp.find(val)!=Fmp.end())
                    {
                        return Token(Float,Fmp[val],line);
                    }
                    else
                    {
                        Floats.push_back(val);
                        pos=Floats.size()-1;
                        Fmp[val]=pos;
                        return Token(Float,pos,line);
                    }
                }
            case Special_Symbol:
                {
                    return Token(Special_Symbol,SSmp[ans],line);
                }
            case Note://注释，实际上注释应该忽略，但老师要求失败并展示出来……
                {
                    return Token(Note,-1,line);
                }
            case Error:
                {
                    ans+=" ! ";
                    ans+=buf[p];//箭头所指为错误点
                    WrongT wt=WrongT(ans,line);
                    throw wt;
                }
            }

        }
        else
        {
            state=to;//更新状态
            ans+=buf[p];
        }
        if(buf[p]=='\n') ++line;
        ++p;
    }
}


