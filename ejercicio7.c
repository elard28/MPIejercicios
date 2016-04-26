#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  clock_t start, end;
  double time_used;

  double t1, t2;

  MPI_Status status;
  int num, rank, size, tag, next, from;

  start = clock();

  MPI_Init(&argc, &argv);

  t1 = MPI_Wtime();

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
 
  tag = 201;
  next = (rank + 1) % size;
  from = (rank + size - 1) % size;

  if (rank == 0) {
    printf("Numero de vueltas: ");
    scanf("%d", &num);
    MPI_Send(&num, 1, MPI_INT, next, tag, MPI_COMM_WORLD); 
  }

  do {
    MPI_Recv(&num, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);
    if (rank == 0) {
      --num;
    }
    MPI_Send(&num, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
  } while (num > 0);

  if (rank == 0)
    MPI_Recv(&num, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);

  t2 = MPI_Wtime();

  printf("TiempoMPI: %1.2f\n", t2-t1);

  end = clock();
  time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("TiempoClock: %1.2f\n", time_used);


  MPI_Finalize();

  return 0;
}