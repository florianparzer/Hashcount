# Hashcount

A Programm that uses a Hashlist to store words and provides further features

Compilation
Following Libraries are required for the compilation

Needed Non-Standard Libraries:
    unistd.h
    getopt.h
Needed Standard Libraries:
    string.h
    stdio.h
    stdlib.h
    ctype.h

Execution

    The user input is solely handled with the attributes of the program call

    Usage/Options:
        To read a file and print out the buckets you need to state the filename of the input file
        For example:
        hashcount words.txt

        -b
        If you only want a certain bucket to be printed use the -b option
        hashcount words.txt -b 5

        -o
        State the output file: -o <file>. This option is needed by the options -r and -c
        
        -r <listOfHashes>
        Writes the words of the input file to the output file, if their hash value matches one of the stated values.
        At the end it prints out the hashlist
        The list can be separated with ',' and ';'.
        This option requires the option -o.
        hashcount words.txt -r 5,82,12 -o newText.txt

        -c <listOfHashes>
        Writes all the words of the input file to the output file and censors the word if its hash matches on of the stated buckets.
        At the end it prints out the hashlist.
        The list can be separated with ',' and ';'.
        This option requires the option -o.
        hashcount words.txt -c 5,82,12 -o newText.txt

        -h
        Prints a help page with a description of the options

    User input while runtime
    There is no user input required while the program is running
    
Solved Tasks
    This program solves following tasks of the 

        Task 1 Main Task

        Task 2
        
        Task 3
        
        Task 4
        
        Task 5
    
        Task 7
    
    There are no additional features
