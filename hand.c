// RAISAAT ATIFA RASHID

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <string.h>
#include <sys/time.h>
#include <gsl/gsl_math.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
	int opt;
	float percentage;
	
	// parse the command line
	while ((opt = getopt(argc, argv, "p:")) != -1)
	{
		switch(opt)
		{
			case 'p': // if command line option is -p
				percentage = atof(optarg); // get the percentage
				break;
		}
	}

	// allocate variables for random number generation
	gsl_rng *_gsl_rng;
	_gsl_rng = gsl_rng_alloc(gsl_rng_mt19937);

	// seed the random number generator
	struct timeval tv;
	gettimeofday(&tv,0);
	unsigned long mySeed = tv.tv_sec + tv.tv_usec;
	gsl_rng_set(_gsl_rng, mySeed);

	// get a random number in the range [1, 100]
	unsigned long int num = gsl_rng_uniform_int(_gsl_rng, 100) + 1;
	gsl_rng_free (_gsl_rng);
	
	// return 1 (success), if the random number is less than or equal to the percentage
	// else return 0 (failure)
	if (num <= percentage)
		return 1;
	else
		return 0;
}
