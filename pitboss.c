// RAISAAT ATIFA RASHID

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// struct for holding information for each percentage in the file
struct fileData
{
	int percentage, success, failure, numOfTrials;
};

// function prototypes
int isNumber (const char *s);
int isBin (char s[]);
void error_handler();

int main (int argc, char **argv)
{
	// check whether 4 arguments are provided and the first argument is -p
	if (argc != 4 || strcmp(argv[1], "-p") != 0)
	{
		error_handler();
	}	
	
	int opt, percentage;
	char *filename = NULL;

	// parse the command line
	while ((opt = getopt(argc, argv, ":p:")) != -1)
	{
		switch(opt)
		{
			case 'p': // if the command line option is -p
				if(!isNumber(optarg) || atoi(optarg) < 0 || atoi(optarg) > 100) // check whether the argument is a number, is greater thanor equal to 0 and less than or equal to 100
				{
					error_handler();
				}
				percentage = atoi(optarg); // get the percentage
				break;
			case ':': // if no argument is provided
				error_handler();
			default:
				error_handler();
		}
	}

	// get the file name
	if (optind < argc)
	{
		if(!isBin(argv[optind])) // check whether the file is binary
		{
			error_handler();
		}
		filename = argv[optind]; // get the filename
	}
	
	FILE *inFile = fopen(filename,"rb"); // open the binary file for reading

	// Count the number of lines in the file and close it
	char c;
	int count = 0;
	for (c = getc(inFile); c != EOF; c = getc(inFile)) 
	{
		if (c == '\n')
			count = count + 1;
	}
	fclose(inFile);

	inFile = fopen(filename,"rb"); // open the file again to shift the file pointer to the beginning of the file
	
	struct fileData input[count]; // define an array of structs to hold information for the percentages
	
	if(inFile == NULL) // print error message and exit if file cannot be opened for reading
	{
		fprintf(stderr, "Error! Could not open file.\n");
		exit(1);
	}

	printf("Reading results from %s ...\n\n", filename );

	// get each line in the file and read file contents till end of file
	char *line = NULL;
	size_t len = 0;
	int i = 0;
	while (getline(&line, &len, inFile) != -1)
	{
		char *token = strtok(line, " \0\n"); // get the first token
		int j;
		// get the percentage, number of trials, success and failure percentages
		for (j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				input[i].percentage = atoi(token); // parse the first token to get the percentage
			}
			else if (j == 1)
			{
				input[i].numOfTrials = atoi(token); // parse the second token to get the number of trials
			}
			else if (j == 2)
			{
				input[i].success = atoi(token); // parse the third token to get the success percentage
			}
			else if (j == 3)
			{
				input[i].failure = atoi(token); // parse the fourth token to get the failure percentage
			}
			token = strtok(NULL, " \0\n"); 
		}
		i++; // increment i as long as there is a next line
	}
	
	printf ("Checking results for -p = %d ...\n\n", percentage);
	i = 0;
	int success = 0; // accumulator for successes
	int failure = 0; // accumulator for failures
	int numOfTrials = 0; // accumulator for number of trials
	// go through the structs array to find the percentage
	for (; i < count; i++)
	{
		// if the percentage is found, accumulate success, failure and numOfTrials
		if (input[i].percentage == percentage) 
		{
			success += (input[i].success*input[i].numOfTrials)/100;
			failure += (input[i].failure*input[i].numOfTrials)/100;
			numOfTrials += input[i].numOfTrials;
		}
	}
	
	// print the results
	
	printf("Found %d trials.\n\n", numOfTrials);
	printf("Success - %d%%\n", (success*100)/numOfTrials);
	printf("Failure - %d%%\n", (failure*100)/numOfTrials);

	fclose(inFile); // close the file

	return 0;
}

//This function checks whether a string is a number
int isNumber (const char *s)
{
	if (s == NULL || *s == '\0' || isspace(*s))
		return 0;
	char * p;
	strtod (s, &p);
	return *p == '\0';
}

// This function checks whether the name of the file ends with a .bin extension
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

// This function prints out an error message and exits from the program
void error_handler()
{
	fprintf(stderr, "Error! Please provide 3 arguments: -p followed by a number between 0 and 100 (inclusive) and a binary file name\n");
	exit(1);
}
