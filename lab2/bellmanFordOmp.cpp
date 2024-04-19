#include<bits/stdc++.h>
#include<omp.h>

using namespace std;
#define MAX_DIST 1E9

vector <int> bellmanFord(vector<tuple <int, int, int>> edges, int vNum, int startV){
    vector<int> dist(vNum, MAX_DIST);
    dist[startV] = 0;
    for(int i = 0; i < vNum - 1; i++){
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                for(int j = 0; j < edges.size()/4; j++){
                    int u = get<0>(edges[j]);
                    int v = get<1>(edges[j]);
                    int w = get<2>(edges[j]);
                    #pragma omp critical
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                    }
                }
            }
            #pragma omp section
            {
                for(int j = edges.size()/4; j < edges.size()/2; j++){
                    int u = get<0>(edges[j]);
                    int v = get<1>(edges[j]);
                    int w = get<2>(edges[j]);
                    #pragma omp critical
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                    }
                }
            }
            #pragma omp section
            {
                for(int j = edges.size()/2; j < 3 * edges.size()/4; j++){
                    int u = get<0>(edges[j]);
                    int v = get<1>(edges[j]);
                    int w = get<2>(edges[j]);
                    #pragma omp critical
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                    }
                }
            }
            #pragma omp section
            {
                for(int j = 3 * edges.size()/4; j < edges.size(); j++){
                    int u = get<0>(edges[j]);
                    int v = get<1>(edges[j]);
                    int w = get<2>(edges[j]);
                    #pragma omp critical
                    if(dist[v] > dist[u] + w){
                        dist[v] = dist[u] + w;
                    }
                }
            }
        }
        // for(int j = 0; j < edges.size(); j++){
        //     int u = get<0>(edges[j]);
        //     int v = get<1>(edges[j]);
        //     int w = get<2>(edges[j]);
        //     #pragma omp critical
        //     if(dist[v] > dist[u] + w){
        //         dist[v] = dist[u] + w;
        //     }
        // }
    }
    return dist;
}

int main(int argc, char* argv[]){
    int threadNum = atoi(argv[1]);
    int vNum, eNum, startV;
    cin >> vNum >> eNum >> startV;
    vector<tuple <int, int, int>> edges(2 * eNum);
    for(int i = 0; i < 2 * eNum; i += 2){
        int u, v, w;
        cin >> u >> v >> w;
        edges[i] = make_tuple(u, v, w);
        edges[i + 1] = make_tuple(v, u, w);
    }
    int start = omp_get_wtime();
    omp_set_num_threads(threadNum);
    vector<int> dist = bellmanFord(edges, vNum, startV);
    int end = omp_get_wtime();
    for(int i = 0; i < vNum; i++){
        cout << dist[i] << " ";
    }
    cout << endl;   
    cout << "Time: " << end - start << endl;
}