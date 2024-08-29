#include <stdio.h>
#include <stdlib.h>


void mat_init(float * mat, int M, int N) {
    for (int i = 0 ; i < M ; i++)
        for (int j = 0 ; j < N ; j++)
            mat[i*N + j] = rand()*0.01 / (i+j+1.);
    return;
}
void mat_zero(float * mat, int M, int N) {
    for (int i = 0 ; i < M ; i++)
        for (int j = 0 ; j < N ; j++)
            mat[i*N + j]= 0.0;
    return;
}

void matmul_naive(float * A, float * B, float * C, int M, int N, int K) {
    for (int i = 0 ; i < M ; i++)
        for (int j = 0 ; j < N ; j++)
            for (int k = 0 ; k < K ; k++)
                C[i*N + j] += A[i*K +k] * B[k*N + j]; 
    return;
}

int main() {
      static int M, N, K;
      M = 128;
      N = 128;
      K = 128;

    float * A, * B, * C;
    A = (float*)malloc(M*K*sizeof(float));
    B = (float*)malloc(K*N*sizeof(float));
    C = (float*)malloc(M*N*sizeof(float));
    
    mat_init(A, M, K); 
    mat_init(B, K, N); 
    mat_zero(C, M, N); 

        matmul_naive(A, B, C, M, N, K); 
    
       free(A);
    free(B);
    free(C);
    return 0;
}