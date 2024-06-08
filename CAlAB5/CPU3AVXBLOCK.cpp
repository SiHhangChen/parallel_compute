#include<bits/stdc++.h>
#include<immintrin.h>
using namespace std;

#define N (1<<10)
const int BLOCK_SIZE = 8;

void avxBlockMultiMat(float* A, float* B, float* C){
    transpose(B);
    for (int i = 0; i < N; i += BLOCK_SIZE) {
        for (int j = 0; j < N; j += BLOCK_SIZE) {
            for (int k = 0; k < N; k += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE; ++ii) {
                    for (int jj = j; jj < j + BLOCK_SIZE; jj += 8) {
                        __m256 c = _mm256_loadu_ps(&C[ii * N + jj]);
                        for (int kk = k; kk < k + BLOCK_SIZE; ++kk) {
                            float B_elements[8] = { B[(jj)*N + kk], B[(jj + 1) * N + kk], B[(jj + 2) * N + kk], B[(jj + 3) * N + kk], B[(jj + 4) * N + kk], B[(jj + 5) * N + kk], B[(jj + 6) * N + kk], B[(jj + 7) * N + kk] };
                            __m256 b = _mm256_set_ps(B_elements[7], B_elements[6], B_elements[5], B_elements[4], B_elements[3], B_elements[2], B_elements[1], B_elements[0]);
                            c = _mm256_fmadd_ps(_mm256_broadcast_ss(&A[ii * N + kk]), b, c);
                        }
                        _mm256_storeu_ps(&C[ii * N + jj], c);
                    }
                }
            }
        }
    }
}

void transpose(float *B) {
    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            float temp = B[j * N + i];
            B[j * N + i] = B[i * N + j];
            B[i * N + j] = temp;
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
    A = (float*)malloc(N*N*sizeof(float));
    B = (float*)malloc(N*N*sizeof(float));
    C = (float*)malloc(N*N*sizeof(float));
    D = (float*)malloc(N*N*sizeof(float));

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i*N+j] = 1.0;
            B[i*N+j] = 1.0;
        }
    }

    avxBlockMultiMat(A, B, C);
    serialMultiMat(A, B, D);

    isSame(C, D);

    free(A);
    free(B);
    free(C);
    free(D);

    return 0;
}