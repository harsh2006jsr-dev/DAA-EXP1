#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Allocate an n x n matrix
int** allocate_matrix(int n) {
    int **mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        mat[i] = (int *)malloc(n * sizeof(int));
    }
    return mat;
}

// Free an n x n matrix
void free_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

// Populate matrix with random integers
void initialize_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = rand() % 10;
        }
    }
}

// Matrix Addition: C = A + B
void add_matrix(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Matrix Subtraction: C = A - B
void subtract_matrix(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// (a) Traditional 3-loop matrix multiplication
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

// (b) Strassen's algorithm using Divide and Conquer
void strassen_multiply(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    int **A11 = allocate_matrix(k);
    int **A12 = allocate_matrix(k);
    int **A21 = allocate_matrix(k);
    int **A22 = allocate_matrix(k);

    int **B11 = allocate_matrix(k);
    int **B12 = allocate_matrix(k);
    int **B21 = allocate_matrix(k);
    int **B22 = allocate_matrix(k);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    int **A_temp = allocate_matrix(k);
    int **B_temp = allocate_matrix(k);

    // M1 = (A11 + A22) * (B11 + B22)
    add_matrix(A11, A22, A_temp, k);
    add_matrix(B11, B22, B_temp, k);
    int **M1 = allocate_matrix(k);
    strassen_multiply(A_temp, B_temp, M1, k);

    // M2 = (A21 + A22) * B11
    add_matrix(A21, A22, A_temp, k);
    int **M2 = allocate_matrix(k);
    strassen_multiply(A_temp, B11, M2, k);

    // M3 = A11 * (B12 - B22)
    subtract_matrix(B12, B22, B_temp, k);
    int **M3 = allocate_matrix(k);
    strassen_multiply(A11, B_temp, M3, k);

    // M4 = A22 * (B21 - B11)
    subtract_matrix(B21, B11, B_temp, k);
    int **M4 = allocate_matrix(k);
    strassen_multiply(A22, B_temp, M4, k);

    // M5 = (A11 + A12) * B22
    add_matrix(A11, A12, A_temp, k);
    int **M5 = allocate_matrix(k);
    strassen_multiply(A_temp, B22, M5, k);

    // M6 = (A21 - A11) * (B11 + B12)
    subtract_matrix(A21, A11, A_temp, k);
    add_matrix(B11, B12, B_temp, k);
    int **M6 = allocate_matrix(k);
    strassen_multiply(A_temp, B_temp, M6, k);

    // M7 = (A12 - A22) * (B21 + B22)
    subtract_matrix(A12, A22, A_temp, k);
    add_matrix(B21, B22, B_temp, k);
    int **M7 = allocate_matrix(k);
    strassen_multiply(A_temp, B_temp, M7, k);

    // Combine results into C
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
    free_matrix(A_temp, k); free_matrix(B_temp, k);
    free_matrix(M1, k); free_matrix(M2, k); free_matrix(M3, k); free_matrix(M4, k);
    free_matrix(M5, k); free_matrix(M6, k); free_matrix(M7, k);
}

// Automatic verification function
bool verify_matrices(int **C_T, int **C_S, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (C_T[i][j] != C_S[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Run single test instance
const char* run_test(int n) {
    int **A   = allocate_matrix(n);
    int **B   = allocate_matrix(n);
    int **C_T = allocate_matrix(n);
    int **C_S = allocate_matrix(n);

    initialize_matrix(A, n);
    initialize_matrix(B, n);

    traditional_multiply(A, B, C_T, n);
    strassen_multiply(A, B, C_S, n);

    bool match = verify_matrices(C_T, C_S, n);

    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C_T, n);
    free_matrix(C_S, n);

    return match ? "Output Verification: SUCCESS" : "Output Verification: FAILED";
}

int main() {
    srand(42);

    int test_sizes[] = {4, 8, 16, 32};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    printf("%-10s | %-12s | %-30s\n", "Test", "Matrix Size", "Output Verification");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < num_tests; i++) {
        const char *result = run_test(test_sizes[i]);
        printf("Test %-5d | %-12d | %-30s\n", i + 1, test_sizes[i], result);
    }

    return 0;
}