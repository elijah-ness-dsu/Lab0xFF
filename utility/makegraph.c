// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Include local dependencies
#include "changearray.h"

// Avoid having to repeat prototype
#include "makegraph.h"

// Function to generate a random cost matrix (non-Euclidean)
// N: 			The Number of nodes in the graph.
// matrix: 		A pointer to be used to pass the array back to the caller.
// maxRand: 	The maxmimum random double value that can be generated for an edge's cost.
// seed: 		The seed the random generator is to use.
void generateRandomCostMatrix(int N, double **matrix, double maxRand, int seed)
{
	// Init the array with zeroes
	(*matrix) = calloc((N * N), sizeof(double));
	
	// Seed the random generator
	srand(seed);
	
	// Generate the symmetrical graph, using -1 to represent absent costs (such as for self edges)
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

// Function to generate a random cost matrix following Euclidean spacial laws
// N: 			The Number of nodes in the graph.
// matrix: 		A pointer to be used to pass the array back to the caller.
// maxRand: 	The maxmimum random double value that can be generated for an x or y position (assume square space).
// seed: 		The seed the random generator is to use.
void generateRandomEuclideanCostMatrix(int N, double **matrix, double maxRand, int seed)
{
	// Create arrays for the x, y coordinates of each node
	double *xArray = malloc(N * sizeof(double));
	double *yArray = malloc(N * sizeof(double));
	
	// Init the array with zeroes
	(*matrix) = calloc((N * N), sizeof(double));
	
	// Seed the random generator
	srand(seed); 
	
	// Fill coordinate arrays with random values up to maxRand.
	for (int i = 0; i < N; ++i)
	{
		xArray[i] = ((double)rand()/(double)(RAND_MAX)) * maxRand;
		yArray[i] = ((double)rand()/(double)(RAND_MAX)) * maxRand;
	}
	
	// Generate the symmetrical graph, using -1 to represent absent costs (such as for self edges)
	// Enforce Euclidean rules for edge costs (distances)
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

// Function to generate a circular Euclidean graph 
// N: 			The Number of nodes in the graph.
// matrix: 		A pointer to be used to pass the array back to the caller.
// radius:		The size of the radius of the circle
// seed: 		The seed the random generator is to use.
void generateRandomCircularGraphCostMatrix(int N, double **matrix, double radius, int seed)
{
	// Create arrays for the x, y coordinates of each node
	double *xArray = malloc(N * sizeof(double));
	double *yArray = malloc(N * sizeof(double));
	
	// Init the array with zeroes
	(*matrix) = calloc((N * N), sizeof(double));
	
	// Seed the random generator
	srand(seed); 
	
	// Fill coordinate arrays with random positions on the circle's circumference
	for (int i = 0; i < N; ++i)
	{
		double stepAngle = 2 * M_PI/(double)N ;
		xArray[i] = radius * sin((double)(i+1) * stepAngle);
		yArray[i] = radius * cos((double)(i+1) * stepAngle);
	}
	
	// Randomize both x,y coordinate arrays with the same seed
	// Should always randomize both array's indexes exactly the same to keep x,y pair integrity
	randomize(xArray, N, seed);
	randomize(yArray, N, seed);
	
	// Generate Euclidean edges connecting each node
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

// Function to read in a Euclidean graph from a file
// matrix: 		A pointer to be used to pass the array back to the caller.
// fileName:	The string value to read to open the file

// return:		Number of nodes in the graph (so the client code can process the matrix)

// FILE MUST BE OF THE FORMAT:
// N (node count)
// E (edge count)
// node node edgeCost
// node node edgeCost
// ... (E times)

int readCostMatrixFromFile(double **matrix, char *fileName)
{
	// Open the file in read mode
	FILE *fp;
	fp = fopen(fileName, "r");
	
	// Initialize variables for amount of Nodes and Edges
	int N;
	int edges;
	
	// Scan in number of nodes and edges
	fscanf(fp, "%d", &N);
	fscanf(fp, "%d", &edges);
	
	// Check if the graph is complete
	// There must be n(n-1)/2 edges in a complete graph
	int requiredEdges = N * (N - 1) / 2;
	if (edges != N * (N - 1) / 2)
	{
		printf("Error: Graph specified in file \"%s\" is not complete and the Edge Cost Matrix cannot be completed.\n", fileName);
		printf("Required %d edges but got %d.", requiredEdges, edges);
		exit(1);
	}
	
	// Init the array with zeroes
	(*matrix) = calloc((N * N), sizeof(double));
	
	// Set the cost matrix to -1 to denote "empty"
	for (int i = 0; i < N; ++i)
		(*matrix)[i*N + i] = -1;
	
	// Read in values from the file and create edges in a symmetrical cost matrix
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
	
	// Close the file
	fclose(fp);
	
	// Return the number of nodes in the graph
	return N;
}
