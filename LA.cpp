#include<LA.h>

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
    case Note://ʵ��ʹ��ʱӦ�ú���
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
    for(i=0;i<Rnum;i++)//���뱣����
    {
        cin>>s;
        RW.push_back(s);
        RWmp[s]=i;
    }
    cin>>Snum;
    for(i=0;i<Snum;i++)//�����������
    {
        cin>>s;
        SS.push_back(s);
        SSmp[s]=i;
    }

    cin>>Snum;
    for(i=0;i<Snum;i++)//�����������
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
    freopen("CON","r",stdin);//�ָ��ض���
    cin.clear();
	cin.sync();
}

Token LA::getNextToken()//����Ԥ����\0
{
    while(p<=len&&isspace(buf[p]))
    {
        if(buf[p]=='\n')
        {
            ++line;//���µ�ǰ�к�
        }
        ++p;//���Կ�ͷ�Ŀհ׷�
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
        if(to==-1)//����
        {
            tp=dfa.getType(state);
            switch(tp)
            {
            case Identifier://ͳһ����ʶ�������������Ǳ�����
                {
                    if(RWmp.find(ans)!=RWmp.end())//�Ǳ�����
                    {
                        return Token(Reserved_Word,RWmp[ans],line);
                    }
                    else
                    {
                        if(IDmp.find(ans)!=IDmp.end())//���ֹ��ı�ʶ��
                        {
                            return Token(Identifier,IDmp[ans],line);
                        }
                        else//������ʶ��
                        {
                            ID.push_back(ans);//�������
                            pos=ID.size()-1;
                            IDmp[ans]=pos;//����ӳ��
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
            case Note://ע�ͣ�ʵ����ע��Ӧ�ú��ԣ�����ʦҪ��ʧ�ܲ�չʾ��������
                {
                    return Token(Note,-1,line);
                }
            case Error:
                {
                    ans+=" ! ";
                    ans+=buf[p];//��ͷ��ָΪ�����
                    WrongT wt=WrongT(ans,line);
                    throw wt;
                }
            }

        }
        else
        {
            state=to;//����״̬
            ans+=buf[p];
        }
        if(buf[p]=='\n') ++line;
        ++p;
    }
}


