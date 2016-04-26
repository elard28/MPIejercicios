#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TAM 8

/*int** multiplicacion(int** ma,int mb,int n)
{
	int **mr=new int*[n];
	for (int i = 0; i < n; ++i)
		mr[i]=new int[n];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			for (int k = 0; k < n; ++k)
				mr[i][j]+=ma[i][k]*[k][j];
		}
	}
	return mr;
}*/

int ma[TAM][TAM];
int mb[TAM][TAM];
int mr[TAM][TAM];

void fill(int m[TAM][TAM])
{
	for (int i = 0; i < TAM; ++i)
	{
		for (int j = 0; j < TAM; ++j)
			m[i][j]=rand()%10;
	}
}

void print(int m[TAM][TAM])
{
	for (int i = 0; i < TAM; ++i)
	{
		for (int j = 0; j < TAM; ++j)
			printf("%d\t", m[i][j]);
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	int myrank;
	int numproc; 
	int from; 
	int to;

  	int tag = 666;
  	MPI_Status status;
  
  	MPI_Init (&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);	
  	MPI_Comm_size(MPI_COMM_WORLD, &numproc);

  	if (myrank==0) 
  	{
    	fill(ma);
    	fill(mb);
  	}

  	MPI_Bcast (mb, TAM*TAM, MPI_INT, 0, MPI_COMM_WORLD);
  	MPI_Scatter (ma, TAM*TAM/numproc, MPI_INT, ma[from], TAM*TAM/numproc, MPI_INT, 0, MPI_COMM_WORLD);

  	for (int i=from; i<to; i++)
  	{ 
    	for (int j=0; j<TAM; j++) 
    	{
      		mr[i][j]=0;
      		for (int k=0; k<TAM; k++)
				mr[i][j] += mr[i][k]*mr[k][j];
    	}
    }

  	MPI_Gather (mr[from], TAM*TAM/numproc, MPI_INT, mr, TAM*TAM/numproc, MPI_INT, 0, MPI_COMM_WORLD);

  	MPI_Finalize();	
  	
	return 0;
}