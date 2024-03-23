#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define MAX 10000
#define BLOCK_SIZE 64  // Adjust the block size according to your cache size

void Read_matrix (char *prompt, float *matrix, int n);
void Print_matrix (char *prompt, float *matrix, int n);
void Set_to_zero (float *matrix, int n);
void Matrix_multiply_block(float *A, float *B, float *C, int n, int block_size);

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int i, j;
    int n;
    float *matrix_A, *matrix_B, *matrix_C;
    double start, end;
    int my_rank, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0) {
        printf("\t\t**********************************************\n");
        printf("\t\t*            Algoritmo Secuencial            *\n");
        printf("\t\t**********************************************\n\n");

        if (argc == 2) {
            sscanf(argv[1], "%d", &n);
        } else {
            printf("¿ Cuál es el orden de las matrices ? \n");
            scanf("%d", &n);
        }

        matrix_A = (float *) malloc (MAX*MAX*sizeof(float));
        matrix_B = (float *) malloc (MAX*MAX*sizeof(float));
        matrix_C = (float *) malloc (MAX*MAX*sizeof(float));

        Read_matrix ("Ingrese A : ", matrix_A, n);
        /*Print_matrix ("Se leyó A : ", matrix_A, n);*/

        Read_matrix ("Ingrese B : ", matrix_B, n);
        /*Print_matrix ("Se leyó B : ", matrix_B, n);*/

        Set_to_zero(matrix_C, n);

        start = MPI_Wtime();
        /*************************************************/
        /* Algoritmo secuencial para la multiplicación de matrices*/
        Matrix_multiply_block(matrix_A, matrix_B, matrix_C, n, BLOCK_SIZE);
        /************************************************/
        end = MPI_Wtime();
        MPI_Finalize();

        /*Print_matrix ("El producto es C : ", matrix_C, n); */ 
        printf("n : %d\nSecuencial : %f segundos.\n", n, end - start);

        return 0;

    } else {    /*Si no es el primer procesador entonces finaliza.*/
        MPI_Finalize();
        return 0;
    }   
}

void Matrix_multiply_block(float *A, float *B, float *C, int n, int block_size) {
    int i, j, k, ii, jj, kk;
    for (i = 0; i < n; i += block_size) {
        for (j = 0; j < n; j += block_size) {
            for (k = 0; k < n; k += block_size) {
                for (ii = i; ii < i + block_size && ii < n; ii++) {
                    for (jj = j; jj < j + block_size && jj < n; jj++) {
                        for (kk = k; kk < k + block_size && kk < n; kk++) {
                            C[ii * n + jj] += A[ii * n + kk] * B[kk * n + jj];
                        }
                    }
                }
            }
        }
    }
}

void Read_matrix (char *prompt, float *matrix, int n) {
    int i, j;
    printf("%s\n", prompt);
    for(i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                matrix[i*n + j] = 1 + rand()%5;
            }
    }
} /* Read_matrix */

void Print_matrix (char *prompt, float *matrix, int n)  {
    int i, j;
    printf("%s\n", prompt);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%4.2f ", matrix[i*n + j]);
        }
        printf("\n");
    }
    printf("\n");
} /* Print_matrix */

void Set_to_zero (float * matrix, int n) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) 
            matrix[j + i*n] = 0.0;
} /* Set_to_zero */
