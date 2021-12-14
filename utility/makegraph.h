#pragma once

// Prototypes
void generateRandomCostMatrix(int N, double **matrix, double maxRand, int seed);

void generateRandomEuclideanCostMatrix(int N, double **matrix, double **xArray, double **yArray, double maxRand, int seed);

void generateRandomCircularGraphCostMatrix(int N, double **matrix, double **xArray, double **yArray, double radius, int seed);

int readCostMatrixFromFile(double **matrix, const char *fileName);