/**********************************/
/*         编译原理 实验三        */
/*           LR语法分析           */
/*        冯业博 121805104        */
/*            计科1202            */
/**********************************/

#include<stdio.h>
#include<string.h>

char inputString[100];
int stringPoint=0;
int flag=1;
char word[3];

struct myStack{
	int stack[100];
	int top;
};

struct myStack analysis1;
struct myStack analysis2;

struct table{
	char *keyWord;
	int nextStep;
} LR[]={
	{"0(",2},{"0i",3},{"0E",1},{"1+",4},
	{"1*",5},{"1#",0},{"2(",2},{"2i",3},
	{"2E",6},{"3+",14},{"3*",14},{"3)",14},
	{"3#",14},{"4(",2},{"4i",3},{"4E",7},
	{"5(",2},{"5i",3},{"5E",8},{"6+",4},
	{"6*",5},{"6)",9},{"7+",11},{"7*",5},
	{"7)",11},{"7#",11},{"8+",12},{"8*",12},
	{"8)",12},{"8#",12},{"9+",13},{"9*",13},
	{"9)",13},{"9#",13},{"EE",0}/*最后一个标志表的结束*/
};

void input(){
	int i=0;
	scanf("%c",&inputString[i]);
	while(inputString[i]!='\n'){
		i++;
		scanf("%c",&inputString[i]);
	}
	inputString[i]='#';
}

void push1(char a) {analysis1.stack[++analysis1.top]=a;}
void push2(char a) {analysis2.stack[++analysis2.top]=a;}

void pop1() {analysis1.top--;}
void pop2() {analysis2.top--;}

void contact(char a, char b){
	word[0]=a;
	word[1]=b;
	word[2]='\0';
}

int search(char *word){
	int n=0,i=0;
	while(strcmp(LR[i].keyWord,"EE")!=0)
		i++;
	while(strcmp(LR[n].keyWord,word)!=0 && n!=i)
		n++;
	if(n==i)
		return -1;
	else 
		return n;
}

int ifDigitChar(char a){
	if((a>='0' && a<='9') || (a>='a' && a<='z') || (a>='A' && a<='Z'))
		return 1;
	else
		return 0;
}

void error(){
	flag=0;
	printf("出错！\n");
	printf("第%d个符号不符合规范！\n",stringPoint);
}

void analyse(){
	int n;
	int flag2=1;
	while(flag && flag2){
		if(ifDigitChar(inputString[stringPoint])){
			while(ifDigitChar(inputString[stringPoint]))
				stringPoint++;
			inputString[--stringPoint]='i';
		}
		contact(analysis1.stack[analysis1.top],inputString[stringPoint++]);
		n=search(word);
		if(n==-1)
			error();
		else if(LR[n].nextStep==0)
			flag2=0;
		else if(LR[n].nextStep>=1 && LR[n].nextStep<=9){
			push1('0'+LR[n].nextStep);
			push2(word[1]);
		}
		else{
			switch(LR[n].nextStep){
			case 11:
			case 12:
			case 13:
				{
					for(int i=1;i<=3;i++){
						pop1();
						pop2();
					}
					contact(analysis1.stack[analysis1.top],'E');
					n=search(word);
					if(n==-1)
						error();
					else{
						push1(LR[n].nextStep+'0');
						push2('E');
					}
					stringPoint--;
					break;
				}
			case 14:
				{
					pop1();
					pop2();
					contact(analysis1.stack[analysis1.top],'E');
					n=search(word);
					if(n==-1)
						error();
					else{
						push1(LR[n].nextStep+'0');
						push2('E');
					}
					stringPoint--;
				}
			}
		}
	}
}

void init(){
	analysis1.top=0;
	analysis1.stack[analysis1.top]='0';
	analysis2.top=0;
	analysis2.stack[analysis2.top]='#';
}

int main()
{
	printf("请输入一个关于标示符与整数的算术表达式（不包括减法、除法运算）！\n");
	input();
	//测试//printf("%s",inputString);
	init();
	analyse();
	if(flag==1)
		printf("表达式正确，分析成功！\n");
	return 1;
}