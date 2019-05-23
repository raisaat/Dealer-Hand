# Dealer-Hand

### Program Details

**hand:**

The hand takes a percentage, p, from the command line and returns success or failure based on that percentage. If the percentage passed to it is 70, then the hand has 70% chance of returning success; if it is 20, then the hand has 20% chance of returning success. This is implemented using a random number generator. If the random number generated is less than or equal to p, then the hand returns success. Otherwise, it returns failure. The hand program uses getopt() to parse the command line options to extract the value of p.

**dealer:**

The dealer takes up to 4 command-line arguments: 1) the same percentage, p, passed to hand, 2) the number of trials (n), 3) an optional -v flag (for verbose mode), and 4) an optional -o flag (for the name of the output file where the results will be written to). The dealer creates n child processes (one at a time), records the result returned by each child process, and finally prints out the number of child processes and their IDs, the percentage of child processes that returned success and the percentage of child processes that returned failure.

The dealer uses getopt() to parse the 3 command-line arguments: \<-p percentage\>, \<n\>, and the optional -v flag. The -p and -v options should always come before the last value, which is n. The -p option and n are required, but the -v option is optional. The following are examples of running the program with and without the -v flag:
  * ./dealer -p 70 -v 1000
  * ./dealer -p 20 1000

After parsing the arguments, the dealer spawns n child processes and each child executes the hand using the "execl" command. The dealer uses the wait() function to wait for each child to finish executing. When each child process finishes executing, the dealer uses it's exit status (1 or 0) to determine whether it returned success (1) or failure (0). 
