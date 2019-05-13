#include<iostream>
#include<cctype>
#include<algorithm>
#include<set>
#include<Parse.h>

using namespace std;

int main()
{
    //ios::sync_with_stdio(false);
    Parse ps("in.txt");
    ps.declaration_list();
    cout<<endl;
    freopen("Tree.txt","w",stdout);
    ps.printTree(0,0);
    freopen("CON","w",stdout);
    system("C:\\Users\\Administrator\\Desktop\\uedit32.exe.lnk E:\\School\\C++\\Compiler\\Tree.txt");
    return 0;
}



