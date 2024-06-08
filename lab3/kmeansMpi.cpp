#include<bits/stdc++.h>
#include<mpi.h>
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

void kmeans(float** data,float** cluster, int local_dataNum, int demension, 
                          int clusterNum, int* belong, int rank, int size){
    // update belong
    for(int i = 0; i < local_dataNum; i++){
        float minDist = MAX_FLOAT;
        for(int j = 0; j < clusterNum; j++){
            float nowDist = distance(data[i], cluster[j], demension);
            if(nowDist < minDist){
                minDist = nowDist;
                belong[i] = j;
            }
        }
    }
}

int main(int argc, char* argv[]){
    int rank, size;
    // ifstream input("case.txt");
    // ofstream output("caseout.txt");
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    //------------------------define----------------------------------------
    int dataNum, demension, clusterNum, all_dataNum;
    int* belong;
    float** data = NULL, ** cluster = NULL;

    //-----------------------input-&&-distribution----------------------------
    if(rank == 0){
        // cout << "Please input the number of data, demension, clusterNum and MAX_SPACE" << endl;
        cin >> dataNum >> demension >> clusterNum;
        // input >> dataNum >> demension >> clusterNum;
        all_dataNum = dataNum;
        data = new float*[dataNum];
        for(int i = 0; i < dataNum; i++){
            data[i] = new float[demension];
            for(int j = 0; j < demension; j++){
                cin >> data[i][j];
                // input >> data[i][j];
                if(data[i][j] > MAX_SPACE){
                    MAX_SPACE = data[i][j];
                }
            }
        }
        
        float* tempdata = new float[dataNum * demension];
        for(int i = 0; i < dataNum; i++){
            for(int j = 0; j < demension; j++){
                tempdata[i * demension + j] = data[i][j];
            }
        }

        for(int i = 1; i < size; i++){
            int nums = dataNum / (size - 1);
            int start = (i - 1) * nums;
            int end;

            if (i == size - 1) end = dataNum;
            else end = i * nums;

            int sendNum = end - start;

            MPI_Send(&sendNum, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&demension, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&clusterNum, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
            MPI_Send(&all_dataNum, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
            MPI_Send((void*)(tempdata + start * demension), sendNum * demension, MPI_FLOAT, i, 5, MPI_COMM_WORLD);
        }
        // delete[] tempdata;
    }
    else{
        MPI_Recv(&dataNum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&demension, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&clusterNum, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&all_dataNum, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        float* tempdata = new float[dataNum * demension];
        memset(tempdata, 0, dataNum * demension * sizeof(float));
        MPI_Recv((void*)tempdata, dataNum * demension, MPI_FLOAT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        data = new float*[dataNum];
        for(int i = 0; i < dataNum; i++){
            data[i] = new float[demension];
            for(int j = 0; j < demension; j++){
                data[i][j] = tempdata[i * demension + j];
            }
        }

        // cout << "rank: " << rank << " local_dataNum: " << dataNum << endl << "data: " << endl;
        // for(int i = 0; i < dataNum; i++){
        //     for(int j = 0; j < demension; j++){
        //         cout << data[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // Free memory
        // delete[] tempdata;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    // ------------------------init-cluster-----------------------------------------
    if(rank == 0){
        srand(time(NULL));
        cluster = new float*[clusterNum];
        // cout << "the init cluster is:" << endl;
        for(int i = 0; i < clusterNum; i++){
            cluster[i] = new float[demension];
            for(int j = 0; j < demension; j++){
                cluster[i][j] = (float)rand()/RAND_MAX * MAX_SPACE;
                // cout << cluster[i][j] << " ";
            }
            // cout << endl;
        }

        float* tempcluster = new float[clusterNum * demension];
        for(int i = 0; i < clusterNum; i++){
            for(int j = 0; j < demension; j++){
                tempcluster[i * demension + j] = cluster[i][j];
            }
        }

        for(int i = 1; i < size; i++){
            MPI_Send((void*)tempcluster, clusterNum * demension, MPI_FLOAT, i, 5, MPI_COMM_WORLD);
        }
    }
    else{
        float* tempcluster = new float[clusterNum * demension];
        memset(tempcluster, 0, clusterNum * demension * sizeof(float));
        MPI_Recv((void*)tempcluster, clusterNum * demension, MPI_FLOAT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cluster = new float*[clusterNum];
        for(int i = 0; i < clusterNum; i++){
            cluster[i] = new float[demension];
            for(int j = 0; j < demension; j++){
                cluster[i][j] = tempcluster[i * demension + j];
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    //------------------------init-belong-----------------------------------------
    belong = new int[dataNum];
    memset(belong, -1, dataNum * sizeof(int));

    //------------------------kmeans----------------------------------------------
    for(int iter = 0; iter < MAXITER; iter++){
        // ------------------------update-belong-----------------------------------------
        if(rank) {
            kmeans(data, cluster, dataNum, demension, clusterNum, belong, rank, size);
            MPI_Send(belong, dataNum, MPI_INT, 0, 6, MPI_COMM_WORLD);
        }
        else{
            for(int i = 1; i < size; i++){
                int nums = dataNum / (size - 1);
                int start = (i - 1) * nums;
                int end;

                if (i == size - 1) end = dataNum;
                else end = i * nums;

                int sendNum = end - start;

                MPI_Recv(belong + start, sendNum, MPI_INT, i, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // ------------------------update-cluster-----------------------------------------
        if(rank == 0){
            // update cluster
            for(int i = 0; i < clusterNum; i++){
                int count = 0;
                float temp[demension] = {0};
                for(int j = 0; j < dataNum; j++){
                    if(belong[j] == i){
                        count++;
                        for(int k = 0; k < demension; k++){
                            temp[k] += data[j][k];
                        }
                    }
                }
                for(int j = 0; j < demension; j++){
                    if(count != 0){
                        cluster[i][j] = temp[j] / count;
                    }
                }
            }

            for(int i = 1; i < size; i++){
                for(int j = 0; j < clusterNum; j++){
                    MPI_Send(cluster[j], demension, MPI_FLOAT, i, 6, MPI_COMM_WORLD);
                }
            }
        }
        else{
            for(int i = 0; i < clusterNum; i++){
                MPI_Recv(cluster[i], demension, MPI_FLOAT, 0, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    
    //------------------------calculate-result-----------------------------------------
    if(rank == 0){
        cout << calculate(data, cluster, demension, clusterNum, belong, dataNum) << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}