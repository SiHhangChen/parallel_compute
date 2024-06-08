#include<bits/stdc++.h>
#include<immintrin.h>
#include<chrono>
using namespace std;

const int N = 1<<10;

void avxMultiMat(float* A, float* B, float* C){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j+=8) {
            __m256 c = _mm256_setzero_ps();
            for (int k = 0; k < N; k++) {
                c = _mm256_fmadd_ps(_mm256_broadcast_ss(&A[i*N+k]), _mm256_loadu_ps(&B[k*N+j]), c);
            }
            _mm256_storeu_ps(&C[i*N+j], c);
        }
    }
}

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

void isSame(float* A, float* B){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(A[i*N+j] != B[i*N+j]){
                cout<<"Different"<<endl;
                return;
            }
        }
    }
    cout<<"Same"<<endl;
}

int main(void){
    float *A, *B, *C, *D;
    A = new float[N*N];
    B = new float[N*N];
    C = new float[N*N];
    D = new float[N*N];

    srand(time(NULL));
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i*N+j] = rand()%10;
            B[i*N+j] = rand()%10;
        }
    }
    auto start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    avxMultiMat(A, B, C);
    auto end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout<<"Time: "<<end-start<<"ms"<<endl;

    start = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    serialMultiMat(A, B, D);
    end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout<<"Time: "<<end-start<<"ms"<<endl;

    isSame(C, D);

    free(A);
    free(B);
    free(C);
    free(D);

    return 0;
}
