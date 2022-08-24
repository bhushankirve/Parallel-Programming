#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define CRT_SECURE_NO_WARNINGS



void main(int argc, char* argv[])
{

	int size;  //store size of processors
	int rank;  //number of processors
	int local_size;  //number of working processor
	int i, j; 

	float  matrix[8][8];  //matrix
	float  vector[8] = { 0.0 };  //vector
	float  recvbuff[8];  // buffer of processor
	float  ans = 0.0; //ans of multiplication
    float	 finalVector[8] = { 0.0 }; //resultant vector 


	FILE* fp = NULL;  //file pointer
	

	// Initialize the MPI environment
	MPI_Init(&argc, &argv);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	//open matrix.txt file in a read mode and verify
	fp = fopen("matrix.txt", "r");
	if (fp == NULL)
	{
		printf("Error:unable to open file matrix file\n");
		exit(0);
	}
	else
	{
		//store matrix into  matrix array
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{

					fscanf(fp, "%f", &matrix[i][j]);
					
				}
				
			}
		

	}
	fclose(fp); //close matrix.txt file



	//open vector.txt file in a read mode and verify
	fp = fopen("vector.txt", "r");
	if (fp == NULL)
	{
		printf("Error:unable to open file vector file \n");
		exit(0);
	}
	else
	{
		
			
			for (int i = 0; i < 8; i++)
			{
				fscanf(fp, "%f", &vector[i]);
				
			}

		//close vector.txt
		fclose(fp);
	}

	if (rank == 0) 
	{
		//print matrix//
		printf("Matrix  :\n");
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				printf("%f \t", matrix[i][j]);
			}
			printf("\n");
		}

		//print vector//
		printf("vector :\n");
		for (int i = 0; i < 8; i++)
		{
			printf("%f\t", vector[i]);
		}
		printf("\n");

	}


	
	// determine how many elements each process will work on
	local_size = 8 / size;


	//scatter distinct element of vector to recvbuff array
	MPI_Scatter(vector, local_size, MPI_FLOAT, recvbuff, local_size, MPI_FLOAT, 0, MPI_COMM_WORLD);


	
	//gather data from every recbuff 
	MPI_Allgather(recvbuff, local_size, MPI_FLOAT, recvbuff, local_size, MPI_FLOAT, MPI_COMM_WORLD);


	//multiple recvbuff * vector
	//print value computed by each processor
	printf("rank %d:\t",rank);
	for (int i = 0; i < 8; i++)
	{
		ans = ans + (recvbuff[i] * vector[i]);
		printf("%f\t", ans);
	}
	printf("\n");
        
	       
	//gather value from each processor
	MPI_Gather(&ans,1, MPI_FLOAT, finalVector,1, MPI_FLOAT, 0, MPI_COMM_WORLD);
		
		
	  //print matrix vector multiplication//
		if (rank == 0)
		{
			printf("Matrix Vector multiplication is:\n");
			for (int i = 0; i < 8; i++)
			{
				printf("%f\n", finalVector[i]);
			}
			
		}

	
	

//Terminate MPI Environment and Processes
	MPI_Finalize();
}
