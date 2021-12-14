#pragma once

// Prototypes
void generateRandomCostMatrix(int N, double **matrix, double maxRand, int seed);
void generateRandomEuclideanCostMatrix(int N, double **matrix, double maxRand, int seed, int plotGraph);
void generateRandomCircularGraphCostMatrix(int N, double **matrix, double radius, int seed);
int readCostMatrixFromFile(double **matrix, char *fileName);