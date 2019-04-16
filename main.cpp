#include<iostream>
#include<DFA.h>
#include<LA.h>
#include<cctype>
#include<algorithm>
#include<set>
using namespace std;

int main()
{
    LA lex("in.txt");
    Token tk;
    Type tp;
    try
    {
        while(1)
        {
            tk=lex.getNextToken();
            tp=tk.type;
            cout<<"line"<<tk.line<<' ';
            switch(tp)
            {
            case Identifier:
                {
                    cout<<"Identifier: "<<lex.ID[tk.p];
                    break;
                }
            case Reserved_Word:
                {
                    cout<<"Reserved_Word: "<<lex.RW[tk.p];
                    break;
                }
            case Special_Symbol:
                {
                    cout<<"Special_Symbol: "<<lex.SS[tk.p];
                    break;
                }
            case Integer:
                {
                    cout<<"Integer: "<<lex.Integers[tk.p];
                    break;
                }
            case Float:
                {
                    cout<<"Float: "<<lex.Floats[tk.p];
                    break;
                }
            case Note://实际使用时应该忽略
                {
                    cout<<"Note";
                }
            }
            cout<<'\n';
        }

    }
    catch(Empty em)
    {
        cout<<"Done! \n";
    }
    catch(WrongT wr)//错误Token
    {
        cout<<"Error! line ";
        cout<<wr.line<<": "<<wr.pre<<'\n';
    }

    return 0;
}



