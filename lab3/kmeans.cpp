#include<bits/stdc++.h>
// #include<mpi.h>
using namespace std;
#define MAXITER 100
#define MAX_FLOAT 1E9
#define EPSILON 1E-6

int MAX_SPACE = 0;

float distance(float* a, float* b, int demension){
    float sum = 0;
    for(int i = 0; i < demension; i++){
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(sum);
}

float calculate(float** data, float** cluster, int demension, 
                             int clusterNum, int* belong, int dataNum){
    float sum = 0;
    for(int i = 0; i < dataNum; i++){
        for(int j = 0; j < clusterNum; j++){
            if(belong[i] == j){
                sum += distance(data[i], cluster[j], demension);
            }
        }
    }
    return sum;
}

float kmeans(float** data, int dataNum, int demension, int clusterNum){
    int belong[dataNum];
    srand(time(NULL));
    float** cluster = new float*[clusterNum];
    for(int i = 0; i < clusterNum; i++){
        cluster[i] = new float[demension];
        for(int j = 0; j < demension; j++){
            cluster[i][j] = (float)rand()/RAND_MAX * MAX_SPACE;
            cout << cluster[i][j] << " ";
        }
        cout << endl;
    }
    
    int iter = 0;
    while(iter < MAXITER){
        // update belong
        for(int i = 0; i < dataNum; i++){
            float minDist = MAX_FLOAT;
            for(int j = 0; j < clusterNum; j++){
                float nowDist = distance(data[i], cluster[j], demension);
                if(nowDist < minDist){
                    minDist = nowDist;
                    belong[i] = j;
                }
            }
        }
        // update cluster
        for(int i = 0; i < clusterNum; i++){
            int count = 0;
            float temp[demension] = {0};
            for(int j = 0; j < dataNum; j++){
                if(belong[j] == i){
                    for(int k = 0; k < demension; k++){
                        temp[k] += data[j][k];
                    }
                    count++;
                }
            }
            for(int j = 0; j < demension; j++){
                if (count > 0 && temp[j] / count - cluster[i][j] > EPSILON){
                    cluster[i][j] = temp[j] / count;
                }
            }
        }

        // for(int i = 0; i < clusterNum; i++){
        //     for(int j = 0; j < demension; j++){
        //         cout << cluster[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        iter++;
    }
    // for(int i = 0; i < clusterNum; i++){
    //     for(int j = 0; j < demension; j++){
    //         cout << cluster[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    return calculate(data, cluster, demension, clusterNum, belong, dataNum);
}

int main(int argc, char* argv[]){
    int dataNum, demension, clusterNum;
    cin >> dataNum >> demension >> clusterNum;
    float** data = new float*[dataNum];
    for(int i = 0; i < dataNum; i++){
        data[i] = new float[demension];
        for(int j = 0; j < demension; j++){
            cin >> data[i][j];
            if(data[i][j] > MAX_SPACE){
                MAX_SPACE = data[i][j];
            }
        }
    }
    float result = kmeans(data, dataNum, demension, clusterNum);
    cout << result << endl;
    return 0;
}