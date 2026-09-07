#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Allocate contiguous n x n matrix
int** allocate_matrix(int n) {
    int **mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        mat[i] = (int *)malloc(n * sizeof(int));
    }
    return mat;
}

void free_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) free(mat[i]);
    free(mat);
}

void initialize_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = rand() % 10;
        }
    }
}

void add_matrix(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract_matrix(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Traditional 3-loop matrix multiplication
void traditional_multiply(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Strassen's algorithm using pure recursion down to n=1
void strassen_multiply(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;
    int **A11 = allocate_matrix(k); int **A12 = allocate_matrix(k);
    int **A21 = allocate_matrix(k); int **A22 = allocate_matrix(k);
    int **B11 = allocate_matrix(k); int **B12 = allocate_matrix(k);
    int **B21 = allocate_matrix(k); int **B22 = allocate_matrix(k);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];         A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];     A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];         B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];     B22[i][j] = B[i + k][j + k];
        }
    }

    int **A_tmp = allocate_matrix(k);
    int **B_tmp = allocate_matrix(k);

    add_matrix(A11, A22, A_tmp, k); add_matrix(B11, B22, B_tmp, k);
    int **M1 = allocate_matrix(k); strassen_multiply(A_tmp, B_tmp, M1, k);

    add_matrix(A21, A22, A_tmp, k);
    int **M2 = allocate_matrix(k); strassen_multiply(A_tmp, B11, M2, k);

    subtract_matrix(B12, B22, B_tmp, k);
    int **M3 = allocate_matrix(k); strassen_multiply(A11, B_tmp, M3, k);

    subtract_matrix(B21, B11, B_tmp, k);
    int **M4 = allocate_matrix(k); strassen_multiply(A22, B_tmp, M4, k);

    add_matrix(A11, A12, A_tmp, k);
    int **M5 = allocate_matrix(k); strassen_multiply(A_tmp, B22, M5, k);

    subtract_matrix(A21, A11, A_tmp, k); add_matrix(B11, B12, B_tmp, k);
    int **M6 = allocate_matrix(k); strassen_multiply(A_tmp, B_tmp, M6, k);

    subtract_matrix(A12, A22, A_tmp, k); add_matrix(B21, B22, B_tmp, k);
    int **M7 = allocate_matrix(k); strassen_multiply(A_tmp, B_tmp, M7, k);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j]         = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + k]     = M3[i][j] + M5[i][j];
            C[i + k][j]     = M2[i][j] + M4[i][j];
            C[i + k][j + k] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    free_matrix(A11, k); free_matrix(A12, k); free_matrix(A21, k); free_matrix(A22, k);
    free_matrix(B11, k); free_matrix(B12, k); free_matrix(B21, k); free_matrix(B22, k);
    free_matrix(A_tmp, k); free_matrix(B_tmp, k);
    free_matrix(M1, k); free_matrix(M2, k); free_matrix(M3, k); free_matrix(M4, k);
    free_matrix(M5, k); free_matrix(M6, k); free_matrix(M7, k);
}