#include <stdio.h>
#include <stdlib.h>
#include<math.h>


int main()
{
	int N = 3;
	int iCnt, i,jCnt;
	float sum = 0;
	float ratio;  
	float c = 1;
	float  matrixA[3][3] = { 0.0 };         
	float  matrixB[3] = { 0.0 };  
	float  x[3] = { 0.0 };
	float  y[3] = { 0.0 };
	float  b[3] = { 0.0 };


	FILE* fp = NULL;
	

	//open matrixA.txt file
	fp = fopen("matrixA.txt", "r");
	if (fp == NULL)
	{
		printf("Unable to Open file\n");
		exit(0);
	}
	else
	{
		
		for (int iCnt = 0; iCnt < N; iCnt++)
		{
			for (int jCnt = 0; jCnt < N; jCnt++)
			{

				fscanf(fp, "%f", &matrixA[iCnt][jCnt]);

			}

		}


	}
	fclose(fp); 


	//open matrixB.txt file 
	fp = fopen("matrixB.txt", "r");
	if (fp == NULL)
	{
		printf("Unable to open file \n");
		exit(0);
	}
	else
	{


		for (int iCnt = 0; iCnt < N; iCnt++)
		{
			fscanf(fp, "%f", &matrixB[iCnt]);

		}

		
		fclose(fp);
	}

		//print matrix A
		printf("Matrix A :\n");
		for (int iCnt = 0; iCnt < N; iCnt++)
		{
			for (int jCnt = 0; jCnt < N; jCnt++)
			{
				printf("%f \t", matrixA[iCnt][jCnt]);
			}
			printf("\n");
		}

		//print matrix B
		printf("matrix B :\n");
		for (int iCnt = 0; iCnt < N; iCnt++)
		{
			printf("%f\t", matrixB[iCnt]);
		}
		printf("\n");


	
			

			for (iCnt = 1; iCnt < N; iCnt++)      
			{

				x[iCnt] = x[iCnt] / ratio;     
				matrixB[iCnt] = matrixB[iCnt] / ratio;  
				matrixA[iCnt][jCnt] = x[iCnt];


			}

			
			for (iCnt = 0; iCnt < N; iCnt++)
			{
				printf("%f\t", x[iCnt]);
				printf("\n");
			}
			printf("B[]:%f\t", matrixB[iCnt]);


		
	

		for (i = N - 1; i > -1; --i)  // Back substitution
		{
			sum = 0;
			if (i == N - 1)
			{
				y[i] = b[i];
				printf("x%d : %f\t", i, x[i]);
			}
			
			printf("\n");

		}
		
}
