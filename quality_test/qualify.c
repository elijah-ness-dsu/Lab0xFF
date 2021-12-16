//USER PARAMETERS
// N_MIN:			The starting nodes in the graph (greater than 4)
// ROWS:			Maximum number of rows to be printed out
// trialsNum:		Number of trials to conduct for each given size of N for good average

// GRAPH_TYPE: 0-2 for Random, Euclidean, Circular Euclidean
// SEED:	   Random generator seed
// MAX:        0.x-100 (or greater if not displaying) Maximum random number to generate. Euclidean graphs displayed best with 100 or less.
// RADIUS:	   0.x-50 (or greater if not displaying) Radius of circular graph. Circular graphs best displayed with 50 or less.
// ALG:		   0-1 for the algorithm to use (Greedy, Ant Colony)
#define N_MIN 4
#define ROWS  100

const int trialsNum = 20;

#define GRAPH_TYPE 1
#define SEED 123
#define MAX 100.0
#define RADIUS 100.0
#define ALG 1
//END USER PARAMETERS



// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>

// Local dependencies
#include "../utility/makegraph.h"
#include "../algorithm/bruteforce.h"
#include "../algorithm/greedy.h"
//#include "../algorithm/dynamic.h"
#include "../algorithm/antcolony.h"

// Init N_MAX to N_MIN. Actually calculated in main().
unsigned long long N_MAX = N_MIN;

// Prototypes
double getTheoryDoublingRatio(unsigned long long nReal, unsigned long long nPrevious);
unsigned long long factorial(unsigned long long N);

int main ()
{
	// Calculate the the max N value
	for (int i = 1; i < ROWS; ++i) {
		++N_MAX;
	}
	
	// Seed the RNG with the current time
	srand(time(NULL));
	
	
	// Print Table headers
	printf("           Graph Size (N)       Avg Solution Cost       Exact Solution Cost                  Avg SQR\n");
	
	// Create a for loop to double the input size up until nMax is reached
	unsigned long long nReal = 0;
	unsigned long long index = 0;		// The current row
	
	
	for (nReal = N_MIN; nReal <= N_MAX; ++nReal) {
		double *matrix;
		double *xs;
		double *ys;
		
		int *bestOrder = calloc((nReal+1), sizeof(int));
		double *bestTotal = calloc(1, sizeof(double));
		
		// Reset trials count for next while loop
		int trialsCount = 0;
		
		double heuristicCost = 0.0;
		
		// Run all trials
		while (trialsCount < trialsNum) {
			
			// Setup
			#if GRAPH_TYPE == 0
				generateRandomCostMatrix(nReal, &matrix, MAX, SEED);
			#elif GRAPH_TYPE == 1
				generateRandomEuclideanCostMatrix(nReal, &matrix, &xs, &ys, MAX, SEED);
				free(xs);
				free(ys);
			#elif GRAPH_TYPE == 2
				generateRandomCircularGraphCostMatrix(nReal, &matrix, &xs, &ys, RADIUS, SEED);
				free(xs);
				free(ys);
			#else
				printf("Error: Invalid GRAPH_TYPE in time.c. Must be in range 0-2.");
				exit(1);
			#endif
			
			// Run
			#if ALG == 0
				greedy(&matrix, nReal, &bestOrder, bestTotal);
			#elif ALG == 1
				int *currentOrder = malloc((nReal+1) * sizeof(int));
				for (int i = 0; i < nReal; ++i)
					currentOrder[i] = i;
				currentOrder[nReal] = 0;
		
				*bestTotal = DBL_MAX; // Max value of an double
		
				antColony(&matrix, nReal, &bestOrder, &currentOrder, bestTotal);
		
				free(currentOrder);
			#else
				printf("Error: Invalid ALG in qualify.c. Must be in range 0-1.");
				exit(1);
			#endif
			
			heuristicCost += *bestTotal;
			
			free(bestOrder);
			free(bestTotal);
			free(matrix);
			
			bestOrder = calloc((nReal+1), sizeof(int));
			bestTotal = calloc(1, sizeof(double));
			
			++trialsCount;
		}
		
		// Reset trials count for next while loop
		trialsCount = 0;
		
		double exactCost;
		// Run dummy trials
		//while (trialsCount < trialsTime) {
			
			// Setup
			#if GRAPH_TYPE == 0
				generateRandomCostMatrix(nReal, &matrix, MAX, SEED);
			#elif GRAPH_TYPE == 1
				generateRandomEuclideanCostMatrix(nReal, &matrix, &xs, &ys, MAX, SEED);
				free(xs);
				free(ys);
			#elif GRAPH_TYPE == 2
				generateRandomCircularGraphCostMatrix(nReal, &matrix, &xs, &ys, RADIUS, SEED);
				free(xs);
				free(ys);
			#else
				printf("Error: Invalid GRAPH_TYPE in time.c. Must be in range 0-2.");
				exit(1);
			#endif
			
			// Run
			int *currentOrder = malloc((nReal+1) * sizeof(int));
			for (int i = 0; i < nReal; ++i)
				currentOrder[i] = i;
			currentOrder[nReal] = 0;
		
			*bestTotal = DBL_MAX; // Max value of an double
		
			bruteForce(&matrix, 0, nReal, &bestOrder, &currentOrder, bestTotal);
			
			exactCost = *bestTotal;
			
			free(currentOrder);
			free(bestOrder);
			free(bestTotal);
			free(matrix);
			
		//	++trialsCount;
		//}
		
		// Calculate average
		double avgHeuristicCost = heuristicCost / trialsNum;

		// Calculate SQR
		double SQR = avgHeuristicCost / exactCost;
		
		printf("%25llu%25lf%25lf%25lf\n", nReal, avgHeuristicCost, exactCost, SQR);

	}
	
	return 0;
}
