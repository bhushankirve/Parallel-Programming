#include<stdio.h>
#include<mpi.h>


int main(int argc, char** argv)
{
	int  a[110];
	int i, size, N = 100, n, rank;
	int offset = 0, upto = 0, sum = 0, sum_of_all = 0;


	// initialize MPI environmnt
	MPI_Init(&argc, &argv);



	//get size of communicator
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//get number of processor in a  communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	//genrating  100 numbers
	for (i = 0; i <= N; i++)
	{
		a[i] = i;
	}

	//calculating size of data to be distribute
	n = N / size;

	// processor 0 receive  the data from  other processors  and computer the total sum 
	// processor 0 compute first 0 - n numbers sum
	if (rank == 0)
	{


		offset = n * rank;
		printf("offset of rank%d : %d \n", rank, offset);
		upto = offset + n;
		printf("upto of rank%d : %d \n", rank, upto);



		//compute sum from 0 - 24 numbers
		for (i = offset; i < upto; i++)
		{
			sum += a[i];
		}
		sum_of_all = sum_of_all + sum;
		printf("sum of rank%d : %d \n", rank, sum);



		// receive  data from  n-1 processor
		for (i = 1; i < size; i++)
		{
			MPI_Recv(&sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum_of_all = (sum_of_all + sum);
		}

		//compute sum of all partial sum  send by n-1 processors
		printf("Total sum of numbers : %d \n", sum_of_all);


	}
	else
	{
		//rank other than zero will compute sum of n numbers
		for (i = 1; i < size; i++)
		{

			if (rank == i)
			{
				//calculating start value
				offset = (n * rank);
				printf("offset of rank%d : %d \n", rank, offset);

				//calculating end value
				upto = offset + n;
				printf("upto of rank%d : %d \n", rank, upto);

				if (upto == N)
				{
					upto = upto + 1;
				}


				for (i = offset; i < upto; i++)

					sum = sum + a[i];
			}

			//print the sum calculated by each processor
			printf("sum of rank%d : %d \n", rank, sum);
		}


		//send partial sum to 0th processor
		MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();

}

