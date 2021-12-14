// USER PARAMETERS
// GRAPH_TYPE: 0-3 for Random, Euclidean, Circular Euclidean, or File Input
// PLOT_GRAPH: 0-1 for Yes/No decision whether the program should draw the graph and solution (only applicable to GRAPH_TYPE 1-2)
// FILE:	   string representing the file path and name for the input file
// SEED:       0-INT_MAX for the seed value the generator will use (only applicable to GRAPH_TYPE 0-2)
// NODES:	   (Keep small for verification) number of nodes in the generated graph (only applicable to GRAPH_TYPE 0-2)
// ALG:		   0-3 for the algorithm to use (BruteForce, Greedy, Dynamic Programming, Ant Colony)
#define GRAPH_TYPE 1
#define PLOT_GRAPH 1

#if GRAPH_TYPE == 3
	const char *FILE = "../input/example.txt";
#endif

#define SEED 1255225
#define NODES 6
#define ALG 1

#if ALG == 0 || ALG == 2
	#define ALG_TYPE 1
#else
	#define ALG_TYPE 2
#endif

// END USER PARAMETERS



// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

// Include local dependencies
#include "../utility/makegraph.h"
#include "../utility/plotgraph.h"
#include "../algorithm/bruteforce.h"

int main()
{
	int *bestOrder;
	bestOrder = calloc((NODES+1), sizeof(int));
	
	double *bestTotal = calloc(1, sizeof(double));
	
	double *matrix;
	
	generateRandomEuclideanCostMatrix(NODES, &matrix, 100, SEED, PLOT_GRAPH);
	
	printf("Matrix of edges: \n");
	for(int i = 0; i < NODES*NODES; ++i)
		printf("%f ", matrix[i]);
	printf("\n--------------------\n\n");
	
	if (ALG == 0)
	{
		int *currentOrder = malloc((NODES+1) * sizeof(int));
		for (int i = 0; i < NODES; ++i)
			currentOrder[i] = i;
		currentOrder[NODES] = 0;
		
		*bestTotal = DBL_MAX; // Max value of an double
		
		bruteForce(&matrix, 0, NODES, &bestOrder, &currentOrder, bestTotal);
		
		free(currentOrder);
	}
	else if (ALG == 1)
	{
		greedy(&matrix, NODES, &bestOrder, bestTotal);
	}
	else if (ALG == 2)
	{
	}
	else if (ALG == 3)
	{
	}
	else
	{
		printf("Error: ALG macro must be in rangee 0-3.\n");
	}
	
	ShowPlot(NODES, &bestOrder, ALG_TYPE);
	
	printf("Solution node order: ");
	for (int i = 0; i <= NODES; ++i)
	{
		printf("%d ", bestOrder[i]);
	}
	printf("\n");
	printf("Solution path cost: %.2lf\n", *bestTotal);
	
	free(bestOrder);
	free(bestTotal);
	free(matrix);
	return 0;
}