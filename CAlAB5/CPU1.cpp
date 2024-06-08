#include<bits/stdc++.h>
#include<chrono>
using namespace std;

#define N (1<<11)

void serialMultiMat(float* A, float* B, float* C){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            C[i*N+j] = 0;
            for(int k=0; k<N; k++){
                C[i*N+j] += A[i*N+k]*B[k*N+j];
            }
        }
    }
}

int main(void){
    float *A, *B, *C;
    A = new float[N*N];
    B = new float[N*N];
    C = new float[N*N]; 
    srand(time(NULL));
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i*N+j] = rand()%10;
            B[i*N+j] = rand()%10;
        }
    }
    int start = clock();
    serialMultiMat(A, B, C);
    int end = clock();

    cout<<"Time: "<<((end-start) * 1000)/CLOCKS_PER_SEC <<"s"<<endl;

    free(A);
    free(B);
    free(C);

    return 0;
}