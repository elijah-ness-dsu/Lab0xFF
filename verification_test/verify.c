// USER PARAMETERS
// GRAPH_TYPE: 0-3 for Random, Euclidean, Circular Euclidean, or File Input
// PLOT_GRAPH: 0-1 for Yes/No decision whether the program should draw the graph and solution (only applicable to GRAPH_TYPE 1-2)
// FILE:	   string representing the file path and name for the input file
// NODES:	   (Keep small for verification) number of nodes in the generated graph (only applicable to GRAPH_TYPE 0-2)
// SEED:       0-INT_MAX for the seed value the generator will use (only applicable to GRAPH_TYPE 0-2)
// MAX:        0.x-100 (or greater if not displaying) Maximum random number to generate. Euclidean graphs displayed best with 100 or less.
// RADIUS:	   0.x-50 (or greater if not displaying) Radius of circular graph. Circular graphs best displayed with 50 or less.
// ALG:		   0-3 for the algorithm to use (BruteForce, Greedy, Dynamic Programming, Ant Colony)
// ALG_TYPE:   1 or 2 (Exact or Hueristic) DO NOT CHANGE. Used for printing.
#define GRAPH_TYPE 1
#define PLOT_GRAPH 1

const char *FILEPATH = "input/example.txt";
int NODES = 15;

#define SEED 25
#define MAX 100.0
#define RADIUS 100.0
#define ALG 3

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
#include "../algorithm/greedy.h"
#include "../algorithm/antcolony.h"

int main()
{
	double *matrix;
	
	// Allocate pointers for returning coordinates if using a Euclidean generation
	double *xs;
	double *ys;
	
	if (GRAPH_TYPE == 0)
	{
		generateRandomCostMatrix(NODES, &matrix, MAX, SEED);
	}
	else if(GRAPH_TYPE == 1)
	{
		generateRandomEuclideanCostMatrix(NODES, &matrix, &xs, &ys, MAX, SEED);
	}
	else if(GRAPH_TYPE == 2)
	{
		generateRandomCircularGraphCostMatrix(NODES, &matrix, &xs, &ys, RADIUS, SEED);
	}
	else if(GRAPH_TYPE == 3)
	{
		NODES = readCostMatrixFromFile(&matrix, FILEPATH);
	}
	
	if (GRAPH_TYPE == 1 || GRAPH_TYPE == 2)
	{
		printf("Node Coordinates: \n");
		for (int i = 0; i < NODES; ++i)
		{
			printf("(%.3lf, %.3lf)\n", xs[i], ys[i]);
		}
	printf("\n");
	}
	
	// Print the edge cost matrix
	printf("Edge Cost Matrix: \n");
	for(int i = 0; i < NODES + 1; ++i)
	{
		for (int j = 0; j < NODES; ++j)
		{
			if (i == NODES)
				printf("-------");
			else
				printf("%8.3f ", matrix[i*NODES + j]);
		}
		printf("\n");
	}
	
	
	int *bestOrder = calloc((NODES+1), sizeof(int));
	double *bestTotal = calloc(1, sizeof(double));
	
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
		int *currentOrder = malloc((NODES+1) * sizeof(int));
		for (int i = 0; i < NODES; ++i)
			currentOrder[i] = i;
		currentOrder[NODES] = 0;
		
		*bestTotal = DBL_MAX; // Max value of an double
		
		antColony(&matrix, NODES, &bestOrder, &currentOrder, bestTotal);
		
		free(currentOrder);
	}
	else
	{
		printf("Error: ALG macro must be in range 0-3.\n");
	}
	
	// Plot the graph
	if (PLOT_GRAPH == 1)
	{
		MakeGraphPlot(NODES, &matrix, xs, ys, GRAPH_TYPE, 0);
		ShowPathPlot(NODES, &bestOrder, ALG_TYPE);	
	}
	
	
	// Print solution path
	printf("\n");
	printf("Solution node order: ");
	for (int i = 0; i <= NODES; ++i)
	{
		printf("%d ", bestOrder[i]);
	}
	printf("\n");
	printf("Solution path cost: %.2lf\n", *bestTotal);
	printf("\n");
	
	
	free(xs);
	free(ys);
	free(bestOrder);
	free(bestTotal);
	free(matrix);
	return 0;
}