// RAISAAT ATIFA RASHID

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void error_handler();
int isInteger(char s[]);
int isNumber (const char *s);
int isBin (char s[]);

int main (int argc, char **argv)
{
	// call error_handler if the no. of command line arguments is less than 6, more than 7 or if the first argument is not "-p"
	if (argc < 6 || argc > 7 || strcmp(argv[1], "-p") != 0)
		error_handler();

	// Parse the command line
	int opt, verbose, num_of_trials, percentage;
	char *filename = NULL;
	while ((opt = getopt(argc, argv, ":p:o:v:")) != -1)
	{
		switch(opt)
		{
			case 'p': // if the option is -p
				if(!isNumber(optarg) || atoi(optarg) < 0 || atoi(optarg) > 100) // check whether the percentage is a number greater than or equal to 0 and less than or equal to 100
					error_handler();
				percentage = atoi(optarg); // get the percentage if it is in the right format
				break;
			case 'v': // if the option is -v
				if(!isInteger(optarg)) // check if the number of trials is an integer
					error_handler();
				num_of_trials = atoi(optarg); // get the number of trials if it is in the right format
				verbose = 1; // set verbose to true
				break;
			case 'o': // if the option is -o
				if(!isBin(optarg)) // check whether the filename provided is a binary file
					error_handler();
				filename = optarg; // get the filename if it is in the right format
				break;
			case ':': // if an option does not have an argument
				error_handler();
			default:
				error_handler();
		}
	}

	// check whether exactly 7 command line arguments are provided if in verbose mode, or if exactly 6 arguments are provided otherwise
	if ((verbose && argc != 7) || (!verbose && argc != 6))
		error_handler();

	// if not in verbose mode, get the no. of trials
	if (!verbose && optind < argc)
	{
		if (!isInteger(argv[optind])) // check whether the number of trials is an integer
			error_handler();
		num_of_trials = atoi(argv[optind]); // get the number of trials if it is in the right format
	}

	FILE *outFile = fopen(filename, "ab"); // open the file for append
	
	// if the file cannot be opened, print an error message and exit from the program
	if (outFile == NULL)
	{
		fprintf(stderr, "Error! Could not open file.\n");
		exit(1);
	}
	
	int i = 0;
	int num_of_success = 0, num_of_failure = 0;

	fprintf(outFile,"%d ", percentage); // print the percentage in the ouput file

	// fork num_of_trials child processes
	for (; i < num_of_trials; i++)
	{
		pid_t pid = fork();
		if (pid < 0) // if there is an error in forking, print error message and exit
		{
			fprintf(stderr, "Error in creating child process.\n");
			exit(1);
		}
		else if (pid == 0) // if in child process
		{
			// execute hand and print an error message if there is an error in executing it
			if(execl("./hand", "hand", argv[1], argv[2], NULL) == -1)
			{	
				fprintf(stderr, "Error in executing execl.\n");
				exit(1);
			}
		}
		else // in parent process
		{	
			int val = -1; // to hold the return value from the child
			
			// if the child returns -1, print an error message and exit
			if (wait(&val) == -1)
			{
				fprintf(stderr, "Wait failed.\n");
				exit(1);
			}

			if (WEXITSTATUS(val)) // if child returns 1, i.e. success
			{
				num_of_success++; // increment the number of successes
				if (verbose) // print PID if in verbose mode
				{
					printf("PID %d returned success.\n", pid);
				}
			}
			else // if child returns 0, i.e. failure
			{
				num_of_failure++; // increment the number of failures
				if (verbose) // print PID if in verbose mode
				{
					printf("PID %d returned failure.\n", pid);
				}
			}
		}
	}

	printf("\nCreated %d processes.\n", num_of_trials);
	// print out the percentages of success and failure to stdout and the output file
	if (num_of_trials != 0)
	{
		printf("Success - %d%%\n", ((num_of_success * 100)/num_of_trials));
		printf("Failure - %d%%\n", ((num_of_failure * 100)/num_of_trials));
		fprintf(outFile, "%d ", (num_of_trials)); // print out the number of trials in the output file
		fprintf(outFile, "%d ", ((num_of_success * 100)/num_of_trials));
		fprintf(outFile, "%d\n", ((num_of_failure * 100)/num_of_trials));
	}
	fclose(outFile); // close the output file
	printf("Results were written to books.bin\n");
	return 0;
}

//This function determines whether a string is a positive integer
int isInteger(char s[])
{
	int len = strlen(s);
	
	if(len > 0)
	{
		int i = 0;
		for(; i < len; ++i)
		{
			if (!isdigit(s[i]))
				return 0;
		}
		return 1;
	}
	return 0;
}

//This function determines whether a string is a number
int isNumber (const char *s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;
	char * p;
	strtod (s, &p);
	return *p == '\0';
}

//This function determines whether a string is the name of a binary file
int isBin (char s[])
{
	int len = strlen(s);

	if (s[len - 4] != '.')
		return 0;
	if (s[len - 3] != 'b')
		return 0;
	if (s[len - 2] != 'i')
		return 0;
	if (s[len - 1] != 'n')
		return 0;
	return 1;
}

//This function prints out an error message and exits from the program
void error_handler()
{
	fprintf(stderr, "Error! You can enter 5 or 6 arguments when running this program. Either enter -p followed by a number between 0 and 100 (the percentage of success), -o (for filename), a binary file name  and a positive integer (the number of trials), or to run it in verbose mode, enter -p followed by a number between 0 and 100 (the percentage of success), -o (for file name), a binary file name, -v (for verbose) and a positive integer (the number of trials).\n");
	exit(1);
}
