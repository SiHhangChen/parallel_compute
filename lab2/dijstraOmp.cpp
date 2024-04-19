#include<bits/stdc++.h>
#include<omp.h>

using namespace std;

vector <int> dijstra(vector<vector<pair<int, int>>> graph, int startV){
    vector<int> dist(graph.size(), INT_MAX);
    dist[startV] = 0;
    queue<pair<int, int>> pq;
    pq.push({0, startV});
    while(!pq.empty()){
        int u = pq.front().second;
        int d = pq.front().first;
        pq.pop();
        if(dist[u] < d) continue;
        for(int i = 0; i < graph[u].size(); i++){
            int v = graph[u][i].first;
            int w = graph[u][i].second;
            if(dist[v] > dist[u] + w){
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
        
    }
    return dist;
}

int main(int argc, char* argv[]){
    int vNum, eNum, startV;
    cin >> vNum >> eNum >> startV;
    vector<vector<pair<int, int>>> graph(vNum);
    for(int i = 0; i < eNum; i++){
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }
    vector<int> dist = dijstra(graph, startV);
    for(int i = 0; i < vNum; i++){
        cout << dist[i] << " ";
    }
    cout << endl;
    
}