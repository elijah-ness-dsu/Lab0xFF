#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Avoid having to repeat prototypes
#include "makegraph.h"

const char fileName[] = "costArrayFile.txt";

void generateRandomCostMatrix(int N, double **matrix, double maxRand, int seed)
{
	(*matrix) = calloc((N * N), sizeof(double));
	srand(seed); 
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (j > i)
			{ 
				(*matrix)[i*N + j] = ((double)rand()/(double)(RAND_MAX)) * maxRand;
			} 
			else if (j < i)
			{
				(*matrix)[i*N + j] = (*matrix)[j*N + i];
			}
			else
			{
				(*matrix)[i*N + j] = -1;
			}
		}
	}
}

void generateRandomEuclideanCostMatrix(int N, double **matrix, double maxRand, int seed)
{
	double *xArray = malloc(N * sizeof(double));
	double *yArray = malloc(N * sizeof(double));
	(*matrix) = calloc((N * N), sizeof(double));
	
	srand(seed); 
	
	for (int i = 0; i < N; ++i)
	{
		xArray[i] = ((double)rand()/(double)(RAND_MAX)) * maxRand;
		yArray[i] = ((double)rand()/(double)(RAND_MAX)) * maxRand;
	}
	
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (j > i)
			{ 
				(*matrix)[i*N + j] = pow(pow(xArray[i]-xArray[j], 2) + pow(yArray[i]-yArray[j], 2), 0.5);
			} 
			else if (j < i)
			{
				(*matrix)[i*N + j] = (*matrix)[j*N + i];
			}
			else
			{
				(*matrix)[i*N + j] = -1;
			}
		}
	}
}

// Necessary functions for generateRandomCircularGraphCostMatrix()
// A utility function to swap to integers
void swap (double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

// A function to generate a random permutation of arr[]
void randomize(double array[], int n, int seed)
{
    srand (seed);
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first two element that's why i > 1
    for (int i = n-1; i > 1; i--)
    {
        // Pick a random index from 1 to i
        int j = (rand() % i) + 1;
 
        // Swap array[i] with the element at random index
        swap(&array[i], &array[j]);
    }
}

void generateRandomCircularGraphCostMatrix(int N, double **matrix, double radius, int seed)
{
	double *xArray = malloc(N * sizeof(double));
	double *yArray = malloc(N * sizeof(double));
	(*matrix) = calloc((N * N), sizeof(double));
	
	srand(seed); 
	
	for (int i = 0; i < N; ++i)
	{
		double stepAngle = 2 * M_PI/(double)N ;
		xArray[i] = radius * sin((double)(i+1) * stepAngle);
		yArray[i] = radius * cos((double)(i+1) * stepAngle);
	}
	
	randomize(xArray, N, seed);
	randomize(yArray, N, seed);
	
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if (j > i)
			{ 
				(*matrix)[i*N + j] = pow(pow(xArray[i]-xArray[j], 2) + pow(yArray[i]-yArray[j], 2), 0.5);
			} 
			else if (j < i)
			{
				(*matrix)[i*N + j] = (*matrix)[j*N + i];
			}
			else
			{
				(*matrix)[i*N + j] = -1;
			}
		}
	}
}

void readCostMatrixFromFile(double **matrix)
{	
	FILE *fp;
	fp = fopen(fileName, "r");
	
	int N;
	int edges;
	
	fscanf(fp, "%d", &N); // Get the number of nodes
	
	(*matrix) = calloc((N * N), sizeof(double));
	
	for (int i = 0; i < N; ++i)
		(*matrix)[i*N + i] = -1;
	
	fscanf(fp, "%d", &edges); // Get the number of edges
	
	for (int i = 0; i < edges; ++i)
	{
		int firstNode;
		int secondNode;
		float value;
		
		fscanf(fp, "%d", &firstNode);
		fscanf(fp, "%d", &secondNode);
		fscanf(fp, "%f", &value);
		
		(*matrix)[firstNode*N + secondNode] = value;
		(*matrix)[secondNode*N + firstNode] = value;
	}
	
	fclose(fp);
}
