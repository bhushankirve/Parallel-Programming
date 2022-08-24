#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define CRT_SECURE_NO_WARNINGS



int main(int argc, char** argv)
{

	int size, rank;
	int local_size;  
	int N = 3;
	int i, p,j;
	float sum = 0;
	float pivot;  
	float c = 1;
	float C = 1;


	float  matrixA[3][3] = { 0.0 };         
	float  matrixB[3] = { 0.0 };  
	float  x[3] = { 0.0 };
	float  y[3] = { 0.0 };
	float  b[3] = { 0.0 };


	FILE* fp = NULL;  //file pointer
	MPI_Status status;

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);



	//open matrixA.txt file
	fp = fopen("matrixA.txt", "r");
	if (fp == NULL)
	{
		printf("ERROR:unable to open file file\n");
		exit(0);
	}
	else
	{
		//store matrix into  matrix array
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{

				fscanf(fp, "%f", &matrixA[i][j]);

			}

		}


	}
	fclose(fp); 


	//open matrixB.txt file 
	fp = fopen("matrixB.txt", "r");
	if (fp == NULL)
	{
		printf("ERROR : unable to open file file \n");
		exit(0);
	}
	else
	{


		for (int i = 0; i < N; i++)
		{
			fscanf(fp, "%f", &matrixB[i]);

		}

		
		fclose(fp);
	}

	if (rank == 0)
	{
		//print matrix A
		printf("Matrix A :\n");
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				printf("%f \t", matrixA[i][j]);
			}
			printf("\n");
		}

		//print matrix B
		printf("matrix B :\n");
		for (int i = 0; i < N; i++)
		{
			printf("%f\t", matrixB[i]);
		}
		printf("\n");

	}

	//size of block
	local_size = 8 / size;
	printf("block size : %d \n", local_size);


	MPI_Scatter(&matrixA, N, MPI_FLOAT, &x, N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&matrixA, N, MPI_FLOAT, &y, N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&matrixB, N, MPI_FLOAT, &b, N, MPI_INT, 0, MPI_COMM_WORLD);



	for (p = 0; p < N; p++)
	{
		if (rank == p)
		{
			pivot = matrixA[p][p];     // initialize pivot element

			for (i = p; i < N; i++)       //division step
			{

				x[i] = x[i] / pivot;     //for  matrix A 
				matrixB[i] = matrixB[i] / pivot;  //  for matrix B
				matrixA[p][i] = x[i];


			}

			// printf row on which division step is perform
			for (i = 0; i < N; i++)
			{
				printf("%f\t", x[i]);
				printf("\n");
			}
			printf("B[]:%f\t", matrixB[p]);


			MPI_Bcast(&x, N, MPI_FLOAT, p, MPI_COMM_WORLD);      //send the modified row to the other processor of matrix A
			MPI_Bcast(&matrixB[p], 1, MPI_FLOAT, p, MPI_COMM_WORLD);  //send the modified element to the other processor  of matrix B

		}
		else
		{
			if (rank > p)
			{
				c = y[p];

				for (i = 0; i < N; i++)
				{
					y[i] = y[i] - c * matrixA[p][i];   // for matrix a 
					b[p] = b[p] - c * matrixB[p];  // for matrix b
				}
			

			}

		}
	}

		for (p = N - 1; p > -1; --p)  // Back substitution
		{
			sum = 0;
			if (p == N - 1)
			{
				y[p] = b[p];
				printf("x%d : %f\t", p, x[p]);
			}
			else
			{
				for (i = p + 1; i < N; i++)
					sum = sum + y[i] * x[i];
				    y[p] = b[p] - sum;
				printf("x%d : %f\t", p, x[p]);
			}
			MPI_Bcast(&y, N, MPI_FLOAT, p, MPI_COMM_WORLD);
			
			printf("\n");

		}
		

	// Finalize the MPI environment.
	MPI_Finalize();
}
