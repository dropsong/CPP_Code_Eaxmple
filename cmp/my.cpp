#include<cstdio>
#include<cstring>
#define MIN(a,b) ((a)<(b)?(a):(b))
using namespace std;

const int maxn = 103;
const int maxm = 5003;
const int INF = 0x7fffffff;
int head[maxn],tot,depth[maxn],fa[maxn],ans,fa2[maxn],dis[maxn][maxn];
int low[maxn],dfn[maxn],stac[maxn],num,n,m,top,uset[maxn];
struct Edge{int v,w,next;}edge[maxm<<1];
bool vis[maxn],ins[maxn];

void makeset(int size){
    for(int i=0;i<size;i++)uset[i]=i;
}

int find(int x){
    if(x!=uset[x])uset[x]=find(uset[x]);
    return uset[x];
}

void unionset(int x,int y){
    x=find(x);
    y=find(y);
    if(x==y)return;
    uset[x]=y;
}

inline void addedge(int x,int y,int z){
    edge[++tot].v=y;
    edge[tot].w=z;
    edge[tot].next=head[x];
    head[x]=tot;
}

void tarjan(int x){
    low[x]=dfn[x]=++num;
    stac[++top]=x; ins[x]=true;
    for(int i=head[x];i;i=edge[i].next){
        int y=edge[i].v;
        if(!dfn[y]){
            fa2[y]=x;
            tarjan(y);
            low[x]=MIN(low[x],low[y]);
        }
        else if(ins[y]&&y!=fa2[x]){ //啊啊啊&&y!=fa2[x]
            low[x]=MIN(low[x],dfn[y]);
        }
    }
    if(dfn[x]==low[x]){
        int y;
        do{
            y=stac[top--];
            ins[y]=false;
            unionset(x,y);
        }while(x!=y);
    }
}

void dfs(int x,int dep){ //可能有重边
    vis[x]=true;
    depth[x]=dep;
    for(int i=head[x];i;i=edge[i].next){
        int y=edge[i].v;
        if(!vis[y]){
            fa[y]=x;
            dfs(y,depth[x]+dis[x][y]);
        }
        else if(y!=fa[x]&&(depth[x]-depth[y]>0)&&(find(x)==find(y))){
            ans=MIN(ans,dis[x][y]+depth[x]-depth[y]);
        }
    }
    return;
}

int main(){
    scanf("%d%d",&n,&m);
    ans=INF;
    makeset(n+1);
    for(int i=1;i<=m;i++){
        int x,y,z;
        scanf("%d%d%d",&x,&y,&z);
        addedge(x,y,z);
        addedge(y,x,z); //以下重边处理，留下最小权值的边
        dis[x][y]=dis[y][x]=((!dis[x][y])?z:MIN(dis[x][y],z));
    }

    for(int i=1;i<=n;i++)
        if(!dfn[i])tarjan(i);
    /*
    printf("\n");
    for(int i=1;i<=n;i++){
        printf("%d %d %d %d\n",i,dfn[i],low[i],find(i));
    }
    return 0;
    
    for(int i=1;i<=n;i++){
        if(!vis[i])dfs(i,0);
    }
    */
    for(int s=1;s<=n;s++){
        memset(fa,0,sizeof(fa));
        memset(vis,false,sizeof(vis));
        memset(depth,0,sizeof(depth));
        dfs(s,0); int i=s+1;
        while(i<=n){
            if(!vis[i])dfs(i,0);
            i++;
        }
        int j=1;
        while(j<s){
            if(!vis[j])dfs(j,0);
            j++;
        }
    }

    if(ans==INF)printf("No solution.");
    else printf("%d",ans);
    return 0;
}