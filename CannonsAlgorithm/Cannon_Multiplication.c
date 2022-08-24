#include <mpi.h>
#include <stdio.h>
#include<stdlib.h>
#include<math.h>

#define CRT_SECURE_NO_WARNINGS

int main(int argc, char** argv) {


	int rank, size;
	int iCnt, jCnt, kCnt;
	int n = 4;		
	int gdims = 2;			  
	int reorder = 1;		 
	int matrixrank;		
	int maxdims = 2;	
	int leftrank, rightrank;	
	int uprank, downrank;	
	int nlocal;			
	int shiftsource, shiftdest;		

	int A[4][4];               
	int B[4][4];               
	int C[4][4] = { 0 };      
	int dim_size[2];		
	int periods[2];		
	int coords[2];			 
	FILE* fp = NULL;	

	

	MPI_Comm new_comm; //store new Cartesian topology
	MPI_Status status;  // status object


	// Initialize the MPI environment
	MPI_Init(&argc, &argv);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	//open A file in a read mode and verify
	fp = fopen("A.txt", "r");
	if (fp == NULL)
	{
		printf("Error:unable to open file matrix file\n");
		exit(0);
	}
	else
	{
		//store matrix into  2D array
		for (int iCnt = 0; iCnt < 4; iCnt++)
		{
			for (int jCnt = 0; jCnt < 4; jCnt++)
			{

				fscanf(fp, "%d", &A[iCnt][jCnt]);

			}

		}


	}
	fclose(fp); //close A file


	//open B.txt file in a read mode and verify
	fp = fopen("B.txt", "r");
	if (fp == NULL)
	{
		printf("Error:unable to open file matrix file\n");
		exit(0);
	}
	else
	{
		//store matrix into  2D array
		for (int iCnt = 0; iCnt < 4; iCnt++)
		{
			for (int jCnt = 0; jCnt < 4; jCnt++)
			{

				fscanf(fp, "%d", &B[iCnt][jCnt]);

			}

		}


	}
	fclose(fp); //close A file


	if (rank == 0)
	{
		//print matrix A
		printf("Matrix A  :\n");
		for (int iCnt = 0; iCnt < 4; iCnt++)
		{
			for (int jCnt = 0; jCnt < 4; jCnt++)
			{
				printf("%d \t", A[iCnt][jCnt]);
			}
			printf("\n");
		}

		//print B//
		printf("Matrix B  :\n");
		for (int iCnt = 0; iCnt < 4; iCnt++)
		{
			for (int jCnt = 0; jCnt < 4; jCnt++)
			{
				printf("%d \t", B[iCnt][jCnt]);
			}
			printf("\n");
		}

	}

	if (sqrt(size) != (int)sqrt(size))
	{
		printf("ERROR : There is no perfect Squre Root of Number of Processors");
		exit(0);
	}

	//Set up the Cartesian topology
	dim_size[0] = sqrt(size);  
	dim_size[1] = sqrt(size); 

	 // Set the periods for wraparound connections 
	periods[0] = 1;  
	periods[1] = 1;  

	// Create the Cartesian topology, with rank reordering
	MPI_Cart_create(MPI_COMM_WORLD, gdims, dim_size, periods, reorder, &new_comm);
	
	
	// Get the rank and coordinates with respect to the new topology 
	MPI_Comm_rank(new_comm, &matrixrank);
	MPI_Cart_coords(new_comm, matrixrank, maxdims, coords);



	// Compute ranks of the up and left shifts 
	MPI_Cart_shift(new_comm, 0, -1, &rightrank, &leftrank);
	MPI_Cart_shift(new_comm, 1, -1, &downrank, &uprank);



	// Determine the dimension of the local matrix block 
	nlocal = n / dim_size[0];
	printf("local size = %d \t\n", nlocal);

	// Perform the initial matrix alignment
	//alignment of A matrix 
	MPI_Cart_shift(new_comm, 0, coords[0], &shiftsource, &shiftdest);
	MPI_Isend(A, nlocal* nlocal, MPI_INT, shiftdest, 1, new_comm, &status);
	MPI_Irecv(A, nlocal* nlocal, MPI_INT, shiftsource, 1, new_comm, &status);
	
	//alignment of B matrix 
	MPI_Cart_shift(new_comm, 1, coords[1], &shiftsource, &shiftdest);
	MPI_Isend(B, nlocal* nlocal, MPI_INT, shiftdest, 1, new_comm, &status);
	MPI_Irecv(B, nlocal* nlocal, MPI_INT, shiftsource, 1, new_comm, &status);

	//Get into the main computation loop 
	for (iCnt = 0; iCnt < dim_size[0]; iCnt++)
	{
		for (iCnt = 0; iCnt < n; iCnt++)
			for (jCnt = 0; jCnt < n; jCnt++)
				for (kCnt = 0; kCnt < n; kCnt++)
					C[iCnt][jCnt] += A[iCnt][kCnt] * B[kCnt][jCnt]; 

     // Shift matrix a left by one 
		MPI_Isend(A, nlocal* nlocal, MPI_INT,leftrank, 1, new_comm, &status);
		MPI_Irecv(A, nlocal* nlocal, MPI_INT, rightrank, 1, new_comm, &status);

     // Shift matrix b up by one
		MPI_Isend(B, nlocal* nlocal, MPI_INT, uprank, 1, new_comm, &status);
		MPI_Irecv(B, nlocal* nlocal, MPI_INT, downrank, 1, new_comm, &status);

	}

	// print matrix c
	printf("resultant matrix:\n");
	if (rank == 0)
	{
		for (iCnt = 0; iCnt < n; iCnt++)
		{
			for (jCnt = 0; jCnt < n; jCnt++)
				printf("%d\t", C[iCnt][jCnt]);
			printf("\n");
		}
		printf("\n");
	}


	MPI_Finalize();
}
