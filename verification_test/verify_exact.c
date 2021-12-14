// USER PARAMETERS
// GRAPH_TYPE: 0-3 for Random, Euclidean, Circular Euclidean, or File Input
// PLOT_GRAPH: 0-1 for Yes/No decision whether the program should draw the graph and solution (only applicable to GRAPH_TYPE 1-2)
// FILE:	   string representing the file path and name for the input file
// SEED:       0-INT_MAX for the seed value the generator will use (only applicable to GRAPH_TYPE 0-2)
#define GRAPH_TYPE 1
#define PLOT_GRAPH 1

#if GRAPH_TYPE == 3
	const char *FILE = "../input/example.txt";
#endif

#define SEED 123
// END USER PARAMETERS



// Include standard libraries
#include <stdio.h>
#include <float.h>

// Include local dependencies
#include "../utility/makegraph.h"
#include "../utility/plotgraph.h"
#include "../algorithm/bruteforce.h"

int main()
{
	int *bestOrder;
	int *currentOrder;

	double bestTotal = DBL_MAX; // Max value of an double
	
	double *matrix;
	int N = 5;
	
	bestOrder = malloc((N+1) * sizeof(int));
	
	currentOrder = malloc((N+1) * sizeof(int));
	
	generateRandomEuclideanCostMatrix(N, &matrix, 100, 12);
	
	printf("Matrix of edges: \n");
	for(int i = 0; i < N*N; ++i)
		printf("%f ", matrix[i]);
	printf("\n--------------------\n\n");
	
	for (int i = 0; i < N; ++i)
		currentOrder[i] = i;
		
	currentOrder[N] = 0;
	
	bruteForce(&matrix, 0, N - 1);
	
	printf("Best order: ");
	
	for (int i = 0; i <= N; ++i)
	{
		printf("%d ", bestOrder[i]);
	}
	
	printf("\n");
	
	free(matrix);
	free(bestOrder);
	free(currentOrder);
	return 0;
}