#include<bits/stdc++.h>
#include<omp.h>

using namespace std;
#define MAX_DIST 1E9

vector <int> bellmanFord(vector<vector<tuple<int, int>>> edges, int vNum, int startV){
    vector<int> dist(vNum, MAX_DIST);
    dist[startV] = 0;
    queue<int> q1, q2, q3, q4;
    q1.push(startV);
    q2.push(startV+1);
    q3.push(startV+2);
    q4.push(startV+3);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            while(!q1.empty()){
                int u = q1.front();
                q1.pop();
                #pragma omp parallel for
                for(auto edge: edges[u]){
                    int v = get<0>(edge);
                    int w = get<1>(edge);
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                        q1.push(v);
                    }
                }
            }
        }

        #pragma omp section
        {
            while(!q2.empty()){
                int u = q2.front();
                q2.pop();
                #pragma omp parallel for
                for(auto edge: edges[u]){
                    int v = get<0>(edge);
                    int w = get<1>(edge);
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                        q2.push(v);
                    }
                }
            }
        }

        #pragma omp section
        {
            while(!q3.empty()){
                int u = q3.front();
                q3.pop();
                #pragma omp parallel for
                for(auto edge: edges[u]){
                    int v = get<0>(edge);
                    int w = get<1>(edge);
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                        q3.push(v);
                    }
                }
            }
        }

        #pragma omp section
        {
            while(!q4.empty()){
                int u = q4.front();
                q4.pop();
                #pragma omp parallel for
                for(auto edge: edges[u]){
                    int v = get<0>(edge);
                    int w = get<1>(edge);
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                        q4.push(v);
                    }
                }
            }
        }
    }
    return dist;
}

int main(int argc, char* argv[]){
    int threadNum = 4;
    int vNum, eNum, startV;
    cin >> vNum >> eNum >> startV;
    // 邻接表
    vector<vector<tuple<int, int>>> edges(vNum);
    for(int i = 0; i < eNum; i++){
        int u, v, w;
        cin >> u >> v >> w;
        edges[u].push_back(make_tuple(v, w));
        edges[v].push_back(make_tuple(u, w));
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