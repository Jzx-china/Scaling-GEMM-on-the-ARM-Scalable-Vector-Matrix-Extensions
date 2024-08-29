#include <stdio.h>
#include <stdlib.h>
#include <arm_sve.h>


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


void matmul_sve(float* A, float* B, float* C, int M, int K, int N) {
    float* ptrA;
    float* ptrB;
    float* ptrC;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0f;
            ptrA = A + i * K;
            ptrB = B + j;

            int vl = svcntw();

            svbool_t pg = svwhilelt_b32(0, K);
            svfloat32_t vsum = svdup_f32(0.0f); 

            for (int k = 0; k < K; k += vl) {
                svfloat32_t va = svld1(pg, ptrA + k);
                svfloat32_t vb = svld1(pg, ptrB + k * N);
                vsum = svmmla_f32(vsum, va, vb);
            }
            sum = svaddv(svptrue_b32(), vsum); 
            ptrC = C + i * N + j;
            *ptrC = sum;
        }
    }
}

int main() {
      static int M, N, K;
      M = 256;
      N = 256;
      K = 256;

    float * A, * B, * C;
    A = (float*)malloc(M*K*sizeof(float));
    B = (float*)malloc(K*N*sizeof(float));
    C = (float*)malloc(M*N*sizeof(float));
    
    mat_init(A, M, K); 
    mat_init(B, K, N); 
    mat_zero(C, M, N); 

    matmul_sve(A, B, C, M, N, K); 
    
    free(A);
    free(B);
    free(C);
    return 0;
}