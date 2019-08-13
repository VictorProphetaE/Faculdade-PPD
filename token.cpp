/**
Descrição: Este programa faz com que cada processo receba um token

Nota:GCC compiler and GDB debugger from TDM-GCC (version 4.7.1, 32 bit)

Nome: Victor Propheta Erbano
RGM: 021052
Matéria: Programação Paralela e Distribuida
Prof: Marcos Alves Mariano
Data de Criação: 15/09/2017
Data Modificação: 20/09/2017
*/


#include <mpi.h>
#include <stdio.h>
 
int main(int argc, char* argv[]) {
   MPI_Init(&argc, &argv);
   int size;
   MPI_Comm_size( MPI_COMM_WORLD, &size );
 
   int rank;
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
 
   int token;
 
   if( rank != 0 ) {
      MPI_Recv( &token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
 
      printf("Processo %d recebe token %d para processo %d\n", rank, token, rank-1);
   } else {

      token = 100;
   }
   MPI_Send( &token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD );
 
   if( rank == 0 ) {
      MPI_Recv( &token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
 
      printf("Processo 0 recebe token %d do processo %d.\n", token, size-1 );
   }
 

   MPI_Finalize();
 
   return 0;
}
