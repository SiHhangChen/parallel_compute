#include<bits/stdc++.h>
using namespace std;

#define INF 1000000000
#define delta 10

int tent[1000000];
vector<int> B[1000000/delta + 1];
vector<vector<tuple<int, int>>> edge;
void relax(int w, int x){
    if(x < tent[w]){
        auto& vec = B[tent[w]/delta];
        vec.erase(std::remove(vec.begin(), vec.end(), w), vec.end());
        B[x/delta].push_back(w);
        tent[w] = x;
    }
}

void relaxRequest(vector<tuple<int, int>> Req){
    for (auto& [w, x] : Req){
        relax(w, x);
    }
}

vector<tuple<int, int>> findRequest(vector<int> V, int kind){
    vector<tuple<int, int>> Req;
    if(kind){
        for(auto& w : V){
            for(auto& [v, x] : edge[w]){
                if(find(V.begin(), V.end(), v) != V.end() && x < delta){
                    Req.push_back({v, tent[w] + x});
                }
            }
        }
    }
    else{
        for(auto& w : V){
            for(auto& [v, x] : edge[w]){
                if(find(V.begin(), V.end(), v) == V.end() && x > delta){
                    Req.push_back({v, tent[w] + x});
                }
            }
        }
    }
    return Req;
}


int main(){
    int vNum, eNum, src;
    cin >> vNum >> eNum >> src;
    vector<vector<pair<int, int>>> graph(eNum);
    for(int i = 0; i < eNum; i++){
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    vector<int> dist(vNum, INF);
    dist[src] = 0;

}