#include <bits/stdc++.h>
using namespace std;

int n, m, visited, counter, Max;

struct puzzle{
    int c , mov;
    int a[2][3];
}node;

int G_node[2][3];

bool operator < (puzzle p , puzzle q){
    return p.c < q.c;
}

priority_queue<puzzle>pq;

void display(puzzle cur_node){

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++) cout << cur_node.a[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
    return;
}

bool Goal(puzzle cur_node){

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(cur_node.a[i][j] != G_node[i][j])
                return false;
        }
    }
    return true;
}


void Next_level(puzzle cur_node){

    display(cur_node);

    for(int i=0;i<n;i++){

        for(int j=0;j<m;j++){

            if(cur_node.a[i][j] == 0){  /// We can discover 4 or few new node here

                puzzle new_node;

                if(i>0 && cur_node.mov != 3){

                    new_node = cur_node;
                    swap(new_node.a[i][j] , new_node.a[i-1][j]);
                    new_node.mov = 1;
                    new_node.c = cur_node.c-1;
                    pq.push(new_node);
                    visited++;
                }
                if(i<n-1 && cur_node.mov != 1){

                    new_node = cur_node;
                    swap(new_node.a[i][j] , new_node.a[i+1][j]);
                    new_node.mov = 3;
                    new_node.c = cur_node.c-1;
                    pq.push(new_node);
                    visited++;

                }

                if(j>0 && cur_node.mov != 2){

                    new_node = cur_node;
                    swap(new_node.a[i][j] , new_node.a[i][j-1]);
                    new_node.mov = 4;
                    new_node.c = cur_node.c-1;
                    pq.push(new_node);
                    visited++;

                }

                if(j<m-1 && cur_node.mov != 4){

                    new_node = cur_node;
                    swap(new_node.a[i][j] , new_node.a[i][j+1]);
                    new_node.mov = 2;
                    new_node.c = cur_node.c-1;
                    pq.push(new_node);
                    visited++;

                }
                return;
            }

        }

    }

}


void bfs(){

    pq.push(node);

    while(pq.size() > 0){

        puzzle temp = pq.top();
        pq.pop();
        counter ++;

        if(temp.c <= Max){
            cout << "Huge steps need"<<endl;
            break;
        }

        if(Goal(temp)){
            Max = temp.c;
            node = temp;
        }
        Next_level(temp);
        cout << pq.size() <<endl;

    }

}

int main(){
    n = 2 , m = 3 , Max = -1e2;
    node.c = 0;
    node.mov = -1;
    visited++;

    /// Initial State
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin >> node.a[i][j];
        }
    }


    /// Goal State
    int val = 1;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++) {
            G_node[i][j] = val++;
        }
    }
    G_node[1][2] = 0;

    bfs();
    cout << "Number of visited node "<<visited <<endl;
    cout << "Total number of steps "<<counter <<endl;
    display(node);


    return 0;
}
