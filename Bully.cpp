/**
Descrição: Este programa 1 dentre 8 processos selecionando assim o lider deles

Nota:GCC compiler and GDB debugger from TDM-GCC (version 4.7.1, 32 bit)

Nome: Victor Propheta Erbano
RGM: 021052
Matéria: Programação Paralela e Distribuida
Prof: Marcos Alves Mariano
Data de Criação: 15/09/2017
Data Modificação: 20/09/2017
*/


#include "mpi.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
	int size, rank, tag, rc, min, lcounter;	
	double t1, t2; 
	int inmsg1;	
	int inmsg2;	
	int msg; 	
	int stage=1;	
	MPI_Status Stat;
	MPI_Request send_req, recv_req;
	rc=MPI_Init(&argc,&argv);
	bool terminated=false;
	bool relayer=false;
	if (rc!=0) {cout << "Erro ao iniciar a MPI." << endl; MPI_Abort(MPI_COMM_WORLD, rc);}
	MPI_Comm_size(MPI_COMM_WORLD, &size);	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	

	t1 = MPI_Wtime(); 
	msg=rank;
	
	while (!terminated){
		if (rank==0){
			MPI_Isend(&msg, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &send_req);
			MPI_Recv(&inmsg1, 1, MPI_INT, size-1, 1, MPI_COMM_WORLD, &Stat);
			MPI_Isend(&msg, 1, MPI_INT, size-1, 1, MPI_COMM_WORLD, &send_req);
			MPI_Recv(&inmsg2, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &Stat);
		}
		else if (rank==size-1){
			MPI_Isend(&msg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &send_req);
			MPI_Recv(&inmsg1, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &Stat);
			MPI_Isend(&msg, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &send_req);	
			MPI_Recv(&inmsg2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);
			
		}
		else {
			MPI_Isend(&msg, 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD, &send_req);
			MPI_Recv(&inmsg1, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &Stat);
			MPI_Isend(&msg, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &send_req);
			MPI_Recv(&inmsg2, 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD, &Stat);
		}
		

		if (relayer) {
			if (rank==0) {
				MPI_Send(&inmsg1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
				MPI_Send(&inmsg2, 1, MPI_INT, size-1, 1, MPI_COMM_WORLD);
			}
			else if (rank==size-1) {
				MPI_Send(&inmsg1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
				MPI_Send(&inmsg2, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
			}
			else {
				MPI_Send(&inmsg1, 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
				MPI_Send(&inmsg2, 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
			}		
		}

			if (!relayer){
			if (inmsg1<rank || inmsg2<rank) 
			{
				relayer=true;
			}
			else if (inmsg1==rank && inmsg2==rank) {
				terminated=true;
				cout << "No " << rank << " eleito como lider!" << endl;
				t2 = MPI_Wtime(); 
				cout << endl << "tempo de processamento: " << (t2-t1) << " segundos" << endl;
				MPI_Finalize();
				break;
			}
			}

		
			
	}
	t2 = MPI_Wtime(); 
	
MPI_Finalize();
if (rank==min) cout << endl << "Elapsed time: " << (t2-t1) << " seconds" << endl;
}
