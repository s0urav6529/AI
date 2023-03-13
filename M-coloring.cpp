#include<bits/stdc++.h>
using namespace std;
int n , e;
vector<int>adj[100];
int color[100][5];
int constr[100];
int colr[100];
int vis[100];

bool dfs(int node , int col){

    vis[node] = 1;
    colr[node] = col;
    //cout << node <<" "<<col<<endl;

    for(int child:adj[node]){

        if(vis[child] == false){

            for(int k=1;k<=4;k++){

                if(color[child][k] == 1){
                    bool ok = true;
                    for(int ch:adj[child]) {
                        if(k == colr[ch]) {
                            ok = false;
                        }
                    }
                    if(ok && dfs(child,k) == false){
                        return false;
                    }
                }
            }
        }
        else{
            if(colr[node] == colr[child]){
                return false;
            }
        }
    }
    return true;

}

bool bfs(int node , int col){

    priority_queue<pair<int,int>>pq;
    colr[node] = col;
    pq.push({constr[node],node});
    while(!pq.empty()){

        int node = pq.top().second;
        vis[node] = 1;
        cout << node <<" " << colr[node]<<endl;

        pq.pop();

        for(int child: adj[node]){

            bool entered = false;
            for(int k=1;k<=4 && vis[child] == false;k++){

                if(color[child][k] == 1){
                    bool ok = true;
                    for(int ch:adj[child]) {
                        if(k == colr[ch]) {
                            ok = false;
                        }
                    }
                    if(ok){
                        pq.push({constr[child] , child});
                        colr[child] = k;
                        entered = true;
                    }
                }
            }
            if(entered == false) return false;
        }
    }
    return true;

}
int main(){
    cin >> n >> e;
    for(int i=1;i<=e;i++){
        int a , b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    ///extra condition
    for(int i=1;i<=n;i++){
        for(int j=1;j<=4;j++) color[i][j] = 1;
    }
    int pc = -1 , st = 1;
    for(int i=1;i<=n;i++){
        int c;
        cin >> c;
        constr[i] = c;
        if(c > pc) st = i , pc = c;
        for(int j=1;j<=c;j++){
            int x;
            cin >> x;
            color[i][x] = 0;
        }
//        for(int j=1;j<=4;j++) cout << color[i][j]<<" ";
//        cout<<endl;
    }

//    for(int j = 1 ; j<=1;j++){
//
//        for(int i=1;i<=n;i++) vis[i] = 0 , colr[i] = -1;
//
//        if(color[1][j] == 1 && dfs(1,j)){
//            cout <<"Possible"<<endl;
//            for(int i=1;i<=n;i++) cout << colr[i]<<" ";
//            cout<<endl;
//            return 0;
//        }
//    }



    for(int j=1;j<=1;j++){

        for(int i=1;i<=n;i++) vis[i] = 0 , colr[i] = -1;

        if(color[st][j] == 1 && bfs(st,j)){
            cout <<"Possible"<<endl;
            for(int i=1;i<=n;i++) cout << colr[i]<<" ";
            cout<<endl;
            return 0;
        }
    }

    cout << "Not Possible"<<endl;
    return 0;
}
