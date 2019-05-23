# Dealer-Hand

### Program Details

**hand:**

The hand takes a percentage, p, from the command line and returns success or failure based on that percentage. For example, if the percentage passed to it is 70, then the hand has 70% chance of returning success; if it is 20, then the hand has 20% chance of returning success. This is implemented using a random number generator. If the random number generated is less than or equal to p, then the hand returns success. Otherwise, it returns failure. The hand program uses getopt() to parse the command line options to extract the value of p.

**dealer:**

The dealer takes up to 4 command-line arguments: 1) the same percentage, p, passed to the hand, 2) the number of trials to specify the number of child processes to create (num_of_trials), 3) an optional -v flag, and 4) an optional -o flag. The -v flag is for verbose mode, i.e. the user can specify this option if the user wants to print the PID and status (success/failure) of each child process. The -o flag allows the user to write the results of the program to a binary file, the name of which is specified by the user after the flag. If the file specified does not exist, the dealer creates the file. Otherwise, the dealer appends the results to the end of the file. The dealer creates num_of_trials child processes (one at a time), records the result returned by each child process, and finally prints num_of_trials, the percentage of child processes that returned success and the percentage of child processes that returned failure both to the console and also to the output file specified by the user if the user had selected the -o flag.

The dealer uses getopt() to parse the 4 command-line arguments: \<-p percentage\>, \<n\>, an optional -v flag and an optional -o flag. The -p option should come first, followed by the -o option (if specified), the -v option (if specified) and finally, num_of_trials, which should always come last. The -p option and num_of_trials are required, but the -v and -o options are optional. The following are examples of running the program:
  * ./dealer -p 70 -v 1000
  * ./dealer -p 20 1000
  * ./dealer -p 50 -o books.bin 1000
  * ./dealer -p 34 -o books.bin -v 1000

After parsing the arguments, the dealer spawns num_of_trials child processes and each child executes the hand using the "execl" command. The dealer uses the wait() function to wait for each child to finish executing. When each child process finishes executing, the dealer uses it's exit status (1 or 0) to determine whether it returned success (1) or failure (0). While it is looping through num_of_trials child processes, it accumulates the number of successful child processes and the number of failed child processes for compting the percentage of successful child processes and the percentage of failed child processes.
