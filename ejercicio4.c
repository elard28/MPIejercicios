#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int MAX_CONTRIB = 20;


int Global_sum(int my_contrib, int my_rank, int p, MPI_Comm comm) {
    int        sum = my_contrib;
    int        temp;
    int        partner;
    unsigned   bitmask = 1;

    while (bitmask < p) {
        partner = my_rank ^ bitmask;
        MPI_Sendrecv(&sum, 1, MPI_INT, partner, 0, 
                    &temp, 1, MPI_INT, partner, 0, 
                    comm, MPI_STATUS_IGNORE);
        sum += temp;
        bitmask <<= 1;
    }

    return sum;
} 

int main(void) {
   int      p, my_rank;
   MPI_Comm comm;
   int      my_contrib;
   int      sum;

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   srandom(my_rank);
   my_contrib = random() % MAX_CONTRIB;

   sum = Global_sum(my_contrib, my_rank, p, comm);

   MPI_Finalize();
   return 0;
} 