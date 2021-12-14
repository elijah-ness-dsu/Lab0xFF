#ifndef MAKEGRAPH_H_INCLUDED
#define MAKEGRAPH_H_INCLUDED

// Prototypes
void generateRandomCostMatrix(int N, double **matrix, double maxRand, int seed);
void generateRandomEuclideanCostMatrix(int N, double **matrix, double maxRand, int seed);
void swap (double *a, double *b);
void randomize(double array[], int n, int seed);
void generateRandomCircularGraphCostMatrix(int N, double **matrix, double radius, int seed);
void readCostMatrixFromFile(double **matrix);
#endif