# Dealer-Hand

### Program Details

**hand:**

The hand takes a percentage, p, from the command line and returns success or failure based on that percentage. For example, if the percentage passed to it is 70, then the hand has 70% chance of returning success; if it is 20, then the hand has 20% chance of returning success. This is implemented using a random number generator. If the random number generated is less than or equal to p, then the hand returns success. Otherwise, it returns failure. The hand program uses getopt() to parse the command line options to extract the value of p.

**dealer:**

The dealer takes up to 4 command-line arguments: 1) the same percentage, p, passed to the hand, 2) the number of simulations to run for that p (num_of_trials), 3) an optional -v flag, and 4) an optional -o flag. The -v flag is for verbose mode, i.e. the user can specify this option if the user wants to print the PID and status (success/failure) of each child process. The -o flag allows the user to write the results of the program to a binary file, the name of which is specified by the user after the flag. If the file specified does not exist, the dealer creates the file. Otherwise, the dealer appends the results to the end of the file. The dealer creates num_of_trials child processes (one at a time), records the result returned by each child process, and finally prints num_of_trials, the percentage of child processes that returned success and the percentage of child processes that returned failure both to the console and also to the output file specified by the user if the user had selected the -o flag.

The dealer uses getopt() to parse the 4 command-line arguments: \<-p percentage\>, \<n\>, an optional -v flag and an optional -o flag. The -p option should come first, followed by the -o option (if specified), the -v option (if specified) and finally, num_of_trials, which should always come last. The -p option and num_of_trials are required, but the -v and -o options are optional. The following are examples of running the program:
  * ./dealer -p 70 -v 10
  * ./dealer -p 20 60
  * ./dealer -p 50 -o books.bin 1009
  * ./dealer -p 34 -o books.bin -v 200

After parsing the arguments, the dealer spawns num_of_trials child processes and each child executes the hand using the "execl" command. The dealer uses the wait() function to wait for each child to finish executing. When each child process finishes executing, the dealer uses it's exit status (1 or 0) to determine whether it returned success (1) or failure (0). While it is looping through num_of_trials child processes, it accumulates the number of successful child processes and the number of failed child processes for compting the percentage of successful child processes and the percentage of failed child processes. It then prints the statistics to the file in the following format: \<percentage p\>\<num_of_trials\>\<percentage of success\>\<percentage of failure\>\<new line\>.

Dealer was run with 1000 trials per percentage p, starting with a percentage of 10 and incrementing by 10 until a 90 is reached for a total of 9 trial runs and 9x1000 simulations. The following commands were used:
 * ./dealer -p 10 -o books.bin 1000
 * ./dealer -p 20 -o books.bin 1000
 * ./dealer -p 30 -o books.bin 1000
 * ...
The resulting file (books.bin) was used by the pitboss program for analysis.

**pitboss:**

The pitboss takes two command-line arguments, a percentage, p, and a binary file name. The binary file contains a set of perecentages (p), each followed by the number of trial runs, the percentage of success and the percentage of failure for that p, formatted in the following manner: \<percentage p\>\<num_of_trials\>\<percentage of success\>\<percentage of failure\>\<new line\>. The user types in a percentage, p, to look for in the binary file. The program finds all the lines containing that p and accumulates the number of trials, as well as calculates and accumulates the total number of success and the total number of failure for that p from the percentages of success and failure for that extracted from those lines. It computes the total percentage of success from the total number of success and the total number of trial runs, as well as the total percentage of failure from the total number of failure and the total number of trial runs for that p. Finally, it prints p, the total number of trials found for that p, the percentage of success and the percentage of failure for that p. The following command is an example of running the pitboss program:
 * ./piboss -p 50 books.bin

The pitboss program was run with the books.bin file from the dealer for each percentage, p, from 10 - 90 (inclusive) in order to find out how accurate the dealer and the hand are in determining the percentage of success for a given p. It was found that the percentage of success for each p was very close to the value of p as expected.
