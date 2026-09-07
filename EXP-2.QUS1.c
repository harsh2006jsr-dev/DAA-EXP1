#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Allocate a contiguous 2D square matrix of size n x n
int** allocate_matrix(int n) {
    int **mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        mat[i] = (int *)malloc(n * sizeof(int));
    }
    return mat;
}

// Free allocated memory for an n x n matrix
void free_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

// Populate matrix with random integers in range [0, 9]
void initialize_matrix(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = rand() % 10;
        }
    }
}

// Print matrix up to a given limit to keep output readable
void print_matrix(int **mat, int n, const char *name) {
    printf("Matrix %s (%dx%d):\n", name, n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Verify whether C_T matches C_S identically
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

// Matrix addition: C = A + B
void add_matrix(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// Matrix subtraction: C = A - B
void subtract_matrix(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// (a) Traditional three-loop matrix multiplication: C_T = A * B
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

// (b) Strassen's Matrix Multiplication using Divide and Conquer
void strassen_multiply(int **A, int **B, int **C, int n) {
    // Base case: 1x1 scalar multiplication
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    // Submatrices for A and B
    int **A11 = allocate_matrix(k);
    int **A12 = allocate_matrix(k);
    int **A21 = allocate_matrix(k);
    int **A22 = allocate_matrix(k);

    int **B11 = allocate_matrix(k);
    int **B12 = allocate_matrix(k);
    int **B21 = allocate_matrix(k);
    int **B22 = allocate_matrix(k);

    // Populate quadrants from A and B
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

    // Allocate intermediate sum and difference buffers
    int **A_res = allocate_matrix(k);
    int **B_res = allocate_matrix(k);

    // M1 = (A11 + A22) * (B11 + B22)
    add_matrix(A11, A22, A_res, k);
    add_matrix(B11, B22, B_res, k);
    int **M1 = allocate_matrix(k);
    strassen_multiply(A_res, B_res, M1, k);

    // M2 = (A21 + A22) * B11
    add_matrix(A21, A22, A_res, k);
    int **M2 = allocate_matrix(k);
    strassen_multiply(A_res, B11, M2, k);

    // M3 = A11 * (B12 - B22)
    subtract_matrix(B12, B22, B_res, k);
    int **M3 = allocate_matrix(k);
    strassen_multiply(A11, B_res, M3, k);

    // M4 = A22 * (B21 - B11)
    subtract_matrix(B21, B11, B_res, k);
    int **M4 = allocate_matrix(k);
    strassen_multiply(A22, B_res, M4, k);

    // M5 = (A11 + A12) * B22
    add_matrix(A11, A12, A_res, k);
    int **M5 = allocate_matrix(k);
    strassen_multiply(A_res, B22, M5, k);

    // M6 = (A21 - A11) * (B11 + B12)
    subtract_matrix(A21, A11, A_res, k);
    add_matrix(B11, B12, B_res, k);
    int **M6 = allocate_matrix(k);
    strassen_multiply(A_res, B_res, M6, k);

    // M7 = (A12 - A22) * (B21 + B22)
    subtract_matrix(A12, A22, A_res, k);
    add_matrix(B21, B22, B_res, k);
    int **M7 = allocate_matrix(k);
    strassen_multiply(A_res, B_res, M7, k);

    // Calculate result quadrants:
    // C11 = M1 + M4 - M5 + M7
    int **C11 = allocate_matrix(k);
    int **C12 = allocate_matrix(k);
    int **C21 = allocate_matrix(k);
    int **C22 = allocate_matrix(k);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C11[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C12[i][j] = M3[i][j] + M5[i][j];
            C21[i][j] = M2[i][j] + M4[i][j];
            C22[i][j] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];

            // Reconstruct full matrix C
            C[i][j]         = C11[i][j];
            C[i][j + k]     = C12[i][j];
            C[i + k][j]     = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }

    // Cleanup local allocations
    free_matrix(A11, k); free_matrix(A12, k); free_matrix(A21, k); free_matrix(A22, k);
    free_matrix(B11, k); free_matrix(B12, k); free_matrix(B21, k); free_matrix(B22, k);
    free_matrix(A_res, k); free_matrix(B_res, k);
    free_matrix(M1, k); free_matrix(M2, k); free_matrix(M3, k); free_matrix(M4, k);
    free_matrix(M5, k); free_matrix(M6, k); free_matrix(M7, k);
    free_matrix(C11, k); free_matrix(C12, k); free_matrix(C21, k); free_matrix(C22, k);
}

// Run test for a specific matrix size
void run_experiment(int n, bool display_full) {
    printf("============================================================\n");
    printf("Testing Matrix Size: %dx%d\n", n, n);
    printf("============================================================\n");

    int **A   = allocate_matrix(n);
    int **B   = allocate_matrix(n);
    int **C_T = allocate_matrix(n);
    int **C_S = allocate_matrix(n);

    initialize_matrix(A, n);
    initialize_matrix(B, n);

    if (display_full) {
        print_matrix(A, n, "A");
        print_matrix(B, n, "B");
    }

    // Traditional Multiplication
    clock_t start = clock();
    traditional_multiply(A, B, C_T, n);
    clock_t end = clock();
    double time_T = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

    // Strassen Multiplication
    start = clock();
    strassen_multiply(A, B, C_S, n);
    end = clock();
    double time_S = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

    if (display_full) {
        print_matrix(C_T, n, "C_T (Traditional)");
        print_matrix(C_S, n, "C_S (Strassen)");
    }

    // Verification
    if (verify_matrices(C_T, C_S, n)) {
        printf("Verification: SUCCESS (C_T matches C_S identically)\n");
    } else {
        printf("Verification: FAILED (Discrepancy detected between C_T and C_S)\n");
    }

    printf("Execution Time (Traditional): %.2f ms\n", time_T);
    printf("Execution Time (Strassen)   : %.2f ms\n\n", time_S);

    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C_T, n);
    free_matrix(C_S, n);
}

int main() {
    srand(42);

    // Step 4: Verification on a small 4x4 matrix with printed matrices
    printf("--- Part 1: Small Size Verification (4x4) ---\n");
    run_experiment(4, true);

    // Required benchmark matrix sizes: 32x32, 64x64, 128x128
    printf("--- Part 2: Benchmark Runs (32x32, 64x64, 128x128) ---\n");
    int sizes[] = {32, 64, 128};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        run_experiment(sizes[i], false);
    }

    return 0;
}