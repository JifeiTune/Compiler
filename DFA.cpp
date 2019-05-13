#include<DFA.h>

DFA::DFA() {
	name=vector<TokenType>(17,Error);
	name[1]=Identifier;//统一处理，还需查表判断是保留字还是普通标识符
	name[2]=Integer;
	name[3]=Float;
	name[4]=Special_Symbol;//实际上为/
	name[7]=Note;//注释/**/
	name[8]=Special_Symbol;
	name[9]=Special_Symbol;
	name[10]=Special_Symbol;
	name[12]=Special_Symbol;
	name[13]=Special_Symbol;
	name[14]=Special_Symbol;
	name[15]=Special_Symbol;
	name[16]=Special_Symbol;
}

int DFA::Trans(int state,char c) {
	switch(state) {
		case 0: {
			if(isdigit(c)) { //数字
				return 2;
			} else if(isalpha(c)||c=='_') {
				return 1;
			} else if(otherP.find(c)!=otherP.end()) {
				return 16;
			} else if(c=='/') {
				return 4;
			} else if(c=='<') {
				return 8;
			} else if(c=='>') {
				return 9;
			} else if(c=='=') {
				return 10;
			} else if(c=='!') {
				return 11;
			} else {
				return -1;//无转移
			}
		}
		case 1: {
			if(isalpha(c)||isdigit(c)||c=='_') {
				return 1;
			} else {
				return -1;
			}
		}
		case 2: {
			if(isdigit(c)) {
				return 2;
			} else if(c=='.') {
				return 3;
			} else {
				return -1;
			}
		}
		case 3: {
			if(isdigit(c)) {
				return 3;
			} else {
				return -1;
			}
		}
		case 4: {
			if(c=='*') {
				return 5;
			} else {
				return -1;
			}
		}
		case 5: {
			if(c=='*') {
				return 6;
			} else {
				return 5;
			}
		}
		case 6: {
			if(c=='*') {
				return 6;
			} else if(c=='/') {
				return 7;
			} else {
				return 5;
			}
		}
		case 8: {
			if(c=='=') {
				return 12;
			} else {
				return -1;
			}
		}
		case 9: {
			if(c=='=') {
				return 13;
			} else {
				return -1;
			}
		}
		case 10: {
			if(c=='=') {
				return 14;
			} else {
				return -1;
			}
		}
		case 11: {
			if(c=='=') {
				return 15;
			} else {
				return -1;
			}
		}
		default: {
			return -1;
		}
	}
}

TokenType DFA::getType(int state) { //获取某个状态类型
	return name[state];
}
