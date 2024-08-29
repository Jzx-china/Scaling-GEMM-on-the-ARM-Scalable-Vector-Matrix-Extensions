#include <stdio.h>
#include <stdlib.h>
#include <arm_sve.h>
#include <arm_sme.h>


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


void preprocess_l(int M, int K, float *inputMatrix, float *outputMatrix) {
    int SVL = svcntw();  // System Vector Length for 32-bit integers

    for (int i = 0; i < M; i += SVL) {  // Loop_outer
        for (int j = 0; j < K; j += 2 * SVL) {  // Loop_inner
            svbool_t p = svptrue_b32();  // Predicate as counter for active elements

            // Load_loop
            for (int k = 0; k < SVL; k++) {  // Handling 4 rows per loop
                float *baseAddr = &inputMatrix[(i + k) * K + j];
                svfloat32_t tile1 = svld1(p, baseAddr);           // Load elements to tile1
                svst1(p, &outputMatrix[j * M + (i + k)], tile1);  // Store elements from tile1
            }
        }
    }
}


void matmul_sme(float* A, float* B, float* C, int M, int K, int N) {
    int svl = svcntw();
    svbool_t pg = svptrue_b32();

    for (int i = 0; i < M; i += svl) {
        for (int j = 0; j < N; j += svl) {
            svfloat32_t acc0 = svdup_f32(0.0);

            for (int k = 0; k < K; k += svl) {
                svfloat32_t a_vec = svld1(pg, &A[i * K + k]);
                svfloat32_t b_vec = svld1(pg, &B[k * N + j]);

                acc0 = svmad_m(pg, a_vec, b_vec, acc0);
                svst1(pg, &C[(i+k) * N + j], acc0); 
            }
            
        }
    }
}

int main() {
      static int M, N, K;
      M = 384;
      N = 384;
      K = 384;

    float * A, * B, * C, *A_mod;
    A = (float*)malloc(M*K*sizeof(float));
    B = (float*)malloc(K*N*sizeof(float));
    C = (float*)malloc(M*N*sizeof(float));
    A_mod = (float*)malloc(M*K*sizeof(float));
    
    mat_init(A, M, K); 
    mat_init(B, K, N); 
    mat_zero(C, M, N); 
    mat_zero(A_mod, M, K);
    
    // preprocess_l(M, K, A, A_mod);
    matmul_sme(A, B, C, M, N, K); 
    
    free(A);
    free(B);
    free(C);
    free(A_mod);    
    return 0;
}