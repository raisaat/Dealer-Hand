#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <gsl/gsl_rng.h>
#include <string.h>
#include <ctype.h>

void error_handler();
int isInteger(char s[]);
int isNumber (const char *s);

int main (int argc, char **argv)
{
	if (argc < 4 || argc > 7 || strcmp(argv[1], "-p") != 0)
		error_handler();

	int opt, verbose = 0, conc = 0, num_of_trials = 0, num_of_conc = 0;

	while ((opt = getopt(argc, argv, ":p:t:v:")) != -1)
	{
		switch(opt)
		{
			case 'p':
				if(!isNumber(optarg) || atof(optarg) < 0 || atof(optarg) > 100)
					error_handler();
				break;
			case 'v':
				if(!isInteger(optarg))
					error_handler();
				num_of_trials = atoi(optarg);
				verbose = 1;
				break;
			case 't':
				if(!isInteger(optarg) || atoi(optarg) < 0 || atoi(optarg) > 8) 
					error_handler();
				num_of_conc = atoi(optarg);
				conc = 1;
				break;
			case ':':
				error_handler();
			default:
				error_handler();
		}
	}

	if ((!verbose && !conc && argc != 4) || (verbose && !conc && argc != 5) || (!verbose && conc && argc != 6) || (verbose && conc && argc != 7))
		error_handler();

	if (!verbose && optind < argc)
	{
		if (!isInteger(argv[optind]))
			error_handler();
		num_of_trials = atoi(argv[optind]);
	}
	
	int i = 0;
	int num_of_success = 0, num_of_failure = 0;

	printf("\nRunning %d processes one at a time...\n", num_of_trials);

	for (; i < num_of_trials; i++)
	{
		pid_t pid = fork();
	
		if (pid < 0)
		{
			fprintf(stderr, "Error in creating child process.\n");
			exit(1);
		}
		else if (pid == 0)
		{
			if(execl("./hand", "hand", argv[1], argv[2], NULL) == -1)
			{	
				fprintf(stderr, "Error in executing execl.\n");
				exit(1);
			}
		}
		else
		{	
			int val = -1;
			if (wait(&val) == -1)
			{
				fprintf(stderr, "Wait failed.\n");
				exit(1);
			}

			if (WEXITSTATUS(val))
			{
				num_of_success++;
				if (verbose)
					printf("PID %d returned success.\n", pid);
			}
			else
			{
				num_of_failure++;
				if (verbose)
					printf("PID %d returned failure.\n", pid);
			}
		}
	}

	printf("\nRunning %d processes concurrently...\n", num_of_conc);

	for (i = 0; i < num_of_conc; i++)
	{
		pid_t pid = fork();
		if (pid < 0)
		{
			fprintf(stderr, "Error in creating child process.\n");
			exit(1);
		}
		else if (pid == 0)
		{
			if(execl("./hand", "hand", argv[1], argv[2], NULL) == -1)
			{
				fprintf(stderr, "Error in executing execl.\n");
				exit(1);
			}
		}
	}

	for (i = 0; i < num_of_conc; i++)
	{
		int val = -1;
		pid_t pid;

		if ((pid = wait(&val)) == -1)
		{
			fprintf(stderr, "Wait failed.\n");
			exit(1);
		}
		if (WEXITSTATUS(val))
		{
			num_of_success++;
			if (verbose)
				printf("PID %d returned success.\n", pid);
		}
		else
		{
			num_of_failure++;
			if (verbose)
				printf("PID %d returned failure.\n", pid);
		}
	}

	printf("\nCreated %d processes.\n", num_of_trials + num_of_conc);
	if (num_of_trials != 0)
	{
		printf("Success - %d%%\n", ((num_of_success * 100)/(num_of_trials + num_of_conc)));
		printf("Failure - %d%%\n", ((num_of_failure * 100)/(num_of_trials + num_of_conc)));
	}
	return 0;
}

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

int isNumber (const char *s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;
	char * p;
	strtod (s, &p);
	return *p == '\0';
}

void error_handler()
{
	fprintf(stderr, "Error!\nYou can only enter 3-6 arguments in one of the following formats:\n");
	fprintf(stderr, "1. -p followed by a number from 1 to 100 inclusive (the percentage of success) and a positive integer (the number of processes to run one at a time)\n");
	fprintf(stderr, "2. -p followed by a number from 1 to 100 inclusive (the percentage of success), -v (for verbose) and a positive integer (the number of trials to run one at a time)\n");
	fprintf(stderr, "3. -p followed by a number from 1 to 100 inclusive (the percentage of success), -t (to run processes concurrently), an integer between 0 and 8 (the number of processes to run concurrently) and a positive integer (the number of processes to run one at a time)\n");
	fprintf(stderr, "4. -p followed by a number from 1 to 100 inclusive (the percentage of success), -t (to run processes concurrently), an integer between 0 and 8 (the number of processes to run concurrently), -v (for verbose) and a positive integer (the number of trials to run one at a time)\n");
	exit(1);
}
