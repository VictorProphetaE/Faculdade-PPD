#include <algorithm>
#include <cmath>
#include <cstdio>
#include "mpi.h"

/* Faz a intercalacao entre v e u e guarda em r */
void Merge(int *v, int *u, int n, int *t){
    int i=0, j=0, k=0;
    while(i < n && j < n)
        t[k++] = (v[i] < u[j]) ? v[i++] : u[j++];
    while(i < n) t[k++] = v[i++];
    while(j < n) t[k++] = u[j++];
}
/* Ordena conjuntos de blocos de tamanho set_size */
void BitonicSplit(int myrank, int *v, int n, 
                  int set_size, bool inc){
    
    int u[n], t[2*n];
    int set_dim = log(set_size)/M_LN2 + 1e-8;
    int eor_bit = 1 << (set_dim - 1);
    for(int i = 0; i < set_dim; i++){
        int pair = myrank ^ eor_bit;
        MPI_Status st;
        MPI_Send(v, n, MPI_INT, pair, 0,
                 MPI_COMM_WORLD);
        MPI_Recv(u, n, MPI_INT, pair, 0, 
                 MPI_COMM_WORLD, &st);

        Merge(v, u, n, t);

        int offset = ((inc && myrank < pair) || 
                      (!inc && myrank > pair)) ? 0 : n;
        for(int j=0; j<n; j++)
            v[j] = t[j+offset];
        eor_bit >>= 1;
    }   
}
void BitonicSort(int myrank, int np, int *v, 
                 int n){
    for(int ssize = 2; ssize <= np; ssize *= 2)
        BitonicSplit(myrank, v, n, ssize, 
                     !(myrank & ssize));
}
int main (int argc, char *argv[]){

    int myrank, np;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    /* Para simplificar, considere que a distribuição 
       dos dados já foi feita p/ cada processador */
    int t[ ] = {9, 12, 16, 23, 26, 39, 42, 61,
                43, 17, 14, 13, 12, 7, 6, 5};

    int n = 16/np;
    int *v = t + myrank*n;

    /* Ordena localmente */
    std::sort(v, v+n);

    BitonicSort(myrank, np, v, n);

    /* Cada processador tem a parcela correspondente
       do vetor ordenado total */
    printf("[%d]", myrank);
    for(int i=0; i<n; i++)
        printf(" %d", v[i]);
    printf("\n");

    MPI_Finalize();

    return 0;
}