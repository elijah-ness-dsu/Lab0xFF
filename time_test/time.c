//USER PARAMETERS
// N_MIN:			The starting nodes in the graph (greater than 4)
// ROWS:			Maximum number of rows to be printed out
// trialsTimeMax:	Maxmimum time in seconds that a trial can take
// trialsCountMax:	Maximum number of trials that can occur on a given size of N

// GRAPH_TYPE: 0-2 for Random, Euclidean, Circular Euclidean
// MAX:        0.x-100 (or greater if not displaying) Maximum random number to generate. Euclidean graphs displayed best with 100 or less.
// RADIUS:	   0.x-50 (or greater if not displaying) Radius of circular graph. Circular graphs best displayed with 50 or less.
// ALG:		   0-3 for the algorithm to use (BruteForce, Greedy, Dynamic Programming, Ant Colony)
#define N_MIN 4
#define ROWS  100

#include <time.h>
const clock_t trialsTimeMax = 0.250 * CLOCKS_PER_SEC;
const int trialsCountMax = 1000000;

#define GRAPH_TYPE 0
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
	
	// Declare all arrays to hold data
	unsigned long long listSizeArray[ROWS];
	double avgExperimentTimeArray[ROWS];
	double experimentDRatioArray[ROWS];
	double theoryDRatioArray[ROWS];
	
	// Print Table headers
	printf("           Graph Size (N)      Avg Experiment Time        Experiment DRatio            Theory DRatio\n");
	
	// Create a for loop to double the input size up until nMax is reached
	unsigned long long nReal = 0;
	unsigned long long index = 0;		// The current row
	
	
	for (nReal = N_MIN; nReal <= N_MAX; ++nReal) {
		double *matrix;
		double *xs;
		double *ys;
		
		int *bestOrder = calloc((nReal+1), sizeof(int));
		double *bestTotal = calloc(1, sizeof(double));
		
		int trialsCount = 0;
		clock_t trialsTime = 0.0;
	
		
		// Get start timestamp 
		clock_t startTime = clock();
		
		// Run all trials
		while (trialsCount < trialsCountMax && trialsTime <= trialsTimeMax) {
			
			// Setup
			#if GRAPH_TYPE == 0
				generateRandomCostMatrix(nReal, &matrix, MAX, time(0));
			#elif GRAPH_TYPE == 1
				generateRandomEuclideanCostMatrix(nReal, &matrix, &xs, &ys, MAX, time(0));
				free(xs);
				free(ys);
			#elif GRAPH_TYPE == 2
				generateRandomCircularGraphCostMatrix(nReal, &matrix, &xs, &ys, RADIUS, time(0));
				free(xs);
				free(ys);
			#else
				printf("Error: Invalid GRAPH_TYPE in time.c. Must be in range 0-2.");
				exit(1);
			#endif
			
			// Run
			#if ALG == 0
				int *currentOrder = malloc((nReal+1) * sizeof(int));
				for (int i = 0; i < nReal; ++i)
					currentOrder[i] = i;
				currentOrder[nReal] = 0;
		
				*bestTotal = DBL_MAX; // Max value of an double
		
				bruteForce(&matrix, 0, nReal, &bestOrder, &currentOrder, bestTotal);
		
				free(currentOrder);
			#elif ALG == 1
				greedy(&matrix, nReal, &bestOrder, bestTotal);
			#elif ALG == 2
				printf("Error: Invalid ALG in time.c. Dynamic Programming not implemented");
				exit(1);
			#elif ALG == 3
				int *currentOrder = malloc((nReal+1) * sizeof(int));
				for (int i = 0; i < nReal; ++i)
					currentOrder[i] = i;
				currentOrder[nReal] = 0;
		
				*bestTotal = DBL_MAX; // Max value of an double
		
				antColony(&matrix, nReal, &bestOrder, &currentOrder, bestTotal);
		
				free(currentOrder);
			#else
				printf("Error: Invalid ALG in time.c. Must be in range 0-3.");
				exit(1);
			#endif
			
			// Get total time since start of loop
			clock_t splitTime = clock();
			trialsTime = (splitTime - startTime);
			
			++trialsCount;
		}
		double trials1 = trialsTime;
		
		// Use as counter maxmimum for the overhead loop
		int numTrialsPerformed = trialsCount;
		
		// Microseconds
		double totalTimeRaw =  (trialsTime * 1000000.0) / CLOCKS_PER_SEC;
		double avgTimePerTrial = (totalTimeRaw)/ (double)numTrialsPerformed;
		
		trialsCount = 0;
		trialsTime = 0;
		startTime = clock();
		
		// Run dummy trials
		while (trialsCount < numTrialsPerformed && trialsTime < trialsTimeMax) {
			
			// Setup
			#if GRAPH_TYPE == 0
				generateRandomCostMatrix(nReal, &matrix, MAX, time(0));
			#elif GRAPH_TYPE == 1
				generateRandomEuclideanCostMatrix(nReal, &matrix, &xs, &ys, MAX, time(0));
				free(xs);
				free(ys);
			#elif GRAPH_TYPE == 2
				generateRandomCircularGraphCostMatrix(nReal, &matrix, &xs, &ys, RADIUS, time(0));
				free(xs);
				free(ys);
			#else
				printf("Error: Invalid GRAPH_TYPE in time.c. Must be in range 0-2.");
				exit(1);
			#endif
			
			// Run
			#if ALG == 0
				int *currentOrder = malloc((nReal+1) * sizeof(int));
				for (int i = 0; i < nReal; ++i)
					currentOrder[i] = i;
				currentOrder[nReal] = 0;
		
				*bestTotal = DBL_MAX; // Max value of an double
		
				bruteForceDummy(&matrix, 0, nReal, &bestOrder, &currentOrder, bestTotal);
		
				free(currentOrder);
			#elif ALG == 1
				greedyDummy(&matrix, nReal, &bestOrder, bestTotal);
			#elif ALG == 2
				printf("Error: Invalid ALG in time.c. Dynamic Programming not implemented");
				exit(1);
			#elif ALG == 3
				int *currentOrder = malloc((nReal+1) * sizeof(int));
				for (int i = 0; i < nReal; ++i)
					currentOrder[i] = i;
				currentOrder[nReal] = 0;
		
				*bestTotal = DBL_MAX; // Max value of an double
		
				antColonyDummy(&matrix, nReal, &bestOrder, &currentOrder, bestTotal);
		
				free(currentOrder);
			#else
				printf("Error: Invalid ALG in time.c. Must be in range 0-3.");
				exit(1);
			#endif

			
			// Get total time since start of loop
			clock_t splitTime = clock();
			trialsTime = (splitTime - startTime);
			
			++trialsCount;
		}
		
		double totalTimeOverhead = (trialsTime * 1000000.0) / CLOCKS_PER_SEC;	// Microseconds
		
		double avgOverheadTimePerTrial = (totalTimeOverhead) / (double)numTrialsPerformed;
		double estimatedTimePerTrial = (avgTimePerTrial - avgOverheadTimePerTrial);
		
		
		double totalTime = totalTimeRaw / 1000.0;		// JUST the total time, we don't need the overhead time here because the original loop already included overhead!
		double algTime = (totalTimeRaw - totalTimeOverhead) / 1000.0;
		
		// Fill in the table data
		listSizeArray[index] = nReal;
		avgExperimentTimeArray[index] = estimatedTimePerTrial;
		
		if (index > 0 && nReal % 2 == 0 && index > 2) {
			
			experimentDRatioArray[index] = avgExperimentTimeArray[index] / avgExperimentTimeArray[(nReal / 2) - 1 - 3];
			
			// Call getTheoryDoublingRatio function
			theoryDRatioArray[index] = getTheoryDoublingRatio(listSizeArray[index], listSizeArray[(nReal / 2) - 1 - 3]);
		}
		
		if (index == 0 || nReal % 2 != 0 || nReal == 2)
		{
			printf("%25llu%23.2lfus%22cNAN%22cNAN\n", listSizeArray[index], avgExperimentTimeArray[index], ' ', ' ');
		}
		else {
			printf("%25llu%23.2lfus%25lf%25.4lf\n", listSizeArray[index], avgExperimentTimeArray[index], experimentDRatioArray[index], theoryDRatioArray[index]);
		}

		free(bestOrder);
		free(bestTotal);
		free(matrix);

		++index;
	}
	
	return 0;
}

double getTheoryDoublingRatio(unsigned long long nReal, unsigned long long nPrevious)
{
	#if ALG == 0
		return (double)factorial(nReal) / (double)factorial(nPrevious);
	#elif ALG == 1
		return (double)(nReal * nReal) / (double)(nPrevious * nPrevious);
	#elif ALG == 2
		return pow(2.0, nReal) / pow(2.0, nReal);
	#elif ALG == 3
		return 0.0;
	#else
		printf("Error: Invalid ALG in time.c. Must be in range 0-3.");
		exit(1);
	#endif
}

unsigned long long factorial(unsigned long long N)
{
	if (N == 0)
        return 1;
    return N * factorial(N - 1);
}
