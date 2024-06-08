#include<bits/stdc++.h>
#include<omp.h>

using namespace std;
#define MAX_DIST 1E9

vector <int> bellmanFord(vector<tuple <int, int, int>> edges, int vNum, int startV){
    vector<int> dist(vNum, MAX_DIST);
    dist[startV] = 0;
    // #pragma omp parallel for
    for(int i = 0; i < vNum - 1; i++){
        int change = 0;
        #pragma omp parallel for reduction(||:change)
        for(int j = 0; j < edges.size(); j++){
            int u = get<0>(edges[j]);
            int v = get<1>(edges[j]);
            int w = get<2>(edges[j]);
            #pragma omp critical
            if(dist[v] > dist[u] + w){
                change = 1;
                dist[v] = dist[u] + w;
            }
            else if (dist[u] > dist[v] + w){
                change = 1;
                dist[u] = dist[v] + w;
            }
        }
        if(change == 0) break;
    }
    return dist;
}

int main(int argc, char* argv[]){
    int threadNum = 4;
    int vNum, eNum, startV;
    cin >> vNum >> eNum >> startV;
    vector<tuple <int, int, int>> edges(eNum);
    for(int i = 0; i < eNum; i += 1){
        int u, v, w; 
        cin >> u >> v >> w;
        edges[i] = make_tuple(u, v, w);
        // edges[i + 1] = make_tuple(v, u, w);
    }
    // int start = omp_get_wtime();
    omp_set_num_threads(threadNum);
    vector<int> dist = bellmanFord(edges, vNum, startV);
    // int end = omp_get_wtime();
    for(int i = 0; i < vNum; i++){
        if(dist[i] == MAX_DIST) cout << "INF ";
        else cout << dist[i] << " ";
    }
    cout << endl;   
    // cout << "Time: " << end - start << endl;
}