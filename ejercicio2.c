#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int rank, size;

  	MPI_Init(&argc, &argv);

  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);


	int number_of_tosses=0;
	int number_in_circle = 0;
	srand(time(NULL));
	for(int toss = 0; toss < number_of_tosses; toss++) 
	{
		double x = (rand()%1001-1000)/1000;
		double y = (rand()%1001-1000)/1000;
		double distance_squared = x∗x + y∗y;
		if (distance_squared <= 1) 
			number_in_circle++;
	}

	int global_number_in_circle;

	MPI_Reduce(&number_in_circle, &global_number_in_circle, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);

	if(rank==0)
	{
		double pi_estimate = 4∗number in circle/((double) number of tosses);
		printf("Sum: %1.4f\n", pi_estimate);
	}

	MPI_Finalize();

	return 0;
}