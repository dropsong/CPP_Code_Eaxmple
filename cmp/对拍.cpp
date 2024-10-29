//递归中retrun 就是返回上层调用的地方！！！
#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
	int t=666;
	while(--t)
        {
		system("data.exe > data.txt");
		system("std.exe < data.txt > std.txt");
		system("my.exe < data.txt > my.txt");
		if(system("fc my.txt std.txt"))
			break;
	}  
	if(t==0)
		cout<<"no error"<<endl;
	else
		cout<<"error"<<endl;
		
	system("pause");
	return 0;
}  
