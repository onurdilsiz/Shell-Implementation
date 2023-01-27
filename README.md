# Simple Command Line Shell Program in C
This is a simple command line shell program written in C for Operating Systems Course(CMPE 322). It takes user input, parses it into tokens, and then performs different actions based on the input. The main functionality includes:

- Getting input from the user using the getline() function
- Parsing the input using the scanLineConvertToTokens() function which uses strtok to tokenize the input and store the tokens in a 2D char array called "tokens"
- The program has a built-in command called "dididothat" which checks if a previous command exists in the history
- The program can execute external commands using the execvp() function
- The history of commands are stored in the "history" 2D char array
- The program runs in a while loop and continues to prompt the user for input until the command "exit" is entered.
