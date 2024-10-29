#include <bits/stdc++.h>
#define MIN(a,b) ((a)<(b)?(a):(b))
using namespace std;

int main() {
    //左闭右闭地生成随机数：
    // dice = rand() % 6 + 1; （生成1~6的随机数）
    srand(time(NULL));
	//printf("%d %d\n",rand()%1000,rand()%1000);
	int n=5+rand()%4;
	int m=10+rand()%10;
	printf("%d %d\n",n,m);

    for(int i=1;i<=m;i++){
        printf("%d %d %d\n",1+rand()%n,1+rand()%n,1+rand()%5);
    }
    return 0;
}