#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *a, int *b, int l, int m, int r) 
{	
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	
	while((h <= m) && (j <= r)) 
	{	
		if(a[h] <= a[j]) 
		{	
			b[i] = a[h];
			h++;	
		}	
		else 
		{	
			b[i] = a[j];
			j++;	
		}
		i++;
	}	
	if(m < h) 
	{	
		for(k = j; k <= r; k++) 
		{	
			b[i] = a[k];
			i++;	
		}	
	}	
	else 
	{	
		for(k = h; k <= m; k++) 
		{	
			b[i] = a[k];
			i++;	
		}
	}
		
	for(k = l; k <= r; k++) 
		a[k] = b[k];	
}

void mergeSort(int *a, int *b, int l, int r) 
{	
	int m;	
	if(l < r) 
	{	
		m = (l + r)/2;	
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
	}		
}

int main(int argc, char *argv[]) 
{	
	int n = atoi(argv[1]);
	int *vec = malloc(n * sizeof(int));
	
	int c;
	srand(time(NULL));
	printf("This is the unsorted array: ");
	for(c = 0; c < n; c++) 
	{	
		vec[c] = rand() % n;
		printf("%d ", vec[c]);		
	}

	printf("\n");
	printf("\n");
	
	int world_rank;
	int world_size;
	
	MPI_INIT(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
	int size = n/world_size;
	
	int *sub = malloc(size * sizeof(int));
	MPI_Scatter(vec, size, MPI_INT, sub, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *tmp = malloc(size * sizeof(int));
	mergeSort(sub, tmp, 0, (size - 1));
	
	int *sorted = NULL;
	if(world_rank == 0) 
	{	
		sorted = malloc(n * sizeof(int));	
	}
	
	MPI_Gather(sub, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(world_rank == 0) 
	{	
		int *other = malloc(n * sizeof(int));
		mergeSort(sorted, other, 0, (n - 1));
		
		printf("This is the sorted array: ");
		for(c = 0; c < n; c++) 
			printf("%d ", sorted[c]);	
			
		printf("\n");
		printf("\n");
			
		free(sorted);
		free(other);	
	}
	
	free(vec);
	free(sub);
	free(tmp);
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();	
}