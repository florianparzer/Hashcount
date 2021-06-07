/** \File
 * \brief Is a header that provides multible Functions for hashlists and wordlists. Author: Parzer Florian
 *
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashcount_func.h"
#include <unistd.h>

/**
 * \brief The main methode of the programm
 *
 * This methode handles the user input and starts depending on it various other methodes
 *
 * \param argc	The number of arguments that the user has written
 * \param argv	The arguments that the user has written
 * \return if there were no errors: 0. Otherwhise -1
 *
 */
int main(int argc, char *argv[]) {
	hashlist *list = NULL;
	wordlist *buckets = NULL;
	char *puff = NULL;
	char *delimiter = ",;";
	int opt;
	char *options = "b:o:r:c:h";
	char *error;
	long bucket = -1;
	int removeWords = 0;
	int censorWords = 0;
	char *output = NULL;


	while((opt = getopt(argc, argv, options)) != -1){
		switch(opt){
			case 'b':
				bucket = strtol(optarg, &error, 10);
				if(*optarg == *error){
					fprintf(stderr, "Please enter a valid number");
					return -1;
				}
				continue;
			case 'r':
				if (censorWords == 1){
					fprintf(stderr, "Too many arguments entered");
					return -1;
				}
				removeWords = 1;
				puff = strtok(optarg, delimiter);
				while(puff != NULL){
					addWord(&buckets, puff);
					puff = strtok(NULL, delimiter);
				}
				continue;
			case 'c':
				if (removeWords == 1){
					fprintf(stderr, "Too many arguments entered");
					return -1;
				}
				censorWords = 1;
				puff = strtok(optarg, delimiter);
				while(puff != NULL){
					addWord(&buckets, puff);
					puff = strtok(NULL, delimiter);
				}
				continue;
			case 'o':
				output = optarg;
				continue;
			case 'h':
				printf("This ist the Help of the hashcount Programm\n");
				printf("Valid Options:\n");
				printf("-b <Bucket>... Limits the output to the words in the stated bucket\n");
				printf("-r <list of buckets> ... Writes the input file in a new file without the words of the "
						"stated buckets\n%25sThe delimiters between the buckets are ; or ,\n%25srequires the option -o\n", " ", " ");
				printf("-c <list of buckets> ... Writes the input file in a new file and censors the words of the "
										"stated buckets\n%25sThe delimiters between the buckets are ; or ,\n%25srequires the option -o\n", " ", " ");
				printf("-o <file> ... The name of the output file for the options -w and -c\n");
				printf("-h ... Prints the help\n");
				return 0;
			case '?':
				fprintf(stderr, "Option not known\n");
				return -1;
			default:
				fprintf(stderr, "An unidentified error occured");
		}
	}

	FILE *file = fopen(argv[optind], "r");
	if(file == NULL){
		fprintf(stderr, "File %s Not Fount\n", argv[optind]);
		return -1;
	}

	readFile(&list, file);

	if(removeWords == 1){
		FILE *outFile = fopen(output, "w");
		if(outFile == NULL){
			fprintf(stderr, "File %s Not Fount\n", output);
			freeHashlist(list);
			return -1;
		}
		printBucketsWords(buckets, file, outFile);
		fclose(outFile);
	}else if(censorWords == 1){
		FILE *outFile = fopen(output, "w");
		if(outFile == NULL){
			fprintf(stderr, "File %s Not Fount\n", output);
			freeHashlist(list);
			return -1;
		}
		printCensorBuckets(buckets, file, outFile);
		fclose(outFile);
	}

	if(bucket >= 0){
		printHashBucket(list, bucket);
	}else{
		printHashlist(list);
	}
	freeHashlist(list);
	fclose(file);
	return 0;
}

