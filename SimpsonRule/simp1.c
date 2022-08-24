#include<mpi.h>
#include<stdio.h>
#include<math.h>
double f(double x)
{
	double y;
	y = -0.1*x*x*x*x + 1.3*x*x*x - 4.9*x*x + 4.1*x + 7.1;
	return y;
}

double simp(double a, double b)
{
	double x;

	x = ((b-a)/8)*(f(a)+3*f((2*a+b)/3)+3*f((a+2*b)/3)+f(b));

	return x;
}

int main(void)
{
	int my_rank, comm_sz;
  int n;
	double range_n;
	double a = 0.0, b = 6.0, h, local_a, local_b;
	double local_int, total_int=0;
	int source;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	h = (double) (b-a)/comm_sz;          // The interval in which integration is to be determined by each processor
	range_n = h*my_rank;        // The starting of interval for each rank

	local_a = range_n;
	local_b = range_n + h;
	local_int = simp(local_a, local_b);
	

	if(my_rank != 0)      // All slave process send their estimate of integration to the master process
	{
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else                  // The master process receives all the estimates and adds them to get the final value of integration
	{
		total_int = local_int;
		for(source=1; source<comm_sz; source++)
		{
   
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_int += local_int;
		}
	}
	
	if(my_rank == 0)
	{
		printf("Integral form of %f to %f  = %f \n", a, b, total_int);
	}
	MPI_Finalize();
	return 0;
}

